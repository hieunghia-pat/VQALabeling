#include "save_json_dialog.hpp"

#include <QFileDialog>
#include <QDir>
#include <QStandardPaths>
#include <QStringList>
#include <QByteArray>
#include <QFile>

SaveJsonDialog::SaveJsonDialog(QWidget* parent)
    :QFileDialog {parent}
{
    QList<QString> locations = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    QString default_location = locations.isEmpty() ? QDir::currentPath() : locations.constLast();
    m_history.append(default_location);

    initializeDialog();
}

void SaveJsonDialog::initializeDialog()
{
    setWindowTitle("Save Json File");
    setAcceptMode(QFileDialog::AcceptSave);
    setMimeTypeFilters({ "application/json" });
    setDefaultSuffix("json");
    selectMimeTypeFilter("application/json");
    m_is_novel_selected_file = false;
}

void SaveJsonDialog::toggleFileSelected(bool novelSelectedFile)
{
    qDebug() << novelSelectedFile;
    m_is_novel_selected_file = novelSelectedFile;
}

QDialog::DialogCode SaveJsonDialog::openDialog()
{
    if (m_is_novel_selected_file)
    {
        setDirectory(m_history.constLast());
        m_is_novel_selected_file = false;
        qDebug() << "Here";
        return static_cast<QDialog::DialogCode>(exec());
    }

    return QDialog::Accepted;
}

SaveJsonDialog::~SaveJsonDialog()
{

}
