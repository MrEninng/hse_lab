#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QMessageBox"
#include <stdexcept>
#include <QDebug>
#include <cmath>
#include <algorithm>
#include "intervalstable.h"


QString changeOnDoTA(QString exmpl);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->chooseFileButton,SIGNAL(clicked()),this,SLOT(choose_file_button_clicked()));
    connect(ui->infoButton,SIGNAL(clicked()),this,SLOT(info_button_clicked()));
    //connect(ui->intervalsButton,SIGNAL(clicked()),this,SLOT(intervals_button_clicked()));
    connect(ui->resultButton,SIGNAL(clicked()),this,SLOT(result_button_clicked()));
    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBox_changed()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::choose_file_button_clicked()
{   try {
        QString fileName = QFileDialog::getOpenFileName(this,QString::fromUtf8("Choose File"),QDir::currentPath(),"Text(*.txt)");
        if (fileName.length() != 0) {
            // ДАЛЕЕ ОЧИСТКА ЗНАЧЕНИЙ!!!
            clear_data();
            clear_lines();

            ui->fileName->setText(fileName);
            QFile my_file(fileName);
            my_file.open(QIODevice::ReadOnly);
            if (my_file.isOpen())
                while (!my_file.atEnd()) {
                    QString exmpl = changeOnDoTA(my_file.readLine());
                    double obj = exmpl.toDouble();
                    started_data.push_back(obj);
                }

            try {

                set_average_data();

               // qDebug() << max_x << " " << min_x;
                set_square_deviation();
                set_Sx();
    //            for (int i = 0; i < started_data.size(); i++)
    //                qDebug() << started_data[i];
                set_r(); //  КИДАЕТ ВНУТРЕННИЙ ЭКСЕПШЕН!
                set_h();

                fill_intervals();
                set_f();
                set_middle_value();
                set_middle_value2();
                set_average_of_intervals();
                set_si();
                set_n_i();
                set_xi_sq();

                choose_quantile();

//                qDebug() << "BEgin";
//                for (int i = 0;i < started_data.size();i++)
//                    qDebug() << started_data[i];
//                qDebug() << "END";

                set_lines();
                for (int i = 0; i < intervals.size();i++) {
                    for (int j = 0; j < intervals[i].size();j++)
                        qDebug() << intervals[i][j];
                    qDebug() << "end_line";
                }

              //  check_result();

                is_chButton_clicked_flag = true;
                my_file.close();
            } catch(std::exception &e){
                clear_data();
                clear_lines();
                is_chButton_clicked_flag = false;
                QMessageBox::critical(this,"Not Valid!!!",e.what());
            }
        }
        else {
            throw (std::invalid_argument("Choose txt File!"));
        }
    } catch(std::exception& e) {
        clear_data();
        clear_lines();
        is_chButton_clicked_flag = false;
       // QMessageBox::critical(this, "Error!",e.what());
    }

}


QString changeOnDoTA(QString exmpl)
{
    int i = 0;
    while (exmpl[i] != '.' && i < exmpl.length()) {
        if (exmpl[i] == ',')
            exmpl[i] = '.';
        i++;
    }
    return exmpl;
}

void MainWindow::set_average_data()
{

    double sum = 0;
//    for (int i = 0; i < started_data.size(); i++)
//        qDebug() << started_data[i];
    if (started_data.size() > 0) {
        min_x = started_data[0];
        max_x = started_data[0];
    }
    for (int i = 0; i < started_data.size(); i++) {
        sum += started_data[i];
        if (started_data[i] > max_x)
            max_x = started_data[i];
        if (started_data[i] < min_x)
            min_x = started_data[i];
    }
    average_data = sum / static_cast<double>(started_data.size());

 //   qDebug() << average_data << " " << sum;
}

void MainWindow::set_square_deviation()
{
    double sum = 0;
    //qDebug() << started_data.size();
    for (int i = 0; i < started_data.size();i++)
        sum += (started_data[i] - average_data) * (started_data[i] - average_data);
    S = sqrt(sum/static_cast<double>(started_data.size() - 1));
    //qDebug() << S;
}

void MainWindow::set_Sx()
{
    Sx = S/static_cast<double>(sqrt(started_data.size()));
  //  qDebug() << Sx << " " << S;
}

void MainWindow::set_r()
{
    if (started_data.size() < 40)
        throw (std::invalid_argument("This sample is not appropriate, "
                                     "because the number of values less than 40"));
    else if (started_data.size() >= 40 && started_data.size() <= 100)
        r = 9;
    else if (started_data.size() > 100 && started_data.size() <= 500)
        r = 9;
    else if (started_data.size() > 500 && started_data.size() <= 1000)
        r = 15;
    else if (started_data.size() > 1000 && started_data.size() <= 10000)
        r = 20;
   // qDebug() << r;

}

