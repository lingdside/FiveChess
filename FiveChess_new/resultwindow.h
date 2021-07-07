#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>

class ResultWindow : public QDialog
{
    Q_OBJECT
public:
    ResultWindow(QWidget*parent=0,int result=0);
    ~ResultWindow();
private:
    QLabel prompt;
    QPushButton back,quit;
};

#endif // RESULTWINDOW_H
