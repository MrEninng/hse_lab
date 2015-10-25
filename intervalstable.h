#ifndef INTERVALSTABLE_H
#define INTERVALSTABLE_H

#include <QDialog>

namespace Ui {
class IntervalsTable;
}

class IntervalsTable : public QDialog
{
    Q_OBJECT

public:
    explicit IntervalsTable(QWidget *parent = 0);
    ~IntervalsTable();

private:
    Ui::IntervalsTable *ui;
};

#endif // INTERVALSTABLE_H
