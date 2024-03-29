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
#include <QMessageBox>
#include <QTransform>

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
    qDebug() << QString("In ImageWidget::setImage - Loading image from %1").arg(image_dir.absolutePath()).toStdString().c_str();

    if (m_image == nullptr)
        m_image = std::make_shared<QImage>();
    
    if (!(m_image->load(image_dir.path())))
    {
        qDebug() << QString("In ImageWidget::setImage - Cannot load image from %1").arg(image_dir.path()).toStdString().c_str();
        QMessageBox::warning(this, QString("Error occured"), QString("Cannot load image from %1").arg(image_dir.path()));
    }

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
    m_image_label->resize(m_image_label->size()*(1 + scaled));
}

void ImageWidget::resetScaling()
{
    scaleImage(0);
}

void ImageWidget::rotateLeft() {
    qfloat16 image_w = m_image_label->width();
    qfloat16 image_h = m_image_label->height();

    QTransform rotate_left = QTransform().rotate(-90);
    m_image_label->setPixmap(m_image_label->pixmap().transformed(rotate_left));
    m_image_label->resize(QSize(image_h, image_w));
    scaleImage(0);
}

void ImageWidget::rotateRight() {
    qfloat16 image_w = m_image_label->width();
    qfloat16 image_h = m_image_label->height();

    QTransform rotate_right = QTransform().rotate(90);
    m_image_label->setPixmap(m_image_label->pixmap().transformed(rotate_right));
    m_image_label->resize(QSize(image_h, image_w));
    scaleImage(0);
}

void ImageWidget::zoomIn()
{
    scaleImage(0.1);
}

void ImageWidget::zoomOut()
{
    scaleImage(-0.1);
}

ImageWidget::~ImageWidget()
{

}
