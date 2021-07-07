#ifndef BOARDWINDOW_H
#define BOARDWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPainter>
#include <QMouseEvent>

#define board_size 19

class Board{
public:
    int at(int x,int y) const
        {
        if(x<0||x>=size||y<0||y>=size)return -2;
        return data[x][y];
        }
private:
    int size;
    int data[board_size][board_size];
    Board(int size=board_size):size(size){memset(data,-1,sizeof(data));}
    void set(int x,int y,int color){data[x][y]=color;}
    bool check_winner(int color);
    bool check_tie();
friend class BoardWindow;
};

class BoardWindow : public QMainWindow
{
    Q_OBJECT

public:
    BoardWindow(QString username,QWidget*parent=0);
    ~BoardWindow();
    void make_move(int x,int y);

private:
    QLabel computer,player;
    Board board;

    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    int mousex,mousey;

    int turn;

    void win();
    void lose();
    void tie();
};



#endif // BOARDWINDOW_H
