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
clock_t ts,te;

ifstream ifp1;
ifstream ifp2;

string targetStr;
void initInput()
{
	if (!ifp1)
	{
		cout << "File open Error" << endl;
		return;
	}

	for (int i = 0; i<5; i++)
	{
		for (int j = 0; j<5; j++)
		{
			ifp1 >> inputArray[i][j];
			// ifp2 >> targetArray[i][j];
			if(inputArray[i][j] == 0)
			{
				blanki = i;
				blankj = j;
			}
		}
	}
}

void initTarget()
{
	if(!ifp2)
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
	char state[5][5];

	Node();
	Node(int _hCost, int _gCost, int _fCost);
	void setFromNode(Node *p);
	void print();
	string toString();
};

string Node::toString()
{
	// ts = clock();
	char temp[26];
	for(int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
		{
			temp[i*5+j] = state[i][j] + 66;
		}
	}
	temp[25] = '\0';
   	// te = clock();
   	// t1 += te-ts;
	return temp;
}

Node::Node() {}

Node::Node(int _hCost, int _gCost, int _fCost)
{
	hCost = _hCost;
	gCost = _gCost;
	fCost = _fCost;
	// next = NULL;
	// parent = NULL;
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
	cout << hCost << ",";
	cout << gCost << ",";
	cout << fCost << ",";
	cout << blanki << ",";
	cout << blankj << ",";
	cout << path << endl;
}



struct cmp
{
	bool operator()(Node *a, Node *b)
	{
		return a->fCost > b->fCost;
	}
};