void MainWindow::clear_data()
{
    max_x = 0;
    min_x = 0;
    average_data = 0;
    S = 0;
    Sx = 0;
    started_data = {};
    h = 0;
    r = 0;
    f = 0;
    intervals = {};
    middle_value2 = {};
    middle_value = {};
    average_data_of_intervals = {};
    xi_sq = {};
    n_i = {};
    koef = 0;
    xi = 0;
    Si = {};



}

void MainWindow::set_lines()
{
    ui->average_line->setText(QString::number(average_data));
    ui->Sx_line->setText(QString::number(Sx));
    ui->max_x_line->setText(QString::number(max_x));
    ui->min_x_line->setText(QString::number(min_x));
    ui->r_line->setText(QString::number(r));
    ui->h_line->setText(QString::number(h));
    ui->n_line->setText(QString::number(started_data.size()));
    ui->S_line->setText(QString::number(S));
    ui->f_line->setText(QString::number(f));
    ui->xi_line->setText(QString::number(xi));
    ui->quantile_line->setText(QString::number(quantile));
}

void MainWindow::clear_lines()
{
    ui->average_line->setText("");
    ui->Sx_line->setText("");
    ui->max_x_line->setText("");
    ui->min_x_line->setText("");
    ui->r_line->setText("");
    ui->h_line->setText("");
    ui->n_line->setText("");
    ui->S_line->setText("");
    ui->f_line->setText("");
    ui->xi_line->setText("");
    ui->quantile_line->setText("");
}

void MainWindow::fill_intervals()
{
    std::sort(started_data.begin(), started_data.end());
//    for (int i = 0; i < started_data.size();i++)
//        qDebug() << started_data[i];
    int i = 0;
    double interval_value = min_x + h;
   // qDebug() << max_x << "KEK" << h << "kek" << min_x;
    try {

        //qDebug() << started_data.size();
        while (i < started_data.size() && interval_value <= max_x ) {
            QVector<double> obj;
            while(i < started_data.size() && started_data[i] < interval_value ) {
                obj.push_back(started_data[i]);
                i++;
            }
//            qDebug() << i;
            intervals.push_back(obj);

            if (i < started_data.size() && interval_value <= max_x)
                interval_value += h;
        }



    } catch(std::exception& e) {
        QMessageBox::critical(this,"Error!!!",e.what());
    }

//    qDebug() << "New Size" << intervals.size();
//    for (int i = 0; i < intervals.size(); i++)
//        qDebug() << intervals[i].size();
}
void MainWindow::set_middle_value()
{
    for (int i = 0; i < intervals.size(); i++) {
        if (intervals[i].size() % 2 == 1) {
            middle_value.push_back(intervals[i][(intervals[i].size() - 1 + 1 ) / 2.0]);//??
        } else {
            double r_value = 0,l_value = 0;
            l_value = intervals[i][intervals[i].size() / 2 - 1];
            r_value = intervals[i][intervals[i].size() / 2];
            double value = (l_value + r_value)/2.0;

            middle_value.push_back(value);
        }
    }
}





void MainWindow::info_button_clicked()
{
    QMessageBox::information(this,"List of Parametrs","Max x - Максимальное х \n"
                             "Min x - Минимальное х\n"
                             "r - Количество интервалов\n"
                             "h - Ширина интервала\n"
                             "Average value - Среднее значение\n"
                             "f - Количество степеней свободы\n"
                             "S - среднеквадратическое отклонение группы\n"
                             "Sx - средне квадратическое отклонение среднего арифметического\n"
                             "a - вероятность\n"
                             "Quantile - квантиль - распределения\n"
                             "X^2 - сумма Xi^2 по всем интервалам  ");
}


void MainWindow::intervals_button_clicked()
{
    if (is_chButton_clicked_flag){
        IntervalsTable i;
        i.setFixedSize(i.size());
        i.show();
    } else {
        QMessageBox::critical(this,"No values!", "Choose correct file with values");
    }
}

//БЕСПОЛЕЗНАЯ ФУНКЦИЯ!
void MainWindow::set_average_of_intervals()
{
    double obj = 0;
    for (int i = 0; i < intervals.size(); i++) {
        for (int j = 0; j < intervals[i].size(); j++)
            obj +=intervals[i][j];
        obj = obj / static_cast<double>(intervals[i].size());
        average_data_of_intervals.push_back(obj);
        obj = 0;
    }
}

