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

    ~SaveJsonDialog();

public slots:
    QDialog::DialogCode openDialog();
    void toggleFileSelected(bool);

private:
    QList<QString> m_history;
    bool m_is_novel_selected_file;
    QString m_selected_file;

    void initializeDialog();
};

#endif // SAVEJSONDIALOG_HPP