#ifndef REGISTER_H
#define REGISTER_H

#include <QMainWindow>

namespace Ui {
class Register;
}

class Register : public QMainWindow
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

private slots:
    void btn1_clicked();
    void btn2_clicked();
    void display_dateAndtime();
    void RegisterSqlite();

private:
    Ui::Register *ui;
};

#endif // REGISTER_H
