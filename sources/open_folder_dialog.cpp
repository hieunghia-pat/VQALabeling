#include "open_folder_dialog.hpp"

#include <QDialog>
#include <QFileDialog>
#include <QStandardPaths>
#include <QString>
#include <QList>
#include <QStringList>
#include <QImageReader>
#include <QByteArray>

OpenFolderDialog::OpenFolderDialog(QWidget *parent)
    : QFileDialog{parent}
{
    QList<QString> picture_locations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    QString default_open_location = picture_locations.isEmpty() ? QDir::currentPath() : picture_locations.constLast();
    m_history << default_open_location;
    initializeDialog();
}

void OpenFolderDialog::initializeDialog()
{
    setWindowTitle("Open folder");
    setFileMode(QFileDialog::Directory);
    setAcceptMode(QFileDialog::AcceptOpen);
}

int OpenFolderDialog::openDialog()
{
    setDirectory(m_history.constLast());
    int returned_code = exec();
    m_history << selectedFiles().constLast();

    return returned_code;
}

OpenFolderDialog::~OpenFolderDialog()
{

}
