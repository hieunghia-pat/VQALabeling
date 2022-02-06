#include "mainwindow.hpp"
#include "annotation_widget.hpp"
#include "open_folder_dialog.hpp"
#include "save_json_dialog.hpp"
#include "save_notification_dialog.hpp"
#include "container.hpp"

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

#include <filesystem>

namespace fs = std::filesystem;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(1024, 800);
    createActions();

    createFileMenu();
    createEditMenu();
    createHelpMenu();

    createToolbar();

    open_folder_dialog = new OpenFolderDialog(this);
    save_json_dialog = new SaveJsonDialog(this);
    m_save_notification_dialog = new SaveNotificationDialog(this);

    m_container = new Container(this);
    setCentralWidget(m_container);

    createConnections();
}

Container *MainWindow::container()
{
    return m_container;
}

MainWindow::~MainWindow()
{

}

void MainWindow::openFolder()
{
    int dialog_returned = open_folder_dialog->openDialog();
    if (dialog_returned == QDialog::Accepted)
    {
        loadJson(open_folder_dialog->selectedFiles().constLast());
        loadData(current_image_idx);
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
            return;
        save_json_dialog->setSelectedFile(save_json_dialog->selectedFiles().constLast());
    }

    saveAnnotatationsForImage(current_image_idx);
    saveJson(save_json_dialog->selectedFile());
    emit saveStatusEnabledChanged(false); // have saved changed things
}

void MainWindow::loadJson(QString const& folder)
{
    m_directory = QDir(folder);
    QList<QString> json_filter;
    json_filter << "*.json";    
    QList<QFileInfo> json_files = m_directory.entryInfoList(json_filter);
    
    if (json_files.size() == 0) // no annotation file
    {
        emit createdNovelFile(true);
        QList<QString> image_filters;
        image_filters << "*.jpeg" << "*.png" << "*.jpg";

        QList<QFileInfo> image_files = m_directory.entryInfoList(image_filters);

        m_data = std::make_shared<QJsonArray>();
        for (QFileInfo const& image_file: image_files)
        {
            QJsonObject object;
            object["filename"] = image_file.fileName();
            object["filepath"] = image_file.dir().dirName();
            object["delete"] = false;
            QJsonArray annotations;
            // create annotations
            for (qsizetype ith = 0; ith < total_initial_annotations; ith++)
            {
                QJsonObject annotation {{
                    QPair<QString, QString>("question", ""),
                    QPair<QString, QString>("answer", "")
                }};
                annotations.append(annotation);
            }
            object["annotations"] = annotations;
            m_data->append(object);
        }
    }
    else // load json data
    {
        emit createdNovelFile(false);
        save_json_dialog->toggleFileSelected(false);
        QFile file(json_files.constLast().absoluteFilePath());
        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::warning(this, "Open workspace error!", QString("Error while opening %1").arg(json_files.constLast().path()));
        }

        save_json_dialog->setSelectedFile(json_files.constLast().absoluteFilePath());

        QTextStream filestream(&file);
        QByteArray content = filestream.readAll().toLocal8Bit();
        m_data = std::make_shared<QJsonArray>(QJsonDocument::fromJson(content).array());
        file.close();
    }

    deleteImageCheckBox->setEnabled(true);
}

void MainWindow::saveJson(QString const& filename)
{
    QByteArray json_data = QJsonDocument(*m_data).toJson(QJsonDocument::Indented);
    QFile json_file(filename);

    json_file.open(QIODevice::WriteOnly);
    json_file.write(json_data);
    json_file.close();
}

void MainWindow::loadData(qsizetype image_idx)
{
    fs::path BASE_DIR(m_directory.path().toStdString());
    QJsonObject data = m_data->at(image_idx).toObject();
    fs::path filename { data["filename"].toString().toStdString() };
    QDir path = QDir(BASE_DIR / filename);
    m_container->m_image_widget->setImage(path);
    m_container->m_annotation_widget->setData(data["annotations"].toArray());
    deleteImageCheckBox->setChecked(data["delete"].toBool());
}

void MainWindow::nextImage()
{
    saveAnnotatationsForImage(current_image_idx);

    if (current_image_idx < m_data->size()-1)
    {
        current_image_idx += 1;
        loadData(current_image_idx);
    }

    previousImageAction->setEnabled(current_image_idx != 0);
    nextImageAction->setEnabled(current_image_idx != m_data->size()-1);
}

void MainWindow::previousImage()
{
    saveAnnotatationsForImage(current_image_idx);

    if (current_image_idx > 0)
    {
        current_image_idx -= 1;
        loadData(current_image_idx);
    }
    
    previousImageAction->setEnabled(current_image_idx != 0);
    nextImageAction->setEnabled(current_image_idx != m_data->size()-1);
}

