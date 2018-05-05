#include "widget.h"
#include "ui_widget.h"
#include <QFont>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QFont font1("lishu-16");
    ui->label_3->setFont(font1);
    ui->label_3->setText("Á¥Êé16");;


    QFont font2("lishu-20");
    ui->label_4->setFont(font2);
    ui->label_4->setText("Á¥Êé20");;


    QFont font3("lishu-24");
    ui->label_5->setFont(font3);
     ui->label_5->setText("Á¥Êé24");;

    QFont font4("lishu-28");
    ui->label_6->setFont(font4);
    ui->label_6->setText("Á¥Êé28");;


    QFont font5("lishu-32");
    ui->label_7->setFont(font5);
     ui->label_7->setText("Á¥Êé32");;

    QFont font6("lishu-36");
    ui->label_8->setFont(font6);
    ui->label_8->setText("Á¥Êé36");;



    QFont font7("youyuan-38");
    ui->label_9->setFont(font7);
    ui->label_9->setText("Ó×Ô²38");;


    QFont font8("youyuan-42");
    ui->label_10->setFont(font8);
    ui->label_10->setText("Ó×Ô²42");;


    QFont font9("youyuan-44");
    ui->label_11->setFont(font9);
     ui->label_11->setText("Ó×Ô²44");;

    QFont font10("youyuan-48");
    ui->label_12->setFont(font10);
    ui->label_12->setText("Ó×Ô²48");;


    QFont font11("youyuan-52");
    ui->label_13->setFont(font11);
     ui->label_13->setText("Ó×Ô²52");;

    QFont font12("youyuan-56");
    ui->label_14->setFont(font12);
    ui->label_14->setText("Ó×Ô²56");;


}

Widget::~Widget()
{
    delete ui;
}