void MainWindow::set_n_i()
{
    //set_average_of_intervals();
    set_koef();
    //qDebug() << " Koef " << koef;
    //qDebug() << " KOEF " << koef << " S " << S << "AVERAGE" << average_data;
    for (int i = 0; i < intervals.size(); i++) {
        double obj = 0;
       // qDebug() << " middle_value2[i] " << middle_value2[i]  << "Average_data" << average_data;
//        double x1 = 0;
//        x1 = fabs(middle_value2[i] - average_data)
        double x = ( (middle_value2[i] - average_data/*average_data_of_intervals[i]*/) / S /*Si[i]*/); // ПО ФОРМУЛЕ АБС НЕ НУЖЕН!!

        double f =sqrt(exp(-1 * x * x ) ) * (1.0 / static_cast<double>((sqrt(2.0 * 3.141592653) ) ) );
        double koef1 = intervals[i].size() * h / Si[i];
        obj = koef/*1*/ * f;
       // qDebug()  << " X " << x << " F " << f << "OBJ" << static_cast<int>(obj) << " I: " << i;

        n_i.push_back(static_cast<int>(obj) );
        f = 0;
        x = 0;
    }
//    for (int i = 0; i < n_i.size(); i++)
//        qDebug() << n_i[i] << "   " << intervals[i].size();
}

void MainWindow::set_xi_sq()
{
    double obj = 0;
    for (int i = 0; i < intervals.size(); i++) {
        qDebug() << intervals[i].size() << "<- количество на данном интервале  ||  предполагаемое -> " << n_i[i] << "i: " << i;

        // ПРИВЕЛ К ТИПУ ИНТА Т,Е, ВЗЯЛ ЦЕЛУЮ ЧАСТЬ
        obj = ((intervals[i].size() - n_i[i] ) * (intervals[i].size() - n_i[i] )) / static_cast<double>(n_i[i]);
        xi_sq.push_back(obj);
        qDebug() << " xi_sq " << obj;
        obj = 0;
    }
 //   qDebug() << xi_sq.size() << " " << r << " ni" << n_i.size() << "intervals" << intervals.size();
    for (int  i = 0; i < xi_sq.size(); i++)
        xi += xi_sq[i];
    //qDebug() << "OTVET!" << xi;
}

void MainWindow::set_middle_value2()
{
    double begin_i = min_x;
    double end_i = min_x + h;
    for (int i = 0; i < intervals.size(); i++)
    {
        double obj = (end_i + begin_i)/2.0;
        middle_value2.push_back(obj);
        begin_i += h;
        end_i +=h;
    }
//    for (int i = 0; i < intervals.size(); i++) {
//        qDebug() << middle_value2[i];
//    }

}

void MainWindow::choose_quantile()
{
    QString obj;
    obj = changeOnDoTA(ui->comboBox->currentText());
    a = obj.toDouble();
    //a = ui->comboBox->currentText().toDouble();
    //qDebug() << " a " << a;
    int i = 0;
    bool flag = true;
    while (i < quantile_table[0].size() && flag) {
        if (a == quantile_table[0][i])
                flag = false;
      //  qDebug() << "cur" << quantile_table[0][i];
        i++;
    }
    quantile = quantile_table[f][i - 1];
   // qDebug() << "qu = " << quantile;
}

void MainWindow::check_result()
{
    if (xi <= quantile)
        QMessageBox::information(this,"Result","Данная выборка принадлежит нормальному распределению");
    else
        QMessageBox::information(this,"Result","Данная выборка НЕ принадлежит нормальному распределению");

}

void MainWindow::result_button_clicked()
{
    if (is_chButton_clicked_flag)
        check_result();
    else
        QMessageBox::warning(this,"Error!","Choose txt file!");

}

void MainWindow::comboBox_changed()
{

   // qDebug() << f << "KEK" << quantile;
    if (is_chButton_clicked_flag) {
        choose_quantile();
        ui->quantile_line->setText(QString::number(quantile));
    }
}

void MainWindow::set_si()
{
    double obj = 0;
    for (int i = 0; i < intervals.size();i++) {
        for(int j = 0; j < intervals[i].size(); j++)
            obj += (intervals[i][j] - average_data_of_intervals[i] ) * (intervals[i][j] - average_data_of_intervals[i]);
        double obj2 = sqrt(obj/(intervals[i].size() - 1));
        Si.push_back(obj2);
        obj = 0;
    }
}
