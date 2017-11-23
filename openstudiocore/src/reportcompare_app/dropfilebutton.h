#ifndef DROPFILEBUTTON_H
#define DROPFILEBUTTON_H

#include <QWidget>
#include <QPushButton>

class DropFileButton : public QPushButton
{
    Q_OBJECT

public:
    DropFileButton(QWidget *parent=0);
    ~DropFileButton();
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

signals:
    void eventDropFilePath(const QString& filePath);
};

#endif // DROPFILEBUTTON_H
