#include <iostream> 
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
using namespace std;

typedef struct ITEM	//the element of heap B[]
{
	int index;		//the index of the vertex
	double dis;		//distance
}item;
//
//int K=10,D=3,N=11;
int K=10, D, N=100000;
double **vertex;
//double vertex[][5]={ {0,0,1,3,8}, {2,0,1,3,8}, {3,1,3,8,1}, {0,3,8,4,1}, {-3,0,3,8,1}, {1,1,2,3,8}, {-1,3,8,1,2}, {-2,1,3,83,2}, {-3,3,3,8,2}, {0,-3,3,8,2}, {0,3,8,100, 1000}};
int *R_size;
int *BR_size;
item **B;
item **R;
item **BR;

void read_file(void);
double distance(double vertex_a[], double vertex_b[]);
item **initialize_list(void);
item **reverse_list(item **B, int *R_size);
item **union_list(item **B, item **R, int *BR_size, int *R_size);
int updateNN(item *Bi, item ni);
int cmp(const void *a, const void *b);

void debug_1(item **B, item **R, item **BR);
void printB(item **B);
void printR(item **R);
void printBR(item **BR);

ofstream fout("output.txt");
//ifstream fin("sift\sift_query.fvecs", ios::binary); 
//D:\\Laboratory\\code\\sift\\sift_query.fvecs
void read_file()
{
	FILE *fp;
	float temp=0;
	if(!(fp=fopen("D:\\Laboratory\\code\\sift\\sift_base.fvecs", "rb")))
		cout<<"open input file error!"<<endl;
	
	fread(&D, 4, 1, fp);	
	vertex=(double **)malloc(N*sizeof(double*));
	for(int i=0; i<N; i++)
		vertex[i]=(double *)malloc(D*sizeof(double));

	fseek(fp, 0, SEEK_SET);	
	for(int i=0; i<N; i++)
	{
		fseek(fp, 4, SEEK_CUR);
		for(int j=0; j<D; j++)
		{		
			fread(&temp, 4, 1, fp);
			vertex[i][j]=temp;
		}
	}	 
	fclose(fp);		
}

int main(void)
{
	clock_t read_start, read_finish, compute_start, compute_finish, write_start, write_finish;
	read_start=clock();
	 
	if(!fout.is_open())
	{
		cout<<"open output file error!"<<endl;
	} 
	
	read_file();
	read_finish=clock();
	cout<<"read time is: "<<(double)(read_finish - read_start) / CLOCKS_PER_SEC<<endl;
	
	compute_start=clock();
	
	B=initialize_list();
	int count=0;		//count the number of update times 
	item ni;
	R_size=(int *)malloc(N*sizeof(int));
	BR_size=(int *)malloc(N*sizeof(int));
	
	while(1)
	{
		R=reverse_list(B, R_size);
		BR=union_list(B, R, BR_size, R_size);
		count=0;
		for(int i=0; i<N; i++)			//item v in V
			for(int j=0; j<BR_size[i]; j++)	//item u1 in BR
			{
				int u1=BR[i][j].index;
				for(int m=0; m<BR_size[u1]; m++)//item u2 in BR
				{
					double l=distance(vertex[i], vertex[BR[u1][m].index]);
					if(l==0)
						continue;
					ni.index=BR[u1][m].index;
					ni.dis=l;
					int temp=updateNN(B[i], ni);
					count=count+temp;
				}
			}
		cout<<count<<endl;
		if(count==0)
			break;
		
	} 
	compute_finish=clock();
	cout<<"compute time is: "<<(double)(compute_finish - compute_start) / CLOCKS_PER_SEC<<endl;
	
	write_start=clock();
	printB(B);
	fout.close();
	write_finish=clock();
	cout<<"write time is: "<<(double)(write_finish - write_start) / CLOCKS_PER_SEC<<endl;
	return 0;
}

//calculate the square Euclidean distance between two points
double distance(double vertex_a[], double vertex_b[])
{
	double dis=0;
	for(int i=0; i<D; i++)
		dis+=(vertex_a[i]-vertex_b[i])*(vertex_a[i]-vertex_b[i]);
		
	return sqrt(dis);
}

