#include "annotation_box.hpp"
#include "annotation_widget.hpp"
#include "selection_box.hpp"

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

#include <memory>

AnnotationBox::AnnotationBox(qsizetype ith, QWidget* container, QWidget *parent)
    : QGroupBox{parent}
{
    m_index = ith;
    setTitle(QString("Annotation %1: ").arg(m_index+1));

    m_questionGroup = new QGroupBox();
    m_questionGroup->setTitle("Question: ");
    m_questionLineEdit = new QLineEdit();
    m_questionLayout = new QVBoxLayout(m_questionGroup);
    m_questionLayout->addWidget(m_questionLineEdit);
    m_tmp_question = m_questionLineEdit->text();

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
    m_tmp_answer = m_answerLineEdit->text();

    m_selectionGroup = new QGroupBox();
    m_selectionGroup->setTitle("Question's type");
    m_selection_box = new SelectionBox(m_selectionGroup);
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

    QObject::connect(m_add_button, &QPushButton::clicked, [container, this]() {
        static_cast<AnnotationWidget*>(container)->addAnnotation(this->m_index);
    });
    QObject::connect(m_del_button, &QPushButton::clicked, [container, this]() {
        static_cast<AnnotationWidget*>(container)->deleteAnnotation(this->m_index);
    });
    QObject::connect(m_questionLineEdit, &QLineEdit::textChanged, this, &AnnotationBox::handleQuestionChanged);
    QObject::connect(m_answerLineEdit, &QLineEdit::textChanged, this, &AnnotationBox::handleAnswerChanged);
    QObject::connect(m_selection_box, &QComboBox::currentIndexChanged, this, &AnnotationBox::handleTypeChanged);
}

std::shared_ptr<QJsonObject> AnnotationBox::annotation()
{
    return std::make_shared<QJsonObject>(std::initializer_list<QPair<QString, QJsonValue>>{
        QPair<QString, QJsonValue>("question", m_questionLineEdit->text()),
        QPair<QString, QJsonValue>("answer", m_answerLineEdit->text()),
        QPair<QString, QJsonValue>("type", m_selection_box->currentIndex())
    });
}

void AnnotationBox::setAnnotation(QJsonObject const& data)
{
    QString question = data["question"].toString();
    m_tmp_question = question;
    m_questionLineEdit->setText(question);

    QString answer = data["answer"].toString();
    m_tmp_answer = answer;
    m_answerLineEdit->setText(answer);

    qint16 type = data["type"].toInt();
    m_tmp_type = type;
    m_selection_box->setCurrentIndex(type);
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
    if (question != m_tmp_question)
        emit contentChanged();
}

void AnnotationBox::handleAnswerChanged(QString const& answer)
{
    if (answer != m_tmp_answer)
        emit contentChanged();
}

void AnnotationBox::handleTypeChanged(qint64 const& type)
{
    if (type != m_tmp_type)
        emit contentChanged();
}

bool AnnotationBox::isEmpty()
{
    return (m_questionLineEdit->text().isEmpty() && m_answerLineEdit->text().isEmpty());
}

AnnotationBox::~AnnotationBox()
{

}