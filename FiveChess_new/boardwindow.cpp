#include "boardwindow.h"
#include "resultwindow.h"
#include <QCoreApplication>
#include <QMessageBox>

bool Board::check_winner(int color)
{
    for(int i=0;i<size;++i)
        for(int j=0;j<size;++j)
            {
            if(data[i][j]!=color)continue;
            int dx=1,dy=0;
            for(int k=0;k<=5;++k)
                {
                if(k==5)return true;
                int tx=i+dx*k,ty=j+dy*k;
                if(tx<0||tx>=size||ty<0||ty>=size||data[tx][ty]!=color)break;
                }
            dx=1,dy=1;
            for(int k=0;k<=5;++k)
                {
                if(k==5)return true;
                int tx=i+dx*k,ty=j+dy*k;
                if(tx<0||tx>=size||ty<0||ty>=size||data[tx][ty]!=color)break;
                }
            dx=0,dy=1;
            for(int k=0;k<=5;++k)
                {
                if(k==5)return true;
                int tx=i+dx*k,ty=j+dy*k;
                if(tx<0||tx>=size||ty<0||ty>=size||data[tx][ty]!=color)break;
                }
            dx=-1,dy=1;
            for(int k=0;k<=5;++k)
                {
                if(k==5)return true;
                int tx=i+dx*k,ty=j+dy*k;
                if(tx<0||tx>=size||ty<0||ty>=size||data[tx][ty]!=color)break;
                }
            }
    return false;
}

bool Board::check_tie()
{
    for(int i=0;i<size;++i)
        for(int j=0;j<size;++j)
            if(data[i][j]==-1)return false;
    return true;
}

BoardWindow::BoardWindow(QString username, QWidget *parent)
    :QMainWindow(parent),mousex(-1),mousey(-1),turn(1)
{
    extern QSize size;
    extern QFont font;
    setFixedSize(size);
    setFont(font);
    setWindowTitle(tr("Gobang"));
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);
    setAttribute (Qt::WA_DeleteOnClose);

    computer.setParent(this);
    computer.setText(tr("Computer"));
    computer.setGeometry(40,200,200,32);

    if(username==tr(""))username=tr("Player1");
    player.setParent(this);
    player.setText(username);
    player.setGeometry(40,400,200,32);
}

void BoardWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QBrush black(Qt::black,Qt::SolidPattern),white(Qt::white,Qt::SolidPattern);

    painter.setPen(QPen(Qt::black,2,Qt::SolidLine));

    painter.setBrush(QBrush(Qt::red,Qt::SolidPattern));
    if(turn==1)painter.drawEllipse(150,450,32,32);
    else if(turn==0) painter.drawEllipse(150,150,32,32);

    painter.setBrush(white);
    painter.drawEllipse(50,150,32,32);
    painter.setBrush(black);
    painter.drawEllipse(50,450,32,32);

    painter.setBrush(QBrush(QColor::fromRgbF(244/256.0,225/256.0,149/256.0,0.8)));
    painter.drawRoundRect(210,10,580,580,10,10);

    for(int i=0;i<board_size;++i)
        {
        painter.drawLine(30*i+230,30,30*i+230,570);
        painter.drawLine(230,30*i+30,770,30*i+30);
        }

    for(int i=0;i<board_size;++i)
        for(int j=0;j<board_size;++j)
            if(board.at(i,j)==0)
                {
                painter.setBrush(white);
                painter.drawEllipse(215+30*j,30*i+15,30,30);
                }
            else if(board.at(i,j)==1)
                {
                painter.setBrush(black);
                painter.drawEllipse(215+30*j,30*i+15,30,30);
                }

}

BoardWindow::~BoardWindow()
{

}

void BoardWindow::make_move(int x, int y)
{
    QCoreApplication::processEvents();
    if(~board.at(x,y))
        {
        QMessageBox::warning(this,tr("ERROR"),QString::fromStdString("Your function give ("+std::to_string(x)+","+std::to_string(y)+") which is invalid.\nCheck it again!"),QMessageBox::Abort,QMessageBox::NoButton);
        close();
        return;
        }
    board.set(x,y,0);
    turn=1;
    update();
    if(board.check_winner(0))lose();
}

void BoardWindow::mousePressEvent(QMouseEvent *e)
{
    if(turn!=1)return;
    if(e->button()!=Qt::LeftButton)return;
    mousex=(e->y()-15)/30;
    mousey=(e->x()-215)/30;
}

/*
 * to do 请将solve_xxx中的xxx替换成你的学号
 */

void play(const Board& board, BoardWindow*Game, int turn);

void BoardWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if(turn!=1)return;
    if(e->button()!=Qt::LeftButton)return;
    if(mousex==(e->y()-15)/30&&mousey==(e->x()-215)/30&&mousex>=0&&mousex<board_size&&mousey>=0&&mousey<board_size&&board.at(mousex,mousey)==-1)
        {
        board.set(mousex,mousey,1);
        turn=0;
        repaint();
        if(board.check_winner(1))win();
        else if(board.check_tie())tie();
        else play(board,this,turn);
        }
    mousex=-1;
    mousey=-1;
}

void BoardWindow::win()
{
    turn=-1;
    ResultWindow w(this,1);
    w.exec();
}

void BoardWindow::lose()
{
    turn=-1;
    ResultWindow w(this,0);
    w.exec();
}

void BoardWindow::tie()
{
    turn=-1;
    ResultWindow w(this,-1);
    w.exec();
}
