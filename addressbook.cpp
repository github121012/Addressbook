#include "addressbook.h"
#include "ui_addressbook.h"
#include "addnew.h"
#include "mainwindow.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTimer>
#include <QDir>
#include <QFileInfo>
#include <QMenu>
#include <QMessageBox>

extern QString name;
extern QTimer *timer;
QString s1;


Addressbook::Addressbook(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Addressbook)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Hello! Welcome to %1's addressbook").arg(name));
    display_dateAndtime();
    ListSqlite(1);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(btn1_clicked()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(btn2_clicked()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(btn3_clicked()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(btn4_clicked()));
    connect(ui->pushButton_5, SIGNAL(released()), this, SLOT(btn5_clicked()));

    connect(timer,SIGNAL(timeout()),this,SLOT(display_dateAndtime()));
    timer->start(1000);

}

Addressbook::~Addressbook()
{
    delete ui;
}

void Addressbook::display_dateAndtime()
{
    MainWindow e1;
    QString date,time;
    e1.get_dateAndtime(date, time);
    ui->label->setText(QString("Date: %1").arg(date));
    ui->lcdNumber->display(time);
}

void Addressbook::ListSqlite(bool q1)
{
    ui->listWidget->clear();
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

    QSqlQuery b1(a1);
    if(q1)
    {
        b1.exec("Select * From contact");
    }
    else
    {
        b1.exec("Select * From contact order by name");
    }


    while(b1.next())
    {
        QString name = b1.value(0).toString();
        QString phone = b1.value(1).toString();
        QString email = b1.value(2).toString();
        QString address = b1.value(3).toString();

        QString a = QString("name: %1   ").arg(name) + '\n'
                   +QString("phone: %2 ").arg(phone) + '\n'
                   +QString("e-mail: %3").arg(email) + '\n'
                   +QString("address: %4").arg(address);

        QListWidgetItem *add_item = new QListWidgetItem(ui->listWidget);
        add_item->setSizeHint(QSize(400,100));
        add_item->setFont(QFont("Source Code Pro",14));

        QFileInfo t1(QString("./pic/%1.png").arg(name));
        if(t1.isFile())
        {
            add_item->setIcon(QIcon(QString("./pic/%1.png").arg(name)));
        }

        t1.setFile(QString("./pic/%1.jpg").arg(name));
        if(t1.isFile())
        {
            add_item->setIcon(QIcon(QString("./pic/%1.jpg").arg(name)));
        }

        ui->listWidget->setIconSize(QSize(100,90));
        add_item->setText(a);
        ui->listWidget->addItem(add_item);
    }


    a1.close();

}

void Addressbook::QuerySqlite()
{

}

void Addressbook::btn1_clicked()
{
    AddNew *w3 = new AddNew;
    w3->show();
}

void Addressbook::btn2_clicked()
{
    QuerySqlite();

}

void Addressbook::btn3_clicked()
{
    this->close();
}

void Addressbook::btn4_clicked()
{
    MainWindow *w = new MainWindow;
    w->show();
    this->close();
}

void Addressbook::btn5_clicked()
{

    this->setWindowFlags( Qt::WindowStaysOnTopHint);
    this->show();

}

void Addressbook::on_comboBox_textActivated(const QString &arg1)
{
    QFileInfo f1("./pic/");
    if(!f1.isDir())
    {
        QDir *path = new QDir;
        path->mkpath("./pic/");
    }

    if(arg1 == "time")
        ListSqlite(1);
    else ListSqlite(0);
}

void Addressbook::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu  *m1 = new QMenu(ui->listWidget);

    m1->setStyleSheet("QMenu{background-color: rgb(89,87,87); border: 5px solid rgb(255,255,255) ;}\
                       QMenu::item{color: rgb(0,0,0);border: 1px solid rgb(0,0,0);padding: 5px;background-color: #ffffff;}\
                       QMenu:item:hover,QMenu:item:selected{background-color: rgb(198,184,255);}");

    QFont f1;
    f1.setPointSize(10);
    m1->setFont(f1);

    QAction  *d1 = new QAction(tr("Delete   "), this);
    QAction  *mod = new QAction(tr("Modify  "), this);

    connect(d1, SIGNAL(triggered()), this, SLOT(deleteSqlite()));
    connect(mod, SIGNAL(triggered()), this, SLOT(modify()));


    m1->addAction(d1);
    m1->addAction(mod);
    m1->popup(ui->listWidget->mapToGlobal(pos));

}

void Addressbook::deleteSqlite()
{
    QList<QListWidgetItem*> items = ui->listWidget->selectedItems();

    foreach(QListWidgetItem* var, items)
    {
        s1 = var->text();
        QStringList s2 = s1.split("\n");
        s1 = s2.first();
        s2 = s1.split(":");
        s1 = s2.last().simplified();
    }


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
    b1.exec(QString("Delete From contact where name = '%1' ").arg(s1));


    a1.close();


}

void Addressbook::modify()
{
    QList<QListWidgetItem*> items = ui->listWidget->selectedItems();

    foreach(QListWidgetItem* var, items)
    {
        s1 = var->text();
        QStringList s2 = s1.split("\n");
        s1 = s2.first();
        s2 = s1.split(":");
        s1 = s2.last().simplified();
    }

    AddNew *w = new AddNew;
    w->setWindowTitle("Modify Contact");
    w->show();
    w->modifySqlite();
}

