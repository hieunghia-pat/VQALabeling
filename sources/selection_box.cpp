#include "selection_box.hpp"

#include <QComboBox>
#include <QWheelEvent>

SelectionBox::SelectionBox(QWidget* parent)
    : QComboBox(parent)
{

}

void SelectionBox::wheelEvent(QWheelEvent* /*event*/)
{
    // ignore this event
}

SelectionBox::~SelectionBox()
{

}