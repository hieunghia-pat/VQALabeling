#include "container.hpp"
#include "image_widget.hpp"
#include "annotation_widget.hpp"

#include <QHBoxLayout>
#include <QPushButton>

Container::Container(QWidget *parent)
    : QWidget(parent)
{
    m_image_widget = new ImageWidget();
    m_annotation_widget = new AnnotationWidget();

    m_layout = new QHBoxLayout(this);
    m_layout->addWidget(m_image_widget);
    m_layout->addWidget(m_annotation_widget);
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
