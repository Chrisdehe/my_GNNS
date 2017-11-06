# author: Wyb
# date: 2017/10/22

import math
import numpy as np
import random
import operator


k=3;K=4;E=3;D=2;N=10;R=5;T=5	
'''
k = 3 	a 3-NN graph
K = 4 	pick the first 4 elements as the result
E = 3 	each step returns the first 3 neighbors of query node in the graph
D = 2 	2 dimension graph
N = 10 	10 points in total
R = 5 	5 random restarts
T = 5 	5 greedy steps
'''

vertex=np.array([[0,0],[2,0],[3,1],[0,4],[-3,0],[1,1],[-1,1],[-2,3],[-3,3],[0,-3]])		#10 input points

# distance(a,b): compute the Euclidean distace of two points a and b
# Input: point a and b
# Return: distance
def distance(a,b):		
	count=0
	for i in range(D):
		count+=(a[i]-b[i])**2
	return math.sqrt(count)
    
# graph_construct(): construct a k-NN graph
# Input: void
# Return: k-NN graph
def graph_construct():		
	graph={}		#save the graph in a dictionary
	for i in range(N):
		dis=[]
		for j in range(N):
			dis.append(distance(vertex[i],vertex[j]))
		dis=np.array(dis)
		sequence=np.argsort(dis)[1:k+1]		#sort the dictionary according to values
		graph[i]=sequence
	return graph

# N_YEG(y, E, G): returns the first E neighbors of query point y in G
# Input: query point: y; the number of neighbors: E; the graph: G
# Return: the first E neighbors of query point y in G as a dictionary
def N_YEG(y, E, G):
	return G[y][:E]


def GNNS(G, Q):  
	S=set()		#save S in a set to avoid the duplication
	U={}		#save U in a dictionary
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

# main
G=graph_construct()
Q=[0,4]
U=GNNS(G, Q)
print U[:K]
























	








    
    





