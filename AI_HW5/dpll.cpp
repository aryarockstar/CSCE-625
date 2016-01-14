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
	return a1.compare(b1);
}

//sorts a clause and removes extra spaces
string cleanup(string cl)
{
  char text[cl.length() + 1];
	strcpy(text, cl.c_str());
	vector<string> words;
  char *word;
  int wmax=0, i;
	map<string,int> m;
	string str;
	string soln="";

  for(word = strtok(text, " "); word ; word = strtok(NULL, " "))
	{
		words.push_back((string)word);wmax++;        
  }
	char *mywords[words.size()];
	for(i=0; i<wmax; i++)
	{
		mywords[i] = new char[words[i].length() + 1];
		strcpy(mywords[i], words[i].c_str());
	}
  qsort(mywords, wmax, sizeof(*mywords), cmp);//calls cmp to do sorting
  for (i=0; i<wmax; ++i)
	{
		str = mywords[i];
		if (m.find(str)==m.end())
		{
			m[str]=1;
		  //printf("%s\n", mywords[i]);
			soln = soln + " " +mywords[i];
		}
	}
	size_t first = soln.find_first_not_of(' ');
	size_t last = soln.find_last_not_of(' ');
	soln = soln.substr(first, (last-first+1));
	for (i = 1; i < soln.length(); ) 
	{
		if (soln[i - 1] == soln[i] && soln[i]==' ') 
			soln.erase(i, 1);
		else 
			++i;
	}
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

//count the number of words (literals) in a string (clause)
int count_elements(string s)
{
	int count = 1;
	for(int i=0; i<s.length(); i++)
		if(s.at(i)==' ')
			count++;
	return count;
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



int propSize=0;

string getProps()
{
	
	map<string,int> m;
	string s="";
	for(int i=0;i<clauses.size();i++)
	{
		char* pch;
		char* r;
		r = new char[clauses[i].length() + 1];
		strcpy(r, clauses[i].c_str());
		pch = strtok (r," ");
		while (pch != NULL)
		{
		  //printf ("%s\n",pch);
			string search(pch);
			if(search.at(0)=='-')
			{
				search.erase(0,1);
			}
			if(m.find(search)==m.end())
				m[search]=1;
			pch = strtok (NULL, " ");
		}
		for(map<string,int>::iterator it = m.begin(); it != m.end(); it++) 
		{
			s=s+(it->first)+" ";
		}
	}
	propSize = m.size();
	return cleanup(s);
}
int nodes_searched = 0;
map<string, bool> finalmodel;

struct part
{
	string P;
	bool val;
};

struct part findUnitClause(map<string,bool> model)
{
	//cout<<"Find Unit Clause"<<endl;
	struct part p;
	p.P = " ";
	p.val = false;
	for(int i=0;i<clauses.size();i++)
	{
		//If clause has one element
		if(count_elements(clauses[i])==1)
		{
			char* pch;
			char* r;
			r = new char[clauses[i].length() + 1];
			strcpy(r, clauses[i].c_str());
			pch = strtok (r," ");
			string search(pch);
			if(search.at(0)=='-')
			{
				search.erase(0,1);
				if(model.find(search)==model.end())
				{
					p.P = search;
					p.val = false;
					cout<<"unit_clause on ("<<printClause(clauses[i])<<") implies "<<p.P<<"="<<"False"<<endl;
					return p;
				}
			}
			else
			{
				if(model.find(search)==model.end())
				{
					p.P = search;
					p.val = true;
					cout<<"unit_clause on ("<<printClause(clauses[i])<<") implies "<<p.P<<"="<<"True"<<endl;
					return p;
				}
			}
		}
		//If clause has more than one element -> Check if all other elements in the clause give false
		//1. If clause has n elements, n-1 elements must be in model already
		//2. Check if all n-1 elements become false through model
		//3. return the element not in model
		else
		{
			int n = count_elements(clauses[i]);

			//cout<<"n is:"<<n<<endl;
			char* pch;
			char* r;
			int count = 0;
			r = new char[clauses[i].length() + 1];
			strcpy(r, clauses[i].c_str());
			pch = strtok (r," ");
			while (pch != NULL)
			{
				string search(pch);
				if(search.at(0)=='-')
				{
					search.erase(0,1);
					if(model.find(search)!=model.end() && model[search]==true)
					{
							count++;
							//cout<<"count is now:"<<count<<endl;
					}
				}
				else
				{
					if(model.find(search)!=model.end() && model[search]==false)
					{
							count++;
							//cout<<"count is now:"<<count<<endl;
					}
				}
				pch = strtok (NULL, " ");
			}
			if(count==n-1)
			{
				char* pch;
				char* r;
				r = new char[clauses[i].length() + 1];
				strcpy(r, clauses[i].c_str());
				pch = strtok (r," ");
				while (pch != NULL)
				{
					string search(pch);
					if(search.at(0)=='-')
					{
						search.erase(0,1);
						if(model.find(search) == model.end())
						{
							p.P = search;
							p.val = false;
							cout<<"unit_clause on ("<<printClause(clauses[i])<<") implies "<<p.P<<"="<<"False"<<endl;
							return p;
						}
					}
					else
					{
						if(model.find(search)==model.end())
						{
							p.P = search;
							p.val = true;
							cout<<"unit_clause on ("<<printClause(clauses[i])<<") implies "<<p.P<<"="<<"True"<<endl;
							return p;
						}
					}
					pch = strtok (NULL, " ");
				}
			}
		}				
	}
	return p;
}

struct part findPureSymbol(string symbols, map<string,bool> model)
{
	struct part p;
	p.P = " ";
	p.val = false;
	if(isempty(symbols))
		return p;
	//populate if current model makes a clause true or not, for all clauses
	//a clause is true if any one of its elements is true
	bool clausesState[clauses.size()];
	for(int i=0;i<clauses.size();i++)
	{
		char* pch;
		char* r;
		int flag = 0;
		r = new char[clauses[i].length() + 1];
		strcpy(r, clauses[i].c_str());
		pch = strtok (r," ");
		while (pch != NULL)
		{
			string search(pch);
			if(search.at(0)=='-')
			{
				search.erase(0,1);
				if(model.find(search)!=model.end())
				{
					if(model[search]==false)
					{
						flag = 1;
						break;
					}
				}
			}
			else
			{
				if(model.find(search)!=model.end())
				{
					if(model[search]==true)
					{
						flag = 1;
						break;
					}
				}
			}
			pch = strtok (NULL, " ");
		}
		if (flag == 1)
			clausesState[i] = true;
		else
			clausesState[i] = false;
	}

	//for each symbol not in model, check if that symbol appears with the same sign in all false clauses
	char* pch;
	char* r;
	r = new char[symbols.length() + 1];
	strcpy(r, symbols.c_str());
	pch = strtok (r," ");
	while (pch != NULL)
	{
		string search(pch);
		if(model.find(search)==model.end())
		{
			int flag = 0;
			//check if search appears with same sign in all false clauses
			for(int i=0;i<clauses.size();i++)
			{
				if(clausesState[i]==false)
				{
					if(clauses[i].find(search)!=string::npos && clauses[i].find("-"+search)==string::npos)
					{
						int pos = clauses[i].find(search);
						if(pos==0 || clauses[i].at(pos-1)==' ')
						{
							if(flag == -1)
							{
								flag = 0;
								break;
							}
							flag = 1;
						}
					}
					else if(clauses[i].find("-"+search)!=string::npos)
					{
						if(flag == 1)
						{
							flag = 0;
							break;
						}
						flag = -1;
					}
				}
			}
			if(flag == 1)
			{
				p.P = search;
				p.val = true;
				cout<<"pure_symbol on "<<p.P<<"="<<(p.val?"True":"False")<<endl;
				return p;
			}
			else if(flag == -1)
			{
				p.P = search;
				p.val = false;
				cout<<"pure_symbol on "<<p.P<<"="<<(p.val?"True":"False")<<endl;
				return p;
			}
		}
		pch = strtok (NULL, " ");
	}
	return p;
}

bool DPLL(string symbols, map<string,bool> model)
{
	nodes_searched++;
	finalmodel.clear();
	finalmodel = model;
	cout<<"model= {";
	int c = 0;
	map<string,bool>::iterator it=model.begin();
	for (it=model.begin(); it!=model.end(); ++it)
	{
		if(c==model.size()-1)
		break;
		c++;
    cout<<"'"<<it->first<<"': "<< (it->second?"True":"False") <<", ";	
	}
	if(model.size()!=0)
	cout<<"'"<<it->first<<"': "<< (it->second?"True":"False") <<"}"<<endl;
	else
	cout<<"}"<<endl;
	//if every clause in clauses is true in model then return true
	if(model.size() == propSize)
	{
		//cout<<"HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"<<endl;
		int i;
		for(i=0;i<clauses.size();i++)
		{
			char* pch;
			char* r;
			int flag = 1;
			r = new char[clauses[i].length() + 1];
			strcpy(r, clauses[i].c_str());
			pch = strtok (r," ");
			while (pch != NULL)
			{
				string search(pch);
				if(search.at(0)=='-')
				{
					search.erase(0,1);
					if(model.find(search)==model.end())
					{
						flag = 0;
						break;
					}
					else
					{
						if(model[search]==true)
						{
							flag = 0;
							break;
						}
					}
				}
				else
				{
					if(model.find(search)==model.end())
					{
						flag = 0;
						break;
					}
					else
					{
						if(model[search]==false)
						{
							flag = 0;
							break;
						}
					}
				}
				pch = strtok (NULL, " ");
			}
			if(flag==0)
				break;
		
		}
		if(i == clauses.size())
		{
			return true;
		}
	}
	//if some clause in clauses is false in model then return false
	int i;
	for(i=0;i<clauses.size();i++)
	{
		char* pch;
		char* r;
		int flag = 0;
		r = new char[clauses[i].length() + 1];
		strcpy(r, clauses[i].c_str());
		pch = strtok (r," ");
		//cout<<"Clause:"<<endl;
		while (pch != NULL)
		{
			string search(pch);
			//cout<<search<<" ";
			if(search.at(0)=='-')
			{
				search.erase(0,1);
				if(model.find(search)==model.end())
				{
					flag = 1;
					break;
				}
				else
				{
					if(model[search]==false)
					{
						flag = 1;
						break;
					}
				}
			}
			else
			{
				if(model.find(search)==model.end())
				{
					flag = 1;
					break;
				}
				else
				{
					if(model[search]==true)
					{
						flag = 1;
						break;
					}
				}
			}
			pch = strtok (NULL, " ");
		}
		//cout<<endl;
		if (flag==0)
		{
			//cout<<"Flag hai zero"<<endl;
			return false;
		}
	}
	if(isempty(symbols))
	{
		return true;
	}
	//Symbols not empty

	//Pure Symbol Heuristic Starts//Comment this set to disable pure_symbol heuristic
	struct part pure = findPureSymbol(symbols, model);
	if(!isempty(pure.P))//P is not empty
	{
		map<string,bool> model2 = model;
		string symbols2=symbols;
		char* pch;
		char* r;
		r = new char[symbols2.length() + 1];
		strcpy(r, symbols2.c_str());
		pch = strtok (r," ");
		string P(pure.P);
		symbols2 = " " + symbols2;
		int start_pos = symbols2.find(" "+P);
		if(start_pos != std::string::npos)
		{
			symbols2.replace(start_pos+1, P.length(), "");
			model2[P] = pure.val;
			if(symbols2.at(0)==' ')
			symbols2.erase(0,1);
			//cout<<"trying "<<P<<"=T"<<endl;
			return DPLL(symbols2, model2);
		}
		if(symbols2.at(0)==' ')
		symbols2.erase(0,1);
	}
	//Pure Symbol Heuristic Ends//

	//Unit Clause Heuristic Starts//Comment this set to disable unit_clause heuristic
	struct part p = findUnitClause(model);
	if(!isempty(p.P))//P is not empty
	{
		map<string,bool> model2 = model;
		string symbols2=symbols;
		char* pch;
		char* r;
		r = new char[symbols2.length() + 1];
		strcpy(r, symbols2.c_str());
		pch = strtok (r," ");
		string P(p.P);
		symbols2 = " " + symbols2;
		int start_pos = symbols2.find(" "+P);
		if(start_pos != std::string::npos)
		{
			symbols2.replace(start_pos+1, P.length(), "");
			model2[P] = p.val;
			//cout<<"trying "<<P<<"=T"<<endl;
			if(symbols2.at(0)==' ')
			symbols2.erase(0,1);
			return DPLL(symbols2, model2);
		}
		if(symbols2.at(0)==' ')
		symbols2.erase(0,1);
	}
	//Unit Clause Heuristic Ends//

	char* pch;
	char* r;
	r = new char[symbols.length() + 1];
	strcpy(r, symbols.c_str());
	pch = strtok (r," ");
	string P(pch);
	symbols = " "+symbols;
  int start_pos = symbols.find(" "+P);
  if(start_pos != std::string::npos)
  symbols.replace(start_pos+1, P.length(), "");
	if(symbols.at(0)==' ')
	symbols.erase(0,1);
	int res=0;
	model[P] = true;
	cout<<"trying "<<P<<"=T"<<endl;
	res = DPLL(symbols, model);
	if(res==true)
	{
		return true;
	}
	model[P] = false;
	cout<<"backtracking"<<endl;
	cout<<"trying "<<P<<"=F"<<endl;
	res = res | DPLL(symbols, model);
	return res;
}

int main(int argc, char *argv[])
{
  if (argc<2)
  {
    cout<<"Invalid parameters. Give filename as 1st command line argument to the program. If any extra facts need to be provided, provide as string separated by space as 2nd command line argument."<<endl;
    return 0;
  }

  freopen(argv[1],"r",stdin);
	string line, result;
	while(getline(cin,line))
	{
		if (!commented(line))//not commented and not empty
		{
			result = cleanup(line);
			if(visited.find(result)==visited.end())
			{
				clauses.push_back(result);
				parent.push_back(pair<int,int>(-1,-1));
				visited[result] = 1;
			}
		}
	}

	if(argc==3)
	{
		char* pch;

		pch = strtok (argv[2]," ");
		//cout<<"Clause:"<<endl;
		while (pch != NULL)
		{
			string search(pch);
			if (!commented(search))//not commented and not empty
			{
				if(visited.find(search)==visited.end())
				{
					clauses.push_back(search);
					parent.push_back(pair<int,int>(-1,-1));
					visited[search] = 1;
				}
			}
			pch = strtok (NULL, " ");
		}
	}
	//find props
	cout<<"props:"<<endl;
	string symbols = getProps();
	cout<<symbols<<endl;

	cout<<"initial clauses:"<<endl;
	for (int i=0;i<clauses.size();i++)
		cout<<i<<": ("<<printClause(clauses[i])<<")"<<endl;

	map<string,bool> model;
	if(DPLL(symbols, model))
	{
		cout<<"------------"<<endl;
		cout<<"nodes searched="<<nodes_searched<<endl;
		cout<<"solution:"<<endl;
		for (map<string,bool>::iterator it=finalmodel.begin(); it!=finalmodel.end(); ++it)
		{
		  cout<<it->first<<"="<< (it->second?"True":"False") <<endl;	
		}
		cout<<"true props:"<<endl;
		for (map<string,bool>::iterator it=finalmodel.begin(); it!=finalmodel.end(); ++it)
		{
			if(it->second==true)
		  cout<<it->first<<endl;	
		}
		if(argc==3)
		{
			cout<<"------------"<<endl;
			cout<<"Considering single character props as agents:"<<endl;
			cout<<"agent team: ";
			for (map<string,bool>::iterator it=finalmodel.begin(); it!=finalmodel.end(); ++it)
			{
				if(it->second==true && (it->first).size()==1)
				cout<<it->first<<" ";	
			}			
			cout<<endl;
		}
	}
	else
	{
		cout<<"------------"<<endl;
		cout<<"Solution not found."<<endl;	
		cout<<"nodes searched="<<nodes_searched<<endl;
	}

  return 0;
}
