#include <iostream>
#include <vector>
#include <string>
#include <cstring> 
#include <queue>
#include <utility>
#include <algorithm>

/*
 	现在你有一个类MAP_BASE，请编写他的子类，继承于MAP_BASE
 	可以看到map_in是一张地图，定义.是空地，#是障碍物，现在有一个的机器人想从左上角到达右下角，请在子类中写一个函数完成这件事情
 	要求：1.将机器人中心走过的路用C表示出来并输出整个地图
		  2.机器人是四向移动的，即只能向上下左右移动
      	  3.机器人走过的路线为所有可行的路线中最短的
	
	注意：答案不唯一，只要输出一种答案就可
	加分条件：现实中机器人是有大小的，不可能完全让中心靠墙移动，现在假设机器人需要3x3的空地才能安全移动，请给出能让机器人安全移动的路径

	示例：

	#############################	#############################			#############################
	#..............###..........#	#.C............###..........#			#.C............###..........#
	#........###................#	#.C......###................#			#.C......###................#
	#.........................###	#.C.......................###			#.C.......................###	
	#.........................###	#.CCCCCCCCCCCCCCCCCC......###			#.C.......................###
	#....................########	#..................C.########			#.C..................########
	#.....#####.................#	#.....#####........C........#			#.C...#####.................#
	#...........................#	#..................CCCCCCCC.#			#.CCCCCCCCCCCCCCCCCCCCCCCCC.#
	#...........................#	#...........................#			#...........................#
	#############################中	#############################是安全的而	 #############################是不安全的因为通过了2x2的空地
	同时
	#############################	           #############################
	#.C............###..........#	           #.C............###..........#
	#.C......###................#	           #.C......###................#
	#.C.......................###	           #.C.......................###	
	#.CCCCCCCCCCCCCC..........###	           #.CCCCCCCCCCCC.CCCCC......###
	#..............C.....########	           #............C.C...C.########
	#.....#####....C............#	           #.....#####..C.C...C........#
	#..............CCCCCCCCCCCC.#	           #............CCC...CCCCCCCC.#
	#...........................#			   #...........................#
	#############################也是可行的而	#############################是不可行的因为这不是最短路

*/
class MAP_BASE
{
protected:
	int n,m,startx,starty,endx,endy;
	std::vector<std::string> map_in;
public:
	MAP_BASE()
	{
		map_in = {
		"#######################################################################",
		"#.............................................######...........##.....#",
		"#...........#############........................######.......#########",
		"#...........#####...........................###...####.......##########",
		"##.............##.............#######.................................#",
		"###...................................................................#",
		"#.........####................######.......................####.......#",
		"#..........##.........#.##...................######...................#",
		"#.....................................................................#",
		"#.............................##................................#######",
		"#.....................................................................#",
		"##########.............#######..########......#########...............#",
		"#...............................###############.......................#",
		"#######################################################################"
		};
		n=map_in.size();
		m=map_in[0].size();
		startx=2,starty=2;
		endx=n-3,endy=m-3;
	};
};

/**
*在map_in中将'.'替换成'C'表示机器人的路径。将最终的结果像示例里那样输出出来
*
* 提示：
* (1) 没有思路的同学可以先去了解一下深度优先搜索算法和广度优先搜索算法，思考应该用哪种方法。
* (2) 考虑使用队列queue数据结构
*
* 考点：
* (1) 类的使用。
* (2) 搜索算法。
*/

class Search: public MAP_BASE
{
private:
	const int dx[4]={0,0,-1,1};
	const int dy[4]={1,-1,0,0};
	bool accessible[100][100];
	bool vis[100][100];
	int dis[100][100];
	int pre[100][100];

	bool check(int x,int y)
	{
		int ret=0;
		if(x!=0&&y!=0) ret+=(map_in[x-1][y-1]=='#');
		if(x!=0) ret+=(map_in[x-1][y]=='#');
		if(x!=0&&y!=m-1) ret+=(map_in[x-1][y+1]=='#');
		if(y!=0) ret+=(map_in[x][y-1]=='#');
		ret+=(map_in[x][y]=='#');
		if(y!=m-1) ret+=(map_in[x][y+1]=='#');
		if(x!=n-1&&y!=0) ret+=(map_in[x+1][y-1]=='#');
		if(x!=n-1) ret+=(map_in[x+1][y]=='#');
		if(x!=n-1&&y!=m-1) ret+=(map_in[x+1][y+1]=='#');
		return ret==0;
	};

	struct Node
	{
		int x,y,f;
		Node(int x,int y,int f):x(x),y(y),f(f){};
		bool operator>(const Node &t)const{return f>t.f;}
	};

	int cost(int x0,int y0,int x1,int y1)
	{
		return abs(x0-x1)+abs(y0+y1);
	}
	// int getId(int x0,int y0){return x0*m+y0;};
public:
	Search()
	{
		memset(dis,0x3f,sizeof(dis));
		memset(vis,0,sizeof(vis));
		memset(pre,-1,sizeof(pre));
		std::priority_queue<Node,std::vector<Node>,std::greater<Node>> q;
		q.push(Node(startx,starty,cost(startx,starty,endx,endy)));
		dis[startx][starty]=0;
		while(!q.empty())
		{
			Node now=q.top();q.pop();
			if(vis[now.x][now.y]) continue;
			vis[now.x][now.y]=true;
			if(now.x==endx&&now.y==endy) return;
			for(int i=0;i<4;i++)
			{
				int tx=now.x+dx[i];
				int ty=now.y+dy[i];
				if(!check(tx,ty)) continue;
				if(vis[tx][ty]) continue;
				int d=dis[now.x][now.y]+1;
				if(d<dis[tx][ty])
				{
					dis[tx][ty]=d;
					pre[tx][ty]=i^1;
					q.push(Node(tx,ty,d+cost(tx,ty,endx,endy)));
				}
			}
		}
	};

	std::vector<std::pair<int,int>> getPath()
	{
		std::vector<std::pair<int,int>> ret;
		std::pair<int,int> now=std::make_pair(endx,endy);
		ret.push_back(now);
		while(pre[now.first][now.second]!=-1)
		{
			int id=pre[now.first][now.second];
			now.first+=dx[id];
			now.second+=dy[id];
			ret.push_back(now);
		}
		std::reverse(ret.begin(),ret.end());
		return ret;
	};

	void print(const std::vector<std::pair<int,int>> &path)
	{
		for(auto t:path) map_in[t.first][t.second]='C';
		for(auto t:map_in) std::cout<<t<<std::endl;
	}

	void test()
	{
		for(int i=0;i<n;i++)
			for(int j=0;j<m;j++)
				std::cout<<accessible[i][j]<<" \n"[j==m-1];
	}
};

//IMPLEMENT YOUR CODE HERE

int main()
{
	Search bot;
	bot.print(bot.getPath());
	return 0;
}

