#include "mainwindow.hpp"
#include "annotation_widget.hpp"
#include "open_folder_dialog.hpp"
#include "save_json_dialog.hpp"
#include "save_notification_dialog.hpp"
#include "image_container.hpp"
#include "constants.hpp"

#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QImageReader>
#include <QToolBar>
#include <QIcon>
#include <QLabel>
#include <QKeySequence>
#include <QLineEdit>
#include <QFloat16>
#include <QJsonDocument>
#include <QPushButton>
#include <QJsonDocument>
#include <QFile>
#include <QIODevice>
#include <QMessageBox>
#include <QDialog>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QMessageBox>
#include <QStatusBar>
#include <QCursor>
#include <QShortcut>

#include <filesystem>

namespace fs = std::filesystem;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(1024, 800);
    setWindowTitle(QString("NLVR Annotation Tool"));

    qDebug() << "In MainWindow::MainWindow - Creating actions";
    createActions();

    qDebug() << "In MainWindow::MainWindow - Creating file menu";
    createFileMenu();

    qDebug() << "In MainWindow::MainWindow - Creating edit menu";
    createEditMenu();

    qDebug() << "In MainWindow::MainWindow - Creating help menu";
    createHelpMenu();

    qDebug() << "In MainWindow::MainWindow - Creating tool bar";
    createToolbar();

    m_default_data = QJsonObject({
            QPair<QString, QJsonValue>(ID, ""),
            QPair<QString, QJsonValue>(IMAGE_1, ":/media/images/no-image.jpg"),
            QPair<QString, QJsonValue>(IMAGE_2, ":/media/images/no-image.jpg"),
            QPair<QString, QJsonObject>(ANNOTATION, QJsonObject({
                                                        QPair<QString, QJsonValue>(CAPTION, "Câu mô tả mặc định"),
                                                        QPair<QString, QJsonValue>(ENG_CAPTION, "Default caption"),
                                                        QPair<QString, QJsonValue>(LABEL, "false"),
                                                    }))
        });

    open_folder_dialog = new OpenFolderDialog(this);
    save_json_dialog = new SaveJsonDialog(this);
    m_save_notification_dialog = new SaveNotificationDialog(this);

    centralWidget = new QWidget(this);
    m_layout = new QVBoxLayout(centralWidget);
    m_imageContainer = new ImageContainer(this);
    m_annotationWidget = new AnnotationWidget(this);
    m_layout->addWidget(m_imageContainer, 5);
    m_layout->addWidget(m_annotationWidget, 1);
    setCentralWidget(centralWidget);

    qDebug() << "In MainWindow::MainWindow - Creating connections";
    createConnections();

    qDebug() << "In MainWindow::MainWindow - Loading the default image";
    loadData(-1); // load the default image

    qDebug() << "In MainWindow::MainWindow - Setting shortcuts";
    // next image shortcut
    nextImageShortcut = new QShortcut(this);
    nextImageShortcut->setContext(Qt::ApplicationShortcut);
    nextImageShortcut->setKey(Qt::Key_Alt | Qt::Key_Right);
    // previous image shortcut
    previousImageShortcut = new QShortcut(this);
    previousImageShortcut->setContext(Qt::ApplicationShortcut);
    previousImageShortcut->setKey(Qt::Key_Alt | Qt::Key_Left);
}

qsizetype MainWindow::dataSize()
{
    return m_data.size();
}

MainWindow::~MainWindow()
{
    qDebug() << "In MainWindow::~MainWindow - Existing ...";
}

qsizetype MainWindow::findFirstEmptyAnnotation()
{
    for (qsizetype ith = 0; ith < dataSize(); ith++)
    {
        QJsonObject annotation = m_data[ith][ANNOTATION].toObject();
        QString const& caption = annotation[CAPTION].toString();
        if (caption == "")
            return ith; // return the first sample which has not been annotated
    }

    return 0; // else return the first sample
}

void MainWindow::openFolder()
{
    qDebug() << "In MainWindow::openFolder - Opening folder";

    int dialog_returned = open_folder_dialog->openDialog();
    if (dialog_returned == QDialog::Accepted)
    {
        loadJson(open_folder_dialog->selectedFiles().constLast());

        // find the first image not annotated
        current_sample_idx = findFirstEmptyAnnotation();
        loadData(current_sample_idx);
        enableImageFeatures(true);
    }

    emit saveStatusEnabledChanged(false); // nothing to save when open a workspace
}

