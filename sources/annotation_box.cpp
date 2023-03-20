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

    // Caption line editor
    m_captionGroup = new QGroupBox();
    m_captionGroup->setTitle("Caption: ");
    
    m_captionLineEdit = new QLineEdit();
    m_current_annotation[CAPTION] = m_captionLineEdit->text();

    m_captionLayout = new QVBoxLayout(m_captionGroup);
    m_captionLayout->addWidget(m_captionLineEdit);

    m_qa_layout = new QVBoxLayout();
    m_qa_layout->addWidget(m_captionGroup);

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

    // main layout
    m_layout = new QVBoxLayout(this);
    m_layout->addLayout(m_qa_layout);
    m_layout->addLayout(m_button_layout);

    QObject::connect(m_add_button, &QPushButton::clicked, [container, this]() {
        static_cast<AnnotationWidget*>(container)->addAnnotation(this->m_index);
    });
    QObject::connect(m_del_button, &QPushButton::clicked, [container, this]() {
        static_cast<AnnotationWidget*>(container)->deleteAnnotation(this->m_index);
    });
    QObject::connect(m_captionLineEdit, &QLineEdit::textChanged, this, &AnnotationBox::handleCaptionChanged);
}

std::shared_ptr<QJsonObject> AnnotationBox::annotation()
{
    return std::make_shared<QJsonObject>(std::initializer_list<QPair<QString, QJsonValue>>{
        QPair<QString, QJsonValue>(CAPTION, m_captionLineEdit->text())
    });
}

void AnnotationBox::setAnnotation(QJsonObject const& annotation)
{
    m_current_annotation = annotation;

    QString caption = annotation[CAPTION].toString();
    m_captionLineEdit->setText(caption);
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
    QString current_caption = m_current_annotation[CAPTION].toString();

    if (caption != current_caption)
    {
        m_current_annotation[CAPTION] = caption;
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
