#include<bits/stdc++.h>
using namespace std;

class board{

public:
	int a[100][100];
	int n;
	int fofn;
	int move;
	int id;
	string dir;
	board *parent;
	int zerox,zeroy;

	board(int n){
		parent=NULL;
		this->n = n;
		dir="";
		fofn=0;
		move=0;
		id=0;
	}
	board(){

	}

	bool equal(board* b){
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++)
				if(a[i][j]!=b->a[i][j]) return 0;
		}
		return 1;
	}

	int Hamming_distance(){
		int count = -1;
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++)
				if(a[i][j]!=(i*n+j+1)) count++;
		}
		return count;
	}

	int Manhattan_distance(){
		int count=0;
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++){
				if(a[i][j]!=0){
					int x = (a[i][j]-1)/n+1;
					int y = a[i][j]%n;
					if(y==0) y=n;
					count+=abs(x-i-1)+abs(y-j-1);
				}	
			}
		}
		return count;
	}

	void find_zeros_position(){
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++)
				if(a[i][j]==0){
					zerox = i; zeroy = j;
					break;
				}
		}
	}

	int inversion(){
		int count=0;
		for(int k=0; k<n; k++){
			for(int l=0; l<n; l++){
				for(int i=0; i<n; i++){
					for(int j=0; j<n; j++)
						if(i*n+j>k*n+l && a[i][j]!=0 && a[k][l]!=0 && a[k][l]>a[i][j])
							count++;
				}
			}
		}
		return count;
	}

	board* right_child(){
		board* b = new board(n);
		copy(b);
		b->find_zeros_position();
		b->move = move+1;
		b->parent = this;
		b->dir = "right";
		swap(b->a[zerox][zeroy],b->a[zerox][zeroy+1]);
		return b;
	}
	board* left_child(){
		board* b = new board(n);
		copy(b);
		b->find_zeros_position();
		b->move = move+1;
		b->parent = this;
		b->dir = "left";
		swap(b->a[zerox][zeroy],b->a[zerox][zeroy-1]);
		return b;
	}
	board* upper_child(){
		board* b = new board(n);
		copy(b);
		b->find_zeros_position();
		b->move = move+1;
		b->parent = this;
		b->dir = "up";
		swap(b->a[zerox][zeroy],b->a[zerox-1][zeroy]);
		return b;
	}
	board* lower_child(){
		board* b = new board(n);
		copy(b);
		b->find_zeros_position();
		b->move = move+1;
		b->parent = this;
		b->dir = "down";
		swap(b->a[zerox][zeroy],b->a[zerox+1][zeroy]);
		return b;
	}

	void copy(board* b){
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++)
				b->a[i][j]=a[i][j];
		}
	}

	void print(){
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++)
				cout<<a[i][j]<<' ';
			cout<<endl;
		}
	}

	string number(){
		string s="";
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++){
				int k = a[i][j];
				char c = k%10+'0';
				k/=10;
				if(k!=0){
					char c = k%10+'0';
					s+=c;
				}
				s+=c;
			}
		}
		return s;
	}


};

class Compare {
    public:
       bool operator()(board* a, board* b){
           if(a->fofn>b->fofn){
               return true;
           }
           return false;
      }
};

map<string,int> mp;
bool check(board* b, int n){
	return !mp[b->number()];
}

void print(board* b){
	if(b->move==0) {
		cout<<b->dir<<endl;
		b->print();cout<<endl;
		return;
	}
	print(b->parent);
	cout<<b->dir<<endl<<endl;
	b->print(); cout<<endl;
}


int main()
{

    int n;
    cin>>n;
    board* b = new board(n);
    for(int i=0; i<n; i++){
    	for(int j=0; j<n; j++)
    		cin>>b->a[i][j];
    }
    board* final = new board(n);
    for(int i=0; i<n; i++){
    	for(int j=0; j<n; j++)
    		final->a[i][j]=i*n+j+1;
    }
    final->a[n-1][n-1]=0;

    b->find_zeros_position();

    if(n%2 && b->inversion()%2) cout<<"Unsolvable puzzle"<<endl;
    else if(n%2==0 && (b->inversion()+n-1-b->zerox)%2) cout<<"Unsolvable puzzle"<<endl;
    else{
	    priority_queue<board*, vector<board*>, Compare> pq;
	    pq.push(b);
	    int explored_nodes=1, expanded_nodes=0;
	    mp[b->number()]=1;
	    while(!pq.empty()){
	    	
	    	board* c = pq.top(); //cout<<c->number()<<endl;
	    
	    	pq.pop();  
	    	if(c->equal(final)){
	    		cout<<"Minimum number of moves = "<<c->move<<endl;
	    		cout<<"Explored nodes = "<<explored_nodes<<endl;
	    		cout<<"Expanded nodes = "<<expanded_nodes<<endl;
	    		cout<<"\n\n";	    		
	    		print(c);
	    		
	    		break;
	    	}
	    	expanded_nodes++;

	    	c->find_zeros_position();
	    	if(c->zeroy!=(n-1)){
	    		board* child = c->right_child();
	    		if(check(child,n)){
		    		child->fofn = child->Manhattan_distance()+child->move;
		    		//child->fofn = child->Hamming_distance()+child->move;
		    		pq.push(child);
		    		explored_nodes++;
		    		mp[child->number()]=1;
		    	}
	    	}
	    	if(c->zeroy!=0){
	    		board* child = c->left_child();
	    		if(check(child,n)){
		    		child->fofn = child->Manhattan_distance()+child->move;
		    		//child->fofn = child->Hamming_distance()+child->move;
		    		pq.push(child);
		    		explored_nodes++;
		    		mp[child->number()]=1;

		    	}
	    	}
	    	if(c->zerox!=(n-1)){
	    		board* child = c->lower_child();
	    		if(check(child,n)){
		    		child->fofn = child->Manhattan_distance()+child->move;
		    		//child->fofn = child->Hamming_distance()+child->move;
		    		pq.push(child);
		    		explored_nodes++;
		    		mp[child->number()]=1;
		    	}
	    	}
	    	if(c->zerox!=0){
	    		board* child = c->upper_child();
	    		if(check(child,n)){
		    		child->fofn = child->Manhattan_distance()+child->move;
		    		//child->fofn = child->Hamming_distance()+child->move;
		    		pq.push(child);
		    		explored_nodes++;
		    		mp[child->number()]=1;
		    	}
	    	}
	    }

	}
    return 0;
}