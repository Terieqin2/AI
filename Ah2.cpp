#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <memory.h>
#include <set>
#include <unordered_map>
#include <sstream>

using namespace std;

int inputArray[5][5];
int targetArray[5][5];
int blanki = 0, blankj = 0;

clock_t t1 = 0;

ifstream ifp1,ifp2;

string targetStr;

void initInput()
{
	
	if (!ifp1)
	{
		cout << "Input File open Error" << endl;
		return;
	}

	for (int i = 0; i<5; i++)
	{
		for (int j = 0; j<5; j++)
		{
			ifp1 >> inputArray[i][j];
			//ifp2 >> targetArray[i][j];
			if(inputArray[i][j] == 0)
			{
				blanki = i;
				blankj = j;
			}
		}
	}
}

void targetInit()
{
	if (!ifp2)
	{
		cout << "Target File open Error" << endl;
		return;
	}

	for (int i = 0; i<5; i++)
	{
		for (int j = 0; j<5; j++)
		{
			// ifp1 >> inputArray[i][j];
			ifp2 >> targetArray[i][j];

		}
	}
	char temp[26];
   	for(int i=0;i<5;i++)
   	{
   		for(int j=0;j<5;j++)
   		{
   			temp[i*5+j] = targetArray[i][j] + 66;
   		}
   	}
   	temp[25] = '\0';
   	targetStr = temp;
}


class Node
{
public :
	int hCost;
	int gCost;
	int fCost;
	int blanki;
	int blankj;
	string path;
	int state[5][5];

	Node();
	Node(int _hCost, int _gCost, int _fCost);
	void setFromNode(Node *p);
	void print();
	void printState();
	string toString();

};

string Node::toString()
{
	char temp[26];
   	for(int i=0;i<5;i++)
   	{
   		for(int j=0;j<5;j++)
   		{
   			temp[i*5+j] = state[i][j] + 66;
   		}
   	}
   	temp[25] = '\0';
   	return temp;
}

Node::Node() {}

Node::Node(int _hCost, int _gCost, int _fCost)
{
	hCost = _hCost;
	gCost = _gCost;
	fCost = _fCost;
	blanki = 0;
	blankj = 0;
	path = "";
}

void Node::setFromNode(Node *p)
{
	hCost = p->hCost;
	gCost = p->gCost+1;
	fCost = p->fCost+1;
	blanki = p->blanki;
	blankj = p->blankj;
	path = p->path;
	//memcpy(&state[0][0], &(p->state[0][0]),25*sizeof(int));
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			state[i][j] = p->state[i][j];
		}
	}

}

void Node::print()
{
	cout << hCost << "\t";
	cout << gCost << "\t";
	cout << fCost << ",";
	cout << blanki << ",";
	cout << blankj << ",";
	cout << path << endl;
}

void Node::printState()
{
	for(int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
		{
			cout << state[i][j] << "\t";
		}
		cout << endl;
	}
}

class Position
{
public:
	int i;
	int j;

	void set(int _i, int _j);
	bool isEqual(int _i, int _j);
};

void Position::set(int _i, int _j)
{
	i = _i;
	j = _j;
}

bool Position::isEqual(int _i, int _j)
{
	if(i!=_i || j!=_j)return false;
	else return true;
}

struct cmp
{
	bool operator()(Node *a, Node *b)
	{
		return a->fCost > b->fCost;
	}
};

int getManhattabDistance(int si,int sj,int ti,int tj)
{
	int dis = abs(si-ti)+abs(sj-tj);
	if(((2>=si && 2<=ti) || (2>=ti && 2<=si)) && ((sj>=1 && tj<+1)||(sj<=1 && tj>=1)))
	{
		dis--;
	}
	if(((2>=si && 2<=ti) || (2>=ti && 2<=si)) && ((sj>=3 && tj<=3)||(sj<=3 && tj>=3)))
	{
		dis--;
	}
	return dis;
}


