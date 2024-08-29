#include "container.hpp"
#include "image_widget.hpp"
#include "annotation_widget.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

Container::Container(QWidget *parent)
    : QWidget(parent)
{
    m_image_widget_top = new ImageWidget();
    m_image_widget_bottom = new ImageWidget();
    m_vlayout = new QVBoxLayout(parent);
    m_vlayout->addWidget(m_image_widget_top);
    m_vlayout->addWidget(m_image_widget_bottom);

    m_annotation_widget = new AnnotationWidget();
    m_annotation_widget->setEnabled(false);

    m_hlayout = new QHBoxLayout(this);
    m_hlayout->addLayout(m_vlayout);
    m_hlayout->addWidget(m_annotation_widget);

}

Container::~Container()
{

}
