#include<bits/stdc++.h>
using namespace std;
#define limit 10000000
#define mx_height 15
int num_of_iteration=0,w1=10000,w2=1000,w3=100,w4=10;


class board{
public:
    int a[15];
    int ind;
    int addi;
    int alpha;
    int beta;
    int height;
    int stat1;
    int stat2;
    int stat3;
    int stat4;
    int additional_move_1;
    int additional_move_2;
    int capture_1;
    int capture_2;

    board(){
        for(int i=0; i<6; i++)
            a[i]=4;
        for(int i=7; i<13; i++)
            a[i]=4;
        a[6]=a[13]=0;
        alpha=-limit;
        beta=limit;
        addi=height=stat1=stat2=stat3=stat4=additional_move_1=additional_move_2=capture_1=capture_2=0;
        ind=-1;
    }

    void set_state(int stat1, int stat2, int stat3, int stat4){
        addi=height=capture_1=capture_2=0;
        ind=-1;
        this->stat1 = stat1;
        this->stat2 = stat2;
        this->stat3 = stat3;
        this->stat4 = stat4;
        alpha=-limit;
        beta = limit;
    }
};
int a_empty(board b)
{
    int flag=1;
    for(int i=0; i<6; i++){
        if(b.a[i]) flag=0;
    }
    if(flag) return 1;
    flag=1;
    for(int i=7; i<13; i++){
        if(b.a[i]) flag=0;
    }
    return flag;
}

int evaluation_function(board b, int player_no)
{
    int val=0;
    int current_stones=0;
    for(int i=0; i<6; i++)
        current_stones+=b.a[i];
    for(int i=7; i<13; i++)
        current_stones-=b.a[i];

    int additional_move=b.additional_move_1;
    if(player_no==2) additional_move = b.additional_move_2;
    int capture = b.capture_1;
    if(player_no==2) capture = b.capture_2;
    val = w1*b.stat1*(b.a[6]-b.a[13])+w2*b.stat2*current_stones+w3*b.stat3*additional_move+w4*b.stat4*capture;
    return val;
}

pair<int,int> run(board* b, int i, int player_no){
    int val=0, capture=0;
    if(player_no==1){
        int amount = b->a[i];
        b->a[i]=0;
        int j=i+1;
        while (amount--)
        {
           if(j>12) j=0;
           b->a[j]++;
           j++;
        }
        if(j==7) val=1;
        else{
            j--;
            if(j<6 && b->a[j]==1 && b->a[12-j]!=0){
                b->a[6]+=b->a[j]+b->a[12-j];
                capture=b->a[12-j];
                b->a[j]=b->a[12-j]=0;
            }
        }
    }else{
        int amount = b->a[i];
        b->a[i]=0;
        int j=i+1;
        while (amount--)
        {
           if(j>13) j=0;
           if(j==6) j++;
           b->a[j]++;
           j++;
        }
        if(j==0) val=1;
        else{
            j--;
            if((j>6 && j<13) && b->a[j]==1 && b->a[12-j]!=0){
                b->a[13]+=b->a[j]+b->a[12-j];
                capture=b->a[12-j];
                b->a[j]=b->a[12-j]=0;
            }
        }
    }
    return {val,capture};
}

pair<int,int> optimal_solution(board b, int player_no)
{
    num_of_iteration++; 
    
    if(a_empty(b) || (num_of_iteration>limit/10 && b.height>mx_height)){
        int val = evaluation_function(b,player_no);
        if(a_empty(b)) val+=w1*(b.a[6]-b.a[13])*10;
        b.alpha=b.beta=val;
        return {val,0};
    }
    else{
        if(player_no==1){
            for(int i=0; i<6; i++){
                board bnew=b;
                bnew.height++;
                if(bnew.a[i]){
                     pair<int,int> val;
                     pair<int,int> p = run(&bnew,i,1); 
                     bnew.addi = p.first;
                     bnew.capture_1 = p.second;
                     bnew.additional_move_1+=bnew.addi;
                     if(b.addi) val = optimal_solution(bnew,1);
                     else val = optimal_solution(bnew,2);
                     if(val.first>b.alpha){
                        b.ind = i;
                        b.alpha=val.first;
                     }
                }
                if(b.alpha>=b.beta) return {b.alpha,b.ind};
            }
            return {b.alpha,b.ind};
        }else{
            for(int i=7; i<13; i++){
                board bnew=b;
                bnew.height++;
                if(bnew.a[i]){
                     pair<int,int> val;
                     pair<int,int> p = run(&bnew,i,2); 
                     bnew.addi = p.first;
                     bnew.capture_2 = p.second;
                     bnew.additional_move_2+=bnew.addi;
                     if(b.addi) val = optimal_solution(bnew,2);
                     else val = optimal_solution(bnew,1);
                     if(val.first<b.beta){
                        b.ind = i;
                        b.beta=val.first;
                     }
                }
                if(b.alpha>=b.beta) return {b.beta,b.ind};
            }
            return {b.beta,b.ind};
        }
    }
}

void print(board b, int player_no){
    cout<<"Turn of Player no: "<<player_no<<endl;
    cout<<"\t";
    for(int i=12; i>6; i--)
        cout<<b.a[i]<<"\t";
    cout<<endl;
    cout<<b.a[13];
    for(int i=0; i<7; i++)
        cout<<"\t";
    cout<<b.a[6]<<endl<<"\t";
    for(int i=0; i<6; i++)
        cout<<b.a[i]<<"\t";
    cout<<"\n\n\n\n";
}

void play(board b, int player_no){
    num_of_iteration=0;
    if(a_empty(b)){
        for(int i=0; i<6; i++)
            b.a[6]+=b.a[i];
        for(int i=7; i<13; i++)
            b.a[13]+=b.a[i];
        cout<<"Game has finished"<<endl;
        cout<<"ScoreBoard: \nfirst player: "<<b.a[6]<<"\nsecond player: "<<b.a[13]<<endl;
        if(b.a[6]>b.a[13]) cout<<"First player Wins"<<endl;
        else if(b.a[6]<b.a[13]) cout<<"Second player Wins"<<endl;
        else cout<<"Draw"<<endl;
        return;
    }
    if(player_no==1){
        b.set_state(1,1,1,1);
        int i = optimal_solution(b,1).second;
        pair<int,int> p = run(&b,i,1);
        print(b,player_no); 
        if(p.first) play(b,1);
        else play(b,2);
        return;
    }else{
        b.set_state(1,0,0,0);
        int i = optimal_solution(b,2).second;
        pair<int,int> p = run(&b,i,2);
        print(b,player_no); 
        if(p.first) play(b,2);
        else play(b,1);
        return;
    }
}
int main()
{
    board b;
    play(b,1);
}