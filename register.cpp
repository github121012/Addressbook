#include "register.h"
#include "ui_register.h"
#include "mainwindow.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTimer>

extern QTimer *timer;

Register::Register(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    display_dateAndtime();
    connect(ui->pushButton_1, SIGNAL(pressed()), this, SLOT(btn1_clicked()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(btn2_clicked()));
    ui->pushButton_1->setShortcut(Qt::Key_Return);

    connect(timer,SIGNAL(timeout()),this,SLOT(display_dateAndtime()));
    timer->start(1000);


}

Register::~Register()
{
    delete ui;
}

void Register::display_dateAndtime()
{
    MainWindow e1;
    QString date,time;
    e1.get_dateAndtime(date, time);
    ui->label_4->setText(QString("Date: %1").arg(date));
    ui->lcdNumber->display(QString("Date: %1").arg(time));

}

void Register::RegisterSqlite()
{
    QString name = ui->lineEdit->text();
    QString pwd = ui->lineEdit_2->text();

    if(name == "" || pwd == "")
    {
        ui->label_3->setText("name or password can't be null!");
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
            a1 = QSqlDatabase::addDatabase("SQLITECIPHER");

        }
        a1.setDatabaseName("addressbook.sqlite3");
        a1.setPassword("sa");
        a1.open();

        QSqlQuery b1,b2;
        b1.exec("Select * From login");
        b2.exec(QString("Select * From login Where name = '%1'; ").arg(name));

        if(!b1.first())
        {

            b1.exec("Create Table login(name string primary key unique not null, password string not null);");
            b1.exec(QString("Insert Into login Values('%1', '%2')").arg(name, pwd));
            ui->label_3->setText("successfully11 registered!");
        }
        else if(b2.first())
        {
            ui->label_3->setText("name exists!!");
        }
        else
        {
            b1.exec(QString("Insert Into login Values('%1', '%2')").arg(name, pwd));
            ui->label_3->setText("successfully registered!");
        }

        a1.close();
    }

}

void Register::btn1_clicked()
{
    RegisterSqlite();
}

void Register::btn2_clicked()
{
    this->close();

}

