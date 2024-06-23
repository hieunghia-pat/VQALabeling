#include "annotation_box.hpp"
#include "annotation_widget.hpp"
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

    // Caption line editor
    m_captionGroup = new QGroupBox(this);
    m_captionGroup->setTitle("Caption: ");
    
    m_captionLineEdit = new QLineEdit(m_captionGroup);
    m_currentAnnotation[CAPTION] = m_captionLineEdit->text();

    m_captionComboBox = new QComboBox(m_captionGroup);
    m_captionComboBox->addItem(QString("multi-sarcasm"));
    m_captionComboBox->addItem(QString("image-sarcasm"));
    m_captionComboBox->addItem(QString("text-sarcasm"));
    m_captionComboBox->addItem(QString("Not-sarcasm"));
    m_captionComboBox->addItem(QString("Image-not-sarcasm"));
    m_captionComboBox->addItem(QString("Text-not-sarcasm"));
    m_captionComboBox->setCurrentIndex(0);

    m_captionLayout = new QVBoxLayout(m_captionGroup);
    m_captionLayout->addWidget(m_captionLineEdit);

    // manipulation button
    m_addButton = new QPushButton();
    m_addButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_addButton->setIcon(QIcon(":/media/icons/add.png"));
    m_delButton = new QPushButton();
    m_delButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_delButton->setIcon(QIcon(":/media/icons/delete.png"));

    m_buttonLayout = new QHBoxLayout();
    m_buttonLayout->addWidget(m_addButton);
    m_buttonLayout->addWidget(m_delButton);
    m_buttonLayout->setAlignment(Qt::AlignRight);

    // main layout
    m_layout = new QVBoxLayout(this);
    // m_layout->addLayout(m_captionLayout);
    m_layout->addWidget(m_captionGroup);
    m_layout->addWidget(m_captionComboBox);
    m_layout->addLayout(m_buttonLayout);

    QObject::connect(m_addButton, &QPushButton::clicked, [container, this]() {
        static_cast<AnnotationWidget*>(container)->addAnnotation(this->m_index);
    });
    QObject::connect(m_delButton, &QPushButton::clicked, [container, this]() {
        static_cast<AnnotationWidget*>(container)->deleteAnnotation(this->m_index);
    });
    QObject::connect(m_captionLineEdit, &QLineEdit::textChanged, this, &AnnotationBox::handleCaptionChanged);
    QObject::connect(m_captionComboBox, &QComboBox::currentTextChanged, this, &AnnotationBox::handleCaptionTypeChanged);
}

std::shared_ptr<QJsonObject> AnnotationBox::annotation()
{
    return std::make_shared<QJsonObject>(std::initializer_list<QPair<QString, QJsonValue>>{
        QPair<QString, QJsonValue>(CAPTION, m_captionLineEdit->text()),
        QPair<QString, QJsonValue>(CAPTION_TYPE, m_captionComboBox->currentText())
    });
}

void AnnotationBox::setAnnotation(QJsonObject const& annotation)
{
    m_currentAnnotation = annotation;

    QString caption = annotation[CAPTION].toString();
    m_captionLineEdit->setText(caption);

    QString captionType = annotation[CAPTION_TYPE].toString();
    m_captionComboBox->setCurrentText(captionType);
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

void AnnotationBox::handleCaptionChanged(QString const& caption)
{
    QString const& currentCaption = m_currentAnnotation[CAPTION].toString();

    if (caption != currentCaption)
    {
        m_currentAnnotation[CAPTION] = caption;
        emit contentChanged();
    }
}

void AnnotationBox::handleCaptionTypeChanged(QString const& captionType)
{
    QString currentCaptionType = m_currentAnnotation[CAPTION_TYPE].toString();

    if (captionType != currentCaptionType)
    {
        m_currentAnnotation[CAPTION_TYPE] = captionType;
        emit contentChanged();
    }
}

bool AnnotationBox::isEmpty()
{
    return (m_captionLineEdit->text().isEmpty());
}

AnnotationBox::~AnnotationBox()
{

}
