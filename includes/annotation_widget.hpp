#ifndef ANNOTATION_WIDGET_HPP
#define ANNOTATION_WIDGET_HPP

#include "annotation_box.hpp"

#include <QList>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QJsonArray>

#include <memory>

static qsizetype total_initial_annotations = 3;

class AnnotationWidget: public QScrollArea
{
    Q_OBJECT
public:
    AnnotationWidget(QWidget* parent = nullptr);

    std::shared_ptr<QJsonArray> data() const;
    void setData(QJsonArray const& data);
    QVBoxLayout* layout();

    ~AnnotationWidget();

signals:
    void haveAdjusted();

public slots:
    void addAnnotation(qsizetype ith);
    void deleteAnnotation(qsizetype ith);

private:
    QVBoxLayout* m_layout = nullptr;

    qint16 const m_defaultNumOfAnns = total_initial_annotations;
    QList<AnnotationBox*> m_annotation_boxes {};

    QGroupBox* m_container = nullptr;
};

#endif // ANNOTATION_WIDGET_HPP