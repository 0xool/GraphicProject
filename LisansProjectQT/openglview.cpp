#include "openglview.h"
#include "ui_openglview.h"

openGlView::openGlView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::openGlView)
{
    ui->setupUi(this);
}
//=============================================================================================================================
openGlView::~openGlView()
{
    delete ui;
}
