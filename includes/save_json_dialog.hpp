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
    Q_OBJECT
public:
    SaveJsonDialog(QWidget* parent=nullptr);

    void setSelectedFile(QString const& filename);
    QString selectedFile();

    bool isFirstOpen;

    ~SaveJsonDialog();

public slots:
    int openDialog(QList<QString> const& open_history);
    void toggleFileSelected(bool);

private:
    QString m_selected_file;

    void initializeDialog();
};

#endif // SAVEJSONDIALOG_HPP