#include "image_widget.hpp"
#include "exception.hpp"

#include <QFloat16>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QSize>
#include <QKeyEvent>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QPushButton>
#include <QImageReader>

#include <memory>

ImageWidget::ImageWidget(QWidget *parent)
    :QScrollArea(parent)
{
    m_image_label = new QLabel(this);
    m_image_label->setScaledContents(true);

    setWidget(m_image_label);
    setBackgroundRole(QPalette::Dark);
    setAlignment(Qt::AlignCenter);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

bool ImageWidget::setImage(QDir const& image_dir)
{
    if (m_image == nullptr)
        m_image = std::make_shared<QImage>();
    m_image->load(image_dir.path());

    m_image_label->setPixmap(QPixmap::fromImage(*m_image));
    m_image_label->adjustSize();
    fitToContainer();
    
    return true;
}

QSize ImageWidget::getImageSize()
{
    return m_image->size();
}

void ImageWidget::fitToContainer()
{
    qfloat16 img_w = m_image->width();
    qfloat16 img_h = m_image->height();
    qfloat16 scale = img_w*1.0 / img_h;
    qsizetype padding = 23;
    if (img_w < img_h) // scale image size according to height
    {
        img_h = height() - padding;
        img_w = int(img_h * scale);
    }
    else // set image size according to width
    {
        img_w = width() - padding;
        img_h = int(img_w*1.0 / scale);
    }

    m_image_label->resize(img_w, img_h);
}

void ImageWidget::scaleImage(qfloat16 scaled)
{
    m_scaled_factor *= scaled;
    
    m_image_label->resize(m_image_label->pixmap().size()*m_scaled_factor);
}

void ImageWidget::resetScaling()
{
    m_scaled_factor = 1.;
}

void ImageWidget::zoomIn()
{
    scaleImage(1.25);
}

void ImageWidget::zoomOut()
{
    scaleImage(0.8);
}

qfloat16 ImageWidget::scaleFactor()
{
    return m_scaled_factor;
}

void ImageWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Left)
        emit backImage();
    
    if (event->key() == Qt::Key_Right)
        emit nextImage();

    QScrollArea::keyPressEvent(event); // pass the event to the base class
}

ImageWidget::~ImageWidget()
{

}