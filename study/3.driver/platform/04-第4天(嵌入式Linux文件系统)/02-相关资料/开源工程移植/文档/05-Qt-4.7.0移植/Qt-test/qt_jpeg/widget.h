#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    QString jpg_name[4];


    int i;

private:
    Ui::Widget *ui;

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_1_clicked();
};

#endif // WIDGET_H
