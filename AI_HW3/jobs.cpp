#include<iostream>
#include<stdio.h>
#include<vector>
#include<string.h>
#include<map>

using namespace std;

int nodes_explored=0;//Number of nodes explored
static int solfound=0;//Flag to check if solution reached

int sum_arr(int a[8])//Calculate sum of an array
{
  int sum=0;
  for(int i=0;i<8;i++)
    sum=sum+a[i];
  return sum;
}

int any_index(int alive[8])//return any unassigned index
{
	int flag=1;
	for(int i=0;i<8;i++)
		if(alive[i]!=1)
			flag=0;
	if(flag==1)//means all variables assigned
		return -1;

	for(int i=0;i<8;i++)
		if(alive[i]!=1)
			return i;
}

int min_index(int alive[8])//return min index other than 1, -1 if all one
{
  int flag=1;
  for(int i=0;i<8;i++)
    if(alive[i]!=1)
      flag=0;
  if(flag==1)//means all variables assigned
    return -1;

  int min;
  for(int i=0;i<8;i++)
    if(alive[i]!=1)
    {
      min=i;
      break;
    }

  for(int i=0;i<8;i++)
    if(alive[i]<alive[min] && alive[i]!=1)
      min=i;
  return min;
}

class Node
{
  public:
    int depth;
    Node *parent;
    int alive_count[8];
    int nodes_alive[8][4];
    vector<Node*> msucc;//list of MRV based successors
    vector<Node*> succ;//list of normal successors
    vector<Node*> mrv_successors()//Successors for MRV based node expansion
    {
      vector<Node*> myv;
      int index = min_index(alive_count);//Find the minimum index -> MRV
      if(index==-1)
        return myv;
      for(int i=0;i<alive_count[index];i++)
      {
        Node *n = new Node();
        n->depth=(this->depth) + 1;
        n->parent=this;

        for(int j=0;j<8;j++)
          n->alive_count[j]=this->alive_count[j];
        (n->alive_count[index])=1;

        for(int j=0;j<8;j++)
          for(int k=0;k<4;k++)
            n->nodes_alive[j][k]=this->nodes_alive[j][k];
        int place;
        for(int k=0;k<4;k++)
        {
          if(nodes_alive[index][k]==1)
          {
            nodes_alive[index][k]=0;
            place=k;
            break;
          }
        }
        for(int k=0;k<4;k++)
        {
          n->nodes_alive[index][k]=0;
        }
        n->nodes_alive[index][place]=1;
        myv.push_back(n);
      }
      return myv;
    }

    vector<Node*> successors()//Successors for non MRV node expansion
    {
      vector<Node*> myv;
      int index = any_index(alive_count);//Take any index -> non MRV
      if(index==-1)
        return myv;

      for(int i=0;i<alive_count[index];i++)
      {
        Node *n = new Node();
        n->depth=(this->depth) + 1;
        n->parent=this;

        for(int j=0;j<8;j++)
          n->alive_count[j]=this->alive_count[j];
        (n->alive_count[index])=1;

        for(int j=0;j<8;j++)
          for(int k=0;k<4;k++)
            n->nodes_alive[j][k]=this->nodes_alive[j][k];
        int place;
        for(int k=0;k<4;k++)
        {
          if(nodes_alive[index][k]==1)
          {
            nodes_alive[index][k]=0;
            place=k;
            break;
          }
        }
        for(int k=0;k<4;k++)
        {
          n->nodes_alive[index][k]=0;
        }
        n->nodes_alive[index][place]=1;
        myv.push_back(n);
      }
      return myv;
    }
};

bool consistency_jobs(Node*);

string printState(Node* a)//Create a string of values assigned currently to node n
{
  string s="";
  int i,k,fl;
  for(i=0;i<8;i++)
  {
    if(a->alive_count[i]==1)
    {
      for(k=0;k<4;k++)
      {
        if(a->nodes_alive[i][k]==1)
        {
          if(k==0)
          {
            s=s+"R";
          }
          else if(k==1)
          {
            s=s+"T";
          }
          else if(k==2)
          {
            s=s+"S";
          }
          else
          {
            s=s+"P";
          }
        }
      }
    }
    else
    {
      s=s+"_";
    }
  }
  return s;
}

