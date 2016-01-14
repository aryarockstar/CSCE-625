#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<queue>
#include<string>
#include<sstream>
#include<map>
#include<time.h>

// Number of moves used to scramble the goal to start state
#define MAX_SCRAMBLE 28

using namespace std;
int num_of_stacks=0;
int num_of_blocks=0;
int maxfront=1;
int total_iterations=0;
map<string,int> visited;

void printStack(vector<int>,int,int);

string stringify(vector<int> state)
{
  string a = "";
  stringstream ss;
  int m;
  for(int i = 0; i < num_of_blocks; i++)
  {
    m = state[i * 2];//making row number of each block to string
    ss.str("");
    ss<<m;
    if (m < 10)//m > 99 => 3 digit => blocks>=100 //currently unsupported
    {
      a = a + "0" + ss.str();//Adding one zero to a single digit number
    }
    else
      a = a + ss.str();//Concatenating 2-digit number to the string directly

    m = state[i * 2 + 1];//doing the same thing for column number
    ss.str("");
    ss<<m;

    if (m < 10)
    {
      a = a + "0" + ss.str();
    }
    else
      a = a + ss.str();
  }
  return a;
}

class Node
{
  public:
    Node* parent;
    int heur;
    int depth;
    vector<int> state;
    vector<int> stack_heights;
    vector<Node*> successors()
    {
      vector<Node*> myv;
      for(int j = 0; j < num_of_stacks; j++)
      {
        for(int i = 0; i < num_of_blocks; i++)
        {
          if ((this->state[2 * i + 1] == ((this->stack_heights[j])-1))
            && (this->state[2 * i] == j))
          {
            for(int k = 0; k < num_of_stacks; k++)
            {
              if(k != j)
              {
		            Node *n = new Node();
		            n->state = this->state;
		            n->state[2 * i] = k;
		            n->state[2 * i + 1] = stack_heights[k];
		            n->stack_heights = this->stack_heights;
		            (n->stack_heights[j])--;
		            (n->stack_heights[k])++;
		            n->depth = (this->depth) + 1;
		            n->parent = this;
		            //node already visited
								if(visited.find(stringify(n->state)) != visited.end())
								{
									if(visited[stringify(n->state)] > n->depth)//found shorter path
									{ //expand node, don't discard
										visited[stringify(n->state)] = n->depth;
										myv.push_back(n);
						        //cout<<"Pushed:"<<endl;
						        //printStack(n->state, num_of_blocks, num_of_stacks);
									}
									else
									{ //node visited and current node has greater depth
										//dont add to successors, discard node
									}

								}
								else//node not visited, add node to visited and frontier
								{
									myv.push_back(n);
									visited[stringify(n->state)] = n->depth;
				          //cout<<"Pushed:"<<endl;
				          //printStack(n->state, num_of_blocks, num_of_stacks);
								}
              }
            }
            break;
          }
        }
      }
      return myv;
    }

    vector<Node*> traceback()
    {
      vector<Node*> myv;
      Node *p;
      p=this;
      if(p->parent == NULL)
      {
        myv.push_back(p);
        return myv;
      }
      while(p != NULL)
      {
        myv.push_back(p);
        p=p->parent;
      }
      return myv;
    }
};

void addToVisited(Node *p);
Node* Search_Astar(Node*,Node*);

