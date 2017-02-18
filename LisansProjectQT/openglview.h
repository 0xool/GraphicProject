#ifndef OPENGLVIEW_H
#define OPENGLVIEW_H

#include <QDialog>

namespace Ui {
class openGlView;
}

class openGlView : public QDialog
{
    Q_OBJECT

public:
    explicit openGlView(QWidget *parent = 0);
    ~openGlView();

private:
    Ui::openGlView *ui;
};

#endif // OPENGLVIEW_H
