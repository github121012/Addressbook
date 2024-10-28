#include "mainwindow.h"
#include "qtimer.h"
#include "ui_mainwindow.h"
#include "register.h"
#include "addressbook.h"


#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>


QString name;
QTimer *timer = new QTimer;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    display_dateAndtime();

    connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(btn1_clicked()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(btn2_clicked()));
    ui->pushButton_2->setShortcut(Qt::Key_Return);

    connect(timer,SIGNAL(timeout()),this,SLOT(display_dateAndtime()));
    timer->start(1000);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::get_dateAndtime(QString &date, QString &time)
{
    QDateTime t1 = QDateTime::currentDateTime();

    date = t1.date().toString();
    time = t1.time().toString();
}

void MainWindow::display_dateAndtime()
{
    QString date,time;
    get_dateAndtime(date, time);

    ui->label_3->setText(QString("Date: %1").arg(date));
    ui->lcdNumber->display(time);
}

int MainWindow::connectsqlite()
{
    QString name = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();
    QString pwd;


    if(name == "" || password == "")
    {
        return 0;
    }
    else
    {
        QSqlDatabase a1;
        if(QSqlDatabase::contains("qt_sql_default_connection"))
        {
            a1 = QSqlDatabase::database("qt_sql_default_connection");
        }
        else
        {
            a1 = QSqlDatabase::addDatabase("QSQLITE");

        }

        a1.setDatabaseName("addressbook.sqlite3");
        a1.setPassword("sa");
        a1.open();

        QSqlQuery b1;
        b1.exec(QString("Select * From login Where name = '%1'").arg(name));

        while(b1.next())
        {
            pwd = b1.value(1).toString();
        }

        a1.close();
    }

    if(pwd == password)
    {
        return 1;
    }
    else
        return 0;
}

void MainWindow::btn1_clicked()
{
    Register *w1 = new Register;
    w1->show();
}

void MainWindow::btn2_clicked()
{

    if(connectsqlite())
    {
        Addressbook *w2 = new Addressbook;
        w2->show();
        this->close();
    }

}





