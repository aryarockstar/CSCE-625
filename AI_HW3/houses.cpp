#include<iostream>
#include<stdio.h>
#include<vector>
#include<string.h>
#include<map>

using namespace std;

int nodes_explored=0;//Number of nodes explored
static int solfound=0;//Flag to check if solution reached

int sum_arr(int a[25])//Calculate sum of an array
{
  int sum=0;
  for(int i=0;i<25;i++)
    sum=sum+a[i];
  return sum;
}

int any_index(int alive[25])//return any unassigned index
{
	int flag=1;
	for(int i=0;i<25;i++)
		if(alive[i]!=1)
			flag=0;
	if(flag==1)//means all variables assigned
		return -1;

	for(int i=0;i<25;i++)
		if(alive[i]!=1)
			return i;
}

int min_index(int alive[25])//return min index other than 1, -1 if all one
{
  int flag=1;
  for(int i=0;i<25;i++)
    if(alive[i]!=1)
      flag=0;
  if(flag==1)//means all variables assigned
    return -1;

  int min;
  for(int i=0;i<25;i++)
    if(alive[i]!=1)
    {
      min=i;
      break;
    }

  for(int i=0;i<25;i++)
    if(alive[i]<alive[min] && alive[i]!=1)
      min=i;
  return min;
}

class Node
{
  public:
    int depth;
    Node *parent;
    int alive_count[25];
    int nodes_alive[25][5];
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

        for(int j=0;j<25;j++)
          n->alive_count[j]=this->alive_count[j];
        (n->alive_count[index])=1;

        int temp = index-(index%5);
        for(int j=temp;j<temp+5;j++)
          if(n->alive_count[j]>1)
            (n->alive_count[j])--;

        for(int j=0;j<25;j++)
          for(int k=0;k<5;k++)
            n->nodes_alive[j][k]=this->nodes_alive[j][k];

        int place;
        for(int k=0;k<5;k++)
        {
          if(nodes_alive[index][k]==1)
          {
            nodes_alive[index][k]=0;
            place=k;
            break;
          }
        }

        for(int k=temp;k<temp+5;k++)
        {
          n->nodes_alive[k][place]=0;
        }

        for(int k=0;k<5;k++)
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

        for(int j=0;j<25;j++)
          n->alive_count[j]=this->alive_count[j];
        (n->alive_count[index])=1;

        int temp = index-(index%5);
        for(int j=temp;j<temp+5;j++)
          if(n->alive_count[j]>1)
            (n->alive_count[j])--;

        for(int j=0;j<25;j++)
          for(int k=0;k<5;k++)
            n->nodes_alive[j][k]=this->nodes_alive[j][k];

        int place;
        for(int k=0;k<5;k++)
        {
          if(nodes_alive[index][k]==1)
          {
            nodes_alive[index][k]=0;
            place=k;
            break;
          }
        }

        for(int k=temp;k<temp+5;k++)
        {
          n->nodes_alive[k][place]=0;
        }

        for(int k=0;k<5;k++)
        {
          n->nodes_alive[index][k]=0;
        }
        //cout<<"Place:"<<place<<endl;
        //cout<<"Index1:"<<index<<endl;
        n->nodes_alive[index][place]=1;
        //if(index!=-1)
        myv.push_back(n);
      }
      return myv;
    }
};

bool consistency_houses(Node*);

