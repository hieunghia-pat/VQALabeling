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
    m_questionLayout = new QVBoxLayout(m_questionGroup);
    m_questionLayout->addWidget(m_questionLineEdit);
    m_current_annotation[QUESTION] = m_questionLineEdit->text();

    m_answerGroup = new QGroupBox();
    m_answerGroup->setTitle("Answer: ");
    m_answerLineEdit = new QLineEdit();
    m_answerLineEdit->setDisabled(true);
    m_answerLineEdit->setFont(*boldFont);
    m_foreignAnswerLineEdit = new QLineEdit();
    m_foreignAnswerLineEdit->setFont(*font);
    m_answerLayout = new QVBoxLayout(m_answerGroup);
    m_answerLayout->addWidget(m_answerLineEdit);
    m_current_annotation[ANSWER] = m_answerLineEdit->text();
    m_answerLayout->addWidget(m_foreignAnswerLineEdit);
    m_current_annotation[ANSWER] = m_foreignAnswerLineEdit->text();

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

    // // QA's type selection
    // m_qa_checkbox_group = new QGroupBox();
    // m_qa_checkbox_group->setTitle("QA's type");

    // m_text_qa_checkbox = new QCheckBox();
    // m_text_qa_checkbox->setText("Text QA");
    // m_text_qa_checkbox->setChecked(true);
    // m_text_qa_checkbox->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    // m_current_annotation["text-QA"] = m_text_qa_checkbox->isChecked();

    // m_state_qa_checkbox = new QCheckBox();
    // m_state_qa_checkbox->setText("State QA");
    // m_state_qa_checkbox->setChecked(false);
    // m_state_qa_checkbox->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    // m_current_annotation["state-QA"] = m_state_qa_checkbox->isChecked();

    // m_action_qa_checkbox = new QCheckBox();
    // m_action_qa_checkbox->setText("Action QA");
    // m_action_qa_checkbox->setChecked(false);
    // m_action_qa_checkbox->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    // m_current_annotation["action-QA"] = m_action_qa_checkbox->isChecked();

    // m_qa_checkbox_layout = new QVBoxLayout(m_qa_checkbox_group);
    // m_qa_checkbox_layout->addWidget(m_text_qa_checkbox);
    // m_qa_checkbox_layout->addWidget(m_state_qa_checkbox);
    // m_qa_checkbox_layout->addWidget(m_action_qa_checkbox);

    // // question's type selection
    // m_q_selection_group = new QGroupBox();
    // m_q_selection_group->setTitle("Question's type");
    // m_q_selection_box = new SelectionBox();
    // m_q_selection_box->insertItems(0, QList<QString>({
    //     "What - 0",
    //     "Who - 1",
    //     "Where - 2",
    //     "When - 3",
    //     "How - 4",
    //     "Other - 5"
    // }));
    // m_q_selection_box->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    // m_current_annotation["question-type"] = m_q_selection_box->currentIndex();

    // m_q_selection_layout = new QHBoxLayout(m_q_selection_group);
    // m_q_selection_layout->addWidget(m_q_selection_box);

    // // answer's type selection
    // m_a_selection_group = new QGroupBox();
    // m_a_selection_group->setTitle("Answer's type");
    // m_a_selection_box = new SelectionBox();
    // m_a_selection_box->insertItems(0, QList<QString>({
    //     "Word - 0",
    //     "Phrase - 1",
    //     "Sentence - 2"
    // }));
    // m_a_selection_box->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    // m_current_annotation["answer-type"] = m_a_selection_box->currentIndex();
    
    // m_a_selection_layout = new QHBoxLayout(m_a_selection_group);
    // m_a_selection_layout->addWidget(m_a_selection_box);

    // // layout for QA's type selection
    // m_selection_layout = new QVBoxLayout();
    // m_selection_layout->addWidget(m_q_selection_group);
    // m_selection_layout->addWidget(m_a_selection_group);

    // // QA's type
    // m_qa_selection_group = new QGroupBox();
    // m_qa_selection_group->setTitle("QA's type");
    // m_qa_selection_box = new SelectionBox();
    // m_qa_selection_box->insertItems(0, QList<QString>{
    //     "Text QA",
    //     "Non-text QA"
    // });
    // m_qa_selection_box->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    // m_qa_selection_box->setDisabled(true);
    // m_current_annotation[QA_TYPE] = m_qa_selection_box->currentIndex();

    // // QA's type selection layout
    // m_qa_selection_layout = new QVBoxLayout(m_qa_selection_group);
    // m_qa_selection_layout->addWidget(m_qa_selection_box);

    // // annotations layout
    // m_annotation_layout = new QHBoxLayout();
    // m_annotation_layout->addLayout(m_selection_layout);

    m_layout = new QVBoxLayout(this);
    m_layout->addLayout(m_qa_layout);
    // m_layout->addLayout(m_annotation_layout);
    // m_layout->addWidget(m_qa_selection_group);
    m_layout->addLayout(m_button_layout);

    QObject::connect(m_add_button, &QPushButton::clicked, [container, this]() {
        static_cast<AnnotationWidget*>(container)->addAnnotation(this->m_index);
    });
    QObject::connect(m_del_button, &QPushButton::clicked, [container, this]() {
        static_cast<AnnotationWidget*>(container)->deleteAnnotation(this->m_index);
    });
    QObject::connect(m_questionLineEdit, &QLineEdit::textChanged, this, &AnnotationBox::handleQuestionChanged);
    QObject::connect(m_foreignAnswerLineEdit, &QLineEdit::textChanged, this, &AnnotationBox::handleAnswerChanged);
    // QObject::connect(m_text_qa_checkbox, &QCheckBox::stateChanged, this, &AnnotationBox::handleTextQATypeChanged);
    // QObject::connect(m_state_qa_checkbox, &QCheckBox::stateChanged, this, &AnnotationBox::handleStateQATypeChanged);
    // QObject::connect(m_action_qa_checkbox, &QCheckBox::stateChanged, this, &AnnotationBox::handleActionQATypeChanged);
    // QObject::connect(m_q_selection_box, &SelectionBox::currentIndexChanged, this, &AnnotationBox::handleQuestionTypeChanged);
    // QObject::connect(m_a_selection_box, &SelectionBox::currentIndexChanged, this, &AnnotationBox::handleAnswerTypeChanged);
    // QObject::connect(m_qa_selection_box, &SelectionBox::currentIndexChanged, this, &AnnotationBox::handleQATypeChanged);
}

