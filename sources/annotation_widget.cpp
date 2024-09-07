#include "annotation_widget.hpp"
#include "constants.hpp"

#include <QList>
#include <QPair>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QWidget>
#include <QJsonArray>
#include <QJsonValue>
#include <QDialog>
#include <QKeyEvent>

#include <memory>

AnnotationWidget::AnnotationWidget(QWidget* parent)
    : QWidget(parent)
{
    m_caption = new QLineEdit(this);
    m_englishCaption = new QLineEdit(this);
    m_englishCaption->setReadOnly(true);

    m_layout = new QVBoxLayout(this);
    m_layout->addWidget(m_englishCaption);
    m_layout->addWidget(m_caption);

    m_layout->setAlignment(Qt::AlignCenter);

    QObject::connect(m_caption, &QLineEdit::textChanged, this, &AnnotationWidget::checkCaptionChanged);
}

void AnnotationWidget::checkCaptionChanged(QString text) {
    QString currentCaption = m_annotation[CAPTION].toString();
    if (currentCaption != text) {
        emit haveAdjusted();
    }
}

std::shared_ptr<QJsonObject> AnnotationWidget::annotation() const
{
    QString caption = m_caption->text();
    QString englishCaption = m_englishCaption->text();
    // bool label = m_comboBox->currentIndex();

    return std::make_shared<QJsonObject>(std::initializer_list<QPair<QString, QJsonValue>>{
        QPair<QString, QJsonValue>(CAPTION, caption),
        QPair<QString, QJsonValue>(ENG_CAPTION, englishCaption)
    });
}

void AnnotationWidget::setAnnotation(QJsonObject const& annotation)
{
    m_annotation = annotation;
    QString caption = annotation[CAPTION].toString();
    QString englishCaption = annotation[ENG_CAPTION].toString();

    m_caption->setText(caption);
    m_englishCaption->setText(englishCaption);
}

bool AnnotationWidget::isEmpty()
{
    return m_caption->text().isEmpty();
}

AnnotationWidget::~AnnotationWidget()
{

}

void AnnotationWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Left)
        emit previousAnnotation();
    
    if (event->key() == Qt::Key_Right)
        emit nextAnnotation();

    QWidget::keyPressEvent(event); // pass the event to the base class
}
