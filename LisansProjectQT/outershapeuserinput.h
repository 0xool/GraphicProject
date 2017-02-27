#ifndef OUTERSHAPEUSERINPUT_H
#define OUTERSHAPEUSERINPUT_H

#include <QDialog>

namespace Ui {
class OuterShapeUserInput;
}

class OuterShapeUserInput : public QDialog
{
    Q_OBJECT

public:
    explicit OuterShapeUserInput(QWidget *parent = 0);
    ~OuterShapeUserInput();

private slots:
    void on_pushButton_pressed();

private:
    Ui::OuterShapeUserInput *ui;
};

#endif // OUTERSHAPEUSERINPUT_H