std::shared_ptr<QJsonObject> AnnotationBox::annotation()
{
    QString question = m_questionLineEdit->text();
    QString answer = m_answerLineEdit->text();
    QString foreignAnswer = m_foreignAnswerLineEdit->text();
    // bool text_QA = m_text_qa_checkbox->isChecked();
    // bool state_QA = m_state_qa_checkbox->isChecked();
    // bool action_QA = m_action_qa_checkbox->isChecked();
    // qint16 q_type = m_q_selection_box->currentIndex();
    // qint16 a_type = m_a_selection_box->currentIndex();

    return std::make_shared<QJsonObject>(std::initializer_list<QPair<QString, QJsonValue>>{
        QPair<QString, QJsonValue>(QUESTION, m_questionLineEdit->text()),
        QPair<QString, QJsonValue>(ANSWER, m_answerLineEdit->text()),
        QPair<QString, QJsonValue>(FOREIGN_ANSWER, m_foreignAnswerLineEdit->text()),
        // QPair<QString, QJsonValue>(TEXT_QA, text_QA),
        // QPair<QString, QJsonValue>(STATE_QA, state_QA),
        // QPair<QString, QJsonValue>(ACTION_QA, action_QA),
        // QPair<QString, QJsonValue>(QUESTION_TYPE, q_type),
        // QPair<QString, QJsonValue>(ANSWER_TYPE, a_type)
        // QPair<QString, QJsonValue>(QA_TYPE, m_qa_selection_box->currentIndex())
    });
}

void AnnotationBox::setAnnotation(QJsonObject const& annotation)
{
    m_current_annotation = annotation;

    QString question = annotation[QUESTION].toString();
    qDebug() << QString("In AnnotationBox::setAnnotation - Setting question line edit to %1").arg(question.size() > 0 ? question : "\"\"").toStdString().c_str();
    m_questionLineEdit->setText(question);

    QString answer = annotation[ANSWER].toString();
    qDebug() << QString("In AnnotationBox::setAnnotation - Setting answer line edit to %1").arg(answer.size() > 0 ? answer : "\"\"").toStdString().c_str();
    m_answerLineEdit->setText(answer);

    QString foreignAnswer = annotation[FOREIGN_ANSWER].toString();
    m_foreignAnswerLineEdit->setText(foreignAnswer);

    // bool text_QA = annotation[TEXT_QA].toBool();
    // qDebug() << QString("In AnnotationBox::setAnnotation - Setting text QA checkbox to %1")
    //                     .arg(check_state[text_QA])
    //                     .toStdString().c_str();
    // m_text_qa_checkbox->setChecked(text_QA);

    // bool state_QA = annotation[STATE_QA].toBool();
    // qDebug() << QString("In AnnotationBox::setAnnotation - Setting state QA checkbox to %1")
    //                     .arg(check_state[state_QA])
    //                     .toStdString().c_str();
    // m_state_qa_checkbox->setChecked(state_QA);

    // bool action_QA = annotation[ACTION_QA].toBool();
    // qDebug() << QString("In AnnotationBox::setAnnotation - Setting action QA checkbox to %1")
    //                     .arg(check_state[action_QA])
    //                     .toStdString().c_str();
    // m_action_qa_checkbox->setChecked(action_QA);

    // qint16 q_type = annotation[QUESTION_TYPE].toInt();
    // qDebug() << QString("In AnnotationBox::setAnnotation - Setting question type to %1")
    //                 .arg(question_type[q_type]).toStdString().c_str();
    // m_q_selection_box->setCurrentIndex(q_type);

    // qint16 a_type = annotation[ANSWER_TYPE].toInt();
    // qDebug() << QString("In AnnotationBox::setAnnotation - Setting answer type to %1")
    //                 .arg(answer_type[a_type]).toStdString().c_str();
    // m_a_selection_box->setCurrentIndex(a_type);

    // qint16 qa_type = m_current_annotation[QA_TYPE].toInt();
    // m_qa_selection_box->setCurrentIndex(qa_type);
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
        qDebug() << QString("In AnnotationBox::handleContentChanged - annotation changed at %1 from %2 to %3")
                        .arg(QUESTION)
                        .arg(QString("\"") + current_question + QString("\""))
                        .arg(QString("\"") + question + QString("\"")).toStdString().c_str();

        m_current_annotation[QUESTION] = question;
        emit contentChanged();
    }
}

