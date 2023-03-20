#include "mainwindow.hpp"
#include "annotation_widget.hpp"
#include "open_folder_dialog.hpp"
#include "save_json_dialog.hpp"
#include "save_notification_dialog.hpp"
#include "container.hpp"
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
#include <QDir>

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(1024, 800);

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

    m_default_data = QJsonObject({QPair<QString, QString>(FILENAME, ":/media/images/no-image.jpg"),
                                  QPair<QString, QString>(FILEPATH, ""),
                                  QPair<QString, bool>(DELETE, false)});

    m_default_annotation = QJsonObject({
        QPair<QString, QString>(QUESTION, ""),
        QPair<QString, QString>(ANSWER, "")
    });
    QJsonArray annotations;
    for (qsizetype ith = 0; ith < total_initial_annotations; ith++)
        annotations.append(m_default_annotation);

    m_default_data[ANNOTATIONS] = annotations;

    open_folder_dialog = new OpenFolderDialog(this);
    save_json_dialog = new SaveJsonDialog(this);
    m_save_notification_dialog = new SaveNotificationDialog(this);

    m_container = new Container(this);
    setCentralWidget(m_container);

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

Container *MainWindow::container()
{
    return m_container;
}

qsizetype MainWindow::dataSize()
{
    if (m_data == nullptr)
        return 0;

    return m_data->size();
}

MainWindow::~MainWindow()
{
    qDebug() << "In MainWindow::~MainWindow - Existing ...";
}

qsizetype MainWindow::findFirstEmptyAnnotation()
{
    for (qsizetype ith = dataSize() - 1; ith >= 0; ith--)
    {
        QJsonObject sample = m_data->at(ith).toObject();
        QJsonArray annotations = sample[ANNOTATIONS].toArray();
        // if (!m_data->at(ith)[DELETE].toBool())
        //     return ith+1 == dataSize() ? ith : ith+1; // return the first image which has not been annotated
        for (auto annotation : annotations)
            if ((annotation[QUESTION].toString() != "") || (annotation[ANSWER].toString() != ""))
                return ith + 1 == dataSize() ? ith : ith + 1; // return the first image which has not been annotated
    }

    return 0; // else return the first image
}

void MainWindow::openFolder()
{
    qDebug() << "In MainWindow::openFolder - Opening folder";

    int dialog_returned = open_folder_dialog->openDialog();
    if (dialog_returned == QDialog::Accepted)
    {
        loadJson(open_folder_dialog->selectedFiles().constLast());

        // find the first image not annotated
        current_image_idx = findFirstEmptyAnnotation();
        loadData(current_image_idx);
        enableImageFeatures(true);
    }

    emit saveStatusEnabledChanged(false); // nothing to save when open a workspace
}

void MainWindow::saveJsonFile()
{
    saveAnnotatationsForImage(current_image_idx);
    fs::path current_dir(open_folder_dialog->selectedFiles()[0].toStdString());
    std::string annotation_path = (current_dir / fs::path(ANNOTATION_FILE.toStdString()));
    saveJson(QString(annotation_path.c_str()));
    emit saveStatusEnabledChanged(false); // have saved changed things
}

void MainWindow::loadJson(QString const &folder)
{
    m_directory = QDir(folder);
    QList<QString> json_filter;
    json_filter << "*.json";
    QList<QFileInfo> json_files = m_directory.entryInfoList(json_filter);

    if (json_files.size() == 0) // no annotation file
    {
        qDebug() << QString("In MainWindow::loadJson - There is no json file in %1, will create new one").arg(m_directory.path()).toStdString().c_str();
        emit createdNovelFile(true);
        QList<QString> image_filters;
        image_filters << "*.jpeg"
                      << "*.png"
                      << "*.jpg";

        QList<QFileInfo> image_files = m_directory.entryInfoList(image_filters);

        m_data = std::make_shared<QJsonArray>();
        for (QFileInfo const &image_file : image_files)
        {
            QJsonObject object;
            object[FILENAME] = image_file.fileName();
            object[FILEPATH] = image_file.dir().dirName();
            object[DELETE] = true;
            QJsonArray annotations;
            // create annotations
            for (qsizetype ith = 0; ith < total_initial_annotations; ith++)
                annotations.append(m_default_annotation);
            object[ANNOTATIONS] = annotations;
            m_data->append(object);
        }
        saveJsonFile();
    }
    else // load json data
    {
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
        m_data = std::make_shared<QJsonArray>(QJsonDocument::fromJson(content).array());
        file.close();
    }

    deleteImageCheckBox->setEnabled(true);
}

