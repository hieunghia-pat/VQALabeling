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
    initializeDialog();
}

void SaveJsonDialog::initializeDialog()
{
    setWindowTitle("Save Json File");
    setAcceptMode(QFileDialog::AcceptSave);
    setMimeTypeFilters({ "application/json" });
    setDefaultSuffix("json");
    selectMimeTypeFilter("application/json");
    m_is_novel_selected_file = true;
    m_selected_file = "annotation.json";
}

void SaveJsonDialog::toggleFileSelected(bool novelSelectedFile)
{
    m_is_novel_selected_file = novelSelectedFile;
}

int SaveJsonDialog::openDialog(QList<QString> const& open_history)
{
    if (m_is_novel_selected_file)
    {
        setDirectory(open_history.constLast());
        int returned_code = static_cast<QDialog::DialogCode>(exec());
        m_is_novel_selected_file = returned_code == QDialog::Accepted ? false : true;
        return returned_code;
    }

    return QDialog::Accepted;
}

void SaveJsonDialog::setSelectedFile(QString const& filename)
{
    m_selected_file = filename;
}

QString SaveJsonDialog::selectedFile()
{
    return m_selected_file;
}

SaveJsonDialog::~SaveJsonDialog()
{

}