int main(int argc,char *argv[])
{
  if (argc!=3)
  {
    cout<<"Insufficient command line arguments"<<endl;
    return 0;
  }
  num_of_blocks = atoi(argv[1]);
  num_of_stacks = atoi(argv[2]);

  cout<<"Blocks: "<<num_of_blocks;
  cout<<" Stacks: "<<num_of_stacks<<endl;
  
  if ((num_of_blocks <= 0) || (num_of_stacks <= 0))
  {
    cout<<"Invalid parameters"<<endl;
    return 0;
  }

  if (num_of_stacks <= 2)
  {
    cout<<"Number of stacks must be >= 3"<<endl;
    return 0;
  }
  
  Node *start = new Node();
  start->depth = 0;
  start->parent = NULL;

/* Uncomment for manual feeding of a bigger start node state
// Give ./a.out 10 5
  start->state.push_back(4);//A's row number and 
  start->state.push_back(0);//A's column number
  start->state.push_back(2);//B's row
  start->state.push_back(0);//B's column and so on....
  start->state.push_back(3);
  start->state.push_back(0);
  start->state.push_back(0);
  start->state.push_back(0);
  start->state.push_back(1);
  start->state.push_back(0);
  start->state.push_back(1);
  start->state.push_back(1);
  start->state.push_back(2);
  start->state.push_back(1);
  start->state.push_back(3);
  start->state.push_back(1);
  start->state.push_back(1);
  start->state.push_back(2);
  start->state.push_back(1);
  start->state.push_back(3);

  start->stack_heights.push_back(1);//Height of stack 1
  start->stack_heights.push_back(4);//Height of stack 2 and so on....
  start->stack_heights.push_back(2);
  start->stack_heights.push_back(2);
  start->stack_heights.push_back(1);
*/

/* Uncomment for manual feeding of a smaller start node state
 //Give ./a.out 5 3    --OR--   ./a.out 5 6
  start->state.push_back(1);
  start->state.push_back(1);
  start->state.push_back(2);
  start->state.push_back(0);
  start->state.push_back(1);
  start->state.push_back(0);
  start->state.push_back(0);
  start->state.push_back(0);
  start->state.push_back(2);
  start->state.push_back(1);

  start->stack_heights.push_back(1);
  start->stack_heights.push_back(2);
  start->stack_heights.push_back(2);

  start->stack_heights.push_back(0);
  start->stack_heights.push_back(0);
  start->stack_heights.push_back(0);
*/
  
  Node *goal = new Node();
  goal->depth = 0;//Required for scrambling
  goal->parent = NULL;//Required for scrambling

  for(int i=0; i<num_of_blocks; i++)
  {
    goal->state.push_back(0);
    goal->state.push_back(i);
  }
  
  /*Comment this entire set of Automated Problem Generation
    if manual feeding the start state */
  //Automated Problem Generation Starts
  (goal->stack_heights).push_back(num_of_blocks);
  for(int i=1;i<num_of_stacks;i++)
    (goal->stack_heights).push_back(0);
  
  time_t t;
  srand((unsigned) time(&t));
 
  start->state = goal->state;
  start->stack_heights = goal->stack_heights;

  for(int i=0;i<MAX_SCRAMBLE;i++)
  {
    int m = rand()%num_of_stacks;
    while(start->stack_heights[m]==0)
      m=rand()%num_of_stacks;
    for(int j=0;j<num_of_blocks;j++)
    {
      if((start->state[2*j + 1] == start->stack_heights[m]-1)
          && (start->state[2*j] == m))
      {
        int k = rand()%num_of_stacks;
        while(k==m)
          k=rand()%num_of_stacks;
        start->state[2*j]=k;
        start->state[2*j + 1] = start->stack_heights[k];
        (start->stack_heights[k])++;
        (start->stack_heights[m])--;
        //cout<<"Scramble #"<<i+1<<endl;
        //printStack(start->state, num_of_blocks, num_of_stacks);
        break;
      }
    }
  }
  //cout<<"////////////////////////////"<<endl;
  //Automated Problem Generation Ends

  start->depth = 0;
  start->parent = NULL;
  visited.clear();

  cout<<"initial state:"<<endl;
  printStack(start->state,num_of_blocks,num_of_stacks);
  cout<<"goal state:"<<endl;
  printStack(goal->state,num_of_blocks,num_of_stacks);

  Node *result = NULL;
  result = Search_Astar(start, goal);

  if(result == NULL)
    cout<<"Element not found."<<endl;
  else
  {
    cout<<"success! depth="<<result->depth<<", total_goal_tests="<<total_iterations<<", max_queue_size="<<maxfront<<endl;
    cout<<"solution path:"<<endl;
    vector<Node*> tb;
    tb = result->traceback();
    for (vector<Node*>::reverse_iterator rit = tb.rbegin(); rit != tb.rend(); rit++)
    {
      printStack((*rit)->state,num_of_blocks,num_of_stacks);
      cout<<endl;
    }
  }
  return 0;
}