string puzzleSolution()
{
	clock_t timeStart;
	clock_t timeEnd;
	int dCount = 0;

	priority_queue<Node*, vector<Node*>, cmp> myQ;
	set<string> myMap;

	Position targetPos[25];
	for(int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
		{
			if(targetArray[i][j] == -1)continue;
			else targetPos[targetArray[i][j]].set(i,j);
		}
	}

	Node *root = new Node(0,0,0);
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			int temp = inputArray[i][j];
			root->state[i][j] = temp;
			if(temp == -1 || temp == 0)continue;
			else if(!targetPos[temp].isEqual(i,j))
			{
				root->hCost += getManhattabDistance(targetPos[temp].i, targetPos[temp].j,i,j);
			}
		}
	}
	root->fCost = root->hCost;
	root->blanki = blanki;
	root->blankj = blankj;
	myQ.push(root);
	myMap.insert(root->toString());
	// root->print();
	int k=0;

	while(1)
	{
		k++;
		Node *p = myQ.top();

		myQ.pop();

		if (p->toString()==targetStr)
		{
			if(p->hCost != 0)
			{
				cout << "Error !!!!!!" << endl;
				return "Hhhh";
			}
			// p->print();
			return p->path;
		}
		p->print();
		// p->printState();
		// up
		if (p->blanki != 0 && p->state[p->blanki-1][p->blankj]!=-1 && (p->path == "" || p->path[p->path.length()-1]!='D'))
		{
			// if(k == 1)cout << "In U" << endl;
			Node *pu = new Node();
			pu->setFromNode(p);

			int dirPos = 0;
			int temp = pu->state[pu->blanki - 1][pu->blankj];
			dirPos = getManhattabDistance(targetPos[temp].i,targetPos[temp].j,pu->blanki,pu->blankj) -  getManhattabDistance(targetPos[temp].i,targetPos[temp].j,pu->blanki-1,pu->blankj);

			pu->hCost +=dirPos;
			pu->fCost += dirPos;

			pu->state[pu->blanki - 1][pu->blankj] = pu->state[pu->blanki][pu->blankj];
			pu->state[pu->blanki][pu->blankj] = temp;

			pu->blanki--;
			pu->path.append("U");
			

			if(myMap.find(pu->toString())!=myMap.end())
			{
				dCount++;
				delete pu;
				pu = NULL;
			}
			else
			{
				timeEnd = clock();
				myQ.push(pu);	
				myMap.insert(pu->toString());				
			} 
		}
		//Down
		if (p->blanki != 4 && p->state[p->blanki+1][p->blankj]!=-1 && (p->path == "" || p->path[p->path.length()-1]!='U'))
		{
			// if(k == 1)cout << "In D" << endl;

			Node *pd = new Node();
			pd->setFromNode(p);

			int dirPos =0;
			int temp = pd->state[pd->blanki + 1][pd->blankj];
			// dirPos = abs(targetPos[temp].i - pd->blanki)-abs(targetPos[temp].i - (pd->blanki+1));
			dirPos = getManhattabDistance(targetPos[temp].i,targetPos[temp].j,pd->blanki,pd->blankj) -  getManhattabDistance(targetPos[temp].i,targetPos[temp].j,pd->blanki+1,pd->blankj);


			pd->hCost +=dirPos;
			pd->fCost += dirPos;

			pd->state[pd->blanki + 1][pd->blankj] = pd->state[pd->blanki][pd->blankj];
			pd->state[pd->blanki][pd->blankj] = temp;

			pd->blanki++;
			pd->path.append("D");

			if(myMap.find(pd->toString())!=myMap.end())
			{
				dCount++;
				delete pd;
				pd = NULL;
			}
			else
			{
				myQ.push(pd);	
				myMap.insert(pd->toString());
			} 	
		}
		//Left
		if (p->blankj != 0 && (p->path == "" || p->path[p->path.length()-1]!='R'))
		{
			// if(k == 1)cout << "In L" << endl;
			Node *pl = new Node();
			pl->setFromNode(p);
			int h;
			if (p->state[p->blanki][p->blankj - 1] != -1)h = 1;
			else h = 2;

			int dirPos = 0;
			int temp = pl->state[pl->blanki][pl->blankj - h];
			// dirPos = abs(targetPos[temp].j - pl->blankj)-abs(targetPos[temp].j - (pl->blankj-h));
			dirPos = getManhattabDistance(targetPos[temp].i,targetPos[temp].j,pl->blanki,pl->blankj) -  getManhattabDistance(targetPos[temp].i,targetPos[temp].j,pl->blanki,pl->blankj-h);



			pl->hCost +=dirPos;
			pl->fCost += dirPos;

			pl->state[pl->blanki][pl->blankj - h] = pl->state[pl->blanki][pl->blankj];
			pl->state[pl->blanki][pl->blankj] = temp;
			pl->blankj-=h;
			
			pl->path.append("L");

			if(myMap.find(pl->toString())!=myMap.end())
			{
				dCount++;
				delete pl;
				pl = NULL;
			}
			else
			{
				myQ.push(pl);	
				myMap.insert(pl->toString());
			} 	
		}
		//Right
		if (p->blankj != 4 && (p->path == "" || p->path[p->path.length()-1]!='L'))
		{
			// if(k == 1)cout << "In R" << endl;
			Node *pr = new Node();
			pr->setFromNode(p);
			int h;
			if (p->state[p->blanki][p->blankj + 1] != -1)h = 1;
			else h = 2;

			int dirPos = 0;
			int temp = pr->state[pr->blanki][pr->blankj + h];
			// dirPos = abs(targetPos[temp].j - pr->blankj)-abs(targetPos[temp].j - (pr->blankj+h));
			dirPos = getManhattabDistance(targetPos[temp].i,targetPos[temp].j,pr->blanki,pr->blankj) -  getManhattabDistance(targetPos[temp].i,targetPos[temp].j,pr->blanki,pr->blankj+h);

			pr->hCost +=dirPos;
			pr->fCost += dirPos;

			pr->state[pr->blanki][pr->blankj + h] = pr->state[pr->blanki][pr->blankj];
			pr->state[pr->blanki][pr->blankj] = temp;
			pr->blankj+=h;

			pr->path.append("R");

			// string strTemp = pr->toString();
			if(myMap.find(pr->toString())!=myMap.end())
			{
				dCount++;
				delete pr;
				pr = NULL;
			}
			else
			{
				myQ.push(pr);	
				myMap.insert(pr->toString());
			} 	
			//timeEnd = clock();
			//t1 += timeEnd-timeStart;
		}
		delete p;
		p=NULL;
	}
	return NULL;
}

int main(int argc, char const *argv[])
{
	ifp1.open("input.txt");
	ifp2.open("target.txt");
	// ifp2.open(targetFilePath);
	targetInit();

	ofstream ofp1("output_Ah2.txt");
	if(!ofp1)
	{
		cout << "output_Ah2.txt open fail" << endl;
		return 1;
	}

	string path;


	for(int i=0;i<1;i++)
	{
		initInput();
		clock_t timeStart = clock();
		path = puzzleSolution();
		clock_t timeEnd = clock();
		cout << (double)(timeEnd-timeStart)/CLOCKS_PER_SEC << "s" << endl;
		// cout << (double)t1/CLOCKS_PER_SEC << endl;

		ofp1 << (double)(timeEnd-timeStart)/CLOCKS_PER_SEC << "s" << endl;
		cout << path << endl;
		cout << path.length() << endl;
		cout << endl;
		ofp1 << path << endl << path.length() << endl;
		ofp1 << endl;
	}

	// string asd = "RULULDDRDRRUUULLDDDLDLUURDDRURULULLDRULULDDRDDRRULDLURRD";
	// cout << asd.length() << endl;
	ifp1.close();
	ifp2.close();
	return 0;
}