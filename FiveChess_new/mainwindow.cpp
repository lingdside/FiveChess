#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "boardwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    extern QSize size;
    extern QFont font;
       setFixedSize(size);
       setFont(font);
       setWindowTitle(tr("Gobang"));
       setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);

       label.setParent(this);
       label.setText(tr("Username:"));
       label.setGeometry(240,200,120,32);

       username.setParent(this);
       username.setText(tr("Player"));
       username.setMaxLength(10);
       username.setAlignment(Qt::AlignCenter);
       username.setGeometry(360,200,200,32);

       start.setParent(this);
       start.setText(tr("Start!"));
       start.setGeometry(320,350,160,32);
       connect(&start,&QPushButton::clicked,this,&MainWindow::on_start_clicked);
}


void MainWindow::on_start_clicked()
{
    auto w=new BoardWindow(username.text());
    w->show();
    close();
}
MainWindow::~MainWindow()
{
    delete ui;
}
