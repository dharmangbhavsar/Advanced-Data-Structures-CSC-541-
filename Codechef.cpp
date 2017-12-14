#include <bits/stdc++.h>
#define fr freopen("input.in","r",stdin)
#define fw freopen("output.out","w",stdout)
#define iOs ios_base::sync_with_stdio(false);
#define INF 111313131
#define all(x) (x).begin(), (x).end()
#define debug cout<<"here\n"
#define debugin cout<<"inhere\n"
#define debugname cout<<"dharmang\n";
using namespace std; 
#define pb push_back
#define mp make_pair
#define sc second
#define fir first
#define No_Of_Inputs 2
#define No_Of_Layer 3
#define HI 14500	
#define LO 12000
typedef long long LL;
typedef pair<int, int> pii;
typedef vector<int> vi;
 
 
 typedef int State[25];
 const  int maxn = 5000000;
 State st [maxn];
 State goal = {1,1,1,1,1,
                0,1,1,1,1,
                0,0,2,1,1,
                0,0,0,0,1,
                0,0,0,0,0};
 int dis[maxn];
  int dx[]={-1,-2,-1,-2,1,2,1,2};
  int dy[]={-2,-1,2,1,-2,-1,2,1};
  
  
  struct cmp
  {
         bool operator ()(int a,int b)const
         {
              return memcmp(&st[a],&st[b],sizeof(st[b]))<0;
         }
  
  };
  
  set<int,cmp> vis;
  int try_to_insert(int s)
  {
      if(vis.count(s))return 0;
      vis.insert(s);
      return 1;
  }
  
  int bfs()
  {
      vis.clear();
  
      int front=0,rear=1;
  
      while(front<rear)
      {
          State& s = st[front];
          if(memcmp(goal,s,sizeof(s))==0) return front;
  
          int z;
          for( z=0;z<25;z++)if(s[z]==2)break;
  
          int x = z/5,y=z%5;
  
          for(int d=0;d<8;d++)
          {
              int newx = x+dx[d];
              int newy = y+dy[d];
              int newz = newx*5+newy;
  
              if(newx>=0&&newx<5 && newy>=0&&newy<5)
              {
  //                cout<<"Old "<<x<<" "<<y<<" "<<z<<endl;
  //
  //               cout<<"New "<<newx<<" "<<newy<<" "<<newz<<endl;
                  State & t = st[rear];
                  memcpy(&t,&s,sizeof(s));
  
                  t[newz]=s[z];
                  t[z]=s[newz];
  
                  dis[rear]=dis[front]+1;
  
                  if(try_to_insert(rear))
                  {
  //                    for(int i=0;i<5;i++)
  //                    {
  //                        for(int j=0;j<5;j++)
  //                        {
  //                            cout<<st[rear][i*5+j];
  //                        }
  //                        cout<<endl;
  //                    }
                      rear++;
                  }
              }
          }
          if(dis[front]>=11)return -1;
          front++;
  
      }
      return -1;
  
  }
  
  int main()
  {
  	fw; fr;
      int tst;
     cin>>tst;
     cin.ignore();
     while(tst--)
     {
        char c;
        for(int i=0;i<5;i++)
        {
            for(int j=0;j<5;j++)
            {
 
                c=getchar();
                if(c==' ')st[0][i*5+j] = 2;
                else
                    st[0][i*5+j]  = c-'0';
            }
            getchar();
        }


 
        int ans;
        ans = bfs();
        if(ans==-1)cout<<"Unsolvable in less than 11 move(s)."<<endl;
       else
            cout<<"Solvable in "<<dis[ans]<<" move(s)."<<endl;
 
        //memset(dis,0,sizeof(dis));
     }
     return 0;
 }



 