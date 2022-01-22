#ifndef SAVEJSONDIALOG_HPP
#define SAVEJSONDIALOG_HPP

#include <QDialog>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QList>

class SaveJsonDialog: public QFileDialog
{
public:
    SaveJsonDialog(QWidget* parent=nullptr);

    ~SaveJsonDialog();

public slots:
    QDialog::DialogCode openDialog();
    void toggleFileSelected(bool);

private:
    QList<QString> m_history;
    bool m_is_novel_selected_file;

    void initializeDialog();
};

#endif // SAVEJSONDIALOG_HPP