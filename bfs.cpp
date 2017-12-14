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
typedef vector<vi> vvi;

map< string, vector <string> > dict;
stack <string> stac;
priority_queue<pair<int,int>,vector<pair<int,int>>,CompareDist> pq;
map<string,vector<string> >::const_iterator d_it;
vector<string>::const_iterator v_it;
map<string,vector<double> > heur;
map<string, pair<double,double> > location;
class CompareDist
{
public:
    bool operator()(pair<int,int> n1,pair<int,int> n2) {
        return n1.second>n2.second;
    }
};
double heuristic (string s, string t)
{
	Lat1 = heur[s][0];
	Long1 = heur[s][1];
	Lat2 = heur[1][0];
	Long2 = heur[t][1];
	sqrt(pow(69.5*(Lat1-Lat2), 2)+
               pow(69.5*(Long1-Long2)*
                        cos(((Lat1+Lat2)/360)*PI), 2))
}

vector<string> greedy (string s, string t)
{


}

vector<string> uniform(string s, string t)
{


}
vector<string> astar(string s, string d)
{
    map< string, bool> visited;
    vector< string> path;
    path.push_back(s);
    visited[s]=true;
    pq.push(s);
    
    while(!pq.empty())
    {
        d_it = dict.find(sq.top());
        pq.pop();
        
        for( v_it = d_it->second.begin();v_it!=d_it->second.end(); v_it++)
        {
            if(visited.find(*v_it) == visited.end())    //Node not present
            {
                path.push_back(*v_it);
                if(*v_it == d)
                    return path;
                pq.push(*v_it);
                visited[*v_it]=true;
            }
        }
    }
    return path;
}

typedef vector<string> (*FnPtr)(string, string);
pair <double, double> p;
int main(int argc, char* argv[]) 
{
    //fw; 
    fr;
    string s,t;
    for(int i=0;i<125;i++)
    { 
        cin>>s>>t;
        dict[s].pb(t);
        dict[t].pb(s);
    }
    for(int i=0;i<112;i++)
    {
        cin>>s; double a,b; cin>>a>>b;
        p.mp(a,b);
        location[s]=p;
        p.clear();
    }
    //myMap is used to map the functions with the input string
    std::map<std::string, FnPtr> myMap;
    myMap["astar"] = astar;
    myMap["greedy"] = greedy;
    myMap["uniform"] = uniform;
    for (std::map< string , vector<string> >::iterator it=dict.begin(); it!=dict.end(); ++it)
        sort(it->second.begin(),it->second.end());

    vector< string> path_1 = myMap[argv[1]](argv[2],argv[3]);
    vector< string> path_2 = myMap[argv[1]](argv[2],argv[3]);
    
    for( vector<string>::const_iterator eptr=path_1.begin(); eptr!=path_1.end(); eptr++)
                cout << *eptr << " ";
    cout << endl << endl;
    for( vector<string>::const_iterator eptr=path_2.begin(); eptr!=path_2.end(); eptr++)
                cout << *eptr << " ";
    return 0;
}
