#include "widget.h"
#include "ui_widget.h"
#include <QLabel>
#include <QPixmap>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    jpg_name[0]="1.jpg";
    jpg_name[1]="2.jpg";
    jpg_name[2]="3.jpg";
    jpg_name[3]="4.jpg";

    ui->label->setPixmap(QPixmap("image/1.jpg"));

    i=1;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_1_clicked()
{
     i--;
    if(i==0)
        i=3;
     ui->label->setPixmap(QPixmap("image/"+jpg_name[i]));
}

void Widget::on_pushButton_2_clicked()
{
    i++;
   if(i==4)
       i=0;
    ui->label->setPixmap(QPixmap("image/"+jpg_name[i]));
}
