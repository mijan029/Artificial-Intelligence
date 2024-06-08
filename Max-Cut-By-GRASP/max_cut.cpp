#include<bits/stdc++.h>
using namespace std;
#define inf 1e9
#define N 100005
#define pii pair<int,int>
#define numOfIteration 5

vector<int>adj[N];
map<pii,int>valOf;
int bestValue, localSearchBestValues, localSearchIterations;
int n, m;

void init(){
	bestValue = -inf;
	localSearchIterations=0;
	localSearchBestValues=0;
}

int random(int range){
	return rand() % range;
}

pair<vector<int>, int> simpleRandomization(){
	vector<int>X, inX(n+1,0);
	int value=0;
	for(int i=1; i<=n; i++){
		if(random(100)>=50) {
			X.push_back(i);
			inX[i]=1;
		}
	}
	for(int i=1; i<=n; i++){
		for(auto u:adj[i]){
			if(inX[i] && !inX[u]) value+=valOf[{i,u}];
		}
	}
	return {X,value};
}

pii maxEdge(){
	int nodea=-1, nodeb = -1, value = -inf;
	for(int i=1; i<=n; i++){
		for(auto u:adj[i]){
			if(valOf[{i,u}] > value){
				nodea = i;
				nodeb = u;
				value = valOf[{i,u}];
			}
		}
	}
	return {nodea, nodeb};
}

vector<pair<pii,int>> produceGreedyFunctionValue(vector<int>& inX){
	vector<pair<pii,int>>greedyValue;
	for(int i=1; i<=n; i++){
		if(!inX[i]){
			int val=0, insetX=1;
			for(auto u:adj[i]){
				if(inX[u]==-1) val+=valOf[{i,u}];
			}
			int mxval = val;
			val=0;
			for(auto u:adj[i]){
				if(inX[u]==1) val+=valOf[{i,u}];
			}
			if(val>mxval){
				mxval = val;
				insetX = -1;
			}
			greedyValue.push_back({{mxval,i},insetX});
		}
	}
	sort(greedyValue.begin(), greedyValue.end());
	return greedyValue;
}

pair<vector<int>, int> simpleGreedy(){
	vector<int>X, inX(n+1,0);
	int value=0;
	pair<int,int> mxEdge = maxEdge();
	X.push_back(mxEdge.first);
	inX[mxEdge.first]=1;
	inX[mxEdge.second] =-1;
	value+= valOf[mxEdge];
	int nn = n-2;
	while(nn--){
		vector<pair<pii,int>> v = produceGreedyFunctionValue(inX);
		pair<pii,int> pp = v[v.size()-1];
		if(pp.second==1){
			X.push_back(pp.first.second);
			inX[pp.first.second] = 1;
		}else{
			inX[pp.first.second] = -1;
		}
		value+=pp.first.first;
	}

	return {X,value};
}

pair<vector<int>, int> semiGreedy(){
	vector<int>X, inX(n+1,0);
	int value=0;
	pair<int,int> mxEdge = maxEdge();
	X.push_back(mxEdge.first);
	inX[mxEdge.first]=1;
	inX[mxEdge.second] =-1;
	value+= valOf[mxEdge];
	int nn = n-2;
	while(nn--){
		vector<pair<pii,int>> v = produceGreedyFunctionValue(inX);
		//here simple and semigreedy differs
		int choose = random(100);
		pair<pii,int>pp;
		if(choose>=50){
			int percent = random(100);
			int cardinality = (percent*v.size())/100;
			cardinality = max(cardinality,3);
			int selectRandomly = random(cardinality);
			int index = v.size()-cardinality+selectRandomly;
			index = max(index,0);
			pp = v[index];
		}else{
			int mn = v[0].first.first;
			int mx = v[v.size()-1].first.first;
			int alpha = random(100);
			int lowerBound = (mn*100+(mx-mn)*alpha)/100;
			int index=0;
			for(int i=0; i<v.size(); i++){
				if(v[i].first.first>=lowerBound) {
					index = i;
					break;
				}
			}
			pp = v[index];
		}

		//here their deiiferrences ends
		if(pp.second==1){
			X.push_back(pp.first.second);
			inX[pp.first.second] = 1;
		}else{
			inX[pp.first.second] = -1;
		}
		value+=pp.first.first;
	}

	return {X,value};
}

