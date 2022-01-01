#include "container.hpp"
#include "image_widget.hpp"
#include "annotation_widget.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

Container::Container(QWidget *parent)
    : QWidget(parent)
{
    m_image_widget = new ImageWidget(parent);
    m_annotation_widget = new AnnotationWidget(parent);
    m_add_anntation_button = new QPushButton(this);
    m_add_anntation_button->setText("Add Annotation");

    m_annotation_layout = new QVBoxLayout();
    m_annotation_layout->addWidget(m_annotation_widget);
    m_annotation_layout->addWidget(m_add_anntation_button);

    m_layout = new QHBoxLayout(this);
    m_layout->addWidget(m_image_widget);
    m_layout->addLayout(m_annotation_layout);

    setLayout(m_layout);

    QObject::connect(m_add_anntation_button, &QPushButton::clicked, m_annotation_widget, &AnnotationWidget::addAnnotation);
}

ImageWidget* Container::imageWidget() const
{
    return m_image_widget;
}

AnnotationWidget* Container::annotationWidget() const
{
    return m_annotation_widget;
}

QHBoxLayout* Container::layout() const
{
    return m_layout;
}

Container::~Container()
{

}
