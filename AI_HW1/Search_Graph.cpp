#include<iostream>
#include<queue>
#include<stdio.h>
#include<stack>
#include<math.h>
#include<string.h>
using namespace std;

#define MAX_NODES 1000

int num_of_nodes;
int edge[MAX_NODES][MAX_NODES];//Adjacency Matrix for edges
int node_coord[MAX_NODES][2];//Node co-ordinates in array for fast access through index
int visited[MAX_NODES];//Visited nodes check flag list
int maxfront=1;//Frontier Size
int total_iterations=0;//No of iterations (goal tests)

class Vertex
{
	public:
		int x,y;
		Vertex(int a,int b)
		{
			x=a; y=b;
		}
};

class Node
{
	public:
		int v; //index to vertices (vector of points with coords)
		Node* parent;
		int depth;
		float heur;//Estimated distance to goal for GBFS
		Node(int I){ v = I; }
		Node(int I,Node *p) { v = I; parent = p; }
		
		vector<Node*> successors() 
		{
			vector<Node*> myv;
			for(int i=0;i<num_of_nodes;i++)
			{
				if(edge[v][i]==1 && visited[i]==0)
				{
					Node *n = new Node(i);
					n->depth = depth + 1;
					n->parent = this;
					myv.push_back(n);
					visited[i]=1;
					cout<<" pushed "<<i<<" ("<<node_coord[i][0]<<","<<node_coord[i][1]<<")"<<endl;
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

Node* Search_BFS(Node* initial_state, Vertex* goal);
Node* Search_DFS(Node* initial_state, Vertex* goal);
Node* Search_GBFS(Node* initial_state, Vertex* goal);

int main(int argc,char *argv[])
{
  freopen("input.txt","r",stdin);//Input file name should be correctly placed here
  char c;
  for(int i=0;i<9;i++)
  cin>>c;

  cin>>num_of_nodes;
  cout<<"vertices="<<num_of_nodes<<", ";

  for(int i=0;i<num_of_nodes;i++)
  {
    cin>>node_coord[i][0];
    cin>>node_coord[i][0];
    cin>>node_coord[i][1];
  }
/* To check if nodes are read correctly
  for(int i=0;i<num_of_nodes;i++)
  {
    cout<<i<<" ";
    cout<<node_coord[i][0]<<" ";
    cout<<node_coord[i][1]<<endl;
  }
*/
  for(int i=0;i<6;i++)
  cin>>c;

  int num_of_edges;
  cin>>num_of_edges;
  cout<<"edges="<<num_of_edges<<endl;


  for(int i=0;i<num_of_nodes;i++)
  {
		for(int j=0;j<num_of_nodes;j++)
		{
			edge[i][j]=0;
		}
	}

  for(int i=0;i<num_of_edges;i++)
  {
		int index1,index2;
    cin>>index1;
    cin>>index1;
		//cout<<index1<<" ";//To check edge input is correct or not
    cin>>index2;
		//cout<<index2<<endl;
    edge[index1][index2]=1;
    edge[index2][index1]=1;
  }

	freopen("/dev/tty","r",stdin);

	int start_x,start_y,end_x,end_y;
	int start_index=0,end_index=0;

	while(1)
	{
		cout<<"Enter starting node coords x,y:"<<endl;
		cin>>start_x;
		cin>>start_y;
		int flag=0;
		for(int i=0;i<num_of_nodes;i++)
		{
			if((node_coord[i][0]==start_x)&&(node_coord[i][1]==start_y))
			{
				flag = 1;
				start_index = i;
				break;
			}
		}
		if(flag==1)
		break;
		else
		cout<<"Invalid coords. Please re-enter."<<endl;
	}
	while(1)
	{
		cout<<"Enter ending node coords x,y:"<<endl;
		cin>>end_x;
		cin>>end_y;
		int flag=0;
		for(int i=0;i<num_of_nodes;i++)
		{
			if((node_coord[i][0]==end_x)&&(node_coord[i][1]==end_y))
			{
				flag = 1;
				end_index=i;
				break;
			}
		}
		if(flag==1)
		break;
		else
		cout<<"Invalid coords. Please re-enter.\n";
	}

	Vertex *v = new Vertex(end_x,end_y); //End vertex
	Node *is = new Node(start_index); //Start node
	is->depth = 0;
	is->parent = NULL;

	cout<<"start=("<<start_x<<","<<start_y<<"), ";
	cout<<"goal=("<<end_x<<","<<end_y<<"), vertices: ";
	cout<<start_index<<" and "<<end_index<<endl;

	//Start Search
	Node *result = NULL;

	if((argc>1) && (strcmp(argv[1],"bfs")==0))
	{
		//BFS
		result = Search_BFS(is, v);
	}
	else if((argc>1) && (strcmp(argv[1],"dfs")==0))
	{
		//DFS
		result = Search_DFS(is, v);
	}
	else
	{
		//GBFS
		result = Search_GBFS(is, v);
	}

	if(result!=NULL)
	{
		vector<Node*> tb;
		tb = result->traceback();
		cout<<"=============="<<endl;
		cout<<"solution path:"<<endl;
		int path_length=0;
		for (vector<Node*>::reverse_iterator rit = tb.rbegin(); rit != tb.rend(); rit++)
    {
			cout<<" vertex "<<(*rit)->v<<" (";
			cout<<node_coord[(*rit)->v][0]<<","<<node_coord[(*rit)->v][1]<<")"<<endl;
			path_length++;
		}
		int vv=0;
		for(int i=0;i<num_of_nodes;i++)
		{
			if(visited[i]==1)
				vv++;
		}
		cout<<"search algorithm\t= ";
		if((argc>1) && (strcmp(argv[1],"bfs")==0))
			cout<<"BFS";
		else if((argc>1) && (strcmp(argv[1],"dfs")==0))
			cout<<"DFS";
		else
			cout<<"GBFS";
		cout<<endl; 
		cout<<"total iterations\t= "<<total_iterations<<endl;
		cout<<"max frontier size\t= "<<maxfront<<endl;		
		cout<<"vertices visited\t= "<<vv<<"/"<<num_of_nodes<<endl;
		cout<<"path length\t\t= "<<path_length-1<<endl;
	}
	else
	{
		cout<<"=============="<<endl;
		int vv=0;
		for(int i=0;i<num_of_nodes;i++)
		{
			if(visited[i]==1)
				vv++;
		}
		cout<<"search algorithm\t= ";
		if((argc>1) && (strcmp(argv[1],"bfs")==0))
			cout<<"BFS";
		else if((argc>1) && (strcmp(argv[1],"dfs")==0))
			cout<<"DFS";
		else
			cout<<"GBFS";
		cout<<endl; 
		cout<<"total iterations\t= "<<total_iterations<<endl;
		cout<<"max frontier size\t= "<<maxfront<<endl;		
		cout<<"vertices visited\t= "<<vv<<"/"<<num_of_nodes<<endl;
		cout<<"Node Unreachable."<<endl;
	}
  return 0;
}

//helper distance calculating function
float distance(int x1,int y1,int x2,int y2)
{
	float t = (float){(x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)};
	return sqrt(t);
}


//bfs
Node* Search_BFS(Node* initial_state, Vertex* goal)
{
	queue<Node*> frontier;
	frontier.push(initial_state);
	Node* popped;
	vector<Node*> myv;
	float dist2goal;
	while(!frontier.empty())
	{
		cout<<"iter="<<total_iterations+1<<", frontier="<<frontier.size()<<", popped="<<(frontier.front())->v<<" ("<<node_coord[(frontier.front())->v][0]<<","<<node_coord[(frontier.front())->v][1] <<"), depth="<<(frontier.front())->depth<<", dist2goal=";		
		total_iterations++;
		popped = frontier.front();
		dist2goal = distance(node_coord[popped->v][0],node_coord[popped->v][1],goal->x,goal->y);
		cout<<dist2goal<<endl;
		frontier.pop();
		visited[popped->v]=1;
		if((node_coord[popped->v][0] == goal->x)&&(node_coord[popped->v][1] == goal->y))
			{
				return popped;
			}
		myv=popped->successors();
		for (vector<Node*>::iterator it = myv.begin(); it != myv.end(); it++)
    {
			frontier.push(*it);
		}				
		if(maxfront < frontier.size())
			maxfront = frontier.size();
	}
	return NULL;
}
//dfs
Node* Search_DFS(Node* initial_state, Vertex* goal)
{
	stack<Node*> frontier;
	frontier.push(initial_state);
	Node* popped;
	vector<Node*> myv;
	float dist2goal;
	while(!frontier.empty())
	{
		cout<<"iter="<<total_iterations+1<<", frontier="<<frontier.size()<<", popped="<<(frontier.top())->v<<" ("<<node_coord[(frontier.top())->v][0]<<","<<node_coord[(frontier.top())->v][1] <<"), depth="<<(frontier.top())->depth<<", dist2goal=";		
		total_iterations++;
		popped = frontier.top();
		dist2goal = distance(node_coord[popped->v][0],node_coord[popped->v][1],goal->x,goal->y);
		cout<<dist2goal<<endl;
		frontier.pop();
		visited[popped->v]=1;
		if((node_coord[popped->v][0] == goal->x)&&(node_coord[popped->v][1] == goal->y))
			{
				return popped;
			}
		myv=popped->successors();
		for (vector<Node*>::iterator it = myv.begin(); it != myv.end(); it++)
    {
			frontier.push(*it);
		}				
		if(maxfront < frontier.size())
			maxfront = frontier.size();
	}
	return NULL;
}

//Priority Queue Comparision Class
class CompareNode
{
	public:
		bool operator()(Node* a, Node* b)	
		{
			return (a->heur > b->heur);
		}
};

//gbfs
Node* Search_GBFS(Node* initial_state, Vertex* goal)
{
	priority_queue<Node*, vector<Node*>, CompareNode> frontier;
	initial_state->heur = distance(node_coord[initial_state->v][0],node_coord[initial_state->v][1],goal->x,goal->y);
	frontier.push(initial_state);
	Node* popped;
	vector<Node*> myv;
	float dist2goal;
	while(!frontier.empty())
	{
		cout<<"iter="<<total_iterations+1<<", frontier="<<frontier.size()<<", popped="<<(frontier.top())->v<<" ("<<node_coord[(frontier.top())->v][0]<<","<<node_coord[(frontier.top())->v][1] <<"), depth="<<(frontier.top())->depth<<", dist2goal=";		
		total_iterations++;
		popped = frontier.top();
		dist2goal = distance(node_coord[popped->v][0],node_coord[popped->v][1],goal->x,goal->y);
		cout<<dist2goal<<endl;
		frontier.pop();
		visited[popped->v]=1;
		if((node_coord[popped->v][0] == goal->x)&&(node_coord[popped->v][1] == goal->y))
			{
				return popped;
			}
		myv=popped->successors();
		for (vector<Node*>::iterator it = myv.begin(); it != myv.end(); it++)
    {
			(*it)->heur = distance(node_coord[(*it)->v][0],node_coord[(*it)->v][1],goal->x,goal->y);			
			frontier.push(*it);
		}				
		if(maxfront < frontier.size())
			maxfront = frontier.size();
	}
	return NULL;
}
