#include<boardwindow.h>

/*
 * You need to complete this function.
 *
 * The first parameter you've got describes a board,
 *
 * You can use "board_size" as a const number. Actually it has been defined as 19.
 *
 * If turn=1, black
 * If turn=0, white
 *
 * you can use "at(x,y)" to know whether there is a stone at (x,y) and what color it is.
 * If it returns -1, it means there is no stone.
 * If it returns -2, it means the position is invalid.
 *
 *
 * After you decide where to put your stone, please use "make_move" function.
 * Use it like this "Game->make_move(x,y)" if you decide to put your stone at (x,y).
 *
 * The coordinates begin from 0.
 * Make sure the coordinate you give is valid, or the application will crash!!!
 *
 * Here is a example for you with a random algorithm.
*/

// Include headers you need here.
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>
//#include <QtTest/QTest>
#include<iostream>
#include<queue>

using namespace std;

/*
 * Your code goes here.
 */
namespace {

const int INF = 1<<31;
const int max_step = 10;

//以下四个值离结束不远了
const int FOURM = 1<<28;//M:me
const int FOURO = 1<<26;//O:opposite
const int THREELM = 1<<23;//执棋方活三
const int THREELO = 1<<20;//对手方活三

const int THREEDM2 = 1<<19;
const int THREEDO2 = (1<<19)-1000;

const int TWOM2 = 1<<18;
const int TWOO2 = 1<<17;
const int TWOM = 12;
const int TWOO = 8;
const int THREEDM = 12;
const int THREEDO = 8;

const int dx[8]={0,1,1,1,0,-1,-1,-1};
const int dy[8]={1,0,1,-1,-1,0,1,-1};


int Min_Value(int State[][board_size], int turn, int me, int step, int& alpha, int& beta);
int Max_Value(int State[][board_size], int turn, int me, int step, int& alpha, int& beta);

int getValue(const int State[][board_size], int x, int y, int i, int d)
{
    int xx = x + dx[i]*d, yy = y + dy[i]*d;
    return (xx<0 || yy<0 || xx>=board_size || yy>=board_size) ? -2 : State[xx][yy];
}

int Adjacent(const int State[][board_size], int x, int y)
{
    for(int i = 0; i < 8; i++)
    {
        if(getValue(State,x,y,i,1)>=0) return 1;
        if(getValue(State,x,y,i,2)>=0) return 2;
    }
    return 0;
}

int level[5]={10,100,1000,10000,100000};
//次序：活四死四>活三>死三*2>活二*2=死三+活二>活二>死三
int EvaluatePoint(const int State[][board_size],int i,int j,int d)
{
    int ret = 0, threedm = 0, threedo = 0, twom = 0, twoo = 0;
    int fouro=0,threelm=0,threelo=0;
    int mesumlive[6],mesumdead[6],mefive=0;
    memset(mesumlive,0,sizeof(mesumlive));
    memset(mesumdead,0,sizeof(mesumdead));
    int symm=0,symo=0;
    int lone=0;
    for(int k = 0; k < d; k++)//枚举方向
    {
        int v1 = getValue(State,i,j,k,1);
        int v2 = getValue(State,i,j,k,2);
        int v3 = getValue(State,i,j,k,3);
        int v4 = getValue(State,i,j,k,4);
        int v5 = getValue(State,i,j,k,5);
        int v_1 = getValue(State,i,j,k,-1);
        int v_2 = getValue(State,i,j,k,-2);

        if (v1>=0 && ((v1==v2 && v2==v3 && v3==v4) || (v1==v2 && v2==v3 && v3==v_1) || (v1==v2 && v1==v_1 && v_1==v_2))) //活四或死四
        {
            if(v1==State[i][j]) return FOURM;//如果本方是活四或死四，直接下在当前位置就赢了
            else if(d==8)fouro=1;//如果对方是死四或活四，先不急着返回，万一在其他方向找到本方活四或死四的状态
        }
        else if (v1>=0 && ((v1==v2 && v2==v3 && v_1==-1 && v4==-1) || (v1==v2 && v1==v_1 && v3==-1 && v_2==-1)))//活三:*表示当前位置，此时-1 0 0 0 * -1/-1 0 0 * 0 -1
        {
            if(v1==State[i][j]) threelm=1;
            else if(d==8)threelo=1;
        }
        else if(v2>=0 && ((v1==-1 && v2==v3 && v3==v4) || (v1==v_1 && v1==v2 && (v3==-1 || v_2==-1))))//死三: 0 0 0 -1 * /-1 0 0 * 0 1
        {
            if(v2==State[i][j]) threedm++;
            else if(d==8)threedo++;
        }
        else if((v1>=0 && v1==v2 && v3==-1 && v_1==-1)|| (v1==-1 && v2>=0 && v2==v3 && v4==-1 && v_1==-1)) //活二
        {
            if(v1==State[i][j]) twom++;
            else if(d==8)twoo++;
        }
        else if  (v1>=0 && v1==v_1 && v2==-1 && v_2==-1)
        {
            if(v1==State[i][j]) twom++,symm=1;
            else if(d==8) twoo++,symo=1;
        }
        else if(v1==State[i][j] && v_1==-1 &&v2==-1) lone++;
    }

    if(fouro) return FOURO;
    if(threelm) return THREELM;
    if(threelo) return THREELO;

    if(d==4 && TWOM>0) return TWOM2;

    if(d==8 && symm>0) twom/=2;
    if(d==8 && symo>0) twoo/=2;

    if(twom>=2) return TWOM2;
    if(twoo>=2) return TWOO2;

    if(threedm>1) ret += THREEDM2;
    if(threedm==1 && twom==1) ret += TWOM2;
    if(twom==1 && threedm==0) ret += TWOM;
    if(threedm==1 && twom==0) ret += THREEDM;

    if(threedo>1) ret += THREEDO2;
    if(threedo==1 && twoo==1) ret += TWOO2;
    if(twoo==1 && threedo==0) ret += TWOO;
    if(threedo==1 && twoo==0) ret += THREEDO;

    ret += lone*5;
    if(Adjacent(State,i,j)==1) ret++;

    return ret;
  /*      int v1 = getValue(State,i,j,k,1);
        int v2 = getValue(State,i,j,k,2);
        int v3 = getValue(State,i,j,k,3);
        int v4 = getValue(State,i,j,k,4);
        int v5 = getValue(State,i,j,k,5);
        int v_1 = getValue(State,i,j,k,-1);

        if(v1==v2 && v2==v3 && v3==v4 && v4==State[i][j]) mefive++;
        else if(v1==State[i][j] && v1==v2 && v2==v3 && v4==-1 && v_1==-1) mesumlive[4]++;
        else if(v1==State[i][j] && v1==v2 && v2==v3 && (v4==-1 || v_1==-1)) mesumlive[4]++;
        else if(v1==State[i][j] && v1==v3 && v4==v3 && v2==-1) mesumdead[4]++;
        else if(v2==State[i][j] && v1==-1 && v2==v3 && v3==v4) mesumdead[4]++;
        else if(v1==State[i][j] && v1==v2 && v2==v4 && v3==-1) mesumdead[4]++;
        else if(v1==State[i][j] && v1==v2 && v_1==-1 && v4==-1) mesumlive[3]++;//111
        else if(v1==State[i][j] && v1==v3 && v_1==-1 && v4==-1 && v2==-1) mesumlive[3]++;//1101
        else if(v2==State[i][j] && v2==v3 && v_1==-1 && v4==-1 && v1==-1) mesumlive[3]++;//1011
        else if(v1==State[i][j] && v1==v2 && (v_1==-1 || v3==-1)) mesumdead[3]++;
        else if(v1==State[i][j] && v1==v3 && (v_1==-1 || v4==-1) && v2==-1) mesumdead[3]++;
        else if(v2==State[i][j] && v2==v3 && (v_1==-1 || v4==-1) && v1==-1) mesumdead[3]++;
        else if(v2==State[i][j] && v2==v4 && v1==-1 && v3==-1) mesumdead[3]++;//10101
        else if(v1==State[i][j] && v_1==-1 && v2==-1) mesumlive[2]++;//0110
        else if(v2==State[i][j] && v1==-1 && v3==-1 && v_1==-1) mesumlive[2]++;//01010
        else if(v1==State[i][j] && (v_1==-1 || v2==-1)) mesumdead[2]++;//011
        else if(v2==State[i][j] && v1==-1 && (v3==-1 || v_1==-1)) mesumdead[2]++;//101
        else if(v1==-1 && v_1==-1) mesumlive[1]++;
    }

    if(mefive>0) return level[4];
    if(mesumdead[4]>1) return level[3]*2;
    if(mesumlive[4]>0) return level[3];
    if(mesumdead[4]>0 && mesumlive[3]>0) return level[3];
    if(mesumlive[3]>1) return level[2]*2;
    if(mesumdead[4]>0 || mesumlive[3]>0) return level[2];
    if(mesumlive[2]) return level[1]*2;
    if(mesumdead[3]) return level[1];
    if(mesumlive[1]) return level[0];*/

}

int EvaluateBoard(const int State[][board_size],int turn, int me)
{
    int scorecom=0,scorehum=0;
   // cout<<turn<<" "<<me<<endl;
  /*  for(int i = 0; i < board_size; i++){
        for(int j = 0; j < board_size; j++)
            cout<<State[i][j]<<" ";cout<<endl;}*/
    bool fouro=0,threelm=0,threelo=0,threedm=0,threedo=0;
    for(int i = 0; i < board_size; i++)
        for(int j = 0; j < board_size; j++)
            if(State[i][j]>=0)
            {
                int temp=EvaluatePoint(State,i,j,4); //cout<<i<<" "<<j<<" "<<temp<<"     ";
                if(temp>=FOURO)
                {
                    if(State[i][j]==turn) return FOURM;//虽然叫4，但实际是成五了
                    fouro=1;
                }
                else if(temp>=THREELO)
                {
                    if(State[i][j]==turn) threelm=1;//活四
                    else threelo=1;
                }
                else if(temp>=THREEDO)
                {
                    if(State[i][j]==turn) threedm=1;//活四
                    else threedo=1;
                }
                else if(State[i][j]==turn) scorecom+=temp;
                else scorehum+=temp;
            }
   // cout<<endl;
/*    if(fouro) return (turn==me) ? FOURO : -FOURO;
    if(threelm) return (turn==me) ? THREELM : -THREELM;
    if(threelo) return (turn==me) ? THREELO : -THREELO;

    if(turn==me) return scorecom-scorehum;
    else return scorehum-scorecom;*/

    if(fouro) return  -FOURM;
    if(threelm) return THREELM;
    if(threelo) return -THREELM;
    if(threedm) return THREEDM;
    if(threedo) return -THREEDO;
    return scorecom-scorehum;
}

int Max_Value(int State[][board_size], int turn, int me, int step, int& alpha, int& beta)
{
    int Max = -INF;//cout<<re<<" ";
    int re=EvaluateBoard(State, turn, me);
    if(step > max_step) return re;
    int flag = -1;
    for(int i = 0; i < board_size; i++)
        for(int j = 0; j < board_size; j++)
            if(State[i][j] == -1 && Adjacent(State,i,j))
            {
                State[i][j] = turn;
                int temp = Min_Value(State, 1-turn, me, step+1, alpha, beta);
                if(temp > Max)
                {
                    Max = temp;
                    flag = i*board_size+j;
                }
                State[i][j] = -1;
                if(Max > alpha) alpha = Max;
                if(Max >= beta) return Max;
            }

    return Max;
}

int Min_Value(int State[][board_size], int turn, int me, int step, int& alpha, int& beta)
{
    int Min = INF;
    int re = EvaluateBoard(State, turn, me);
    if(step > max_step) return re;
    int flag=-1;
    for(int i = 0; i < board_size; i++)
        for(int j = 0; j < board_size; j++)
            if(State[i][j] == -1 && Adjacent(State,i,j))
            {
                State[i][j] = turn;
                int temp = Max_Value(State, 1-turn, me, step+1, alpha, beta);
                if(temp < Min)
                {
                    Min = temp;
                    flag = i*board_size+j;
                }
                State[i][j] = -1;
                if(Min < beta) beta = Min;
                if(Min <= alpha) return Min;
            }

    return Min;
}

void solve(const Board& board,BoardWindow* Game,int turn)//turn表示执行的角色，1执行黑棋
{
    //Write your algorithm here.
    printf("%d ", turn);
    srand(time(0));
    int flag = -1;
    int Max = -INF;
    int State[board_size][board_size];
    for(int i = 0; i < board_size; i++)
        for(int j = 0; j < board_size; j++)
            State[i][j] = board.at(i,j);

    int alpha = -INF, beta = INF;
   // Max_Value(State, 1-turn, turn, 0, alpha, beta, flag);

    priority_queue<pair<int,int>> Que;
    int dis[board_size*board_size];
    for(int i = 0; i < board_size; i++)
        for(int j = 0; j < board_size; j++)
            if(State[i][j] == -1)
            {
                int pos = i*board_size+j;
                dis[pos]=Adjacent(State,i,j);
                if(dis[pos]==0) continue;
                State[i][j] = turn;
                int temp = EvaluatePoint(State,i,j,8);
                if(dis[pos]==1) temp++;
                Que.push(make_pair(temp,pos));
                State[i][j] = -1;
            }

    while(!Que.empty())
    {
        int pos = Que.top().second; //cout<<"pos"<<pos<<endl;
        Que.pop();
        int i = pos / board_size, j = pos % board_size;
        State[i][j] = turn;
        int temp = Min_Value(State, 1-turn, turn, 0, alpha, beta);// : Max_Value(State, i*board_size+j, -turn, turn, 0);
        if(dis[pos]==1) temp++;//cout<<i<<" "<<j<<" "<<temp<<"\n";
        if(temp > Max)
        {
            Max = temp;
            flag = pos;
        }
        State[i][j] = -1;
    }//cout<<endl;
    //printf("%d,%d\n", x,y);
    //Remember to recall "make_move" function.
    //Sleep(10);
    if(flag==-1)
    {
        Game->make_move(10,10);
        return;
    }
    int x = flag / board_size;
    int y = flag % board_size;//cout<<endl<<endl;
    Game->make_move(x,y);//在指定的(x,y)下一颗棋
}

}


void
#if !defined(USER_COLOR)
play
#elif USER_COLOR == BLACK
play_black
#elif USER_COLOR == WHITE
play_white
#else
#error "Invalid complier option"
#endif
(const Board& board, BoardWindow *Game, int turn)
{
    return solve(board, Game, turn);
}