void MainWindow::zoomIn()
{
    m_container->m_image_widget->zoomIn();

    zoomOutAction->setEnabled(m_container->m_image_widget->scaleFactor() >= 0.5);
    zoomInAction->setEnabled(m_container->m_image_widget->scaleFactor() <= 3.);
}

void MainWindow::zoomOut()
{
    m_container->m_image_widget->zoomOut();

    zoomOutAction->setEnabled(m_container->m_image_widget->scaleFactor() >= 0.5);
    zoomInAction->setEnabled(m_container->m_image_widget->scaleFactor() <= 3.);
}

void MainWindow::resetScaling()
{
    m_container->m_image_widget->resetScaling();
}

void MainWindow::saveAnnotatationsForImage(qsizetype image_idx)
{
    QJsonObject datapoint = m_data->at(image_idx).toObject();
    datapoint["annotations"] = *(m_container->m_annotation_widget->data());
    (*m_data)[image_idx] = datapoint;
}

void MainWindow::setSaveStatus(bool enabled)
{
    saveJsonAction->setEnabled(enabled);
}

void MainWindow::updateImageDeletingStatus(int checkState)
{
    QJsonObject datapoint = m_data->at(current_image_idx).toObject();
    datapoint["delete"] = checkState == Qt::Checked ? true : false;
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
    QMenu* fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(openFolderAction);
    fileMenu->addAction(saveJsonAction);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);
}

void MainWindow::createEditMenu()
{
    QMenu* editMenu = menuBar()->addMenu("&Edit");
    // editMenu->addAction(cutAction);
    // editMenu->addAction(copyAction);
    // editMenu->addAction(pasteAction);
    // editMenu->addSeparator();
    // editMenu->addAction(undoAction);
    // editMenu->addAction(redoAction);
    // editMenu->addSeparator();
    editMenu->addAction(zoomInAction);
    editMenu->addAction(zoomOutAction);
    editMenu->addAction(fitToWindowAction);
}

void MainWindow::createHelpMenu()
{
    QMenu* helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction(aboutAction);
}

void MainWindow::createToolbar()
{
    QToolBar* toolbar = this->addToolBar("main_toolbar");
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
    toolbar->addAction(zoomInAction);
    toolbar->addAction(zoomOutAction);
    toolbar->addAction(fitToWindowAction);
    toolbar->addSeparator();
    toolbar->addAction(quitAction);

    toolbar->setMovable(false);
}

void MainWindow::createConnections()
{
    // create connections for actions
    QObject::connect(openFolderAction, &QAction::triggered, this, &MainWindow::openFolder);
    QObject::connect(saveJsonAction, &QAction::triggered, this, &MainWindow::saveJsonFile);
    QObject::connect(this, &MainWindow::createdNovelFile, save_json_dialog, &SaveJsonDialog::toggleFileSelected);
    QObject::connect(this, &MainWindow::saveStatusEnabledChanged, this, &MainWindow::setSaveStatus);
    QObject::connect(m_container->m_annotation_widget, &AnnotationWidget::haveAdjusted, this, &MainWindow::setActiveSaveStatus);

    QObject::connect(zoomInAction, &QAction::triggered, this, &MainWindow::zoomIn);
    QObject::connect(zoomOutAction, &QAction::triggered, this, &MainWindow::zoomOut);
    QObject::connect(fitToWindowAction, &QAction::triggered, this, &MainWindow::fitToWindow);

    QObject::connect(nextImageAction, &QAction::triggered, this, &MainWindow::nextImage);
    QObject::connect(nextImageAction, &QAction::triggered, this, &MainWindow::resetScaling);
    QObject::connect(previousImageAction, &QAction::triggered, this, &MainWindow::previousImage);
    QObject::connect(previousImageAction, &QAction::triggered, this, &MainWindow::resetScaling);
    
    QObject::connect(deleteImageCheckBox, &QCheckBox::stateChanged, this, &MainWindow::updateImageDeletingStatus);
    QObject::connect(deleteImageCheckBox, &QCheckBox::stateChanged, this, &MainWindow::setActiveSaveStatus);

    QObject::connect(quitAction, &QAction::triggered, this, &MainWindow::onQuitAction);
}

void MainWindow::enableImageFeatures(bool enabled)
{
    nextImageAction->setEnabled(enabled);
    previousImageAction->setEnabled(enabled);
    zoomInAction->setEnabled(enabled);
    zoomOutAction->setEnabled(enabled);
    fitToWindowAction->setEnabled(enabled);
}

void MainWindow::setActiveSaveStatus()
{
    emit saveStatusEnabledChanged(true);
}

void MainWindow::closeEvent(QCloseEvent* /* event */)
{
    onQuitAction();
}