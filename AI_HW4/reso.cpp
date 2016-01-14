#include<iostream>
#include<vector>
#include<map>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<queue>

using namespace std;

vector<string> clauses;
vector<pair<int,int> > parent;
map<string,int> visited;
int iter;
int maxq;
/*
//Used to sort a clause
int cmp(const void *a, const void *b)
{
	//cout<<"CMP:"<<*(char**)a<<" "<<*(char**)b<<endl;
	string a1(*(char**)a);
	string b1(*(char**)b);
	if(a1.at(0)=='-')
	a1.erase(0,1);
	if(b1.at(0)=='-')
	b1.erase(0,1);
	cout<<"Comparing: "<<a1<<" and "<<b1<<endl;
	return a1.compare(b1);
}
*/
int mycomp(string a, string b)
{
	if(a.length()<=b.length())
	{
			for(int i=0;i<a.length();i++)
			{
				if(a.at(i)>b.at(i))
					return 1;
				else if(a.at(i)<b.at(i))
					return -1;
			}
			if(a.length()==b.length())
				return 0;
			else
				return -1;
	}
	else
	{
			for(int i=0;i<b.length();i++)
			{
				if(a.at(i)>b.at(i))
					return 1;
				else if(a.at(i)<b.at(i))
					return -1;
			}
			if(a.length()==b.length())
				return 0;
			else
				return 1;		
	}
}
class cmp
{
	public:
		bool operator()(string a1, string b1)	
		{
	//string a1(a);
	//string b1(b);
	if(a1.at(0)=='-')
	a1.erase(0,1);
	if(b1.at(0)=='-')
	b1.erase(0,1);
	cout<<"Comparing: "<<a1<<" and "<<b1<<endl;
	return mycomp(a1,b1);
		}
};


//sorts a clause and removes extra spaces
string cleanup(string cl)
{
	cout<<"Got String:"<<cl<<endl;
  char text[(cl.length()) + 1];
	strcpy(text, cl.c_str());
	cout<<"Text is now:"<<text<<endl;
	vector<string> words;
	words.clear();
  char *word;
  int wmax=0, i;
	map<string,int> m;
	m.clear();
	//string str;
	string soln="";

  for(word = strtok(text, " \n"); word ; word = strtok(NULL, " \n"))
	{
		cout<<"Word is:"<<word<<endl;
    string m(word);
		m=m+'\0';
		words.push_back(m);
		wmax++;        
  }

	char *myword[words.size()];
priority_queue<string, vector<string>, cmp> swords;
	for(i=0; i<wmax; i++)
	{
		myword[i] = new char[words[i].length() + 1];
		strncpy(myword[i], words[i].c_str(), words[i].length());
		myword[i][words[i].length()]='\0';
		swords.push(words[i]);
	}

//	cout<<"mywords is:"<<soln<<endl;
  //qsort(mywords, wmax, sizeof(*mywords), cmp);//calls cmp to do sorting
	
	vector<string> mywords;


	while(!swords.empty())
	{
		mywords.push_back(swords.top());
		cout<<"Swords:"<<swords.top()<<endl;
		swords.pop();
	}

  for (i=0; i<wmax; ++i)
	{
		string str(mywords[i]);
		if (m.find(str)==m.end())
		{
			m[str]=1;
		  //printf("%s\n", mywords[i]);
			soln = soln + " " +str;
		}
		str.clear();
	}
	cout<<"Soln is now:"<<soln<<endl;
	int first;
	for(i=0;i<soln.length();i++)
	{
		if(soln.at(i)!=' ')
		{
			first = i;
			break;
		}
	}
	//size_t first = soln.find_first_not_of(' ');
	int last;
	for(i=(soln.length())-1;i>=0;i--)
	{
		if(soln.at(i)!=' ')
		{
			last = i;
			break;
		}
	}

	//size_t last = soln.find_last_not_of(' ');
	soln = soln.substr(first, (last-first+1));
	for (i = 1; i < soln.length(); ) 
	{
		if (soln[i - 1] == soln[i] && soln[i]==' ') 
			soln.erase(i, 1);
		else 
			++i;
	}
	cout<<"Soln: "<<soln<<endl;
	return soln;
}

//checks if a clause is empty or not
bool isempty(string str)
{
	int i=0;
	for(i=0;i<str.length();i++)
	{
		if(str.at(i)!=' ')
			break;
	}
	if(i==str.length())
		return true;
	return false;
}

//checks if a line in input .kb file is commented or not
//note that comment cannot start after a clause in the same line
//comment must start with # at the beginning of the line
bool commented(string str)
{
	int i=0;
	for(i=0;i<str.length();i++)
	{
		if(str.at(i)!=' ')
			break;
	}
	if(i==str.length())
		return true;
	size_t first = str.find_first_not_of(' ');
	size_t last = str.find_last_not_of(' ');
	string s = str.substr(first, (last-first+1));
	//cout<<"S is:"<<s<<endl;
	if(s.at(0)=='#')
		return true;
	else
		return false;
}

