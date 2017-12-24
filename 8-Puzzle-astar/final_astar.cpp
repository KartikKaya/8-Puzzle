#include<iostream>
#include<queue>
#include <cstdlib>
#include<vector>

using namespace std ;

//state-for state , op- for operators , gn for g(n) path cost, hn-h(n) heuristic function, fn -f(n)=g(n)+f(n), zpos - storing posistion of 0(blank tile)
//next-store next node address, parent_gn-stores path cost of parent 
struct anode {
	char* state;
	int op[6];
	int gn;
	int hn;
	int fn;
	int zpos;
	anode* next;
	int parent_gn;
};

//GLOBAL VARIABLES
char g[12];              //stores goal state
int GOAL_FOUND=1;       // Flag when goal state is found
vector <char*> V;      // Vector used in comparing OLD nodes
int CNT_NODES_EXP=0;  //Counts number of nodes expanded
int CNT_NODES_CREATED=0;

//CREATING PRIORITY QUEUE
struct node
{
	anode* val;
	int pr;
	struct node *next;
}*start;	


//FUNCTIONS
int cal_man(char* , char* );//Calculate Manhattan Distance
int* create_operator(int  , int* );//Create Operators that needs to be performed on each state to generate its Sucessor
anode* create_node(char* ,char* ,int );//Creates Intial Node from Intial State
void push(anode* ,int );// Pushes the node into priority queue
anode* pop();// pops out the node with min f(n) , and deletes it from the priority queue
char* create_state(char*,int,int);// Creates states from the given operator
int goal_test(char*,char*);//checks for goal
int old_nodes_check(char* );// checks for repeated states 
void create_succesor();// expands a given poped out node frim priority queue


int cal_man(char* g, char* s){
	
	int a[12]={0,1,2,10,11,12,20,21,22};
	
	int sum =0;
	int d1=0,d2=0,j=0;
	int x1,y1,x2,y2;
	int t1,t2;
	
    for(int i=0; i<9;i++){
	  if(s[i] == '0'){continue;}
	    d1=i;
		for(int j=0; j<9;j++){
			if(g[j]== '0'){continue;}
			d2=j;
		if(s[i]==g[j]){break;}	
			
		}	
	
	if(d1 >2 || d1==0){x1=a[d1]/10;y1=a[d1]%10;}
	if(d2 >2 || d2==0){x2=a[d2]/10;y2=a[d2]%10;}
	if(d1==1){x1=0; y1=1;}
	if(d2==1){x2=0;y2=1;}
	if(d1==2){x1=0; y1=2;}
	if(d2==2){x2=0;y2=2;}
	   		
		
	t1=abs(x1-x2);
	t2=abs(y1-y2);	
		
	sum=sum+t1+t2;	
	
	}
	
	return sum;
	
}

int* create_operator(int zpos , int* a){
	
	
	int k=0;
	if(zpos != 0 && zpos != 3 && zpos != 6){a[k]=1;k++;}
	if(zpos != 2 && zpos != 5 && zpos != 8){a[k]=2;k++;}
    if(zpos != 0 && zpos != 1 && zpos != 2){a[k]=3;k++;}
	if(zpos != 6 && zpos != 7 && zpos != 8){a[k]=4;k++;}		
    a[k]=-1;
	return a;
											  
}



int goal_test(char* s , char* g){

	for (int i=0 ; i<9 ; i++){
	if( s[i]!=g[i]){return 0;}
	}
return 1 ;
}

int old_nodes_check(char* cs){
int k;
for(int i=0; i<V.size();i++){
 k=0;
	for(int j=0;j<9;j++){
		if(V[i][j]!=cs[j]){k=1;break;}
		
	}
		if(k==0){return 1;}
}
return 0;
}

void create_succesor(){
	
anode* poped = pop();
CNT_NODES_EXP++;	
	
//insert into old node	
V.push_back(poped->state);

// Generate new node for each Operator	
for(int i=0;i<4;i++){
	if (poped->op[i]==-1){break;}
	
	anode* s1;
	s1 = new anode;
	
	char* poped_state;
	poped_state= new char[12];
	for(int k=0; k<9;k++){poped_state[k]=poped->state[k];}
	int zpos =poped->zpos;
    int op=poped->op[i];
	
	s1->state=create_state(poped_state,zpos,op);
	CNT_NODES_CREATED++;
		
	if(goal_test(s1->state,g)){
		cout<<"****GOAL FOUND****"<<endl ;
		cout<<endl<<s1->state[0]<<' '<<s1->state[1]<<' '<<s1->state[2]<<endl<<s1->state[3]<<' '<<s1->state[4]<<' '<<s1->state[5]<<endl<<s1->state[6]<<' '<<s1->state[7]<<' '<<s1->state[8]<<endl;
		cout<<"GOAL FOUND AT DEPTH: "<<poped->gn+1<<endl;
		GOAL_FOUND=0;
		break;
	}
	//Check for OLD_NODES/REPEATED STATES
	if(old_nodes_check(s1->state)){
		continue;
	}
	s1->gn=poped->gn+1;
	s1->hn=cal_man(g,s1->state);
	s1->fn=s1->gn+s1->hn;
	//caluclate zero position(blank tile position)
	for(int j=0;j<9;j++){
	if(s1->state[j]=='0'){
		s1->zpos=j;break;
	}
	} 
    int a[6];
	int* b;
	b=create_operator(s1->zpos,a);
	for(int h=0;h<3;h++){
	s1->op[h]=b[h];}
	
	s1->next=NULL;
	s1->parent_gn=poped->gn;		
	
	//PUSH exapnded Nodes into Priority queue
	push(s1,s1->fn);
	

}
	

	}


