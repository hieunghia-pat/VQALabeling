#include "image_container.hpp"
#include "image_widget.hpp"

#include <QHBoxLayout>
#include <QPushButton>

ImageContainer::ImageContainer(QWidget *parent)
    : QWidget(parent)
{
    m_image_widget_top = new ImageWidget();
    m_image_widget_bottom = new ImageWidget();
    m_imageLayout = new QHBoxLayout(this);
    m_imageLayout->addWidget(m_image_widget_top);
    m_imageLayout->addWidget(m_image_widget_bottom);

}

ImageContainer::~ImageContainer()
{

}
