#ifndef ADDNEW_H
#define ADDNEW_H

#include <QMainWindow>

namespace Ui {
class AddNew;
}

class AddNew : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddNew(QWidget *parent = nullptr);
    ~AddNew();
    void AddSqlite();
    void modifySqlite();

private slots:
    void btn1_clicked();
    void btn2_clicked();
    void btn3_clicked();
    void display_dateAndtime();

private:
    Ui::AddNew *ui;
};

#endif // ADDNEW_H