//Prints the state after taking the string of values of the state
void printSol(string s)
{
  map<char, string> names;
  names['R']="Roberta";
  names['T']="Thelma";
  names['S']="Steve";
  names['P']="Pete";

  cout<<"Chef: "<<names[s.at(0)]<<endl;
  cout<<"Guard: "<<names[s.at(1)]<<endl;
  cout<<"Nurse: "<<names[s.at(2)]<<endl;
  cout<<"Clerk: "<<names[s.at(3)]<<endl;
  cout<<"Police Officer: "<<names[s.at(4)]<<endl;
  cout<<"Teacher: "<<names[s.at(5)]<<endl;
  cout<<"Actor: "<<names[s.at(6)]<<endl;
  cout<<"Boxer: "<<names[s.at(7)]<<endl;
	cout<<"=========================="<<endl;
}

bool backtrack_mrv(Node* n)
{
  vector<Node*> myv = n->msucc;//assigns MRV based successors
  for(vector<Node*>::iterator it = myv.begin(); it != myv.end(); it++)
  {
		if (solfound==1)
			return true;
		cout<<"Iterating: "<<printState(*it)<<endl;
    if(consistency_jobs(*it)==true)
    {
      if(sum_arr((*it)->alive_count)==8)
      {
        cout<<"Solution: "<<printState(*it)<<endl;
				cout<<"=========================="<<endl;
				printSol(printState(*it));
				solfound=1;
        return true;
      }
      //expand
      //Create children
      (*it)->msucc = (*it)->mrv_successors();//assigns MRV based successors
      backtrack_mrv(*it);
    }
  }
  return false;
}

bool backtrack_no_mrv(Node* n)
{
  vector<Node*> myv = n->succ;//assigns non MRV based successors
  for(vector<Node*>::iterator it = myv.begin(); it != myv.end(); it++)
  {
			if(solfound==1)
				return true;
		cout<<"Iterating: "<<printState(*it)<<endl;
    if(consistency_jobs(*it)==true)
    {
      if(sum_arr((*it)->alive_count)==8)
      {
        cout<<"Solution: "<<printState(*it)<<endl;
				cout<<"=========================="<<endl;
				printSol(printState(*it));
				solfound=1;
        return true;
      }
      //expand
      //Create children
      (*it)->succ = (*it)->successors();
      backtrack_no_mrv(*it);
    }
  }
  return false;
}