void MainWindow::saveJsonFile()
{
    if (save_json_dialog->isFirstOpen)
    {
        save_json_dialog->openDialog(open_folder_dialog->m_history);
        if (save_json_dialog->selectedFiles().isEmpty())
        {
            qDebug() << "In MainWindow::saveJsonFile - Cannot specified any selected folder, maybe user did not choose one";
            return;
        }
        save_json_dialog->setSelectedFile(save_json_dialog->selectedFiles().constLast());
    }

    saveAnnotatationsForSample(current_sample_idx);
    saveJson(save_json_dialog->selectedFile());
    emit saveStatusEnabledChanged(false); // have saved changed things
}

void MainWindow::loadJson(QString const &folder)
{
    m_directory = QDir(folder);
    QList<QString> json_filter;
    json_filter << "*.json";
    QList<QFileInfo> json_files = m_directory.entryInfoList(json_filter);

    qDebug() << QString("In MainWindow::loadJson - Found %1, loading this json file").arg(json_files.constLast().absoluteFilePath()).toStdString().c_str();
    emit createdNovelFile(false);
    save_json_dialog->toggleFileSelected(false);
    QFile file(json_files.constLast().absoluteFilePath());
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "Open workspace error!", QString("Error while opening %1").arg(json_files.constLast().path()));
    }

    save_json_dialog->setSelectedFile(json_files.constLast().absoluteFilePath());

    QTextStream filestream(&file);
    QByteArray content = filestream.readAll().toUtf8();
    QJsonArray annotations = QJsonDocument::fromJson(content).array();
    for (auto annotation: annotations) {
        m_data.append(annotation.toObject());
    }
    file.close();
}

void MainWindow::saveJson(QString const &filename)
{
    QJsonArray jsonArray;
    for (auto item: m_data) {
        jsonArray.append(item);
    }
    QByteArray json_data = QJsonDocument(jsonArray).toJson(QJsonDocument::Indented);
    QFile json_file(filename);

    if (!json_file.open(QIODevice::WriteOnly))
    {
        qDebug() << QString("In MainWindow::saveJson - Failed to open %1").arg(filename).toStdString().c_str();
        QMessageBox::warning(this, QString("Open file error"), QString("Failed to open %1").arg(filename));
        return;
    }

    json_file.write(json_data);
    json_file.close();

    qDebug() << QString("In MainWindow::saveJson - Saved annotations to %1").arg(filename).toStdString().c_str();
}

void MainWindow::loadData(qint16 sample_idx)
{
    setCursor(QCursor(Qt::WaitCursor));

    if (sample_idx >= dataSize())
    {
        QMessageBox::warning(this, "Loadding error occured!", "Can not load image");
        m_annotationWidget->setEnabled(false);
        return;
    }

    fs::path BASE_DIR(m_directory.path().toStdString());
    QDir path_top, path_bottom;
    QJsonObject data;
    bool enableAnnotationWidget = true;

    if (sample_idx < 0) // no image
    {
        data = m_default_data;
        enableAnnotationWidget = false;
        path_top = QDir(data[IMAGE_1].toString());
        path_bottom = QDir(data[IMAGE_2].toString());
    }
    else
    {
        data = m_data[sample_idx];
        fs::path filename_1{data[IMAGE_1].toString().toStdString()};
        fs::path filename_2{data[IMAGE_2].toString().toStdString()};
        path_top = QDir(BASE_DIR / filename_1);
        path_bottom = QDir(BASE_DIR / filename_2);
    }

    m_imageContainer->m_image_widget_top->setImage(path_top);
    m_imageContainer->m_image_widget_bottom->setImage(path_bottom);
    m_annotationWidget->setAnnotation(data[ANNOTATION].toObject());

    if (dataSize() > 0)
    {
        previousImageAction->setEnabled(sample_idx > 0);
        nextImageAction->setEnabled(sample_idx < dataSize() - 1);
        emit imageChanged(m_data[sample_idx][IMAGE_1].toString());
    }
    m_annotationWidget->setEnabled(enableAnnotationWidget);

    setCursor(QCursor(Qt::ArrowCursor));
}

void MainWindow::nextImage()
{
    saveJsonFile();

    if (current_sample_idx < m_data.size() - 1)
    {
        current_sample_idx += 1;
        loadData(current_sample_idx);
    }
}

void MainWindow::previousImage()
{
    saveJsonFile();

    if (current_sample_idx > 0)
    {
        current_sample_idx -= 1;
        loadData(current_sample_idx);
    }
}

