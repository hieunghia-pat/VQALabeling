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
    m_captionComboBox->addItem(label2text[true]);
    m_captionComboBox->addItem(label2text[false]);
    m_captionComboBox->setCurrentText(label2text[true]);

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
    QObject::connect(m_captionComboBox, &QComboBox::currentTextChanged, this, &AnnotationBox::handleLabelChanged);
}

std::shared_ptr<QJsonObject> AnnotationBox::annotation()
{
    QString caption = m_captionLineEdit->text();
    QString labelText = m_captionComboBox->currentText();
    bool label = text2label[labelText];
    return std::make_shared<QJsonObject>(std::initializer_list<QPair<QString, QJsonValue>>{
        QPair<QString, QJsonValue>(CAPTION, caption),
        QPair<QString, QJsonValue>(LABEL, label)
    });
}

void AnnotationBox::setAnnotation(QJsonObject const& annotation)
{
    m_currentAnnotation = annotation;

    QString caption = annotation[CAPTION].toString();
    m_captionLineEdit->setText(caption);

    QString label = label2text[annotation[LABEL].toInt()];
    m_captionComboBox->setCurrentText(label);
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

void AnnotationBox::handleLabelChanged(QString const& label)
{
    bool currentLabel = m_currentAnnotation[LABEL].toBool();
    bool newLabel = text2label[label];

    if (currentLabel != newLabel)
    {
        m_currentAnnotation[LABEL] = newLabel;
        emit contentChanged();
    }
}

bool AnnotationBox::isEmpty()
{
    return (m_captionComboBox->currentText() != NONE);
}

AnnotationBox::~AnnotationBox()
{

}