int main(int argc, char* argv[])
{
  Node *start = new Node();
  Node *result;

	//Variables - Chef, Guard, Nurse, Clerk, Police, Teacher, Actor, Boxer in that sequence
	//Domain - Roberta, Thelma, Steve, Pete in that sequence
	//Initial State - T_S______ (Chef: Thelma, Nurse: Steve, Rest all are empty so it can be any1; 
	//													 initial state based on evident conditions; 
	//													 same initial state for MRV and non MRV)

	if(argc != 1)
	{	//For MRV - Setting the initial count of number of values the variables can take
		start->alive_count[0]=1;
		start->alive_count[1]=4;
		start->alive_count[2]=1;
		start->alive_count[3]=2;
		start->alive_count[4]=2;
		start->alive_count[5]=3;
		start->alive_count[6]=2;
		start->alive_count[7]=3;
	}
	else
	{ //For Non MRV (Normal Backtracking) - Setting the initial count of number of values the variables can take
		start->alive_count[0]=1;
		start->alive_count[1]=4;
		start->alive_count[2]=1;
		for(int i=3;i<8;i++)
			start->alive_count[i]=4;
	}  

  if(argc != 1)
	{
	//For MRV
	//nodes_alive refers to which variable can take what values from the domain
  //start->alive_count={1,4,1,2,2,3,2,3};
  start->nodes_alive[0][0]=0;//Roberta
  start->nodes_alive[0][1]=1;//Thelma - assigned to variable 1 now (Chef)
  start->nodes_alive[0][2]=0;//Steve
  start->nodes_alive[0][3]=0;//Pete
	//For Guard
  start->nodes_alive[1][0]=1;
  start->nodes_alive[1][1]=1;
  start->nodes_alive[1][2]=1;
  start->nodes_alive[1][3]=1;
	//For Nurse
  start->nodes_alive[2][0]=0;
  start->nodes_alive[2][1]=0;
  start->nodes_alive[2][2]=1;
  start->nodes_alive[2][3]=0;
	//For Clerk
  start->nodes_alive[3][0]=0;
  start->nodes_alive[3][1]=0;
  start->nodes_alive[3][2]=1;
  start->nodes_alive[3][3]=1;

  start->nodes_alive[4][0]=0;
  start->nodes_alive[4][1]=1;
  start->nodes_alive[4][2]=1;
  start->nodes_alive[4][3]=0;

  start->nodes_alive[5][0]=1;
  start->nodes_alive[5][1]=1;
  start->nodes_alive[5][2]=1;
  start->nodes_alive[5][3]=0;

  start->nodes_alive[6][0]=0;
  start->nodes_alive[6][1]=0;
  start->nodes_alive[6][2]=1;
  start->nodes_alive[6][3]=1;

  start->nodes_alive[7][0]=0;
  start->nodes_alive[7][1]=1;
  start->nodes_alive[7][2]=1;
  start->nodes_alive[7][3]=1;
	}
	else
	{
	//For Non MRV
	//T is set for 1st variable (Chef) i.e. Thelma is assigned as Chef
  start->nodes_alive[0][0]=0;//R
  start->nodes_alive[0][1]=1;//T
  start->nodes_alive[0][2]=0;//S
  start->nodes_alive[0][3]=0;//P
	//For 2nd Variable i.e. Guard
  start->nodes_alive[1][0]=1;//2nd Variable can take Roberta
  start->nodes_alive[1][1]=1;//2nd Variable can take Thelma
  start->nodes_alive[1][2]=1;//2nd Variable can take Steve
  start->nodes_alive[1][3]=1;//2nd Variable can take Pete

  start->nodes_alive[2][0]=0;
  start->nodes_alive[2][1]=0;
  start->nodes_alive[2][2]=1;
  start->nodes_alive[2][3]=0;

	for(int i=3;i<8;i++)
		for(int j=0;j<4;j++)
			start->nodes_alive[i][j]=1;//No values assigned for last 5 variables - can take all values R,T,S,P
	}

  start->depth=0;
  start->parent=NULL;
  cout<<"Initial State: "<<printState(start)<<endl;
	if(argc!=1)
	{
  	start->msucc = start->mrv_successors();
  	backtrack_mrv(start);
	}
	else
	{
		start->succ = start->successors();
  	backtrack_no_mrv(start);
	}
	if (solfound==0)
		cout<<"Solution not found"<<endl;	
  cout<<"Nodes explored: "<<nodes_explored<<endl;

  return 0;
}

//Consistency Check function
bool consistency_jobs(Node* n)
{
  char state[8];
  string s1 = (string) printState(n);
  nodes_explored++;

  state[0]=s1.at(0);
  state[1]=s1.at(1);
  state[2]=s1.at(2);
  state[3]=s1.at(3);
  state[4]=s1.at(4);
  state[5]=s1.at(5);
  state[6]=s1.at(6);
  state[7]=s1.at(7);

  int r = 0, t = 0, s = 0, p = 0;
  for(int i=0;i<8;i++)
  {
    if(state[i]=='R')
      r++;
    else if(state[i]=='S')
      s++;
    else if(state[i]=='T')
      t++;
    else if(state[i]=='P')
      p++;
  }

  if(r>2||s>2||t>2||p>2)//Maximum number of jobs a person can take is 2
    return false;

  
  //index 7:Roberta cannot be Boxer
  if(state[7]=='R')
    return false;

  //index 2:Nurse 4:Police Officer 5:Teacher --> None can be Pete
  //As Pete is not educated past the ninth grade
  if(state[2]=='P' || state[4]=='P' || state[5]=='P')
    return false;
  
  //index 2:Nurse is a male, so can't be Roberta or Thelma
  if(state[2]=='R' || state[2]=='T')
    return false;
  
  //index 6:Actor has to be male
  if(state[6]=='R' || state[6]=='T')
    return false;
  
  //index 0:Chef is female as husband of the chef is the clerk
  if(state[0]=='S' || state[0]=='P')
    return false;
  
  //index 3:Clerk has to be male as husband of the chef is the clerk
  if(state[3]=='R' || state[3]=='T')
    return false;

  //Roberta, chef and police are 3 different people
  if(state[0]==state[4] || state[0]=='R' || state[4]=='R')
    return false;

  return true;
}
