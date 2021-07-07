#include "resultwindow.h"
#include <QApplication>

ResultWindow::ResultWindow(QWidget *parent, int result)
    :QDialog(parent)
{
    extern QFont font;
    font.setPointSize(12);
    setFixedSize(500,200);
    setFont(font);
    setWindowTitle(tr("Result"));
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);

    prompt.setParent(this);
    if(result==1)prompt.setText(tr("Congratulations!\nYou've beaten the computer!"));
    else if(result==0) prompt.setText(tr("Sorry,you lost.\nBetter luck next time."));
    else prompt.setText(tr("How amazing!\nYou tied with the computer?"));
    prompt.setAlignment(Qt::AlignCenter);
    prompt.setGeometry(50,50,400,64);

    back.setParent(this);
    back.setText(tr("Back"));
    back.setGeometry(300,150,50,32);
    connect(&back,&QPushButton::clicked,this,&QDialog::close);

    quit.setParent(this);
    quit.setText(tr("Quit"));
    quit.setGeometry(400,150,50,32);
    connect(&quit,&QPushButton::clicked,&QApplication::quit);
}

ResultWindow::~ResultWindow()
{

}
