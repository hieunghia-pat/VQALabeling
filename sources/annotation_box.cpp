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

    // Question-answer line editor
    m_questionGroup = new QGroupBox();
    m_questionGroup->setTitle("Question: ");
    m_questionLineEdit = new QLineEdit();
    m_questionLayout = new QVBoxLayout(m_questionGroup);
    m_questionLayout->addWidget(m_questionLineEdit);
    m_tmp_data["question"] = m_questionLineEdit->text();

    m_answerGroup = new QGroupBox();
    m_answerGroup->setTitle("Answer: ");
    m_answerLineEdit = new QLineEdit();
    m_answerLayout = new QVBoxLayout(m_answerGroup);
    m_answerLayout->addWidget(m_answerLineEdit);
    m_tmp_data["answer"] = m_answerLineEdit->text();

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

    // QA's type selection
    m_qa_checkbox_group = new QGroupBox();
    m_qa_checkbox_group->setTitle("QA's type");

    m_text_qa_checkbox = new QCheckBox();
    m_text_qa_checkbox->setText("Text QA");
    m_text_qa_checkbox->setCheckState(Qt::Checked);
    m_text_qa_checkbox->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    m_tmp_data["text-QA"] = m_text_qa_checkbox->checkState() == Qt::Checked ? true : false;

    m_state_qa_checkbox = new QCheckBox();
    m_state_qa_checkbox->setText("State QA");
    m_state_qa_checkbox->setCheckState(Qt::Unchecked);
    m_state_qa_checkbox->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    m_tmp_data["state-QA"] = m_state_qa_checkbox->checkState() == Qt::Checked ? true : false;

    m_action_qa_checkbox = new QCheckBox();
    m_action_qa_checkbox->setText("Action QA");
    m_action_qa_checkbox->setCheckState(Qt::Unchecked);
    m_action_qa_checkbox->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    m_tmp_data["action-QA"] = m_action_qa_checkbox->checkState() == Qt::Checked ? true : false;

    m_qa_checkbox_layout = new QVBoxLayout(m_qa_checkbox_group);
    m_qa_checkbox_layout->addWidget(m_text_qa_checkbox);
    m_qa_checkbox_layout->addWidget(m_state_qa_checkbox);
    m_qa_checkbox_layout->addWidget(m_action_qa_checkbox);

    // question's type selection
    m_q_selection_group = new QGroupBox();
    m_q_selection_group->setTitle("Question's type");
    m_q_selection_box = new SelectionBox();
    m_q_selection_box->insertItems(0, QList<QString>({
        "What - 0",
        "Who - 1",
        "Where - 2",
        "When - 3",
        "How - 4",
        "Other - 5"
    }));
    m_q_selection_box->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    m_tmp_data["question-type"] = m_q_selection_box->currentIndex();

    m_q_selection_layout = new QHBoxLayout(m_q_selection_group);
    m_q_selection_layout->addWidget(m_q_selection_box);

    // answer's type selection
    m_a_selection_group = new QGroupBox();
    m_a_selection_group->setTitle("Answer's type");
    m_a_selection_box = new SelectionBox();
    m_a_selection_box->insertItems(0, QList<QString>({
        "Word - 0",
        "Phrase - 1",
        "Sentence - 2"
    }));
    m_a_selection_box->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    m_tmp_data["answer-type"] = m_a_selection_box->currentIndex();
    
    m_a_selection_layout = new QHBoxLayout(m_a_selection_group);
    m_a_selection_layout->addWidget(m_a_selection_box);

    // layout for QA's type selection
    m_selection_layout = new QVBoxLayout();
    m_selection_layout->addWidget(m_q_selection_group);
    m_selection_layout->addWidget(m_a_selection_group);

    // annotations layout
    m_annotation_layout = new QHBoxLayout();
    m_annotation_layout->addWidget(m_qa_checkbox_group);
    m_annotation_layout->addLayout(m_selection_layout);

    m_layout = new QVBoxLayout(this);
    m_layout->addLayout(m_qa_layout);
    m_layout->addLayout(m_annotation_layout);
    m_layout->addLayout(m_button_layout);

    QObject::connect(m_add_button, &QPushButton::clicked, [container, this]() {
        static_cast<AnnotationWidget*>(container)->addAnnotation(this->m_index);
    });
    QObject::connect(m_del_button, &QPushButton::clicked, [container, this]() {
        static_cast<AnnotationWidget*>(container)->deleteAnnotation(this->m_index);
    });
    QObject::connect(m_questionLineEdit, &QLineEdit::textChanged, this, &AnnotationBox::handleQuestionChanged);
    QObject::connect(m_answerLineEdit, &QLineEdit::textChanged, this, &AnnotationBox::handleAnswerChanged);
    QObject::connect(m_text_qa_checkbox, &QCheckBox::stateChanged, this, &AnnotationBox::handleTextQATypeChanged);
    QObject::connect(m_state_qa_checkbox, &QCheckBox::stateChanged, this, &AnnotationBox::handleStateQATypeChanged);
    QObject::connect(m_action_qa_checkbox, &QCheckBox::stateChanged, this, &AnnotationBox::handleActionQATypeChanged);
    QObject::connect(m_q_selection_box, &SelectionBox::currentIndexChanged, this, &AnnotationBox::handleQuestionTypeChanged);
    QObject::connect(m_a_selection_box, &SelectionBox::currentIndexChanged, this, &AnnotationBox::handleAnswerTypeChanged);
}

