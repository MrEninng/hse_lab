#include "intervalstable.h"
#include "ui_intervalstable.h"

IntervalsTable::IntervalsTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IntervalsTable)
{
    ui->setupUi(this);
    ui->intervalsTable->insertRow(ui->intervalsTable->rowCount());

}

IntervalsTable::~IntervalsTable()
{
    delete ui;
}
