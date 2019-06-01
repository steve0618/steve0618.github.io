#include <iostream>
#include <algorithm>
#include <cstring>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#define rii register int
#define CLS system("cls")
using namespace std;

bool bs,chk;

struct Game
{
    int m[1010][1010];
    bool ck[1010][1010];
    bool flg[1010][1010];
    int width,height,num;

    inline void print_map()
    {
        int asd=0;
        cout<<"   ";
        for(rii i=1; i<=width; ++i)
            cout<<i%10<<" ";
        cout<<"Y\n\n";
        for(rii i=1; i<=height; ++i)
        {
            cout<<i%10<<"  ";
            for(rii j=1; j<=width; ++j)
            {
                if(flg[i][j])
                    cout<<"$ ",ck[i][j]=1,++asd;
                else if(!ck[i][j])
                    cout<<"# ";
                else if(m[i][j]==0)
                    cout<<"  ",ck[i][j]=1;
                else if(m[i][j]==9)
                    cout<<"* ",chk=1;
                else
                    cout<<m[i][j]<<" ",ck[i][j]=1;
            }
            cout<<"\n";
        }
        cout<<"\nX                           剩余雷数:"<<num-asd<<"\n\n";
    }

    inline bool check()
    {
        for(rii i=1; i<=height; ++i)
            for(rii j=1; j<=width; ++j)
            {
                if(!ck[i][j]&&m[i][j]!=9)
                    return 0;
            }
        return 1;
    }

    inline void spawn_mines(int x,int y)
    {
        int xx,yy;
        srand(time(0));
        for(rii i=1; i<=num; ++i)
        {
            xx=rand()%(height-1)+1;
            srand(rand());
            yy=rand()%(width-1)+1;
            while(m[xx][yy]==9||(xx==x&&yy==y))
            {
                xx=rand()%(height-1)+1;
                srand(rand());
                yy=rand()%(width-1)+1;
            }
            m[xx][yy]=9;
        }
    }

    inline void update()
    {
        for(rii i=1; i<=height; ++i)
            for(rii j=1; j<=width; ++j)
            {
                if(m[i][j]==9)
                    continue;
                int ans=0;
                for(rii dx=-1; dx<=1; ++dx)
                    for(rii dy=-1; dy<=1; ++dy)
                        if((dx!=0||dy!=0)&&m[i+dx][j+dy]==9)
                            ++ans;
                m[i][j]=ans;
            }
    }

    inline int count_flag(int x,int y)
    {
        int ans=0;
        for(rii dx=-1; dx<=1; ++dx)
            for(rii dy=-1; dy<=1; ++dy)
                if((dx!=0||dy!=0)&&flg[x+dx][y+dy])
                    ++ans;
        return ans;
    }

    inline void spread(int x,int y,int mode)
    {
        if(x<=0||x>height+1||y<=0||y>width+1)
            return;
        if(flg[x][y]&&mode!=1) return;
        if(m[x][y]==9&&mode==0&&(!flg[x][y]))
        {
            ck[x][y]=1;
            return;
        }
        //0 is first click,1 is insert flag,2 is spread
        update();
        if(mode==1)
        {
            if(ck[x][y]&&(!flg[x][y]))
                return;
            if(!flg[x][y])
                flg[x][y]=ck[x][y]=1;
            else
                flg[x][y]=ck[x][y]=0;
        }
        else if(m[x][y]==9&&!flg[x][y])
        {
            ck[x][y]=1;
            return;
        }
        else if(mode==2&&m[x][y]>=1&&m[x][y]<=8)
        {
            ck[x][y]=1;
            return;
        }
        else if(mode==0&&m[x][y]>=1&&m[x][y]<=8)
        {
            ck[x][y]=1;
            if(m[x][y]!=count_flag(x,y))
                return;
            for(rii dx=-1; dx<=1; ++dx)
                for(rii dy=-1; dy<=1; ++dy)
                    if((dx!=0||dy!=0)&&(!flg[x+dx][y+dy])&&(!ck[x+dx][y+dy]))
                        spread(x+dx,y+dy,2);
        }
        else if(m[x][y]==0)
        {
            ck[x][y]=1;
            for(rii dx=-1; dx<=1; ++dx)
                for(rii dy=-1; dy<=1; ++dy)
                    if((dx!=0||dy!=0)&&(!flg[x+dx][y+dy])&&(!ck[x+dx][y+dy]))
                        spread(x+dx,y+dy,2);
        }
    }

    inline void get_command()
    {
        cout<<">>> ";
        char c;
        int a,b;
        cin>>c>>a>>b;
        if(bs==0)
            bs=1,spawn_mines(a,b);
        if(c=='s')
            spread(a,b,0);
        if(c=='f')
            spread(a,b,1);
    }

} game;

inline void pause()
{
    string sdsdss;
    cout<<"按Enter键以继续...\n";
    getline(cin,sdsdss);
    getline(cin,sdsdss);
}

int main()
{
    ios::sync_with_stdio(0);

_input:
    CLS;
    memset(game.m,0,sizeof(game.m));
    memset(game.ck,0,sizeof(game.ck));
    memset(game.flg,0,sizeof(game.flg));
    chk=bs=0;
    cout<<"XRT的扫雷  作者:XRT\n输入对局的长、宽和雷数(格式:长 宽 雷数):\n";
    cin>>game.width>>game.height>>game.num;
    if(game.width*game.height<game.num||game.width<=0||game.height<=0||game.num<=0)
    {
        cout<<"非法输入。请重新输入。\n\n";
        CLS;
        goto _input;
    }

_tutorial:
    CLS;
    cout<<"教程\n\n#字符代表未开启的格子。\n空格是空的地方。\n数字代表附近有几个雷(1~8个)。\n*字符代表雷。别踩到了!\n$字符代表你插的旗。\n\n本游戏采用命令操控。命令如下:\ns [x] [y]:打开坐标(x,y)，支持直接掀开和数字扩展。\nf [x] [y]:在坐标(x,y)处插旗。\n支持一行多句命令，请用空格隔开。\n\n目标:遍历完所有的格子，找到全部的雷，但不要引爆它们。\nHave fun!\n\n";
    pause();

_game:
    CLS;
    game.print_map();
    if(chk)
    {
        cout<<"地雷把你炸得一败涂地!\n\n";
        pause();
        goto _input;
    }
    if(game.check())
    {
        cout<<"恭喜你，游戏胜利!\n\n";
        pause();
        goto _input;
    }
    game.get_command();
    goto _game;
    return 0;
}
