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

private:
    QList<QString> m_history;

    void initializeDialog();
};

#endif // SAVEJSONDIALOG_HPP