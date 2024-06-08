#include<bits/stdc++.h>
using namespace std;
#define N 10000
#define n 1728

vector<vector<string>> cardata(n+2);
map<string,vector<string>> valOfAttributes{
	{"buying", {"vhigh", "high", "med", "low"}},
	{"maint",  {"vhigh", "high", "med", "low"}},
	{"doors",  {"2", "3", "4", "5more"}},
	{"persons", {"2", "4", "more"}},
	{"lug_boot",{"small", "med", "big"}},
	{"safety",  {"low", "med", "high"}}
};
vector<int>values[N][4];
vector<pair<string,int>>vnode[N];
string attributeOfNode[N];
vector<string> vals{
	"unacc", "acc", "good", "vgood"
};
map<string,int>mpgoal{
	{"unacc", 0},
	{"acc", 1},
	{"good", 2},
	{"vgood", 3}
};
map<string,int>mpattribute{
	{"buying", 0},
	{"maint", 1},
	{"doors", 2},
	{"persons", 3},
	{"lug_boot", 4},
	{"safety", 5},
	{"goal", 6}
};
int leaf[N], parent[N];
int nodeCount=1;
int allocateId(){
	return nodeCount++;
}

void init(){
	nodeCount=1;
	for(int i=0; i<N; i++){
		leaf[i]=parent[i]=0;
		attributeOfNode[i].clear();
		vnode[i].clear();
		for(int j=0; j<4; j++)
			values[i][j].clear();
	}
}

void splitData(vector<int>& training, vector<int>& testing, int percent){
	vector<int>v, v1;
	for(int i=0; i<n; i++)
		v.push_back(i);
	while(v.size()){
		//int r = 0;
		int r = rand()%v.size();
		if(v.size()*100>(100-percent)*n)
			training.push_back(v[r]);
		else testing.push_back(v[r]);

		for(int i=0; i<v.size(); i++)
			if(i!=r) v1.push_back(v[i]);

		v.clear();
		v=v1;
		v1.clear();
	}
}

void splitString(string s, int ind){
	string ss="";
	for(int i=0; i<s.size(); i++){
		if(s[i]==','){
			cardata[ind].push_back(ss);
			ss="";
		}else{
			ss+=s[i];
		}
	}
	cardata[ind].push_back(ss);
}
double f(double a, double sz){
	if(a<=0) return 0;
	else return (a/sz)*log(sz/a);
}
double findEntropy(double a, double b, double c, double d){
	double sz = a+b+c+d;
	return f(a,sz)+f(b,sz)+f(c,sz)+f(d,sz);
}

double findGain(int node, string attribute){
	//cout<<"Check"<<endl;
	double mthersz = values[node][0].size()+values[node][1].size()+values[node][2].size()+values[node][3].size();
	double motherEntropy = findEntropy(values[node][0].size(),values[node][1].size(),values[node][2].size(),values[node][3].size());

	double vall=0;
	
	for(auto x:valOfAttributes[attribute]){
		 int arr[4]={0};
		 for(int j=0; j<4; j++){
			 for(int i=0; i<values[node][j].size(); i++){
			 	if(cardata[values[node][j][i]][mpattribute[attribute]]==x)
			 		arr[j]++;
			 }
		}
		vall += (arr[0]+arr[1]+arr[2]+arr[3])*findEntropy(arr[0],arr[1],arr[2],arr[3]);
	}
	//cout<<"Something findgain "<<node<<' '<<mthersz<<endl;
	return motherEntropy - vall/mthersz;
}

string pluraityOfParent(int node){
	int val=0, ind=0;
	for(int i = 0; i<4; i++){
		int val1=values[node][i].size();
		if(val1>val){
			ind = i;
			val = val1;
		}
	} 
	return vals[ind];
}
void decisionTree(int node, vector<string> attributes)
{
	//cout<<"Something "<<node<<endl;
	int sum=0, ind=-1;
	for(int i = 0; i<4; i++){
		sum+=values[node][i].size();
		if(values[node][i].size()>0){
			ind = i;
		}
	}
	if(ind==-1){
		attributeOfNode[node] = pluraityOfParent(parent[node]);
		leaf[node]=1;
		return;
	}
	else if(sum == values[node][ind].size()){
		attributeOfNode[node] = vals[ind];
		leaf[node]=1;
		return;
	}
	else if(attributes.size()==0){
		attributeOfNode[node] = pluraityOfParent(node);
		leaf[node]=1;
		return;
	}

	double maingain=-1;
	ind=-1;
	for(int i=0; i<attributes.size(); i++){
		double gain = findGain(node, attributes[i]);
		if(gain>maingain){
			ind = i;
			maingain = gain;
		}
		//cout<<gain<<endl;
	}
	string attribute = attributes[ind];

    attributeOfNode[node] = attribute;
    //cout<<attribute<<endl;


	vector<string> newattributes;
	for(auto x:attributes){
		if(x!=attribute) newattributes.push_back(x);
	}
	for(auto x:valOfAttributes[attribute]){
		 int newnode = allocateId();
		 for(int j=0; j<4; j++){
			 for(int i=0; i<values[node][j].size(); i++){
			 	if(cardata[values[node][j][i]][mpattribute[attribute]]==x)
			 		values[newnode][j].push_back(values[node][j][i]);
			 }
		}


		decisionTree(newnode, newattributes);
		parent[newnode] = node;
		vnode[node].push_back({x,newnode});
	}
}

string findAns(int index, int node){
	if(leaf[node]) return attributeOfNode[node];
	string attribute = attributeOfNode[node];
	string edge = cardata[index][mpattribute[attribute]];
	int newnode;
	for(auto x:vnode[node]){
		if(x.first == edge){
			newnode = x.second;
			break;
		}
	}
	return findAns(index,newnode);
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    //take cardata int the vector cardata from the text file
    vector<double>ans;
    ifstream in("car.data");
	int ind=0;
	while(!in.eof()){
		string text;
		getline(in, text);
		splitString(text,ind);
		ind++;
	}
    //ends here
    for(int test=1; test<=20; test++){
	    
	    srand(time(0));
		vector<int>training, testing;
		splitData(training, testing, 80);
	    
		for(int i=0; i<training.size(); i++){
			values[0][mpgoal[cardata[training[i]][6]]].push_back(training[i]);
		}
		vector<string> attributes={
			"buying", "maint",  "doors",  "persons", "lug_boot","safety"
		};
		//cout<<"OKAY"<<endl;
		decisionTree(0,attributes);

		double match = 0;
		for(int i=0; i<testing.size(); i++){
			//cout<<i<<endl;
			string goal = findAns(testing[i],0);
			//cout<<i<<' '<<goal <<' '<< cardata[testing[i]][6]<<endl;
			if(goal == cardata[testing[i]][6])
				match++;
		}
		double accuracy = (match*100)/testing.size();
		ans.push_back(accuracy);
		cout<<"Test "<<test<<"\t:\t"<<accuracy<<endl;

		init();
	}

	double average=0, deviation=0, square=0;
	for(auto x:ans){
		average+=x;
		square+=x*x;
	}
	average/=ans.size();
	square/=ans.size();
	deviation=sqrt(square-average*average);
	cout<<endl<<"Average:\t"<<average<<endl<<"Deviation:\t"<<deviation/100<<endl;
    return 0;
}

