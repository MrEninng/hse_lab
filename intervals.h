#ifndef INTERVALS_H
#define INTERVALS_H

#include <QDialog>

namespace Ui {
class interval;
}

class interval : public QDialog
{
    Q_OBJECT

public:
    explicit interval(QWidget *parent = 0);
    ~interval();

private:
    Ui::interval *ui;
};

#endif // INTERVALS_H
