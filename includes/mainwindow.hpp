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

#include <memory>

#include "open_folder_dialog.hpp"
#include "save_json_dialog.hpp"
#include "save_notification_dialog.hpp"
#include "container.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    Container* container();

    qsizetype dataSize();

    void closeEvent(QCloseEvent* event);

    ~MainWindow();

signals:
    void createdNovelFile(bool);
    void saveStatusEnabledChanged(bool);
    void imageChanged(QString image_name);

public slots:
    // folder interaction slots
    void openFolder();
    void saveJsonFile();
    void loadData(qint16 image_idx);
    // image slots
    void nextImage();
    void previousImage();
    void fitToWindow();
    void zoomIn();
    void zoomOut();
    void resetScaling();
    
    void changeWindowTitle(QString title);

    // for saving annotations
    void updateImageDeletingStatus(int checkState);
    void saveAnnotatationsForImage(qsizetype image_idx);
    void setSaveStatus(bool);
    void setActiveSaveStatus();
    void checkAnnotationBox();
    void onDeleteImageStatusChanged(qint16 state);

    void onQuitAction();

private:
    // File Menu actions
    QAction* openFolderAction = nullptr;
    QAction* saveJsonAction = nullptr;
    QAction* quitAction = nullptr;
    qsizetype current_image_idx = 0;

    // Edit Menu actions
    // QAction* cutAction;
    // QAction* copyAction;
    // QAction* pasteAction;
    // QAction* undoAction;
    // QAction* redoAction;
    // QAction* deleteImageAction = nullptr;
    QCheckBox* deleteImageCheckBox = nullptr;
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
    Container* m_container = nullptr;

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
    std::shared_ptr<QJsonArray> m_data = nullptr;
    QJsonObject m_default_data;
    QDir m_directory;
    qint16 m_tmp_check_state;

protected:
    void keyPressEvent(QKeyEvent* event) override;
};

#endif // MAINWINDOW_HPP