void MainWindow::zoomIn()
{
    m_imageContainer->m_image_widget_top->zoomIn();
    m_imageContainer->m_image_widget_bottom->zoomIn();
}

void MainWindow::zoomOut()
{
    m_imageContainer->m_image_widget_top->zoomOut();
    m_imageContainer->m_image_widget_bottom->zoomOut();
}

void MainWindow::resetScaling()
{
    m_imageContainer->m_image_widget_top->resetScaling();
    m_imageContainer->m_image_widget_bottom->resetScaling();
}

void MainWindow::saveAnnotatationsForSample(qsizetype image_idx)
{
    if (image_idx < 0 || image_idx >= dataSize())
    {
        qDebug() << QString("In MainWindow::saveAnnotatationsForImage - image_idx must be in range [%1, %2], received image_idx = %3").arg(0).arg(dataSize() - 1).arg(image_idx).toStdString().c_str();
        return;
    }

    QJsonObject datapoint = m_data[image_idx];
    datapoint[ANNOTATION] = *(m_annotationWidget->annotation());
    m_data[image_idx] = datapoint;
    setSaveStatus(false);
}

void MainWindow::setSaveStatus(bool enabled)
{
    saveJsonAction->setEnabled(enabled);
}

void MainWindow::fitToWindow()
{
    m_imageContainer->m_image_widget_top->fitToContainer();
    m_imageContainer->m_image_widget_bottom->fitToContainer();
}

void MainWindow::onQuitAction()
{
    if (saveJsonAction->isEnabled())
    {
        int dialog_returned = m_save_notification_dialog->exec();
        if (dialog_returned == QDialog::Accepted)
        {
            saveJsonAction->trigger(); // save the json
        }
    }

    close();
}

void MainWindow::createActions()
{
    // create actions for File Menu
    openFolderAction = new QAction(QIcon(":/media/icons/open-image.png"), "Open Folder", this);
    openFolderAction->setShortcut(QKeySequence::Open);
    saveJsonAction = new QAction(QIcon(":/media/icons/save-file.png"), "Save Json File", this);
    saveJsonAction->setShortcut(QKeySequence::Save);
    saveJsonAction->setEnabled(false); // nothing to save when newly open the tool
    quitAction = new QAction(QIcon(":/media/icons/quit.png"), "Quit", this);
    quitAction->setShortcut(QKeySequence::Quit);

    nextImageAction = new QAction(QIcon(":/media/icons/next-image.png"), "Next Image", this);
    previousImageAction = new QAction(QIcon(":/media/icons/previous-image.png"), "Previous Image", this);

    rotateLeftAction = new QAction(QIcon(":/media/icons/rotate-left.png"), "Rotate left", this);
    rotateRightAction = new QAction(QIcon(":/media/icons/rotate-right.png"), "Rotate right", this);
    zoomInAction = new QAction(QIcon(":/media/icons/zoom-in.png"), "Zoom In", this);
    zoomInAction->setShortcut(QKeySequence::ZoomIn);
    zoomOutAction = new QAction(QIcon(":/media/icons/zoom-out.png"), "Zoom Out", this);
    zoomOutAction->setShortcut(QKeySequence::ZoomOut);
    fitToWindowAction = new QAction(QIcon(":/media/icons/fit-to-screen.png"), "Fit To Window", this);

    enableImageFeatures(false);

    // create actions for Help Menu
    aboutAction = new QAction(QIcon(), "About", this);
}

void MainWindow::createFileMenu()
{
    QMenu *fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(openFolderAction);
    fileMenu->addAction(saveJsonAction);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);
}

void MainWindow::createEditMenu()
{
    QMenu *editMenu = menuBar()->addMenu("&Edit");
    editMenu->addAction(rotateLeftAction);
    editMenu->addAction(rotateRightAction);
    editMenu->addAction(zoomInAction);
    editMenu->addAction(zoomOutAction);
    editMenu->addAction(fitToWindowAction);
}

void MainWindow::createHelpMenu()
{
    QMenu *helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction(aboutAction);
}

