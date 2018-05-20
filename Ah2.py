import time
import queue

inputArray =[]
targetArray = []
blanki = 0
blankj = 0

targetStr = ""

def readInt(fp):
	a = ''
	rStr = ""
	while 1:
		a = fp.read(1)
		if (a >= '0' and a <='9') or a=='-':
			break

	while 1:
		rStr = rStr + a
		a = fp.read(1)
		if a <'0' or a>'9':
			break

	return int(rStr)

def targetInit():
	for i in range(5):
		tempArray = []
		for j in range(5):
			a = readInt(ifp2)
			tempArray.append(a)
			global targetStr
			targetStr = targetStr + str(a)
		targetArray.append(tempArray)
	# print(targetArray)
	# print(targetStr)

def inputInit():
	global inputArray
	inputArray = []
	for i in range(5):
		tempArray = []
		for j in range(5):
			a = readInt(ifp1)
			tempArray.append(a)
			if a==0:
				global blanki
				blanki = i
				global blankj
				blankj = j
		inputArray.append(tempArray)
	# print(inputArray)

class Node:
	hCost = 1
	gCost = 1
	fCost = 1
	blanki = 1
	blankj = 1
	path = ""
	state = []

	def __init__(self,h,g,f):
		self.hCost = h
		self.gCost = g
		self.fCost = f
		self.state = []

		for i in range(5):
			tempArray = []
			for j in range(5):
				tempArray.append(0)
			self.state.append(tempArray)
			# print(i)
		# print(self.state)

	def toString(self):
		strTemp = ""
		for i in range(5):
			for j in range(5):
				strTemp = strTemp + str(self.state[i][j])
		return strTemp

	def setFromNode(self,p):
		self.hCost = p.hCost
		self.gCost = p.gCost+1
		self.fCost = p.fCost+1
		self.blanki = p.blanki
		self.blankj = p.blankj
		self.path = p.path

		for i in range(5):
			for j in range(5):
				self.state[i][j] = p.state[i][j]

	def __lt__(self, other):
		return self.fCost <  other.fCost

	def print(self):
		print(self.hCost,self.gCost,self.fCost,self.blanki,self.blankj,self.path)
		# print(self.gCost)
		# print(self.fCost)
		# print(self.blanki)
		# print(self.blankj)
		# print(self.path)

class Position:
	i = 0
	j = 0

	def set(self,_i,_j):
		self.i = _i
		self.j = _j

	def isEqual(self, _i, _j):
		if self.i!=_i or self.j!=_j:
			return False
		else:
			return True

def getManhattanDistance(si,sj,ti,tj):
	dis = abs(si-ti)+abs(sj-tj)
	if(((2>=si and 2<=ti) or (2>=ti and 2<=si)) and ((sj>=1 and tj<+1) or (sj<=1 and tj>=1))):
		dis = dis - 1
	
	if(((2>=si and 2<=ti) or (2>=ti and 2<=si)) and ((sj>=3 and tj<=3) or (sj<=3 and tj>=3))):
		dis = dis - 1
	return dis

