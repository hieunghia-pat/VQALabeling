#ifndef IMAGE_WIDGET_HPP
#define IMAGE_WIDGET_HPP

#include <QDir>
#include <QAction>
#include <QImage>
#include <QLabel>
#include <QString>
#include <QScrollArea>
#include <QFloat16>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <memory>

class ImageWidget: public QScrollArea
{
    Q_OBJECT
public:
    ImageWidget(QWidget* parent = nullptr);

    bool setImage(QDir const& image_dir);
    qfloat16 scaleFactor();

    ~ImageWidget();

public slots:
    void fitToContainer();
    void zoomIn();
    void zoomOut();

private:
    QDir m_default_image_dir {QString("../media/images/no-image.jpg")};
    std::shared_ptr<QImage> m_image = nullptr;
    QLabel* m_image_label = nullptr;
    qfloat16 m_scaled_factor = 1.;

    void scaleImage(qfloat16 scaled);
};

#endif // IMAGE_WIDGET_HPP