string puzzleSolution()
{
	t1 = 0;
	clock_t timeStart;
	clock_t timeEnd;
	priority_queue<Node*,vector<Node*>,cmp> myQ;
	set<string> myMap;

	Node *root = new Node(0,0,0);
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			root->state[i][j] = inputArray[i][j];
			if (root->state[i][j] != targetArray[i][j] && root->state[i][j] != 0)
			{
				root->hCost++;
				root->fCost++;
			}
		}
	}
	root->blanki = blanki;
	root->blankj = blankj;
	myQ.push(root);
	myMap.insert(root->toString());

	cout << root->toString() << endl;

	int k=0;
	while(1)
	{

		Node *p = myQ.top();
		myQ.pop();

		if (p->hCost == 0 && p->toString()==targetStr)
		{
			// cout << myMap.size() << endl;

			return p->path;
			return NULL;
		}
		// p->print();
		if (p->blanki != 0 && p->state[p->blanki-1][p->blankj]!=-1 && p->path[p->path.length()-1]!='D')
		{
			Node *pu = new Node();
			pu->setFromNode(p);
			int indexi = pu->blanki;
			int indexj = pu->blankj;

			int temp = pu->state[indexi - 1][indexj];
			pu->state[indexi - 1][indexj] = pu->state[indexi][indexj];
			pu->state[indexi][indexj] = temp;
			// clock_t timeStart = clock();
			// clock_t timeEnd;
			if(myMap.find(pu->toString()) != myMap.end())
			{
				// timeEnd = clock();
				delete pu;
				pu = NULL;

			}
			else
			{
				timeEnd = clock();
				pu->blanki--;
				pu->path.append("U");
				indexi = p->blanki;
				indexj = p->blankj;
				if(p->state[indexi - 1][indexj] !=0)
				{
					if (p->state[indexi - 1][indexj] != targetArray[indexi-1][indexj])
					{
						if (p->state[indexi - 1][indexj] == targetArray[indexi][indexj])
						{
							pu->hCost--;
							pu->fCost--;
						}
					}
					else
					{
						pu->hCost++;
						pu->fCost++;
					}
				}
				if(p->state[indexi][indexj]!=0)
				{
					if (p->state[indexi][indexj] != targetArray[indexi][indexj])
					{
						if (p->state[indexi][indexj] == targetArray[indexi-1][indexj])
						{
							pu->hCost--;
							pu->fCost--;
						}
					}
					else
					{
						pu->hCost++;
						pu->fCost++;
					}
				}

				myQ.push(pu);	
				// clock_t timeStart = clock();
				myMap.insert(pu->toString());
				// clock_t timeEnd = clock();
				// t1+= timeEnd - timeStart;
			}
		}
		//Down
		if (p->blanki != 4 && p->state[p->blanki+1][p->blankj]!=-1 && p->path[p->path.length()-1]!='U')
		{
			Node *pd = new Node();
			pd->setFromNode(p);

			int temp = pd->state[pd->blanki + 1][pd->blankj];
			pd->state[pd->blanki + 1][pd->blankj] = pd->state[pd->blanki][pd->blankj];
			pd->state[pd->blanki][pd->blankj] = temp;

			if(myMap.find(pd->toString()) != myMap.end())
			{
				delete pd;
				pd = NULL;
			}
			else
			{
				pd->blanki++;
				pd->path.append("D");
				if(p->state[p->blanki + 1][p->blankj]!=0)
				{
					if (p->state[p->blanki + 1][p->blankj] != targetArray[p->blanki + 1][p->blankj])
					{
						if (p->state[p->blanki + 1][p->blankj] == targetArray[p->blanki][p->blankj])
						{
							pd->hCost--;
							pd->fCost--;
						}
					}
					else
					{
						pd->hCost++;
						pd->fCost++;
					}
				}
				if(p->state[p->blanki][p->blankj]!=0)
				{
					if (p->state[p->blanki][p->blankj] != targetArray[p->blanki][p->blankj])
					{
						if (p->state[p->blanki][p->blankj] == targetArray[p->blanki + 1][p->blankj])
						{
							pd->hCost--;
							pd->fCost--;
						}
					}
					else
					{
						pd->hCost++;
						pd->fCost++;
					}
				}
				myQ.push(pd);	
				myMap.insert(pd->toString());
			}
		}
		//Left
		if (p->blankj != 0 && p->path[p->path.length()-1]!='R')
		{
			Node *pl = new Node();
			pl->setFromNode(p);
			int h;
			if (p->state[p->blanki][p->blankj - 1] != -1)h = 1;
			else h = 2;

			int temp = pl->state[pl->blanki][pl->blankj - h];
			pl->state[pl->blanki][pl->blankj - h] = pl->state[pl->blanki][pl->blankj];
			pl->state[pl->blanki][pl->blankj] = temp;

			if(myMap.find(pl->toString()) != myMap.end())
			{
				delete pl;
				pl = NULL;
			}
			else
			{
				pl->blankj-=h;
				pl->path.append("L");
				if(p->state[p->blanki][p->blankj-h]!=0)
				{
					if (p->state[p->blanki][p->blankj-h] != targetArray[p->blanki][p->blankj-h])
					{
						if (p->state[p->blanki][p->blankj-h] == targetArray[p->blanki][p->blankj])
						{
							pl->hCost--;
							pl->fCost--;
						}
					}
					else
					{
						pl->hCost++;
						pl->fCost++;
					}
				}
				if(p->state[p->blanki][p->blankj]!=0)
				{
					if (p->state[p->blanki][p->blankj] != targetArray[p->blanki][p->blankj])
					{
						if (p->state[p->blanki][p->blankj] == targetArray[p->blanki][p->blankj-h])
						{
							pl->hCost--;
							pl->fCost--;
						}
					}
					else
					{
						pl->hCost++;
						pl->fCost++;
					}
				}
				myQ.push(pl);	
				myMap.insert(pl->toString());
			}
		}
		//Right
		if (p->blankj != 4 && p->path[p->path.length()-1]!='L')
		{
			Node *pr = new Node();
			pr->setFromNode(p);
			int h;
			if (p->state[p->blanki][p->blankj + 1] != -1)h = 1;
			else h = 2;
			int temp = pr->state[pr->blanki][pr->blankj + h];
			pr->state[pr->blanki][pr->blankj + h] = pr->state[pr->blanki][pr->blankj];
			pr->state[pr->blanki][pr->blankj] = temp;

			if(myMap.find(pr->toString()) != myMap.end())
			{
				// cout << 5 << endl;
				delete pr;
				pr = NULL;
			}
			else
			{
				pr->blankj += h;

				pr->path.append("R");
				if(p->state[p->blanki][p->blankj + h]!=0)
				{
					if (p->state[p->blanki][p->blankj + h] != targetArray[p->blanki][p->blankj + h])
					{
						if (p->state[p->blanki][p->blankj+h] == targetArray[p->blanki][p->blankj])
						{
							pr->hCost--;
							pr->fCost--;
							// cout << 1;
							// cout << endl;
						}
					}
					else
					{
						pr->hCost++;
						pr->fCost++;
						// cout << 2;
						// cout << endl;
					}
				}
				if(p->state[p->blanki][p->blankj]!=0)
				{
					if (p->state[p->blanki][p->blankj] != targetArray[p->blanki][p->blankj])
					{
						if (p->state[p->blanki][p->blankj] == targetArray[p->blanki][p->blankj+h])
						{
							pr->hCost--;
							pr->fCost--;
							// cout << 3;
							// cout << endl;
						}
					}
					else
					{
						pr->hCost++;
						pr->fCost++;
						// cout << 4;
						// cout << endl;
					}
				}
				myQ.push(pr);	
				myMap.insert(pr->toString());
			}
		}
		delete p;
		p = NULL;
	}

	return NULL;
}

int main(int argc, char const *argv[])
{
	ifp1.open("input.txt");
	ifp2.open("target.txt");

	initTarget();
	string path;

	ofstream ifp3("output_Ah1.txt");
	if(!ifp3)
	{
		cout << "output_Ah1.txt open fail" << endl;
		return 1;
	}
	
	for(int i=0;i<8;i++)
	{
		initInput();
		clock_t timeStart = clock();
		path = puzzleSolution();
		clock_t timeEnd = clock();
		cout << (double)(timeEnd-timeStart)/CLOCKS_PER_SEC << endl;
		ifp3 << (double)(timeEnd-timeStart)/CLOCKS_PER_SEC << endl;

		cout << path << endl << path.length() << endl;
		// cout << "ToString take" << (double)t1/CLOCKS_PER_SEC << endl;
		cout << endl;

		ifp3 << path << endl << path.length() << endl;
		ifp3 << endl;
	}

	ifp1.close();
	ifp2.close();
	return 0;
}