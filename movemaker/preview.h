#ifndef PREVIEW_H
#define PREVIEW_H
#include <QWidget>

class Preview : public QWidget
{
    Q_OBJECT

public:
    Preview(QWidget *parent=NULL);
    ~Preview();
};

#endif // PREVIEW_H
