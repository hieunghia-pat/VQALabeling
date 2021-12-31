#ifndef ANNOTATION_WIDGET_HPP
#define ANNOTATION_WIDGET_HPP

#include "annotation_box.hpp"

#include <QList>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QJsonArray>

#include <memory>

class AnnotationWidget: public QScrollArea
{
public:
    AnnotationWidget(QWidget* parent = nullptr);

    std::shared_ptr<QJsonArray> data() const;
    void setData(QJsonArray const& data) const;
    QVBoxLayout* layout();

    ~AnnotationWidget();

public slots:
//    void addAnnotation();


private:
    QVBoxLayout* m_layout = nullptr;

    qint16 const m_defaultNumOfAnns = 5;
    QList<AnnotationBox*> m_annotation_boxes;

    QGroupBox* m_container = nullptr;

//    QPushButton* m_addButton = nullptr;
};

#endif // ANNOTATION_WIDGET_HPP