#include "addressbook.h"
#include "qlineedit.h"
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
#include <QInputDialog>

extern QString name;
extern QTimer *timer;
QString s1;
QString a1; //sort order


Addressbook::Addressbook(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Addressbook)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Hello! Welcome to %1's addressbook").arg(name));
    display_dateAndtime();
    ListSqlite(1);
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
        a1 = QSqlDatabase::addDatabase("QSQLITE");

    }
    a1.setDatabaseName("addressbook.sqlite3");
    a1.setPassword("sa");
    a1.open();

    QSqlQuery b1(a1);
    if(q1)
    {
        b1.exec("Select * From contact order by date ASC");
    }
    else
    {
        b1.exec("Select * From contact order by name ASC");
    }

    qint64 i=1;
    while(b1.next())
    {
        QString name = b1.value(0).toString();
        QString phone = b1.value(1).toString();
        QString email = b1.value(2).toString();
        QString address = b1.value(3).toString();
        QString date = b1.value(4).toString();


        QString a = QString("contact: %1").arg(i) + '\n'
                   +QString("name: %1   ").arg(name) + '\n'
                   +QString("phone: %1 ").arg(phone) + '\n'
                   +QString("e-mail: %1").arg(email) + '\n'
                   +QString("address: %1").arg(address)+ '\n'
                   +QString("date: %1").arg(date);

        ++i;

        QListWidgetItem *add_item = new QListWidgetItem(ui->listWidget);
        add_item->setSizeHint(QSize(400,140));
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

        ui->listWidget->setIconSize(QSize(140,140));
        add_item->setText(a);
        if(i%2)
        {
            add_item->setBackground((Qt::lightGray));
        }
        else  add_item->setBackground(Qt::color0);

        ui->listWidget->addItem(add_item);
    }
    a1.close();
}

void Addressbook::QuerySqlite()
{
    QInputDialog m1;
    m1.setWindowTitle("Search for Contact");
    m1.setLabelText("Input a name:");
    m1.exec();
    s1 = m1.textValue();
    m1.setAttribute(Qt::WA_DeleteOnClose);

    qint8 j = 0;
    bool f1 = true;
    while(f1)
    {
        for(int i = 0; i < ui->listWidget->count();i++)
        {
            QString s2 =  ui->listWidget->item(i)->text();
            QStringList s3 = s2.split("\n");
            QString s4 = s3.at(1);
            QString s5 = s3.at(0);
            QStringList s6 = s4.split(":");
            s4 = s6.last().simplified();  //name search for
            s6 = s5.split(":");
            s5 = s6.last().simplified();  //index

            if(s1 == s4)
            {
                j = s5.toInt();
                ui->listWidget->setCurrentItem(ui->listWidget->item(j));
                ui->listWidget->scrollToItem(ui->listWidget->item(j));
                f1 = false;
            }
        }
    }
}

void Addressbook::on_pushButton_clicked()
{
    AddNew *w3 = new AddNew;
    w3->show();
}

void Addressbook::on_pushButton_2_clicked()
{
    QuerySqlite();

}

void Addressbook::on_pushButton_3_clicked()
{
    this->close();
}

void Addressbook::on_pushButton_4_clicked()
{
    MainWindow *w = new MainWindow;
    w->show();
    this->close();
}

void Addressbook::on_pushButton_5_clicked()
{

    a1 = ui->comboBox->currentText();
    if(a1 == "Time")
    {
        ListSqlite(1);
    }
    else ListSqlite(0);

}

void Addressbook::on_comboBox_textActivated(const QString &arg1)
{
    QFileInfo f1("./pic/");
    if(!f1.isDir())
    {
        QDir *path = new QDir;
        path->mkpath("./pic/");
    }

    if(arg1 == "Time")
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
        s1 = s2.at(2);
        s2 = s1.split(":");
        s1 = s2.last().simplified();
    }

    AddNew *w = new AddNew;
    w->setWindowTitle("Modify Contact");
    w->modifySqlite();
    w->show();
}