string printState(Node* a)//Create a string of values assigned currently to node n
{
  string s="";
  int i,k,fl;
  for(i=0;i<25;i++)
  {
    if(a->alive_count[i]==1)
    {
      for(k=0;k<5;k++)
      {
        if(a->nodes_alive[i][k]==1)
        {
          if(i<5)//Eat
          {
          if(k==0)
            s=s+"I";
          else if(k==1)
            s=s+"K";
          else if(k==2)
            s=s+"M";
          else if(k==3)
            s=s+"H";
          else
            s=s+"S";
          }
          else if(i<10)//Drink
          {
          if(k==0)
            s=s+"W";
          else if(k==1)
            s=s+"T";
          else if(k==2)
            s=s+"M";
          else if(k==3)
            s=s+"C";
          else
            s=s+"O";
          }
          else if(i<15)//Pet
          {
          if(k==0)
            s=s+"S";
          else if(k==1)
            s=s+"H";
          else if(k==2)
            s=s+"F";
          else if(k==3)
            s=s+"D";
          else
            s=s+"Z";
          }
          else if(i<20)//Color
          {
          if(k==0)
            s=s+"Y";
          else if(k==1)
            s=s+"B";
          else if(k==2)
            s=s+"I";
          else if(k==3)
            s=s+"G";
          else
            s=s+"R";
          }
          else//Country
          {
          if(k==0)
            s=s+"N";
          else if(k==1)
            s=s+"U";
          else if(k==2)
            s=s+"J";
          else if(k==3)
            s=s+"S";
          else
            s=s+"E";
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
  map<char, string> eat;
  map<char, string> drink;
  map<char, string> pet;
  map<char, string> color;	
  map<char, string> country;

  eat['I']="Smarties";
  eat['K']="KitKats";
  eat['M']="Milky Ways";
  eat['H']="Hershey Bars";
	eat['S']="Snickers";
	
	drink['W']="Water";
	drink['T']="Tea";
	drink['M']="Milk";
	drink['C']="Coffee";
	drink['O']="Orange Juice";

	pet['S']="Snail";
	pet['H']="Horse";
	pet['F']="Fox";
	pet['D']="Dog";
	pet['Z']="Zebra";

	color['Y']="Yellow";
	color['B']="Blue";
	color['I']="Ivory";
	color['G']="Green";
	color['R']="Red";

	country['N']="Norwegian";
	country['U']="Ukrainian";
	country['J']="Japanese";
	country['S']="Spaniard";
	country['E']="Englishman";

	for(int i=0;i<5;i++)
		cout<<eat[s.at(i)]<<" | ";
	cout<<endl;
	for(int i=5;i<10;i++)
		cout<<drink[s.at(i)]<<" | ";
	cout<<endl;
	for(int i=10;i<15;i++)
		cout<<pet[s.at(i)]<<" | ";
	cout<<endl;
	for(int i=15;i<20;i++)
		cout<<color[s.at(i)]<<" | ";
	cout<<endl;
	for(int i=20;i<25;i++)
		cout<<country[s.at(i)]<<" | ";
	cout<<endl;
	cout<<"========================"<<endl;
}

bool backtrack_mrv(Node* n)
{
  vector<Node*> myv = n->msucc;//assigns MRV based successors
  for(vector<Node*>::iterator it = myv.begin(); it != myv.end(); it++)
  {
		if (solfound==1)
			return true;
		cout<<"Iterating: "<<printState(*it)<<endl;
    if(consistency_houses(*it)==true)
    {
      if(sum_arr((*it)->alive_count)==25)
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
    if(consistency_houses(*it)==true)
    {
      if(sum_arr((*it)->alive_count)==25)
      {
        cout<<"Solution: "<<printState(*it)<<endl;
				cout<<"=========================="<<endl;
				printSol(printState(*it));
				solfound=1;
        return true;
      }
      //expand
      //Create children
      (*it)->succ = (*it)->successors();//assigns non MRV based successors
      backtrack_no_mrv(*it);
    }
  }
  return false;
}

int main(int argc, char* argv[])
{
  Node *start = new Node();
  Node *result;

	//Variables - Position of all the 5 things in each of the 5 sets
	//Domain - (in that order)
	//				 Eat     {Smarties, KitKats, Milky Way, Hersheys, Snickers}
	//				 Drink   {Water, Tea, Milk, Coffee, Orange Juice}
	//				 Pet	   {Snail, Horse, Fox, Dog, Zebra}
	//	 House Color	 {Yellow, Blue, Ivory, Green, Red}
	// Country Men 		 {Norwegian, Ukrainian, Japanese, Spaniard, Englishman}
	//Initial State - _______M________B___N____
	//First five spaces for eatables, 2nd five for drinks, 3rd five for pets, next 5 for
	//house colors and next five for country of the men
	//Initially, we know milk will go to the middle house, so its at index 7 (5-9 indices for drinks, so 7 is middle)
	//Also we know, Norwegian lives in the first house on the left - so its index is 20 (20-24 indices for countries)
	//Also, Blue house is next to where Norwegian lives, so its index is 16 (15-19 for color of houses)
	
	//Setting the initial count of number of values the variables can take
	//alive_count keeps the number of values that the variables can take
	//alive_count same for MRV and non MRV initially as the possible values is not evident immediately
  for(int i=0;i<15;i++)
  start->alive_count[i]=5;
  for(int i=15;i<25;i++)
  start->alive_count[i]=4;

  start->alive_count[16]=1;//Blue house fixed
  start->alive_count[20]=1;//Norwegian fixed
	start->alive_count[7]=1;//Milk in the middle is fixed

	//Setting the initial count of number of values the variables can take
	//nodes_alive same for MRV and non MRV initially as the possible values is not evident immediately
  for(int i=0;i<15;i++)//for eat, drink and pet
    for(int j=0;j<5;j++)//for each of the above's 5 positions
      start->nodes_alive[i][j]=1;//all of them can take 5 positions, so we fill 11111 for 0 to 14

  for(int i=5;i<10;i++)//for drink
  {//Milk is fixed, and milk's index is 2, so that is put 0 and rest all 1 for each of the drinks
    start->nodes_alive[i][0]=1;
    start->nodes_alive[i][1]=1;
    start->nodes_alive[i][2]=0;
    start->nodes_alive[i][3]=1;
    start->nodes_alive[i][4]=1;
  }
	//Milk itself is in position at index 7
  start->nodes_alive[7][0]=0;//Water
  start->nodes_alive[7][1]=0;//Tea
  start->nodes_alive[7][2]=1;//Milk
  start->nodes_alive[7][3]=0;//Coffee
  start->nodes_alive[7][4]=0;//Orange
	
  for(int i=15;i<20;i++)//for house color
  {//Blue is fixed, and its index is 16, so that is put 0 and rest all 1 for each of the color positions
    start->nodes_alive[i][0]=1;//Yellow
    start->nodes_alive[i][1]=0;//Blue
    start->nodes_alive[i][2]=1;//Ivory
    start->nodes_alive[i][3]=1;//Green
    start->nodes_alive[i][4]=1;//Red
  }
	//Blue itself is in position at index 16
  start->nodes_alive[16][0]=0;
  start->nodes_alive[16][1]=1;
  start->nodes_alive[16][2]=0;
  start->nodes_alive[16][3]=0;
  start->nodes_alive[16][4]=0;

  for(int i=20;i<25;i++)//for country
  {//Norwegian is fixed, and its index is 20, so that is put 0 and rest all 1 for each of the country positions
    start->nodes_alive[i][0]=0;//Norwegian
    start->nodes_alive[i][1]=1;//Ukrainian
    start->nodes_alive[i][2]=1;//Japanese
    start->nodes_alive[i][3]=1;//Spaniard
    start->nodes_alive[i][4]=1;//Englishman
  }
	//Norwegian itself is in position at index 20
  start->nodes_alive[20][0]=1;
  start->nodes_alive[20][1]=0;
  start->nodes_alive[20][2]=0;
  start->nodes_alive[20][3]=0;
  start->nodes_alive[20][4]=0;

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
  cout<<"Nodes explored:"<<nodes_explored<<endl;

  return 0;
}

bool consistency_houses(Node* n)
{
  char state[25];
  string s1 = (string) printState(n);
	nodes_explored++;

	for(int k=0;k<25;k++)
  state[k]=s1.at(k);

  int i;
	
	//Eatables 0-4 index
	//Drinks 5-9 index
	//Pet 10-14 index
	//Color 15-19 index
	//Country 20-24 index

  //Englishman lives in the red house
  for(i=0;i<5;i++)
  if(state[i+20] == 'E' && state[i+15] != 'R' && state[i+15] != '_')
    return false;

  //Spaniard owns the dog
  for(i=0;i<5;i++)
    if(state[i+20] == 'S' && state[i+10] != 'D' && state[i+10] != '_')
      return false;

  //Norwegian lives in the first house on the left
  if(state[20]!='N' && state[20]!='_')
    return false;

  //Green House is to the immediate right of Ivory
  for(i=0;i<4;i++)
    if(state[i+15]=='I' && state[i+16]!='G' && state[i+16]!='_')
      return false;
  if(state[19]=='I')
    return false;

  //Hershey Bar eater lives next to the Fox owner
  for(i=1;i<4;i++)
  {
    if(state[i]=='H' && state[i+9]!='F' && state[i+11]!='F' 
        && state[i+9]!='_' && state[i+11]!='_')
      return false;
    if(state[i+10]=='F' && state[i-1]!='H' && state[i+1]!='H'
        && state[i-1]!='_' && state[i+1]!='_')
      return false;
  }
  if(state[0]=='H' && state[11]!='F' && state[11]!='_')
    return false;
  if(state[4]=='H' && state[13]!='F' && state[13]!='_')
    return false;
  if(state[10]=='F' && state[1]!='H' && state[1]!='_')
    return false;
  if(state[14]=='F' && state[3]!='H' && state[3]!='_')
    return false;

  //KitKats are eaten in the yellow house
  for(i=0;i<5;i++)
    if(state[i]=='K' && state[i+15]!='Y' && state[i+15]!='_')
      return false;

  //Norwegian lives next to the blue house
  if(state[16]!='B' && state[16]!='_')
    return false;

  //Smarties(I) eater owns snails
  for(i=0;i<5;i++)
    if(state[i]=='I' && state[i+10]!='S' && state[i+10]!='_')
      return false;

  //Snickers eater drinks orange juice
  for(i=0;i<5;i++)
    if(state[i]=='S' && state[i+5]!='O' && state[i+5]!='_')
      return false;

  //Ukrainian drinks tea
  for(i=0;i<5;i++)
    if(state[i+20]=='U' && state[i+5]!='T' && state[i+5]!='_')
      return false;

  //Japanese eats Milky Way
  for(i=0;i<5;i++)
    if(state[i+20]=='J' && state[i]!='M' && state[i]!='_')
      return false;

  //Kitkat eaten next to house with Horse
  for(i=1;i<4;i++)
  {
    if(state[i]=='K' && state[i+9]!='H' && state[i+11]!='H'
        && state[i+9]!='_' && state[i+11]!='_')
      return false;
    if(state[i+10]=='H' && state[i-1]!='K' && state[i+1]!='K'
        && state[i-1]!='_' && state[i+1]!='_')
      return false;
  }
  if(state[0]=='K' && state[11]!='H' && state[11]!='_')
    return false;
  if(state[4]=='K' && state[13]!='H' && state[13]!='_')
    return false;
  if(state[10]=='H' && state[1]!='K' && state[1]!='_')
    return false;
  if(state[14]=='H' && state[3]!='K' && state[3]!='_')
    return false;

  //Coffee is drunk in the Green house
  for(i=0;i<5;i++)
    if(state[i+5]=='C' && state[i+15]!='G' && state[i+15]!='_')
      return false;

  //Milk is drunk in the middle house
  if(state[7]!='M' && state[7]!='_')
    return false;

  //Zebra can be in any one house
  //Water can be any one person's drink
  
  return true;
}
