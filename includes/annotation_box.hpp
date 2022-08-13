#ifndef ANNOTATION_BOX_HPP
#define ANNOTATION_BOX_HPP

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QJsonObject>
#include <QPushButton>
#include <QFont>

#include "selection_box.hpp"

class AnnotationBox: public QGroupBox
{
    Q_OBJECT
public:
    // method
    AnnotationBox(qsizetype ith, QWidget* container, QWidget *parent = nullptr);

    std::shared_ptr<QJsonObject> annotation();
    void setAnnotation(QJsonObject const& data);

    qint16 index();
    void setIndex(qint16 index);

    bool isEmpty();

    ~AnnotationBox();

    // attributes
    QLineEdit* m_questionLineEdit = nullptr;
    QLineEdit* m_answerLineEdit = nullptr;
    QLineEdit* m_foreignAnswerLineEdit = nullptr;

    QPushButton* m_add_button = nullptr;
    QPushButton* m_del_button = nullptr;
    
    // QCheckBox* m_text_qa_checkbox = nullptr;
    // QCheckBox* m_state_qa_checkbox = nullptr;
    // QCheckBox* m_action_qa_checkbox = nullptr;

    SelectionBox* m_qa_selection_box = nullptr;

    // SelectionBox* m_q_selection_box = nullptr;
    // SelectionBox* m_a_selection_box = nullptr;

    QGroupBox* m_questionGroup = nullptr;
    QGroupBox* m_answerGroup = nullptr;
    // QGroupBox* m_qa_checkbox_group = nullptr;
    // QGroupBox* m_q_selection_group = nullptr;
    // QGroupBox* m_a_selection_group = nullptr;

    QGroupBox* m_qa_selection_group = nullptr;

    QVBoxLayout* m_questionLayout = nullptr;
    QVBoxLayout* m_answerLayout = nullptr;

    QVBoxLayout* m_qa_layout = nullptr;
    // QVBoxLayout* m_qa_checkbox_layout = nullptr;
    // QHBoxLayout* m_q_selection_layout = nullptr;
    // QHBoxLayout* m_a_selection_layout = nullptr;
    // QVBoxLayout* m_selection_layout = nullptr;
    // QHBoxLayout* m_annotation_layout = nullptr;
    QVBoxLayout* m_qa_selection_layout = nullptr;
    QHBoxLayout* m_button_layout = nullptr;
    QVBoxLayout* m_layout = nullptr;

    QJsonObject m_current_annotation;

    QFont* font = nullptr;
    QFont* boldFont = nullptr;

signals:
    void contentChanged();

public slots:
    void handleQuestionChanged(QString const& question);
    void handleAnswerChanged(QString const& answer);
    // void handleQuestionTypeChanged(qint16 const& question_type);
    // void handleAnswerTypeChanged(qint16 const& answer_type);
    // void handleTextQATypeChanged(bool const& text_qa_state);
    // void handleStateQATypeChanged(bool const& state_qa_state);
    // void handleActionQATypeChanged(bool const& action_qa_state);
    void handleQATypeChanged(qint16 const& type);

private:
    qint16 m_index;

};

#endif // ANNOTATION_BOX_HPP