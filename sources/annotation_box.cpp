#include "annotation_box.hpp"
#include "annotation_widget.hpp"

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QJsonObject>
#include <QPair>
#include <QString>
#include <QPushButton>
#include <QIcon>

#include <memory>

AnnotationBox::AnnotationBox(qsizetype ith, QWidget* container, QWidget *parent)
    : QGroupBox{parent}
{
    m_questionGroup = new QGroupBox();
    m_questionGroup->setTitle("Question: ");
    m_questionLineEdit = new QLineEdit();
    m_questionLayout = new QVBoxLayout(m_questionGroup);
    m_questionLayout->addWidget(m_questionLineEdit);

    m_add_button = new QPushButton();
    m_add_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_add_button->setIcon(QIcon(":/media/icons/add.png"));
    m_del_button = new QPushButton();
    m_del_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_del_button->setIcon(QIcon(":/media/icons/delete.png"));

    m_answerGroup = new QGroupBox();
    m_answerGroup->setTitle("Answer: ");
    m_answerLineEdit = new QLineEdit();
    m_answerLayout = new QVBoxLayout(m_answerGroup);
    m_answerLayout->addWidget(m_answerLineEdit);

    m_selectionGroup = new QGroupBox();
    m_selectionGroup->setTitle("Question's type");
    m_selection_box = new QComboBox(m_selectionGroup);
    m_selection_box->insertItems(0, QList<QString>({
        "0 - text",
        "1 - state",
        "2 - behaviour"
    }));
    m_selection_box->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    m_selectionLayout = new QVBoxLayout(m_selectionGroup);
    m_selectionLayout->addWidget(m_selection_box);

    m_button_layout = new QHBoxLayout();
    m_button_layout->addWidget(m_add_button);
    m_button_layout->addWidget(m_del_button);
    m_button_layout->setAlignment(Qt::AlignRight);

    m_qa_layout = new QVBoxLayout();
    m_qa_layout->addWidget(m_questionGroup);
    m_qa_layout->addWidget(m_answerGroup);
    m_qa_layout->addWidget(m_selectionGroup);

    m_layout = new QVBoxLayout(this);
    m_layout->addLayout(m_qa_layout);
    m_layout->addLayout(m_button_layout);

    QObject::connect(m_add_button, &QPushButton::clicked, [container, ith]() {
        static_cast<AnnotationWidget*>(container)->addAnnotation(ith);
    });
    QObject::connect(m_del_button, &QPushButton::clicked, [container, ith]() {
        static_cast<AnnotationWidget*>(container)->deleteAnnotation(ith);
    });
}

std::shared_ptr<QJsonObject> AnnotationBox::annotation() const
{
    return std::make_shared<QJsonObject>(std::initializer_list<QPair<QString, QJsonValue>>{
        QPair<QString, QJsonValue>("question", m_questionLineEdit->text()),
        QPair<QString, QJsonValue>("answer", m_answerLineEdit->text()),
        QPair<QString, QJsonValue>("type", m_selection_box->currentIndex())
    });
}

void AnnotationBox::setAnnotation(QJsonObject const& data) const
{
    m_questionLineEdit->setText(data["question"].toString());
    m_answerLineEdit->setText(data["answer"].toString());
    m_selection_box->setCurrentIndex(data["type"].toInt());
}

AnnotationBox::~AnnotationBox()
{

}