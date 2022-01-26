#include "save_notification_dialog.hpp"

#include <QDialog>
#include <QLabel>
#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPixmap>
#include <QPushButton>
#include <QPixmap>

SaveNotificationDialog::SaveNotificationDialog(QWidget* parent)
    : QDialog(parent)
{
    m_label = new QLabel();
    m_label->setText("Save before closing?");
    m_icon = new QLabel();
    m_icon->setPixmap(QString(":/media/images/warning.png"));
    m_icon->setScaledContents(true);
    m_icon->resize(m_label->width(), m_label->width());

    m_accept_button = new QPushButton();
    m_accept_button->setText("Save");
    m_cancel_button = new QPushButton();
    m_cancel_button->setText("Cancel");
    
    m_content_layout = new QGridLayout();
    m_content_layout->addWidget(m_icon, 0, 0, 1, 1);
    m_content_layout->addWidget(m_label, 0, 1, 2, 13);
    m_content_layout->setAlignment(Qt::AlignCenter);

    m_button_layout = new QHBoxLayout();
    m_button_layout->addWidget(m_accept_button);
    m_button_layout->addWidget(m_cancel_button);
    m_button_layout->setAlignment(Qt::AlignRight);

    m_layout = new QVBoxLayout();
    m_layout->addLayout(m_content_layout);
    m_layout->addLayout(m_button_layout);

    setLayout(m_layout);
    
    setFixedSize(512, 256);

    createConnections();
}

void SaveNotificationDialog::onSaveClick()
{
    accept();
}

void SaveNotificationDialog::onCancelClick()
{
    reject();
}

void SaveNotificationDialog::createConnections()
{
    QObject::connect(m_accept_button, &QPushButton::clicked, this, &SaveNotificationDialog::onSaveClick);
    QObject::connect(m_cancel_button, &QPushButton::clicked, this, &SaveNotificationDialog::onCancelClick);
}

SaveNotificationDialog::~SaveNotificationDialog()
{

}