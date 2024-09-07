#ifndef ANNOTATION_WIDGET_HPP
#define ANNOTATION_WIDGET_HPP

#include <QWidget>
#include <QList>
#include <QGroupBox>
#include <QLineEdit>
#include <QJsonObject>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QJsonArray>
#include <QKeyEvent>

#include <memory>

class AnnotationWidget: public QWidget
{
    Q_OBJECT
public:
    AnnotationWidget(QWidget* parent = nullptr);

    std::shared_ptr<QJsonObject> annotation() const;
    void setAnnotation(QJsonObject const& annotation);

    bool isEmpty();

    ~AnnotationWidget();

signals:
    void haveAdjusted();
    void nextAnnotation();
    void previousAnnotation();

private:
    QJsonObject m_annotation;
    QLineEdit* m_caption = nullptr;
    QLineEdit* m_englishCaption = nullptr;
    // QComboBox* m_comboBox = nullptr;
    QVBoxLayout* m_layout = nullptr;

    void checkCaptionChanged(QString text);

protected:
    void keyPressEvent(QKeyEvent* event) override;
};

#endif // ANNOTATION_WIDGET_HPP
