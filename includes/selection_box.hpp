#ifndef SELECTION_BOX_HPP
#define SELECTION_BOX_HPP

#include <QComboBox>
#include <QWheelEvent>

class SelectionBox: public QComboBox
{
public:
    SelectionBox(QWidget* parent = nullptr);

    ~SelectionBox();

protected:
    void wheelEvent(QWheelEvent* event) override;

};

#endif // SELECTION_BOX_HPP