#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include <QMainWindow>

namespace Ui {
class Addressbook;
}

class Addressbook : public QMainWindow
{
    Q_OBJECT

public:
    explicit Addressbook(QWidget *parent = nullptr);
    ~Addressbook();

private slots:
    void display_dateAndtime();
    void btn1_clicked();
    void btn2_clicked();
    void btn3_clicked();
    void btn4_clicked();
    void btn5_clicked();
    void QuerySqlite();
    void deleteSqlite();
    void modifySqlite();

    void ListSqlite(bool q1);

    void on_comboBox_textActivated(const QString &arg1);

    void on_listWidget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::Addressbook *ui;
};

#endif // ADDRESSBOOK_H
