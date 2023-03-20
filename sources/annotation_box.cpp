#include "annotation_box.hpp"
#include "annotation_widget.hpp"
#include "selection_box.hpp"
#include "constants.hpp"

#include <QLabel>
#include <QString>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QJsonObject>
#include <QPair>
#include <QString>
#include <QPushButton>
#include <QIcon>
#include <QFont>

#include <memory>

AnnotationBox::AnnotationBox(qsizetype ith, QWidget* container, QWidget *parent)
    : QGroupBox{parent}
{
    QFont* font = new QFont();
    font->setPointSize(17);

    QFont* boldFont = new QFont();
    boldFont->setPointSize(17);
    boldFont->setWeight(QFont::Black);

    m_index = ith;
    setTitle(QString("Annotation %1: ").arg(m_index+1));
    setFont(*font);

    // Question-answer line editor
    m_questionGroup = new QGroupBox();
    m_questionGroup->setTitle("Question: ");
    
    m_questionLineEdit = new QLineEdit();
    m_current_annotation[QUESTION] = m_questionLineEdit->text();

    m_questionLayout = new QVBoxLayout(m_questionGroup);
    m_questionLayout->addWidget(m_questionLineEdit);

    m_answerGroup = new QGroupBox();
    m_answerGroup->setTitle("Answer: ");
    
    m_answerLineEdit = new QLineEdit();
    m_current_annotation[ANSWER] = m_answerLineEdit->text();
    
    m_answerLayout = new QVBoxLayout(m_answerGroup);
    m_answerLayout->addWidget(m_answerLineEdit);

    m_qa_layout = new QVBoxLayout();
    m_qa_layout->addWidget(m_questionGroup);
    m_qa_layout->addWidget(m_answerGroup);

    // manipulation button
    m_add_button = new QPushButton();
    m_add_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_add_button->setIcon(QIcon(":/media/icons/add.png"));
    m_del_button = new QPushButton();
    m_del_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_del_button->setIcon(QIcon(":/media/icons/delete.png"));

    m_button_layout = new QHBoxLayout();
    m_button_layout->addWidget(m_add_button);
    m_button_layout->addWidget(m_del_button);
    m_button_layout->setAlignment(Qt::AlignRight);

    // main layout
    m_layout = new QVBoxLayout(this);
    m_layout->addLayout(m_qa_layout);
    m_layout->addLayout(m_button_layout);

    QObject::connect(m_add_button, &QPushButton::clicked, [container, this]() {
        static_cast<AnnotationWidget*>(container)->addAnnotation(this->m_index);
    });
    QObject::connect(m_del_button, &QPushButton::clicked, [container, this]() {
        static_cast<AnnotationWidget*>(container)->deleteAnnotation(this->m_index);
    });
    QObject::connect(m_questionLineEdit, &QLineEdit::textChanged, this, &AnnotationBox::handleQuestionChanged);
    QObject::connect(m_answerLineEdit, &QLineEdit::textChanged, this, &AnnotationBox::handleAnswerChanged);
}

std::shared_ptr<QJsonObject> AnnotationBox::annotation()
{
    return std::make_shared<QJsonObject>(std::initializer_list<QPair<QString, QJsonValue>>{
        QPair<QString, QJsonValue>(QUESTION, m_questionLineEdit->text()),
        QPair<QString, QJsonValue>(ANSWER, m_answerLineEdit->text())
    });
}

void AnnotationBox::setAnnotation(QJsonObject const& annotation)
{
    m_current_annotation = annotation;

    QString question = annotation[QUESTION].toString();
    m_questionLineEdit->setText(question);

    QString answer = annotation[ANSWER].toString();
    m_answerLineEdit->setText(answer);
}

qint16 AnnotationBox::index()
{
    return m_index;
}

void AnnotationBox::setIndex(qint16 index)
{
    m_index = index;
    setTitle(QString("Annotation %1").arg(m_index+1));
}

void AnnotationBox::handleQuestionChanged(QString const& question)
{
    QString current_question = m_current_annotation[QUESTION].toString();

    if (question != current_question)
    {
        m_current_annotation[QUESTION] = question;
        emit contentChanged();
    }
}

void AnnotationBox::handleAnswerChanged(QString const& answer)
{
    QString current_answer = m_current_annotation[ANSWER].toString();

    if (answer != current_answer)
    {
        m_current_annotation[ANSWER] = answer;
        emit contentChanged();
    }
}

bool AnnotationBox::isEmpty()
{
    return (m_questionLineEdit->text().isEmpty() && m_answerLineEdit->text().isEmpty());
}

AnnotationBox::~AnnotationBox()
{

}