void printStack(vector<int> state, int blocks, int stacks)
{
  for (int i=0; i<stacks; i++)
  {
    cout<<i+1<<" | ";
    for (int j=0; j<blocks; j++)
    {
      for (int k=0; k<(state.size()/2); k++)
      {
        if (state[2*k]==i && state[2*k + 1]==j)
        {
          char c = 'A' + k;
          cout<<c<<" ";
        }
      }
    }
    cout<<endl;
  }
}

//Compares f(n) = g(n) + h(n)
class CompareNode
{
  public:
    bool operator()(Node* a, Node* b)
    {
      return (a->depth + a->heur) > (b->depth + b->heur);
    }
};

//h(n)- More informative heuristic
int calc_heur(Node* p, Node* goal)
{
  int sum = 0;
  for(int i=0; i<num_of_stacks; i++)
  {
    if (i==0 && p->stack_heights[i]==0)
    {
      sum += num_of_blocks;//case 1
    }
    if (i!=0)
    {
      sum += p->stack_heights[i];//case 2
    }
    if(i==0 && p->stack_heights[i]!=0)
    {
      int k;
      for(k=0;k<p->stack_heights[0];k++)
      {
        if((p->state[2*k]==goal->state[2*k])
          && (p->state[2*k+1]==goal->state[2*k+1]))
        {}
        else
          break;
      }
      if(k != p->stack_heights[0])
      {
        sum+= (p->stack_heights[0]-k);//case 3
      }
      sum+=(num_of_blocks - k);//case 4 //case 3
    }
    
    //Looking ahead***
    //if top is A and stack 1 is empty dec heur by 1
    if(i==0 && p->stack_heights[i]==0)
    {
      if(p->state[2*i + 1] == ((p->stack_heights[p->state[2*i]])-1))
        sum--;
    }
    //if stack 1 is sorted and next char is on top of any other stack
    //dec heur by 1
    if(i==0)
    {
      int st;
      for(st=0;st<num_of_blocks;st++)
      {
        if(p->state[2*st]==0 && p->state[2*st + 1]==st)
        {}
        else
          break;
      }
      if(st!=num_of_blocks && p->stack_heights[0]==st+1)
      {
        if(p->state[(2*(st+1))+1]==((p->stack_heights[p->state[2*i]])-1))
          sum--;
      }
    }
    
  }
  return sum;
}

/*
//h(n) - Blocks out of place
int calc_heur(Node* start, Node* goal)
{
  //No of blocks out of place
  int sum=0;
  for(int i=0;i<num_of_blocks;i++)
  {
    if ((start->state[2*i] == goal->state[2*i]) && 
        (start->state[2*i + 1] == goal->state[2*i + 1]))
    {
    }
    else
      sum++;
  }
  return sum;
}
*/
//helper to compareWithGoal
int compNode(Node* start, Node* goal)
{
  //No of blocks out of place
  int sum=0;
  for(int i=0;i<num_of_blocks;i++)
  {
    if ((start->state[2*i] == goal->state[2*i]) && 
        (start->state[2*i + 1] == goal->state[2*i + 1]))
    {
    }
    else
      sum++;
  }
  return sum;
}

bool compareWithGoal(Node* a, Node* goal)
{
  int s;
  s = compNode(a, goal);
  if (s == 0)
    return true;
  else
    return false;
}
//Add a node to visited list
void addToVisited(Node *p)
{
  visited[stringify(p->state)]=p->depth;
}
//A* search
Node* Search_Astar(Node* start, Node* goal)
{
  priority_queue<Node*, vector<Node*>, CompareNode> frontier;
  start->heur = calc_heur(start, goal);
  frontier.push(start);
  Node* popped;
  vector<Node*> myv;
  while(!frontier.empty())
  {
    popped = frontier.top();
    cout<<"iter="<<total_iterations+1<<", queue="<<frontier.size()<<", f=g+h="<<(popped->depth + popped->heur)<<", depth="<<popped->depth<<endl;
    total_iterations++;
    frontier.pop();
    addToVisited(popped);
   // cout<<stringify(popped->state);
    if (compareWithGoal(popped,goal))
      return popped;
    myv=popped->successors();
    for (vector<Node*>::iterator it = myv.begin(); it != myv.end(); it++)
    {
      (*it)->heur = calc_heur((*it),goal);
      frontier.push(*it);
    }
    if(maxfront < frontier.size())
      maxfront = frontier.size();
  }
  return NULL;
}
