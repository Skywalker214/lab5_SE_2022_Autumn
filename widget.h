#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <vector>
#include <QLineEdit>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QTextEdit *te1;
    QTextEdit *te2;
    QPushButton *pb1;
    QPushButton *pb2;
    QPushButton *pb3;
    QPushButton *pb4;
    QLabel *l1;
    QLabel *l2;
    QLabel *l3;
//    QLabel *l4;
    QLineEdit *le;

    QString path;
    std::vector<QPair<QString,QString>> raw;
    std::vector<QPair<QString,QString>> equal;
    std::vector<QPair<QString,QString>> inequal;
    std::vector<QPair<QString,QString>> notsure;
    std::vector<QString> codes;
    std::vector<int> parent;
    int pos;

    void load();
    void equ();
    void inequ();
    void ns();
    void display();
    int my_find(int x);
    void my_union(int x, int y);
    void end();

};
#endif // WIDGET_H
