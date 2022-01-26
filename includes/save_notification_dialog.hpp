#ifndef SAVE_NOTIFICATION_DIALOG
#define SAVE_NOTIFICATION_DIALOG

#include <QDialog>
#include <QLabel>
#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>

class SaveNotificationDialog: public QDialog
{
    Q_OBJECT
public:
    SaveNotificationDialog(QWidget* parent = nullptr);
    ~SaveNotificationDialog();

private slots:
    void onSaveClick();
    void onCancelClick();

private:
    QLabel* m_label = nullptr;
    QLabel* m_icon = nullptr;

    QPushButton* m_accept_button = nullptr;
    QPushButton* m_cancel_button = nullptr;

    QGridLayout* m_content_layout = nullptr;
    QHBoxLayout* m_button_layout = nullptr;
    QVBoxLayout* m_layout = nullptr;

    void createConnections();
};


#endif // SAVE_NOTIFICATION_DIALOG