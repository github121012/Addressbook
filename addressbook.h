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
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void QuerySqlite();
    void deleteSqlite();
    void modify();
    void ListSqlite(bool q1);
    void on_comboBox_textActivated(const QString &arg1);
    void on_listWidget_customContextMenuRequested(const QPoint &pos);


private:
    Ui::Addressbook *ui;
};

#endif // ADDRESSBOOK_H
