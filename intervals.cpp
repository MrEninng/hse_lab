#include "intervals.h"
#include "ui_intervals.h"

interval::interval(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::interval)
{
    ui->setupUi(this);
}

interval::~interval()
{
    delete ui;
}