def puzzleSolution():
	myQueue = queue.PriorityQueue()
	mySet = set()

	dCount = 0

	targetPos = []
	for i in range(25):
		targetPos.append(Position())

	for i in range(5):
		for j in range(5):
			if targetArray[i][j] == -1:
				continue
			else:
				targetPos[targetArray[i][j]].set(i,j)

	# for i in range(25):
	# 	print(targetPos[i].i,targetPos[i].j)
	# print(inputArray)

	root = Node(0,0,0)
	for i in range(5):
		for j in range(5):
			temp = inputArray[i][j]
			root.state[i][j] = temp
			if temp == -1 or temp == 0:
				continue
			elif (not targetPos[temp].isEqual(i,j)):
				root.hCost += getManhattanDistance(targetPos[temp].i, targetPos[temp].j,i,j)

	# print(root.state)
	root.fCost = root.hCost
	root.blanki = blanki
	root.blankj = blankj
	# root.print()
	# print(root.toString())
	myQueue.put(root)
	mySet.add(root.toString())

	k = 0
	while True:
		k += 1
		p = myQueue.get()
		# p.print()
		# print(p.state)
		# print("\n")
		# if k > 7:
		# 	return "HHHHHH"

		if p.hCost == 0:

			if p.toString() != targetStr:
				print("Wrong String!!!")
			# print(targetArray)
			# print(p.state)
			# print(p.toString())
			# print(targetStr)
			return p.path

		if (p.blanki != 0 and p.state[p.blanki-1][p.blankj]!=-1 and (p.path == "" or p.path[len(p.path)-1]!='D')):
			#  if(k == 1):cout << "In U" << endl
			pu = Node(0,0,0)
			pu.setFromNode(p)

			dirPos = 0
			temp = pu.state[pu.blanki - 1][pu.blankj]
			dirPos = getManhattanDistance(targetPos[temp].i,targetPos[temp].j,pu.blanki,pu.blankj) -  getManhattanDistance(targetPos[temp].i,targetPos[temp].j,pu.blanki-1,pu.blankj)

			pu.hCost +=dirPos
			pu.fCost += dirPos

			pu.state[pu.blanki - 1][pu.blankj] = pu.state[pu.blanki][pu.blankj]
			pu.state[pu.blanki][pu.blankj] = temp

			pu.blanki-=1
			pu.path = pu.path + "U"
			

			if pu.toString() in mySet:
				dCount+=1
				del pu
			else:
				myQueue.put(pu)
				mySet.add(pu.toString())
		# Down
		if (p.blanki != 4 and p.state[p.blanki+1][p.blankj]!=-1 and (p.path == "" or p.path[len(p.path)-1]!='U')):
			#  if(k == 1)cout << "In D" << endl

			pd = Node(0,0,0)
			pd.setFromNode(p)

			dirPos =0
			temp = pd.state[pd.blanki + 1][pd.blankj]
			#  dirPos = abs(targetPos[temp].i - pd.blanki)-abs(targetPos[temp].i - (pd.blanki+1))
			dirPos = getManhattanDistance(targetPos[temp].i,targetPos[temp].j,pd.blanki,pd.blankj) -  getManhattanDistance(targetPos[temp].i,targetPos[temp].j,pd.blanki+1,pd.blankj)


			pd.hCost +=dirPos
			pd.fCost += dirPos

			pd.state[pd.blanki + 1][pd.blankj] = pd.state[pd.blanki][pd.blankj]
			pd.state[pd.blanki][pd.blankj] = temp

			pd.blanki+=1
			pd.path = pd.path + "D"

			if pd.toString() in mySet:
				dCount+=1
				del pd
			else:
				myQueue.put(pd)	
				mySet.add(pd.toString())
		# Left
		if (p.blankj != 0 and (p.path == "" or p.path[len(p.path)-1]!='R')):
			#  if(k == 1)cout << "In L" << endl
			pl = Node(0,0,0)
			pl.setFromNode(p)
			h = 1
			if (p.state[p.blanki][p.blankj - 1] != -1):
				h = 1
			else:
				h = 2

			dirPos = 0
			temp = pl.state[pl.blanki][pl.blankj - h]
			#  dirPos = abs(targetPos[temp].j - pl.blankj)-abs(targetPos[temp].j - (pl.blankj-h))
			dirPos = getManhattanDistance(targetPos[temp].i,targetPos[temp].j,pl.blanki,pl.blankj) -  getManhattanDistance(targetPos[temp].i,targetPos[temp].j,pl.blanki,pl.blankj-h)



			pl.hCost +=dirPos
			pl.fCost += dirPos

			pl.state[pl.blanki][pl.blankj - h] = pl.state[pl.blanki][pl.blankj]
			pl.state[pl.blanki][pl.blankj] = temp
			pl.blankj-=h
			
			pl.path = pl.path + "L"

			if pl.toString() in mySet:
				dCount+=1
				del pl
			else:
				myQueue.put(pl)	
				mySet.add(pl.toString())
		# Right
		if (p.blankj != 4 and (p.path == "" or p.path[len(p.path)-1]!='L')):
			#  if(k == 1)cout << "In R" << endl
			pr = Node(0,0,0)
			# print(pr.state)
			pr.setFromNode(p)
			# print(p.state)
			# print(pr.state)
			h = 1
			if (p.state[p.blanki][p.blankj + 1] != -1):
				h = 1
			else:
				h = 2

			dirPos = 0
			temp = pr.state[pr.blanki][pr.blankj + h]
			#  dirPos = abs(targetPos[temp].j - pr.blankj)-abs(targetPos[temp].j - (pr.blankj+h))
			dirPos = getManhattanDistance(targetPos[temp].i,targetPos[temp].j,pr.blanki,pr.blankj) -  getManhattanDistance(targetPos[temp].i,targetPos[temp].j,pr.blanki,pr.blankj+h)

			pr.hCost +=dirPos
			pr.fCost += dirPos

			pr.state[pr.blanki][pr.blankj + h] = pr.state[pr.blanki][pr.blankj]
			pr.state[pr.blanki][pr.blankj] = temp
			pr.blankj+=h

			pr.path =  pr.path + "R"

			#  string strTemp = pr.toString()
			if pr.toString() in mySet:
				dCount+=1
				del pr
			else:
				myQueue.put(pr)	
				mySet.add(pr.toString())
			# timeEnd = clock()
			# t1 += timeEnd-timeStart
		del p
	return "None"


ifp1 = open("input.txt")
ifp2 = open("target.txt")

def main():
	targetInit()

	ofp1 = open("output.txt")

	path = ""

	for i in range(10):
		inputInit()
		ts = time.time()
		path = puzzleSolution()
		te = time.time()

		print(len(path))
		print("Cost time " + str(te-ts))

if __name__ == '__main__':
	main()