void MainWindow::saveJson(QString const &filename)
{
    QByteArray json_data = QJsonDocument(*m_data).toJson(QJsonDocument::Indented);
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

void MainWindow::loadData(qint16 image_idx)
{
    if (image_idx >= dataSize())
    {
        QMessageBox::warning(this, "Loadding error occured!", "Can not load image");
        m_container->m_annotation_widget->setEnabled(false);
        return;
    }

    fs::path BASE_DIR(m_directory.path().toStdString());
    QDir path;
    QJsonObject data;
    bool enableAnnotationWidget = true;

    if (image_idx < 0)
    {
        data = m_default_data;
        enableAnnotationWidget = false;
        path = QDir(data[FILENAME].toString());
        qDebug() << QString("In MainWindow::loadData - Current image index is %1 => loading %2").arg(image_idx).arg(path.path()).toStdString().c_str();
    }
    else
    {
        data = m_data->at(image_idx).toObject();
        fs::path filename{data[FILENAME].toString().toStdString()};
        path = QDir(BASE_DIR / filename);
    }

    setCursor(QCursor(Qt::WaitCursor));

    m_container->m_image_widget->setImage(path);
    m_container->m_annotation_widget->setData(data[ANNOTATIONS].toArray());

    deleteImageCheckBox->setChecked(data[DELETE].toBool());

    if (dataSize() > 0)
    {
        previousImageAction->setEnabled(image_idx > 0);
        nextImageAction->setEnabled(image_idx < dataSize() - 1);
        emit imageChanged(m_data->at(image_idx).toObject()[FILENAME].toString() + " - " +
                          QString("[%1x%2]")
                              .arg(m_container->m_image_widget->getImageSize().width())
                              .arg(m_container->m_image_widget->getImageSize().height()));
    }
    m_container->m_annotation_widget->setEnabled(enableAnnotationWidget);

    setCursor(QCursor(Qt::ArrowCursor));
}

void MainWindow::nextImage()
{
    saveAnnotatationsForImage(current_image_idx);

    if (current_image_idx < m_data->size() - 1)
    {
        current_image_idx += 1;
        loadData(current_image_idx);
    }
}

void MainWindow::previousImage()
{
    saveAnnotatationsForImage(current_image_idx);

    if (current_image_idx > 0)
    {
        current_image_idx -= 1;
        loadData(current_image_idx);
    }
}

void MainWindow::zoomIn()
{
    m_container->m_image_widget->zoomIn();
}

void MainWindow::zoomOut()
{
    m_container->m_image_widget->zoomOut();
}

void MainWindow::resetScaling()
{
    m_container->m_image_widget->resetScaling();
}

void MainWindow::saveAnnotatationsForImage(qsizetype image_idx)
{
    if (image_idx < 0 || image_idx >= dataSize())
    {
        qDebug() << QString("In MainWindow::saveAnnotatationsForImage - image_idx must be in range [%1, %2], received image_idx = %3").arg(0).arg(dataSize() - 1).arg(image_idx).toStdString().c_str();
        return;
    }

    QJsonObject datapoint = m_data->at(image_idx).toObject();
    datapoint[ANNOTATIONS] = *(m_container->m_annotation_widget->data());
    (*m_data)[image_idx] = datapoint;
}

void MainWindow::setSaveStatus(bool enabled)
{
    saveJsonAction->setEnabled(enabled);
}

void MainWindow::updateImageDeletingStatus(int checkState)
{
    QJsonObject datapoint = m_data->at(current_image_idx).toObject();
    datapoint[DELETE] = checkState == Qt::Checked ? true : false;
    (*m_data)[current_image_idx] = datapoint;
}

void MainWindow::fitToWindow()
{
    m_container->m_image_widget->fitToContainer();
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

    // create actions for Edit Menu
    // cutAction = new QAction(QIcon(":/icons/cut.png"), "Cut", this);
    // cutAction->setShortcut(QKeySequence::Cut);
    // copyAction = new QAction(QIcon(":/icons/copy.png"), "Copy", this);
    // copyAction->setShortcut(QKeySequence::Copy);
    // pasteAction = new QAction(QIcon(":/icons/paste.png"), "Paste", this);
    // pasteAction->setShortcut(QKeySequence::Paste);
    // undoAction = new QAction(QIcon(":/icons/undo.png"), "Undo", this);
    // undoAction->setShortcut(QKeySequence::Undo);
    // redoAction = new QAction(QIcon(":/icons/redo.png"), "Redo", this);
    // redoAction->setShortcut(QKeySequence::Redo);

    nextImageAction = new QAction(QIcon(":/media/icons/next-image.png"), "Next Image", this);
    previousImageAction = new QAction(QIcon(":/media/icons/previous-image.png"), "Previous Image", this);

    // deleteImageAction = new QAction(QIcon(":/media/icons/delete-image.png"), "Mark as delete", this);
    // deleteImageAction->setCheckable(true);
    deleteImageCheckBox = new QCheckBox(this);
    deleteImageCheckBox->setText("Mark as delete");
    deleteImageCheckBox->setChecked(false);
    deleteImageCheckBox->setEnabled(false);
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
    // editMenu->addAction(cutAction);
    // editMenu->addAction(copyAction);
    // editMenu->addAction(pasteAction);
    // editMenu->addSeparator();
    // editMenu->addAction(undoAction);
    // editMenu->addAction(redoAction);
    // editMenu->addSeparator();
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
    // toolbar->addAction(cutAction);
    // toolbar->addAction(copyAction);
    // toolbar->addAction(pasteAction);
    // toolbar->addAction(undoAction);
    // toolbar->addAction(redoAction);
    // toolbar->addAction(deleteImageAction);
    toolbar->addWidget(deleteImageCheckBox);
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
    QObject::connect(m_container->m_annotation_widget, &AnnotationWidget::haveAdjusted, this, &MainWindow::setActiveSaveStatus);
    QObject::connect(m_container->m_annotation_widget, &AnnotationWidget::haveAdjusted, this, &MainWindow::checkAnnotationBox);

    QObject::connect(rotateLeftAction, &QAction::triggered, m_container->m_image_widget, &ImageWidget::rotateLeft);
    QObject::connect(rotateRightAction, &QAction::triggered, m_container->m_image_widget, &ImageWidget::rotateRight);
    QObject::connect(zoomInAction, &QAction::triggered, this, &MainWindow::zoomIn);
    QObject::connect(zoomOutAction, &QAction::triggered, this, &MainWindow::zoomOut);
    QObject::connect(fitToWindowAction, &QAction::triggered, this, &MainWindow::fitToWindow);

    QObject::connect(nextImageAction, &QAction::triggered, this, &MainWindow::nextImage);
    QObject::connect(m_container->m_image_widget, &ImageWidget::nextImage, this, &MainWindow::nextImage);
    QObject::connect(m_container->m_annotation_widget, &AnnotationWidget::nextImage, this, &MainWindow::nextImage);
    QObject::connect(nextImageAction, &QAction::triggered, this, &MainWindow::resetScaling);
    QObject::connect(previousImageAction, &QAction::triggered, this, &MainWindow::previousImage);
    QObject::connect(m_container->m_image_widget, &ImageWidget::backImage, this, &MainWindow::previousImage);
    QObject::connect(m_container->m_annotation_widget, &AnnotationWidget::backImage, this, &MainWindow::previousImage);
    QObject::connect(previousImageAction, &QAction::triggered, this, &MainWindow::resetScaling);

    QObject::connect(deleteImageCheckBox, &QCheckBox::stateChanged, this, &MainWindow::onDeleteImageStatusChanged);

    QObject::connect(quitAction, &QAction::triggered, this, &MainWindow::onQuitAction);
}

void MainWindow::enableImageFeatures(bool enabled)
{
    previousImageAction->setEnabled(current_image_idx > 0);
    nextImageAction->setEnabled(current_image_idx < dataSize() - 1);
    rotateLeftAction->setEnabled(enabled);
    rotateRightAction->setEnabled(enabled);
    zoomInAction->setEnabled(enabled);
    zoomOutAction->setEnabled(enabled);
    fitToWindowAction->setEnabled(enabled);
}

void MainWindow::onDeleteImageStatusChanged(qint16 state)
{
    qint16 current_state = m_data->at(current_image_idx)[DELETE].toBool();
    bool new_state = state == Qt::Checked ? true : false;
    if (new_state != current_state)
    {
        updateImageDeletingStatus(state);
        setActiveSaveStatus();
    }
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

void MainWindow::checkAnnotationBox()
{
    if (m_container->m_annotation_widget->isEmpty())
        deleteImageCheckBox->setCheckState(Qt::Checked);
    else
        deleteImageCheckBox->setCheckState(Qt::Unchecked);
}
