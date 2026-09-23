/*本题目主要考查类的使用,不了解类的同学建议先去学习相关知识。

    背景介绍：在Robomaster比赛中，一个机器人在进行自瞄的时候有时会同时识别到多个敌方目标。此时机器人需要
    选择其中一个来作为最佳打击目标并进行打击。通常我们会锁定距离准心（操作手端的屏幕中心）最近的目标。
    示例    ————————————————————————————————————————————
            |                       T                  |
            |        H            (1,4)                |
            |      (-9,3)                              |         
            |                                          |
            |                     +                    |      此时应当锁定S目标
            |                S                         |
            |              (-3,1)                      |
            |                              I           |
            |                            (7,-3)        |
            ————————————————————————————————————————————

    题目：编写一个程序，记录4个敌方目标的二维坐标，锁定距离准心最近的目标并输出对应的兵种ID。
    
    要求：采用面向对象的方法，设计两个类：
    Enemy 类：包含敌人的坐标和兵种ID，以及相应的设置和获取函数。
    Target 类：包含一个 Enemy 类的对象数组，并具备选择并返回最佳打击目标和输出的功能。

    PS：获取输入数据的框架已经替各位实现好了，在对应的地方调用你们编写的设置函数即可。
*/

#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

// ==================== 在此处编写 Enemy和Target 类 ====================

class Enemy
{
private:
    double x,y;
    char id;
public:
    Enemy(double x=0,double y=0,char id=0):x(x),y(y),id(id){};
    ~Enemy(){};
    pair<double,double> getPos(){return make_pair(x,y);};
    char getId(){return id;};
    double getDis(double x0=0,double y0=0){return sqrt(pow(x-x0,2)+pow(y-y0,2));};
    void print(){printf("(%c,%lf,%lf)\n",id,x,y);};
};

class Target
{
private:
    vector<Enemy> tar;
    const int Inf=1e6;
public:
    Target(){};
    Target(vector<Enemy> vec){tar=vec;};
    ~Target(){};
    void newEnemy(Enemy item){tar.push_back(item);};
    Enemy select(double x0=0,double y0=0)
    {
        if(tar.empty()) return Enemy();
        int ans=-1;
        double dis=Inf;
        for(int i=0;i<tar.size();i++)
        {
            double d=tar[i].getDis();
            if(d<dis) dis=d,ans=i;
        }
        if(ans!=-1) return tar[ans];
        return Enemy();
    }
};


// ====================================================================


int main() {
    Target target;

    for (int i = 0; i < 4; i++) {
        double x, y;
        char id;
        cout << "请输入第 " << i + 1 << " 个目标的兵种ID和坐标(x y): ";
        cin >> id >> x >> y;

        //在此处调用你的Enemy的设置函数，传入id,x,y
        target.newEnemy(Enemy(x,y,id));
    }
    target.select().print();
    // 调用查找并输出最佳目标

    return 0;
}
