#include "usage.h"
#include "ui_usage.h"

Usage::Usage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Usage)
{
    ui->setupUi(this);
}

Usage::~Usage()
{
    delete ui;
}
