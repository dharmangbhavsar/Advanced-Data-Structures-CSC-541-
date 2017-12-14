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
#define HI 0.5
#define LO -0.5
typedef long long LL;
typedef pair<int, int> pii;
typedef vector<int> vi;
typedef vector<vi> vvi;
vector< vector<double> > layer,getinweight;
vector< vector< vector< double> > > weight; 
vector<double> lol,col,pol;
//sigmoid function used is pow((1+pow(e,-x)),-1);
bool flag1=false,flag2=false; int counter=0;
vector<double> opgrad;//contains gradient of outputs
vector< vector< double > > hidgrad,duphidgrad;//contains gradients of hidden layer        
int main()
{
    iOs;
    //Getting the number of layers needed in the neural net
    int n; cout<<"Enter the number of layers needed in the neural net\n"; cin>>n;
    //Getting the number of omputing units needed in each layer of the net
    for(int i=0;i<n;i++)
    {
    	int t; 
    	cout<<"Enter the number of computing units needed in layer number "<<i+1<<endl;
    	cin>>t; lol.pb(t);
    }
    //initialising all the computing units in the net at zero.
    for(int i=0;i<lol.size();i++)
    {
    	for(int j=0;j<lol[i];j++)
    	{
    		pol.pb(0);
    	}
    	layer.pb(pol);
    	pol.clear();
    }
    //Getting input from user for the neural network
    for(int i=0;i<layer[0].size();i++)
    {
        cout<<"Enter the input for the network:\n";
        double d; cin>>d;
        layer[0][i]=d;
        //cout<<endl<<layer[0][i]<<endl;
    }
    //Getting the expected output of the neural net for the given input according to the output layer involved
    vector<int> expected;
    for(int i=0;i<layer[layer.size()-1].size();i++)
    {
        cout<<"Enter the expected output of the neural net:\n";
        int x; cin>>x; 
        expected.pb(x);
    }
    lol.clear();
    //debug;
    //Setting the weight value for all the weights in the neural net
    //Weights are set as the nodes coming out from a computing unit
    //Thus easier to access the weights whenever needed
    for(int i=0;i<layer.size()-1;i++)
    {
    	//debugname;
        for(int j=0;j<layer[i].size();j++)
        {
            for(int k=0;k<layer[i+1].size();k++)
            {
                pol.pb( LO + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(HI-LO))));
            }
            getinweight.pb(pol);
            pol.clear();
        }
        weight.pb(getinweight);
        getinweight.clear();
        //cout<<"Done:"<<i+1<<endl;
    }
    /*
    Just to  check the weights.
    for(int i=0;i<weight.size();i++)
        {for(int j=0;j<weight[i].size();j++)
            {for(int k=0;k<weight[i][j].size();k++)
                {cout<<weight[i][j][k]<<" ";}
        cout<<endl;}
    cout<<endl;}*/
    WhereLifeTakesMe:
    double summer=0.0; 
    /*variable to save the sum of the produc of (computing unit output and weight of the node getting it to the next layer)*/
    //first propagation through the network. 
	for(int i=0;i<layer.size()-1;i++)
    {
        for(int j=0;j<layer[i+1].size();j++)
        {
            summer=0.0;
            for(int k=0;k<layer[i].size();k++)
            {
                summer+=(layer[i][k]*weight[i][k][j]);
                cout<<"layer "<<i<<" "<<k<<'\t'<<"weight "<<i<<" "<<k<<" "<<j<<" \n";
            }
            cout<<"add:\n";
            layer[i+1][j]=pow(1+exp(-(summer)),-1);
        }
    }
    //Checking the output of the layers
    cout<<"Output Of Layers:\n";
    for(int i=0;i<layer.size();i++)
    {
        cout<<i+1<<"th layer: ";
        for(int j=0;j<layer[i].size();j++)
        {
            cout<<layer[i][j]<<" ";
        }
        cout<<endl;
    }
    //Checking if the outputs of the network is as it was desired or not
    vector<int> check; bool checker=true;
    //We will change the flag if the output will not be equal to the input
    for(int i=0;i<expected.size();i++)
    {
        check.pb(round(layer[layer.size()-1][i]));
    }
    for(int i=0;i<expected.size();i++)
    {
        //changing the boolean value of checker to false if the desired output is not equal to the net output
        if(expected[i]!=check[i])
            {checker=false; }
    }
    //If the output of net is not the expected output then we go into the if condition
    if(checker==false)
    {
        counter++;
        cout<<"    \n\n        Training Number:                "<<counter<<endl;
        //Now we should get the gradients of the net 
        //Mainly the output gradients and hidden layer gradients
        //"opgrad" contains the gradient of output layer
        //cout<<"\n\n\n\nHello Before\n\n\n\n";
        opgrad.clear();
        for(int i=0;i<layer[layer.size()-1].size();i++)
        {
        	cout<<"check this->   "<<layer[layer.size()-1][i]<<" "<<layer[0][i]<<endl;
            opgrad.pb((double)(expected[i]-layer[layer.size()-1][i])*(double)((double)1-layer[layer.size()-1][i])*(double)layer[layer.size()-1][i]);
        }
        // cout<<"\n\n\n Hello There\n\n\n";
        //Printing the output layer gradient
        cout<<"Gradient of the output:\n";
        for(int i=0;i<opgrad.size();i++)
        {
            cout<<opgrad[i]<<" ";
        }
        debugname;
        cout<<endl;
        //Now, calculating the hidden layer gradient of the network for every hidden layer computing unit
        //"hidgrad" contains the gradient of hidden layer
        //initialising the hidden layer gradient
        if(counter==1)
        {
            pol.pb(0);
            hidgrad.pb(pol);
            hidgrad.clear();
            pol.clear();
            for(int i=1;i<layer.size()-1;i++)
            {
                for(int j=0;j<layer[i].size();j++)
                {
                    pol.pb( LO + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(HI-LO))));
                }
                hidgrad.pb(pol);
                duphidgrad.pb(pol);
                pol.clear();
            }
        }
        debug;
        summer=0.0;
        //cout<<hidgrad.size()<<"\t\tDepicting the size of hidgrad helping in making the summer function\n";
        //summer adds the product of (gradient of next layer*weight taking it to the next layer)
        //hidgrad is the hidden layer
        //hidgrad is equal to the product of summer*same computing unit output*(1-same computing unit output)
        for(int i=layer.size()-2;i>0;i--)
        {
            for(int j=0;j<layer[i].size();j++)
            {
                for(int k=0;k<weight[i][j].size();k++)
                {
                    if(i==layer.size()-2)
                    {
                        summer+=(double)((double)weight[i][j][k]*(double)opgrad[k]);
                    }
                    else
                    {
                        summer+=(double)((double)weight[i][j][k]*(double)hidgrad[i][k]);
                    }
                }
          		//duphidgrad is for batch updates and doing the updates simultaneously
          		//So that the updated hidgrad doesnt interfere.
                duphidgrad[i-1][j] = (double)summer*(double)layer[i-1][j]*(double)((double)1-layer[i-1][j]);
            }
        }
        //Batch updating the hidden layer gradients
        //Printing the hidden layer's gradients
        for(int i=0;i<duphidgrad.size();i++)
        {
        	for(int j=0;j<duphidgrad[i].size();j++)
        	{
        		hidgrad[i][j]=duphidgrad[i][j];
        	}
        }
        cout<<"Hidden layer gradients are:\n";
        for(int i=0;i<hidgrad.size();i++)
        {
            for(int j=0;j<hidgrad[i].size();j++)
            {
                cout<<hidgrad[i][j]<<" ";
            }
            cout<<endl;
        }
        cout<<"Higrad ended\n";
        //Now changing the weights of the network according to the gradients of the computing units
        //all error gradients found
        //Now going towards finding the change in weights of all the connections of computing units
        //learning rate is the size of jumps that we will have on the graph of our network so that we 
        //achieve out desired output
        double learningrate=0.1;
        cout<<"Change in weights:\nLayer\tC.U.\n\n";
        for(int i=0;i<layer.size()-1;i++)
        {
            for(int j=0;j<layer[i].size();j++)
            {
            	cout<<i<<'\t'<<j<<endl;
                for(int k=0;k<weight[i][j].size();k++)
                {
                    if(i==layer.size()-2)
                    {
                        weight[i][j][k]-=(double)((double)learningrate*(double)layer[i][j]*(double)opgrad[k]);
                    }
                    else
                    {
                        weight[i][j][k]-=(double)((double)learningrate*(double)layer[i][j]*(double)hidgrad[i][k]);
                    }
                }
            }
        }
        //Printing the weights of the network after making the changes according to the gradient
        cout<<"The weights of the network after applying the change in them:\n";
        for(int i=0;i<weight.size();i++)
        {
            for(int j=0;j<weight[i].size();j++)
            {
                for(int k=0;k<weight[i][j].size();k++)
                {
                    cout<<weight[i][j][k]<<" ";
                }
                cout<<endl;
            }
            cout<<endl;
        }
        //if(counter<100)
        goto WhereLifeTakesMe;
    }
    return 0;
}