void AnnotationBox::handleAnswerChanged(QString const& answer)
{
    QString current_answer = m_current_annotation[FOREIGN_ANSWER].toString();

    if (answer != current_answer)
    {
        qDebug() << QString("In AnnotationBox::handleContentChanged - annotation changed at %1 from %2 to %3")
                        .arg(FOREIGN_ANSWER)
                        .arg(QString("\"") + current_answer + QString("\""))
                        .arg(QString("\"") + answer + QString("\"")).toStdString().c_str();

        m_current_annotation[FOREIGN_ANSWER] = answer;
        emit contentChanged();
    }
}

// void AnnotationBox::handleQuestionTypeChanged(qint16 const& question_type)
// {
//     qint16 current_question_type = m_current_annotation[QUESTION_TYPE].toInt();

//     if (question_type != current_question_type)
//     {
//         qDebug() << QString("In AnnotationBox::handleContentChanged - annotation changed at %1 from %2 to %3")
//                         .arg(QUESTION_TYPE)
//                         .arg(current_question_type)
//                         .arg(question_type).toStdString().c_str();

//         m_current_annotation[QUESTION_TYPE] = question_type;
//         emit contentChanged();
//     }
// }

// void AnnotationBox::handleAnswerTypeChanged(qint16 const& answer_type)
// {
//     qint16 current_question = m_current_annotation[ANSWER_TYPE].toInt();

//     if (answer_type != current_question)
//     {
//         qDebug() << QString("In AnnotationBox::handleContentChanged - annotation changed at %1 from %2 to %3")
//                         .arg(ANSWER_TYPE)
//                         .arg(current_question)
//                         .arg(answer_type).toStdString().c_str();

//         m_current_annotation[ANSWER_TYPE] = answer_type;
//         emit contentChanged();
//     }
// }

// void AnnotationBox::handleTextQATypeChanged(bool const& text_qa_state)
// {
//     bool current_text_qa_state = m_current_annotation[TEXT_QA].toBool();

//     if (text_qa_state != current_text_qa_state)
//     {
//         qDebug() << QString("In AnnotationBox::handleContentChanged - annotation changed at %1 from %2 to %3")
//                         .arg(TEXT_QA)
//                         .arg(check_state[current_text_qa_state])
//                         .arg(check_state[text_qa_state]).toStdString().c_str();

//         m_current_annotation[TEXT_QA] = text_qa_state;
//         emit contentChanged();
//     }
// }

// void AnnotationBox::handleStateQATypeChanged(bool const& state_qa_state)
// {
//     bool current_state_qa_state = m_current_annotation[STATE_QA].toBool();

//     if (state_qa_state != current_state_qa_state)
//     {
//         qDebug() << QString("In AnnotationBox::handleContentChanged - annotation changed at %1 from %2 to %3")
//                         .arg(STATE_QA)
//                         .arg(check_state[current_state_qa_state])
//                         .arg(check_state[state_qa_state]).toStdString().c_str();

//         m_current_annotation[STATE_QA] = state_qa_state;
//         emit contentChanged();
//     }
// }

// void AnnotationBox::handleActionQATypeChanged(bool const& action_qa_state)
// {
//     bool current_action_qa_state = m_current_annotation[ACTION_QA].toBool();

//     if (action_qa_state != current_action_qa_state)
//     {
//         qDebug() << QString("In AnnotationBox::handleContentChanged - annotation changed at %1 from %2 to %3")
//                         .arg(ACTION_QA)
//                         .arg(check_state[current_action_qa_state])
//                         .arg(check_state[action_qa_state]).toStdString().c_str();

//         m_current_annotation[ACTION_QA] = action_qa_state;
//         emit contentChanged();
//     }
// }

void AnnotationBox::handleQATypeChanged(qint16 const& type) {
    qint16 current_qa_type = m_current_annotation[QA_TYPE].toInt();

    if (type != current_qa_type) {
        m_current_annotation[QA_TYPE] = type;
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