std::shared_ptr<QJsonObject> AnnotationBox::annotation()
{
    QString question = m_questionLineEdit->text();
    QString answer = m_answerLineEdit->text();
    bool text_QA = m_text_qa_checkbox->checkState() == Qt::Checked ? true : false;
    bool state_QA = m_state_qa_checkbox->checkState() == Qt::Checked ? true : false;
    bool action_QA = m_state_qa_checkbox->checkState() == Qt::Checked ? true : false;
    qint16 q_type = m_q_selection_box->currentIndex();
    qint16 a_type = m_a_selection_box->currentIndex();
    return std::make_shared<QJsonObject>(std::initializer_list<QPair<QString, QJsonValue>>{
        QPair<QString, QJsonValue>("question", m_questionLineEdit->text()),
        QPair<QString, QJsonValue>("answer", m_answerLineEdit->text()),
        QPair<QString, QJsonValue>("text-QA", text_QA),
        QPair<QString, QJsonValue>("state-QA", state_QA),
        QPair<QString, QJsonValue>("action-QA", action_QA),
        QPair<QString, QJsonValue>("question-type", q_type),
        QPair<QString, QJsonValue>("answer-type", a_type)
    });
}

void AnnotationBox::setAnnotation(QJsonObject const& data)
{
    m_tmp_data = data;

    QString question = data["question"].toString();
    m_questionLineEdit->setText(question);

    QString answer = data["answer"].toString();
    m_answerLineEdit->setText(answer);

    bool text_QA = data["text-QA"].toBool();
    m_text_qa_checkbox->setCheckState(text_QA ? Qt::Checked : Qt::Unchecked);

    bool state_QA = data["state-QA"].toBool();
    m_state_qa_checkbox->setCheckState(state_QA ? Qt::Checked : Qt::Unchecked);

    bool action_QA = data["action-QA"].toBool();
    m_action_qa_checkbox->setCheckState(action_QA ? Qt::Checked : Qt::Unchecked);

    qint16 q_type = data["question-type"].toInt();
    m_q_selection_box->setCurrentIndex(q_type);

    qint16 a_type = data["answer-type"].toInt();
    m_a_selection_box->setCurrentIndex(a_type);
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
    if (question != m_tmp_data["question"].toString())
        emit contentChanged();
}

void AnnotationBox::handleAnswerChanged(QString const& answer)
{
    if (answer != m_tmp_data["answer"].toString())
        emit contentChanged();
}

void AnnotationBox::handleTextQATypeChanged(bool const& is_text_qa)
{
    if (is_text_qa != m_tmp_data["text-QA"].toBool())
        emit contentChanged();
}

void AnnotationBox::handleStateQATypeChanged(bool const& is_state_qa)
{
    if (is_state_qa != m_tmp_data["state-QA"].toBool())
        emit contentChanged();
}

void AnnotationBox::handleActionQATypeChanged(bool const& is_action_qa)
{
    if (is_action_qa != m_tmp_data["action-QA"].toBool())
        emit contentChanged();
}

void AnnotationBox::handleQuestionTypeChanged(qint16 const& is_q_type)
{
    if (is_q_type != m_tmp_data["question-type"].toInt())
        emit contentChanged();
}

void AnnotationBox::handleAnswerTypeChanged(qint16 const& is_a_type)
{
    if (is_a_type != m_tmp_data["answer-type"].toInt())
        emit contentChanged();
}

bool AnnotationBox::isEmpty()
{
    return (m_questionLineEdit->text().isEmpty() && m_answerLineEdit->text().isEmpty());
}

AnnotationBox::~AnnotationBox()
{

}