//Create New state according to operator
char* create_state( char* s, int zp , int op){

// 1- indicates move 0(blank tile) left	
// 2- indicates move 0(blank tile) right	
// 3- indicates move 0(blank tile) up
// 4- indicates move 0(blank tile) down

	if(op==1){
	s[zp]=s[zp-1];
	s[zp-1]='0';	
	}
	if(op==2){
	s[zp]=s[zp+1];
	s[zp+1]='0';	
	}
	if(op==3){
	s[zp]=s[zp-3];
	s[zp-3]='0';	
	}
	if(op==4){
	s[zp]=s[zp+3];
	s[zp+3]='0';	
	}	

return s;

}

//Create Initial Node from Initial State
anode* create_node(char* t,char* g,int zp){	
	char * tile =t;
	int a[6];
	int* b;
	b=create_operator(zp,a);

	anode *init;
	init= new anode;
	init->state=tile;
	for(int i=0;i<3;i++){
	init->op[i]=b[i];}
	init->gn=0;
	init->hn=cal_man(g,init->state);
	init->fn=init->gn+init->hn;
	init->zpos=zp;	
	init->next=NULL;
	init->parent_gn=-1;
//PUSH into priority queue
	push(init,init->fn);
	
	return init; 
	
}


// PUSH nodes into priority queue based in f(n)
void push(anode* ele,int pri)
{
	//struct
	node *temp, *t;
	temp = new node;
	temp->val=ele;
	temp->pr=pri;
	temp->next=NULL;
	
	if(start==NULL)
		start = temp;
	else if(start->pr>pri)
	{
		temp->next=start;
		start=temp;
	}
	else
	{
		t=start;
		while(t->next!=NULL && (t->next)->pr<=pri )
			t=t->next;
		temp->next=t->next;
		t->next=temp;
	}

}

//Remove Elements from Priority Queue and return to succesor function to expand
anode* pop() 
{
	anode* temp ;
	if(start!=NULL)
	{
	temp=start->val;
	start = start->next;
	}else{
	cerr << "QUEUE IS EMPTY ";
	}
	return temp;
}



//MAIN FUNCTION
int main (int argc, char* argv[]) {

if (argc < 19) {
    std::cerr<<"usage: "<<argv[0]<<"< PLEASE Enter Intial State of 8 Tiles followed by goal state tiles(USE 0 TO INDICATE BLANK TILES) <INTIAL STATE><GOAL STATE> "<<std::endl;
	std::cerr<<"IF INTIAL STATE IS:"<<std::endl;
	std::cerr<<"7"<<"8"<<"0"<<endl<<"1"<<"2"<<"3"<<endl<<"4"<<"5"<<"6"<<endl<<std::endl;
	std::cerr<<"IF GOAL STATE IS:"<<std::endl;
	std::cerr<<"1"<<"2"<<"3"<<endl<<"4"<<"5"<<"6"<<endl<<"7"<<"8"<<"0"<<endl<<std::endl;
	std::cerr<<"ENTER AS SHOWN BELOW:"<<endl;
	std::cerr<<argv[0]<<" 7 8 0 1 2 3 4 5 6 1 2 3 4 5 6 7 8 0"<<endl;
	
	
    return -1;
  }

	char t[12];
	//Calculate postion of 0 (blank tile)
	int zpos;	
	for(int i=1;i<10;i++){
	if(*argv[i]=='0'){
		zpos=i-1;
	}
	t[i-1]=*argv[i];
	} 
	cout<<"Given Intial STATE is :"<<endl;
	cout<<t[0]<<' '<<t[1]<<' '<<t[2]<<endl<<t[3]<<' '<<t[4]<<' '<<t[5]<<endl<<t[6]<<' '<<t[7]<<' '<<t[8]<<endl;
	
	
	
	for(int i=10;i<19;i++){
	g[i-10]=*argv[i];
	} 
	cout<<"Given GOAL STATE is :"<<endl;
	cout<<g[0]<<' '<<g[1]<<' '<<g[2]<<endl<<g[3]<<' '<<g[4]<<' '<<g[5]<<endl<<g[6]<<' '<<g[7]<<' '<<g[8]<<endl;;
	
	//Create Initial Node using intial state
	create_node(t,g,zpos);
	//Execute Succesor until Goal State is found
	while(GOAL_FOUND){
	create_succesor();
	}
	    cout<<"THE TOTAL NUMBER OF NODES CREATED : "<<CNT_NODES_CREATED<<endl;
		cout<<"THE TOTAL NUMBER OF NODES EXPANDED : "<<CNT_NODES_EXP<<endl;	
	   

}