//resolution pair that can be resolved
class ResPair
{
	public:
		int p1;//Parent 1	
		int p2;//Parent 2
};

//count the number of words (literals) in a string (clause)
int count_elements(string s)
{
	int count = 1;
	for(int i=0; i<s.length(); i++)
		if(s.at(i)==' ')
			count++;
	return count;
}

//checks if two clauses can be resolved or are resolvable
bool resolvable(string a, string b)
{
	char* pch;
	char* r;
	int found;
	r = new char[b.length() + 1];
	strcpy(r, b.c_str());
	pch = strtok (r," ");
  while (pch != NULL)
  {
    //printf ("%s\n",pch);
		string search(pch);
		if(search.at(0)=='-')
		{
			search.erase(0,1);
		}
		else
		{
			search = '-' + search;
		}
		//cout<<"Search: "<<search<<endl;
		found = a.find(search);
		int ll=0;//length of literal
		for(int j=found;j<a.length();j++)
		{
			if(a.at(j)==' ')
				break;
			ll++;
		}

		if(found!=string::npos && search.length()==ll)
		{
			if(found==0)
        return true;
			else if(a.at(found-1)==' ')
				return true;
		}

    pch = strtok (NULL, " ");
  }
	return false;
}

//Priority Queue Comparision Class - Heuristic function
class CompareResPair
{
	public:
		bool operator()(ResPair* a, ResPair* b)	
		{
			//return (a->heur > b->heur);
			//length of pair a > length of pair b
			int la = count_elements(clauses[a->p1]) + count_elements(clauses[a->p2]);
			int lb = count_elements(clauses[b->p1]) + count_elements(clauses[b->p2]);
			//cout<<"LA:"<<la<<" LB:"<<lb<<endl;
			return la>lb;
		}
};

//find resolvents while resolving two clauses
vector<string> findresolvents(string a, string b)
{
	char* pch;
	char* r;
	int found;
	vector<string> resolvents;
	r = new char[b.length() + 1];
	strcpy(r, b.c_str());
	pch = strtok (r," ");
  while (pch != NULL)
  {
		string s1(a);
		string s2(b);
    //printf ("%s\n",pch);
		string search(pch);
		if(search.at(0)=='-')
		{
			search.erase(0,1);
		}
		else
		{
			search = '-' + search;
		}
		//cout<<"Search: "<<search<<endl;
		found = a.find(search);
		int ll=0;//length of literal
		for(int j=found;j<a.length();j++)
		{
			if(a.at(j)==' ')
				break;
			ll++;
		}

		if(found!=string::npos && search.length()==ll)
		{
			if(found==0 || a.at(found-1)==' ')
			{
				string part(pch);
				s2.replace(s2.find(part),part.length(),"");
				s1.replace(s1.find(search),search.length(),"");
				s1 = s1 + " " + s2;
				for (int i = 1; i < s1.length(); ) 
				{
						if (s1[i - 1] == s1[i] && s1[i]==' ') 
								s1.erase(i, 1);
						else 
								++i;
				}
				//cout<<"Resolvent: |"<<s1<<"|"<<endl;//cleanup this
				resolvents.push_back(s1);
			}
		}
    pch = strtok (NULL, " ");
  }
	return resolvents;
}

//find max of two numbers
int max(int a,int b)
{
	if(a>=b)
		return a;
	else
		return b;
}

//returns "A v B" for input "A B"
string printClause(string sa)
{
	string res = "";
	char *pch;
	char *a;

	a = new char[sa.length() + 1];
	strcpy(a, sa.c_str());
	pch = strtok (a," ");
  if (pch != NULL)
  {
		string k(pch);
    res = res + k;
    pch = strtok (NULL, " ");
  }
  while (pch != NULL)
  {
		string k(pch);
		res = res + " v " + k;
    pch = strtok (NULL, " ");
  }

	return res;
}

//print two resolving strings
void printResolving(string sa, string sb)
{
	cout<<"resolving ";
	char *pch;
	char *a;
	char *b;
	a = new char[sa.length() + 1];
	strcpy(a, sa.c_str());
	pch = strtok (a," ");
  if (pch != NULL)
  {
    printf ("%s",pch);
    pch = strtok (NULL, " ");
  }
  while (pch != NULL)
  {
    printf (" v %s",pch);
    pch = strtok (NULL, " ");
  }
	cout<<" and ";
	b = new char[sb.length() + 1];
	strcpy(b, sb.c_str());
	pch = strtok (b," ");
  if (pch != NULL)
  {
    printf ("%s",pch);
    pch = strtok (NULL, " ");
  }
  while (pch != NULL)
  {
    printf (" v %s",pch);
    pch = strtok (NULL, " ");
  }
	cout<<endl;
}

