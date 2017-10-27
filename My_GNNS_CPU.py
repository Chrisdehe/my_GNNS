import math
import numpy as np
import random
import operator
#100 points, 3dimensions

k=3;K=2;E=3;D=2;N=10;R=5;T=5	

vertex=np.array([[0,0],[2,0],[3,1],[0,4],[-3,0],[1,1],[-1,1],[-2,3],[-3,3],[0,-3]])

def distance(a,b):
	count=0
	for i in range(D):
		count+=(a[i]-b[i])**2
	return math.sqrt(count)
    

def graph_construct():
	graph={}
	for i in range(N):
		dis=[]
		for j in range(N):
			dis.append(distance(vertex[i],vertex[j]))
		dis=np.array(dis)
		sequence=np.argsort(dis)[1:k+1]
		graph[i]=sequence
	return graph

def N_YEG(y, E, G):
	return G[y][:E]


def GNNS(G, Q):  
	S=set()
	U={}
	for r in range(R):
		Y=[]
		Y.append(random.randint(0, N-1))
		for t in range(T):
			neighbor=N_YEG(Y[t], E, G)
			min=10000
			for i in range(E):
				if (distance(vertex[neighbor[i]],Q)<min):
					min=distance(vertex[neighbor[i]],Q)
					Y.append(neighbor[i])
			S.union(set(neighbor))
			for x in neighbor:
				U[x]=distance(vertex[x], Q)
	U=sorted(U.items(),key=operator.itemgetter(1))
	return U

G=graph_construct()
Q=[0,4]
U=GNNS(G, Q)
print U
























	








    
    





