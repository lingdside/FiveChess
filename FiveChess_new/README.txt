gobang-基于QT写的五子棋UI界面

1. 棋盘大小被设置为19*19，你可以用board_size得到这个值（事实上，它是一个在boardwindow.h 中定义的宏）

2. 修改xx.cpp文件名，将xx替换成你的学号

3. 修改xx.cpp文件中的solve_xx函数名，将xx替换成你的学号

4. 修改boardwindow.cpp文件中的solve_xx函数声明，将xx替换成你的学号

5. 修改boardwindow.cpp文件中mouseReleaseEvent函数中调用solve_xx的代码，将xx替换成你的学

6. void solve_xx(const Board& board,BoardWindow*Game,int turn)中board是棋盘类，Game是棋盘图形界面类，turn是你下棋的顺序，如果是turn是1，表示你是黑色棋子；如果turn是0，表示你是白色棋子。默认是白色棋子。

7. 你需要在xx.cpp中的solve_xx函数中完善你的算法，计算出下一步要走的棋子坐标（x,y）,最后调用Game->make_move(x,y)函数改变棋盘状态。你可以在solve_xx函数中添加自己的辅助函数以辅助计算。


你可以使用board.at(x,y)函数查看棋盘上的（x,y）点是否有棋子，并且知道棋子的种类。
返回值解释：
0：白色棋子
1：黑色棋子
-1：没有棋子
-2：位置是不允许的

请保证在solve_xx（）中给出的下棋坐标在棋盘范围之内。



