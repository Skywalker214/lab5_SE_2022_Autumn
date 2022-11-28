#include "widget.h"

#include <QGridLayout>
#include <QScrollBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QProcess>
#include <QApplication>

// D:/testcase
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    pos=0;

    this->setFixedSize(1280, 900);

    te1=new QTextEdit;
    te2=new QTextEdit;
    l1=new QLabel;
    l2=new QLabel;
    l1->setText("file1");
    l2->setText("file2");
    pb1=new QPushButton("等价", this);
    pb2=new QPushButton("不等价", this);
    pb3=new QPushButton("存疑", this);
    pb4=new QPushButton("加载结果", this);
    l3=new QLabel;
    l3->setText("请导入input和output文件夹的共同路径");
    le=new QLineEdit;
    le->setAlignment(Qt::AlignCenter);

    QHBoxLayout *layout1=new QHBoxLayout;
    layout1->addWidget(l1);
    layout1->addWidget(l2);
    QHBoxLayout *layout2=new QHBoxLayout;
    layout2->addWidget(te1);
    layout2->addWidget(te2);
    QHBoxLayout *layout3=new QHBoxLayout;
    layout3->addWidget(pb4);
    layout3->addWidget(pb1);
    layout3->addWidget(pb2);
    layout3->addWidget(pb3);
    QVBoxLayout *layout=new QVBoxLayout;
    layout->addLayout(layout1);
    layout->addLayout(layout2);
    layout->addWidget(l3);
    layout->addWidget(le);
    layout->addLayout(layout3);
    setLayout(layout);

    connect(pb4, &QPushButton::clicked, this, &Widget::load);
    connect(pb1, &QPushButton::clicked, this, &Widget::equ);
    connect(pb2, &QPushButton::clicked, this, &Widget::inequ);
    connect(pb3, &QPushButton::clicked, this, &Widget::ns);
}

Widget::~Widget()
{
}

int Widget::my_find(int x)
{
    if (x != parent[x])
    {
        parent[x] = my_find(parent[x]);
    }
    return parent[x];
}
void Widget::my_union(int x, int y)
{
    parent[my_find(x)] = my_find(y);
}

void Widget::display()
{
    if(pos==raw.size())
    {
        end();
        return;
    }
    int x,y;
    for(int i=0;i<codes.size();i++)
    {
        if(codes[i]==raw[pos].first)
            x=i;
        if(codes[i]==raw[pos].second)
            y=i;
    }
    if(my_find(x)==my_find(y))
    {
        equal.push_back(raw[pos]);
        pos++;
        display();
    }
    else
    {
        QString f1_name,f2_name;
        f1_name=path+"/"+raw[pos].first;
        f2_name=path+"/"+raw[pos].second;

        l1->setText(f1_name);
        QFile f1(f1_name);
        f1.open(QFile::ReadOnly);
        QByteArray arr1=f1.readAll();
        te1->setText(arr1);
        f1.close();

        l2->setText(f2_name);
        QFile f2(f2_name);
        f2.open(QFile::ReadOnly);
        QByteArray arr2=f2.readAll();
        te2->setText(arr2);
        f2.close();
    }

}

void Widget::load()
{
    path=le->text();
    QString file_name=QFileDialog::getOpenFileName(this,tr("请导入input和output文件夹的共同路径"),path);
    if(!file_name.isNull())
    {
        QFile file(file_name);
        file.open(QFile::ReadOnly);
        QTextStream in(&file);
        QString temp,f1,f2;
        while(!file.atEnd())
        {
            temp=file.readLine();
            if(temp!="file1,file2\n")
            {
                f1=temp.mid(0,temp.indexOf(','));
                f2=temp.mid(temp.indexOf(',')+1,temp.length()-temp.indexOf(',')-2);
                QPair<QString,QString> p(f1,f2);
                raw.push_back(p);
                if(std::find(codes.begin(),codes.end(),f1)==codes.end())
                {
                    codes.push_back(f1);
                    parent.push_back(parent.size());
                }
                if(std::find(codes.begin(),codes.end(),f2)==codes.end())
                {
                    codes.push_back(f2);
                    parent.push_back(parent.size());
                }
            }
        }
        file.close();
    }
    display();
}

void Widget::equ()
{
    int x,y;
    for(int i=0;i<codes.size();i++)
    {
        if(codes[i]==raw[pos].first)
            x=i;
        if(codes[i]==raw[pos].second)
            y=i;
    }
    my_union(x,y);
    equal.push_back(raw[pos]);
    pos++;
    display();
}

void Widget::inequ()
{
    inequal.push_back(raw[pos]);
    pos++;
    display();
}

void Widget::ns()
{
    notsure.push_back(raw[pos]);
    pos++;
    display();
}

void Widget::end()
{
    QString f1_name=path+"/output/ConfirmedEqual.csv";
    QFile f1(f1_name);
    f1.open(QFile::WriteOnly);
    QTextStream out1(&f1);
    out1<<"file1,file2\n";
    for(int i=0;i<equal.size();i++)
        out1<<equal[i].first<<","<<equal[i].second<<"\n";
    f1.close();

    QString f2_name=path+"/output/NotSure.csv";
    QFile f2(f2_name);
    f2.open(QFile::WriteOnly);
    QTextStream out2(&f2);
    out2<<"file1,file2\n";
    for(int i=0;i<notsure.size();i++)
        out2<<notsure[i].first<<","<<notsure[i].second<<"\n";
    f2.close();

    QPushButton *okbtn = new QPushButton("确定");
    QMessageBox *mymsgbox = new QMessageBox;
    mymsgbox->setFixedSize(500,500);
    mymsgbox->setWindowTitle("提示");
    mymsgbox->setText("已完成等价确认。\n可前往output文件夹检查ConfirmedEqual.csv和NotSure.csv以查看结果");
    mymsgbox->addButton(okbtn, QMessageBox::AcceptRole);
    mymsgbox->show();
    mymsgbox->exec();
    if (mymsgbox->clickedButton()==okbtn)
    {
        qApp->quit();
        //QProcess::startDetached(qApp->applicationFilePath(), QStringList());
    }
}