//initialize the list B and fill with random value
item **initialize_list(void)
{
	int flag=0;
	item **B=(item **)malloc(N*sizeof(item *));
	for(int i=0; i<N; i++)
		B[i]=(item *)malloc(K*sizeof(item));
		
	for(int i=0; i<N; i++)
	{
		for(int j=0; j<K; j++)
		{
			flag=0;
			int tmp=rand()%N;
			for(int m=0; m<j; m++)
				if(B[i][m].index==tmp)
					flag=1;
			if(tmp==i | flag==1)
			{
				j--;
				continue;
			}

			B[i][j].index=tmp;
			B[i][j].dis=distance(vertex[i], vertex[tmp]);
		}
		qsort(B[i], K, sizeof(item), cmp);
	}
	return B;
}

//Reverse function in the algorithm
item **reverse_list(item **B, int *R_size)
{
	if(R!=NULL)
		free(R);
	R=(item **)malloc(N*sizeof(item *));
	for(int i=0; i<N; i++)
		R[i]=(item *)malloc(20*K*sizeof(item));  //10 need to be modified
		
	for(int i=0; i<N; i++)
		R_size[i]=0;

	for(int i=0; i<N; i++)
		for(int j=0; j<K; j++)
		{
			R[B[i][j].index][R_size[B[i][j].index]].index=i;
			R[B[i][j].index][R_size[B[i][j].index]++].dis=B[i][j].dis;
		}	
	return R;
}
item **union_list(item **B, item **R, int *BR_size, int *R_size)
{
	if(BR!=NULL)
	{
		free(BR);
	}
		
	BR=(item **)malloc(N*sizeof(item *));
	for(int i=0; i<N; i++)
		BR[i]=(item *)malloc(21*K*sizeof(item));
	int flag=0;
	for(int i=0; i<N; i++)
		BR_size[i]=0;	

	for(int i=0; i<N; i++)
	{
		for(int j=0; j<K; j++)		//check B[]
		{
			flag=0;
			for(int m=0; m<BR_size[i]; m++)
			{
				if(B[i][j].index==BR[i][m].index)
					flag=1;
			}
			if(flag==0)
			{				
				BR[i][BR_size[i]].index=B[i][j].index;	
				BR[i][BR_size[i]++].dis=B[i][j].dis;		
			}
		}
		for(int j=0; j<R_size[i]; j++)		//check R[]
		{
			flag=0;
			for(int m=0; m<BR_size[i]; m++)
				if(R[i][j].index==BR[i][m].index)
					flag=1;
			if(flag==0)
			{	
				BR[i][BR_size[i]].index=R[i][j].index;
				BR[i][BR_size[i]++].dis=R[i][j].dis;				
			}
		}
	}
	
	return BR;
}

int updateNN(item *Bi, item ni)
{
	for(int i=0; i<K; i++)
		if(ni.index==Bi[i].index)
			return 0;
	for(int i=0; i<K; i++)
	{
		if(ni.dis>=Bi[i-1].dis && ni.dis<Bi[i].dis)
		{
			for(int j=K-1; j>i; j--)
			{
				Bi[j].index=Bi[j-1].index;
				Bi[j].dis=Bi[j-1].dis;
			}
			Bi[i].index=ni.index;
			Bi[i].dis=ni.dis;	
			return 1;
		}
	}
	return 0;
}

int cmp(const void *a, const void *b)
{
	if(((item *)a)->dis-((item *)b)->dis>0)
		return 1;
	else if(((item *)a)->dis-((item *)b)->dis==0)
		return 0;
	else 
		return -1;
}

void debug_1(item **B, item **R, item **BR)
{
	fout<<"debug start: "<<endl;
	printB(B);
	printR(R);
	printBR(BR);
	fout<<"debug finish: "<<endl;
}

void printB(item **B)
{
	fout<<"List B: "<<endl;
	for(int i=0; i<N; i++)
	{
		fout<<"		i= "<<i<<endl;
		for(int j=0; j<K; j++)
			fout<<B[i][j].index<<"  "<<B[i][j].dis<<endl;
	}
		
	fout<<endl;
}

void printR(item **R)
{
	fout<<"List R: "<<endl;
	for(int i=0; i<N; i++)
	{
		fout<<"		i= "<<i<<endl;
		for(int j=0; j<R_size[i]; j++)
			fout<<R[i][j].index<<"  "<<R[i][j].dis<<endl;
	}
		
	fout<<endl;
}

void printBR(item **BR)
{
	fout<<"List BR: "<<endl;
	for(int i=0; i<N; i++)
	{
		fout<<"		i= "<<i<<endl;
		for(int j=0; j<BR_size[i]; j++)
			fout<<BR[i][j].index<<"  "<<BR[i][j].dis<<endl;
	}
	
	fout<<endl;
}

