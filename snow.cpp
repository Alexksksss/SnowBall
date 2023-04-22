#include "snow.h"
#include "ui_snow.h"

snow::snow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::snow)
{
    ui->setupUi(this);
}

snow::~snow()
{
    delete ui;
}
