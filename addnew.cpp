#include "addnew.h"
#include "ui_addnew.h"
#include "mainwindow.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTimer>
#include <QFileDialog>
#include <QFile>

extern QTimer *timer;
QString filename;
extern QString s1;

AddNew::AddNew(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddNew)
{
    ui->setupUi(this);
    display_dateAndtime();
    ui->pushButton->setShortcut(Qt::Key_Return);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(btn1_clicked()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(btn2_clicked()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(btn3_clicked()));
    connect(timer,SIGNAL(timeout()),this,SLOT(display_dateAndtime()));
    timer->start(1000);
}

AddNew::~AddNew()
{
    delete ui;
}

void AddNew::display_dateAndtime()
{
    MainWindow e1;
    QString date,time;
    e1.get_dateAndtime(date, time);
    ui->label_6->setText(QString("Date: %1").arg(date));
    ui->lcdNumber->display(time);
}

void AddNew::AddSqlite()
{
    QString name = ui->lineEdit->text();
    QString phone = ui->lineEdit_2->text();
    QString email = ui->lineEdit_3->text();
    QString address = ui->lineEdit_4->text();

    QString contact = QString("'%1', '%2', '%3', '%4'").arg(name, phone, email, address);

    if(name == "" || phone == "")
    {
        ui->label_5->setText("name or phone can't be null!!");
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

        QSqlQuery b1;
        b1.exec("Select * From contact");

        if(!b1.first())
        {
            b1.exec("Create Table contact(name text  primary key not null, phone text unique not null, email text, address text)");
        }

        b1.exec(QString("Select * "
                        "From contact "
                        "Where phone = '%1' ").arg(phone));

        if(b1.first())
        {
            ui->label_5->setText("name and phone exists!!");
            ui->label_5->setStyleSheet("background: green");
        }
        else
        {
            b1.exec(QString("Insert Into contact"
                           "(name, phone, email, address)"
                            "Values(%1)").arg(contact));

            if(!filename.isEmpty()){
            QStringList r1 = filename.split("/");
            QStringList r2 = r1.last().split(".");
            QString r3 = r2.last();
            qDebug()<<r3;
            QFile currentfile(filename);

            QFileInfo f1("./pic/");
            if(!f1.isDir())
            {
                QDir *path = new QDir;
                path->mkpath("./pic/");
            }

            currentfile.copy(QString("./pic/%1.%2").arg(name, r3));}
            ui->label_5->setText("successfully added!");
        }

        a1.close();
    }
}

void AddNew::btn1_clicked()
{
    AddSqlite();
}

void AddNew::btn2_clicked()
{   
    this->close();
}


void AddNew::btn3_clicked()
{
    filename = QFileDialog::getOpenFileName(this, "choose avatar", "*.png;; *.jpg");
    if(!filename.isEmpty())
    {
        ui->lineEdit_5->setText(filename);
    }
}

void AddNew::modifySqlite()
{
    qDebug()<<"called";
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

    QSqlQuery b1;
    b1.exec(QString("Select * From contact where name = '%1' ").arg(s1));

    while(b1.next())
    {
        QString name = b1.value(0).toString();
        QString phone = b1.value(1).toString();
        QString email = b1.value(2).toString();
        QString address = b1.value(3).toString();
        ui->lineEdit->setPlaceholderText(name);
        ui->lineEdit_2->setPlaceholderText(phone);
        ui->lineEdit_3->setPlaceholderText(email);
        ui->lineEdit_4->setPlaceholderText(address);
    }

    a1.close();



}