//resolution function
bool resolution()
{
	priority_queue<ResPair*, vector<ResPair*>, CompareResPair> candidates;
	for(int i=0; i<clauses.size(); i++)
		for(int j=i+1; j<clauses.size(); j++)
			{
				if(resolvable(clauses[i], clauses[j]))
				{
					ResPair *n = new ResPair();
					n->p1 = i; 
					n->p2 = j;
					//cout<<"Pushing: "<<i<<" and "<<j<<endl;
					candidates.push(n);
				}
			}
	iter = 0;
	maxq = candidates.size();
	while(!candidates.empty())
	{
		if(maxq<candidates.size())
			maxq = candidates.size();
		ResPair *popped;
		popped = candidates.top();
		//cout<<"Candidate popped is:("<<popped->p1<<","<<popped->p2<<")"<<endl;
		cout<<"iteration "<<iter+1<<", queue size "<<candidates.size()<<", resolution on "<<popped->p1<<" and "<<popped->p2<<endl;
		candidates.pop();
		printResolving(clauses[popped->p1], clauses[popped->p2]);
		vector<string> myv = findresolvents(clauses[popped->p1],clauses[popped->p2]);
		for (vector<string>::iterator it = myv.begin(); it != myv.end(); it++)
    {
			if(isempty(*it))
			{
				cout<<"success! empty clause found"<<endl;
				clauses.push_back("[]");
				parent.push_back(pair<int,int>(popped->p1,popped->p2));
				return true;
			}
			if(visited.find(cleanup(*it))==visited.end()) //New Clause
			{
				clauses.push_back(cleanup(*it));
				parent.push_back(pair<int,int>(popped->p1,popped->p2));
				visited[cleanup(*it)] = 1;

				cout<<(clauses.size())-1<<": "<<printClause(clauses[(clauses.size())-1])<<" generated from "<<parent[(clauses.size())-1].first<<" and "<<parent[(clauses.size())-1].second<<endl;
				//Insert resolvent clauses formed with this new clause and existing clauses
				for(int i=0;i<(clauses.size()) - 1;i++)
				{
					if(resolvable(clauses[i],clauses[(clauses.size())-1]))
					{
						ResPair *n = new ResPair();
						n->p2 = i; 
						n->p1 = (clauses.size()) - 1;
						//cout<<"Pushing: "<<i<<" and "<<(clauses.size()) - 1<<endl;
						candidates.push(n);
						//cout<<(clauses.size()) - 1<<": "<<clauses[(clauses.size()) - 1]<<" "<<parent[(clauses.size()) - 1].first<<" "<<parent[(clauses.size()) - 1].second<<" "<<endl;
					}
				}			
			}
		}
		iter++;
	}
	return false;
}

//print the trace of the empty clause generation from its parents
void preprint(int i, int depth)
{
		for(int j=0;j<depth;j++)
		cout<<" ";
	if(parent[i].first!=-1)
	{
cout<<i<<": "<<printClause(clauses[i])<<" ["<<parent[i].first<<","<<parent[i].second<<"]"<<endl;
		preprint(parent[i].first, depth+1);
		preprint(parent[i].second, depth+1);
	}
	else
		cout<<i<<": "<<printClause(clauses[i])<<" [input]"<<endl;	
}

int main(int argc, char *argv[])
{
  if (argc!=2)
  {
    cout<<"Invalid parameters. Give filename as 1st command line argument to the program."<<endl;
    return 0;
  }

  freopen(argv[1],"r",stdin);

	string line="";
/*	char c;
	//scanf("%c",&c);
	while(c=getchar())
	{
		if(c=='\n')
			{
				cout<<"Line:"<<cleanup(line)<<endl;
				line.clear();
			}
		else
			{
				line = line + c;
				//scanf("%c",&c);
			}
	}
	cout<<"Line:"<<cleanup(line)<<endl;
*/
	while(getline(cin,line))
	{
		if (!commented(line))//not commented and not empty
		{
			string result;
			result = cleanup(line);
			if(visited.find(result)==visited.end())
			{
				clauses.push_back(result);
        cout<<"Pushing:"<<result<<endl;
				parent.push_back(pair<int,int>(-1,-1));
				visited[result] = 1;
			}
		}
		line.clear();
	}

	freopen("/dev/tty","r",stdin);

	for (int i=0;i<clauses.size();i++)
		cout<<i<<": "<<clauses[i]<<endl;

  int i;
  cin>>i;
/*
	if(!resolution())
		cout<<"failure. empty clause not found"<<endl;
	else
		{
			int i = clauses.size() - 1;
			preprint(i, 1);
		}

	cout<<"---------------------------------------"<<endl;
	cout<<"Total number of resolutions (iterations of the main loop) : "<<iter+1<<endl;
	cout<<"Max queue size : "<<maxq<<endl;
*/
  return 0;
}
