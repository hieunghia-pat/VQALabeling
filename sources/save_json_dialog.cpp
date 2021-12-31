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
}

QDialog::DialogCode SaveJsonDialog::openDialog()
{
    setDirectory(m_history.constLast());

    return static_cast<QDialog::DialogCode>(exec());
}

SaveJsonDialog::~SaveJsonDialog()
{

}
