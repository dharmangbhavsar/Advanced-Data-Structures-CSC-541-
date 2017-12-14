/*Author:@abs51295*/
#include <bits/stdc++.h>
#include<fstream>
#define fr freopen("input.in","r",stdin)
#define fw freopen("output.out","w",stdout)
#define iOs ios_base::sync_with_stdio(false);
#define INF 1000000009
#define MOD 1000000007
#define all(x) (x).begin(), (x).end()
#define pb push_back
#define mp make_pair
using namespace std;
typedef long long ll;
typedef long double ld;
typedef pair<int, int> pii;
typedef vector<int> vi;
typedef vector<vi> vvi;
int stoi(
const string& _Str,
size_t *_Idx = 0,
int _Base = 10
);
std::string::size_type sz;
int main() {
    fw;fr;
    iOs;
    int score[2] = {0};
    bool first[2] = {false};
    bool invalid = false, ques = false;
    int t;
    int r_ans=1,u_ans;
    cin >> t;
    string temp;
    //vi v;
    while(t--) {
        string type;
        string name;
        string numbers;
        string uans;
        //ques = false;
        vi v;
        //int r_ans=1,u_ans;
        cin >> type;
        if(type[0] == 'A' and ques) {
            ques = false;
            cin >> name;
            cin >> uans;
            if(uans[0] == 'P') {
                    if(name[0] == 'D') {
                        //score[0] += 10;
                        cout << "Sally's question is: " << temp << endl;
                        cout <<  "Question is PASSed" << endl;
                        cout << "Answer is: " << r_ans << endl;
                        cout << "Darrell: " << score[0] << "points" << endl;
                    } else if(name[0] == 'S') {
                        //score[1] += 10;
                        cout << "Darrell's question is: " << temp << endl;
                        cout << "Question is PASSed" << endl;
                        cout << "Answer is: " << r_ans << endl;
                        cout << "Sally: " << score[1] << "points" << endl;
                    } else {
                        cout << "Invalid Input";
                        return 0;
                    }

            } else {
                u_ans = stoi(uans,nullptr,20);
                if(u_ans == r_ans) {
                    if(name[0] == 'D') {
                        score[0] += 10;
                        cout << "Sally's question is: " << temp << endl;
                        cout <<  "Correct Answer" << endl;
                        cout << "Darrell: " << score[0] << "points" <<  endl;
                    } else if(name[0] == 'S') {
                        score[1] += 10;
                        cout << "Darrell's question is: " << temp << endl;
                        cout << "Correct Answer" << endl;
                        cout << "Sally: " << score[1] << "points"<< endl;
                    } else {
                        cout << "Invalid Input";
                        return 0;
                    }
                } else {
                    if(name[0] == 'D') {
                        //score[0] += 10;
                        cout << "Sally's question is: " << temp << endl;
                        cout <<  "Wrong Answer" << endl;
                        cout << "Darrell: " << score[0] <<"points" <<endl;
                    } else if(name[0] == 'S') {
                        //score[1] += 10;
                        cout << "Darrell's question is: " << temp << endl;
                        cout << "Wrong Answer" << endl;
                        cout << "Sally: " << score[1] << "points"<<endl;
                    } else {
                        cout << "Invalid Input";
                        return 0;
                    }
                }
            }
            //r_ans=1;
        } else if(!ques and type[0]!= 'A') {
            r_ans=1;
            ques = true;
            if(!(first[0] or first[1])){
                if(type[0] == 'D'){
                    first[0] = true;
                }
                else{
                    first[1] = true;
                }
            }
            //cin >> name;
            cin >> numbers;
            if(numbers == "") {
                cout << "Invalid Input";
                return 0;
            }
            //string temp(numbers);
            temp = numbers;
            stringstream ss(numbers);
            int x;
            while(ss >> x) {
                v.pb(x);
                if(ss.peek() == ',') {
                    ss.ignore();
                }
            }
            for(int i=0; i<v.size(); i++) {
                r_ans = r_ans*v[i]/__gcd(r_ans,v[i]);
            }
        }
        else{
            cout << "Invalid Input";
            return 0;
        }
    }
    cout << "Total Points:" << endl;
    if(first[0]){
        cout << "Darrell: " << score[0] << "points" << endl;
        cout << "Sally: " << score[1] << "points" << endl;
    }
    else if(first[1]){
        cout << "Sally: " << score[1] << "points" << endl;
        cout << "Darrell: " << score[0] << "points" << endl;
    }
    cout << "Game Result: ";
    if(score[0] > score[1]){
        cout << "Darrell is winner";
    }
    else if(score[0] < score[1]){
        cout << "Sally is winner";
    }
    else{
        cout << "Draw";
    }
}

