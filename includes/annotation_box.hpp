#ifndef ANNOTATION_BOX_HPP
#define ANNOTATION_BOX_HPP

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QJsonObject>
#include <QPushButton>
#include <QComboBox>

class AnnotationBox: public QGroupBox
{
    Q_OBJECT
public:
    AnnotationBox(qsizetype ith, QWidget* container, QWidget *parent = nullptr);

    std::shared_ptr<QJsonObject> annotation() const;
    void setAnnotation(QJsonObject const& data) const;

    ~AnnotationBox();

private:
    QLineEdit* m_questionLineEdit = nullptr;
    QLineEdit* m_answerLineEdit = nullptr;

    QPushButton* m_add_button = nullptr;
    QPushButton* m_del_button = nullptr;
    QComboBox* m_selection_box = nullptr;

    QGroupBox* m_questionGroup = nullptr;
    QGroupBox* m_answerGroup = nullptr;
    QGroupBox* m_selectionGroup = nullptr;

    QVBoxLayout* m_questionLayout = nullptr;
    QVBoxLayout* m_answerLayout = nullptr;
    QVBoxLayout* m_selectionLayout = nullptr;
    QHBoxLayout* m_button_layout = nullptr;
    QVBoxLayout* m_qa_layout = nullptr;
    QVBoxLayout* m_layout = nullptr;

};

#endif // ANNOTATION_BOX_HPP