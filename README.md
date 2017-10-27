# my_GNNS
This is a **CPU** version of **GNNS** algorithm implemented in **python**, with [Fast Approximate Nearest-Neighbor Search with k-Nearest Neighbor 
Graph](https://www.researchgate.net/profile/Kiana_Hajebi/publication/220811886_Fast_Approximate_Nearest-Neighbor_Search_with_k-Nearest_Neighbor_Graph/links/5440ec780cf251bced614997/Fast-Approximate-Nearest-Neighbor-Search-with-k-Nearest-Neighbor-Graph.pdf) for reference.

The example input of this demo are 10 points, and parameters are as follows: 

| parameter | implication |
|:-------------:|:-------------:|
| k = 3 |  a 3-NN graph |
| K = 4 |  pick the first 4 elements as the result |
| E = 3 |  each step returns the first 3 neighbors of query node in the graph |
| D = 2 |  2 dimension graph |
| N = 10| 10 points in total |
| R = 5 |  5 random restarts |
| T = 5 |  5 greedy steps |
