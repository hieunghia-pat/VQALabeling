#ifndef ANNOTATION_BOX_HPP
#define ANNOTATION_BOX_HPP

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QJsonObject>
#include <QPushButton>
#include <QFont>

class AnnotationBox: public QGroupBox
{
    Q_OBJECT
public:
    // method
    AnnotationBox(qsizetype ith, QWidget* container, QWidget *parent = nullptr);

    std::shared_ptr<QJsonObject> annotation();
    void setAnnotation(QJsonObject const& data);

    qint16 index();
    void setIndex(qint16 index);

    bool isEmpty();

    ~AnnotationBox();

    // attributes
    QLineEdit* m_captionLineEdit = nullptr;
    QComboBox* m_captionComboBox = nullptr;

    QPushButton* m_addButton = nullptr;
    QPushButton* m_delButton = nullptr;

    QGroupBox* m_captionGroup = nullptr;

    QVBoxLayout* m_captionLayout = nullptr;
    QHBoxLayout* m_buttonLayout = nullptr;
    QVBoxLayout* m_layout = nullptr;

    QJsonObject m_currentAnnotation;

    QFont* font = nullptr;
    QFont* boldFont = nullptr;

signals:
    void contentChanged();

public slots:
    void handleCaptionChanged(QString const& caption);
    void handleLabelChanged(QString const& label);

private:
    qint16 m_index;

};

#endif // ANNOTATION_BOX_HPP
