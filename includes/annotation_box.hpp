#ifndef ANNOTATION_BOX_HPP
#define ANNOTATION_BOX_HPP

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QJsonObject>

class AnnotationBox: public QGroupBox
{
    Q_OBJECT
public:
    AnnotationBox(QWidget *parent = nullptr);

    std::shared_ptr<QJsonObject> annotation() const;
    void setAnnotation(QJsonObject const& data) const;

    ~AnnotationBox();

private:
    QLineEdit* m_questionLineEdit = nullptr;
    QLineEdit* m_answerLineEdit = nullptr;

    QGroupBox* m_questionGroup = nullptr;
    QGroupBox* m_answerGroup = nullptr;

    QVBoxLayout* m_questionLayout = nullptr;
    QVBoxLayout* m_answerLayout = nullptr;

    QVBoxLayout* m_layout = nullptr;

};

#endif // ANNOTATION_BOX_HPP