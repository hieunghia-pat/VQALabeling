#ifndef OPEN_FOLDER_DIALOG_HPP
#define OPEN_FOLDER_DIALOG_HPP

#include <QDialog>
#include <QFileDialog>
#include <QList>
#include <QString>

class OpenFolderDialog : public QFileDialog
{
    Q_OBJECT
public:
    explicit OpenFolderDialog(QWidget *parent = nullptr);

    ~OpenFolderDialog();

public slots:
    QDialog::DialogCode openDialog();

private:
    QList<QString> m_history;
    void initializeDialog();
};

#endif // OPEN_FOLDER_DIALOG_HPP