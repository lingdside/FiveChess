#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    void on_start_clicked();

private:
    Ui::MainWindow *ui;
    QLineEdit username;
    QPushButton start;
    QLabel label;
};

#endif // MAINWINDOW_H
