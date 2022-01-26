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

public: // attributes
    QList<QString> m_history;

public slots:
    int openDialog();

private:
    void initializeDialog();
};

#endif // OPEN_FOLDER_DIALOG_HPP