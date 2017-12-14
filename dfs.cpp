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
map<string,vector<string> >::const_iterator d_it;
vector<string>::const_iterator v_it;


vector<string> dfs(string s, string d)
{
    map< string, bool> visited;
    vector< string> path;
    string tmp;
    
    visited[s]=true;
    stac.push(s);
    
    while(!stac.empty())
    {
        tmp = stac.top();
        path.push_back(tmp);
        if(tmp == d)
            return path;
        d_it = dict.find(tmp); 
        stac.pop();
        
        for( v_it = d_it->second.begin();v_it!=d_it->second.end(); v_it++)
        {
            if(visited.find(*v_it) == visited.end())    //Node not present
            {
                
                stac.push(*v_it);
                visited[*v_it]=true;
            }
        }
    }
    return path;
}



int main() 
{
    //fw; 
    fr;
    string s,t;
    for(int i=0;i<23;i++)
    { 
        cin>>s>>t;
        dict[s].pb(t);
        dict[t].pb(s);
    }

    for (std::map< string , vector<string> >::iterator it=dict.begin(); it!=dict.end(); ++it)
        sort(it->second.rbegin(),it->second.rend());

    vector< string> path_1 = dfs("dobreta","fagaras");
    vector< string> path_2 = dfs("fagaras","dobreta");
    
    for( vector<string>::const_iterator eptr=path_1.begin(); eptr!=path_1.end(); eptr++)
                cout << *eptr << " ";
    cout << endl << endl;
    for( vector<string>::const_iterator eptr=path_2.begin(); eptr!=path_2.end(); eptr++)
                cout << *eptr << " ";
    return 0;
}
