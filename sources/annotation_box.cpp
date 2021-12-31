#include "annotation_box.hpp"

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QJsonObject>
#include <QPair>
#include <QString>

#include <memory>

AnnotationBox::AnnotationBox(QWidget *parent)
    : QGroupBox{parent}
{
    m_questionGroup = new QGroupBox(this);
    m_questionGroup->setTitle("Question: ");
    m_questionLineEdit = new QLineEdit(this);
    m_questionLayout = new QVBoxLayout(m_questionGroup);
    m_questionLayout->addWidget(m_questionLineEdit);

    m_answerGroup = new QGroupBox(this);
    m_answerGroup->setTitle("Answer: ");
    m_answerLineEdit = new QLineEdit(this);
    m_answerLayout = new QVBoxLayout(m_answerGroup);
    m_answerLayout->addWidget(m_answerLineEdit);

    m_layout = new QVBoxLayout(this);
    m_layout->addWidget(m_questionGroup);
    m_layout->addWidget(m_answerGroup);
}

std::shared_ptr<QJsonObject> AnnotationBox::annotation() const
{
    return std::make_shared<QJsonObject>(std::initializer_list<QPair<QString, QJsonValue>>{
        QPair<QString, QJsonValue>("question", m_questionLineEdit->text()),
        QPair<QString, QJsonValue>("answer", m_answerLineEdit->text())
    });
}

void AnnotationBox::setAnnotation(QJsonObject const& data) const
{
    m_questionLineEdit->setText(data["question"].toString());
    m_answerLineEdit->setText(data["answer"].toString());
}

AnnotationBox::~AnnotationBox()
{

}