int check(int i, vector<int>& inX){
	int val=0;
	for(int u:adj[i]){
		if(inX[u]==1) val+=valOf[{i,u}];
		else val-=valOf[{i,u}];
	}
	if(inX[i]==0) val=-val;
	return val;
}

void localSearch(string s, vector<int>&inX, int& value0){
	//cout<<n<<' '<<m<<' '<<value0<<' '<<s<<endl;
	if(s == "First improvement"){
		while(1){
			int key=0;
			for(int i=1; i<=n; i++){
				int val = check(i,inX);
				if(val>0){
					value0+=val;
					inX[i]=1-inX[i];
					key=1; //cout<<val<<' '<<s<<endl;
					break;
				}
			}
			if(!key) break;
			localSearchIterations++;
		}
	}else{
		while(1){
			int index=0, mxval=-inf;
			for(int i=1; i<=n; i++){
				int val = check(i,inX);
				if(val>0 && val>mxval){
					mxval = val; //cout<<val<<' '<<s<<endl;
					index = i;
				}
			}
			if(!index) break;
			value0 +=mxval;
			inX[index]=1-inX[index];
			localSearchIterations++;
		}
	}
}

void grasp(string s){
	for(int i=0; i<numOfIteration; i++){
		// constructing a solution
		pair<vector<int>,int> aSolution;
		vector<int>X;
		int value0;
		if(s == "simple randomization"){
			aSolution = simpleRandomization();
		}if(s == "simple greedy"){
			aSolution = simpleGreedy();

		}if(s == "semi greedy"){
			aSolution = semiGreedy();
		}
		X = aSolution.first;
		value0 = aSolution.second;

		//cout<<n<<' '<<m<<' '<<value0<<endl;
		//Local search from aSolution
		vector<int>inX(n+1,0);
		for(int u:X) inX[u]=1;

		int choose = random(100);
		if(choose>=50) localSearch("First improvement", inX, value0);
		else localSearch("Best improvement",inX, value0);

		// localSearch("First improvement", inX, value0);
		// localSearch("Best improvement",inX, value0);

		bestValue = max(bestValue,value0);
		localSearchBestValues+=value0;

	}
}

int main(void)
{	

	srand(time(NULL));

	//freopen(ss,"r",stdin); 
	//freopen("out.txt","a",stdout);

	cin>>n>>m;

	for(int i=0; i<m; i++){
		int x, y, w;
		cin>>x>>y>>w;
		adj[x].push_back(y);
		adj[y].push_back(x);
		valOf[{x,y}] = valOf[{y,x}] = w;
	}
	
	

	//cout<<"\tn"<<"\t"<<"m\t"<<"construction\t\t"<<"localSearchBestValues\t"<<"localSearchIterations\t"<<"bestValue"<<"\t"<<"knownValue"<<endl;
	//for(int i=0; i<120; i++)cout<<"-";
	//cout<<endl;

	init();
	grasp("simple randomization");
	cout<<"\t\t\t"<<"simple rand"<<"\t\t"<<(double)localSearchBestValues/numOfIteration<<"\t\t"<<(double)localSearchIterations/numOfIteration<<"\t\t"<<numOfIteration<<"\t\t"<<bestValue<<"\t"<<endl;

	init();
	grasp("simple greedy");
	cout<<n<<"\t"<<m<<"\t";
	cout<<"simple greedy"<<"\t\t"<<(double)localSearchBestValues/numOfIteration<<"\t\t"<<(double)localSearchIterations/numOfIteration<<"\t\t"<<numOfIteration<<"\t\t"<<bestValue<<"\t"<<endl;

	init();
	grasp("semi greedy");
	cout<<"\t\t\t"<<"semi greedy"<<"\t\t"<<(double)localSearchBestValues/numOfIteration<<"\t\t"<<(double)localSearchIterations/numOfIteration<<"\t\t"<<numOfIteration<<"\t\t"<<bestValue<<"\t"<<endl;
	cout<<"\n\n\n";
	
	return 0;
}
