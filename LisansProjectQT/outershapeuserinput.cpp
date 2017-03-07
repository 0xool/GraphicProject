#include "outershapeuserinput.h"
#include "ui_outershapeuserinput.h"

OuterShapeUserInput::OuterShapeUserInput(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OuterShapeUserInput)
{
    ui->setupUi(this);
}
//==========================================================================================================================================
OuterShapeUserInput::~OuterShapeUserInput()
{
    delete ui;
}
//==========================================================================================================================================
void OuterShapeUserInput::on_pushButton_pressed()
{

    this->close();

}
