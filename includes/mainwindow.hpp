#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QAction>
#include <QToolBar>
#include <QJsonObject>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QCloseEvent>
#include <QDir>
#include <QKeyEvent>
#include <QShortcut>
#include <QList>

#include "annotation_widget.hpp"
#include "open_folder_dialog.hpp"
#include "save_json_dialog.hpp"
#include "save_notification_dialog.hpp"
#include "image_container.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    qsizetype dataSize();

    // shorcuts
    QShortcut* nextImageShortcut = nullptr;
    QShortcut* previousImageShortcut = nullptr;

    void closeEvent(QCloseEvent* event) override;

    ~MainWindow();

signals:
    void createdNovelFile(bool);
    void saveStatusEnabledChanged(bool);
    void imageChanged(QString image_name);

public slots:
    // folder interaction slots
    void openFolder();
    void saveJsonFile();
    void loadData(qint16 sample_idx);
    // image slots
    void nextImage();
    void previousImage();
    void fitToWindow();
    void zoomIn();
    void zoomOut();
    void resetScaling();
    
    void changeWindowTitle(QString title);

    // for saving annotations
    void saveAnnotatationsForSample(qsizetype sample_idx);
    void setSaveStatus(bool);
    void setActiveSaveStatus();

    void onQuitAction();

private:
    // File Menu actions
    QAction* openFolderAction = nullptr;
    QAction* saveJsonAction = nullptr;
    QAction* quitAction = nullptr;
    qsizetype current_sample_idx = 0;

    // Edit Menu actions
    // QAction* cutAction;
    // QAction* copyAction;
    // QAction* pasteAction;
    // QAction* undoAction;
    // QAction* redoAction;
    QAction* rotateLeftAction = nullptr;
    QAction* rotateRightAction = nullptr;
    QAction* zoomInAction = nullptr;
    QAction* zoomOutAction = nullptr;
    QAction* fitToWindowAction = nullptr;

    // image direction actions
    QAction* nextImageAction = nullptr;
    QAction* previousImageAction = nullptr;

    // Help Menu actions
    QAction* aboutAction = nullptr;

    // open image dialog
    OpenFolderDialog* open_folder_dialog = nullptr;

    // save json dialog
    SaveJsonDialog* save_json_dialog = nullptr;

    // saving notification dialog
    SaveNotificationDialog* m_save_notification_dialog = nullptr;

    // main container
    QWidget* centralWidget = nullptr;
    ImageContainer* m_imageContainer = nullptr;
    AnnotationWidget* m_annotationWidget = nullptr;
    QVBoxLayout* m_layout = nullptr;

    // initializing methods
    void createActions();

    void createFileMenu();
    void createEditMenu();
    void createHelpMenu();

    void createToolbar();

    void createConnections();

    void enableImageFeatures(bool enabled);

    void loadJson(QString const& folder);
    void saveJson(QString const& filename);

    qsizetype findFirstEmptyAnnotation();

    // internal data
    QList<QJsonObject> m_data;
    QJsonObject m_default_data;
    QDir m_directory;

protected:
    void keyPressEvent(QKeyEvent* event) override;
};

#endif // MAINWINDOW_HPP
