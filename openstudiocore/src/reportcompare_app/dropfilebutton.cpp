#include "dropfilebutton.h"

#include <QDropEvent>
#include <QMimeData>

DropFileButton::DropFileButton(QWidget *parent)
    : QPushButton(parent)
{
    setAcceptDrops(true);
}

DropFileButton::~DropFileButton()
{

}

void DropFileButton::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasText())
            event->acceptProposedAction();
}

void DropFileButton::dropEvent(QDropEvent *event)
{
    eventDropFilePath(event->mimeData()->text());
    event->acceptProposedAction();
}