void MainWindow::createToolbar()
{
    QToolBar *toolbar = this->addToolBar("main_toolbar");
    toolbar->addAction(openFolderAction);
    toolbar->addAction(saveJsonAction);

    toolbar->addSeparator();
    toolbar->addAction(previousImageAction);
    toolbar->addAction(nextImageAction);

    toolbar->addSeparator();
    toolbar->addAction(rotateLeftAction);
    toolbar->addAction(rotateRightAction);
    toolbar->addAction(zoomInAction);
    toolbar->addAction(zoomOutAction);
    toolbar->addAction(fitToWindowAction);
    toolbar->addSeparator();
    toolbar->addAction(quitAction);

    toolbar->setMovable(false);
}

void MainWindow::createConnections()
{
    QObject::connect(this, &MainWindow::imageChanged, this, &MainWindow::changeWindowTitle);

    QObject::connect(nextImageShortcut, &QShortcut::activated, this, &MainWindow::nextImage);
    QObject::connect(previousImageShortcut, &QShortcut::activated, this, &MainWindow::previousImage);

    // create connections for actions
    QObject::connect(openFolderAction, &QAction::triggered, this, &MainWindow::openFolder);
    QObject::connect(saveJsonAction, &QAction::triggered, this, &MainWindow::saveJsonFile);
    QObject::connect(this, &MainWindow::createdNovelFile, save_json_dialog, &SaveJsonDialog::toggleFileSelected);
    QObject::connect(this, &MainWindow::saveStatusEnabledChanged, this, &MainWindow::setSaveStatus);
    QObject::connect(m_annotationWidget, &AnnotationWidget::haveAdjusted, this, &MainWindow::setActiveSaveStatus);

    QObject::connect(rotateLeftAction, &QAction::triggered, m_imageContainer->m_image_widget_top, &ImageWidget::rotateLeft);
    QObject::connect(rotateRightAction, &QAction::triggered, m_imageContainer->m_image_widget_top, &ImageWidget::rotateRight);
    QObject::connect(rotateLeftAction, &QAction::triggered, m_imageContainer->m_image_widget_bottom, &ImageWidget::rotateLeft);
    QObject::connect(rotateRightAction, &QAction::triggered, m_imageContainer->m_image_widget_bottom, &ImageWidget::rotateRight);
    QObject::connect(zoomInAction, &QAction::triggered, this, &MainWindow::zoomIn);
    QObject::connect(zoomOutAction, &QAction::triggered, this, &MainWindow::zoomOut);
    QObject::connect(fitToWindowAction, &QAction::triggered, this, &MainWindow::fitToWindow);

    QObject::connect(nextImageAction, &QAction::triggered, this, &MainWindow::nextImage);
    QObject::connect(m_imageContainer->m_image_widget_top, &ImageWidget::nextImage, this, &MainWindow::nextImage);
    QObject::connect(m_imageContainer->m_image_widget_bottom, &ImageWidget::nextImage, this, &MainWindow::nextImage);
    QObject::connect(m_annotationWidget, &AnnotationWidget::nextAnnotation, this, &MainWindow::nextImage);
    QObject::connect(nextImageAction, &QAction::triggered, this, &MainWindow::resetScaling);
    QObject::connect(previousImageAction, &QAction::triggered, this, &MainWindow::previousImage);
    QObject::connect(m_imageContainer->m_image_widget_top, &ImageWidget::backImage, this, &MainWindow::previousImage);
    QObject::connect(m_imageContainer->m_image_widget_bottom, &ImageWidget::backImage, this, &MainWindow::previousImage);
    QObject::connect(m_annotationWidget, &AnnotationWidget::previousAnnotation, this, &MainWindow::previousImage);
    QObject::connect(previousImageAction, &QAction::triggered, this, &MainWindow::resetScaling);

    QObject::connect(quitAction, &QAction::triggered, this, &MainWindow::onQuitAction);
}

void MainWindow::enableImageFeatures(bool enabled)
{
    previousImageAction->setEnabled(current_sample_idx > 0);
    nextImageAction->setEnabled(current_sample_idx < dataSize() - 1);
    rotateLeftAction->setEnabled(enabled);
    rotateRightAction->setEnabled(enabled);
    zoomInAction->setEnabled(enabled);
    zoomOutAction->setEnabled(enabled);
    fitToWindowAction->setEnabled(enabled);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left)
        previousImage();

    if (event->key() == Qt::Key_Right)
        nextImage();

    QMainWindow::keyPressEvent(event);
}

void MainWindow::setActiveSaveStatus()
{
    emit saveStatusEnabledChanged(true);
}

void MainWindow::closeEvent(QCloseEvent * /* event */)
{
    onQuitAction();
}

void MainWindow::changeWindowTitle(QString title)
{
    setWindowTitle(title);
}

