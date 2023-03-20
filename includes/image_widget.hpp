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

class ImageWidget : public QScrollArea
{
    Q_OBJECT
public:
    ImageWidget(QWidget *parent = nullptr);

    bool setImage(QDir const &image_dir);

    QSize getImageSize();

    ~ImageWidget();

signals:
    void nextImage();
    void backImage();

public slots:
    void rotateLeft();
    void rotateRight();
    void fitToContainer();
    void zoomIn();
    void zoomOut();
    void resetScaling();

private:
    std::shared_ptr<QImage> m_image = nullptr;
    QLabel *m_image_label = nullptr;

    void scaleImage(qfloat16 scaled);
};

#endif // IMAGE_WIDGET_HPP
