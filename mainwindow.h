#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int connectsqlite();
    void get_dateAndtime(QString &date, QString &time);



private slots:
    void btn1_clicked();
    void btn2_clicked();
    void display_dateAndtime();


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
