/*This implementation of the Dijkstra's shortest path algorithm is based on maintaining 2 sets of values. The 1st set 
of values are stored in objects of class "table", which store the total
 edges, with information about starting node, ending node, distance, status(whether the edge has already been considered or not), 
 and relevance(whether the edge we have to consider is relevant yet or not).
 From this list, we find the shortest edge which is relevant, and then add the end-node and distance to our 2nd set of values, the "c_set" object values.
 The "c-set" contains node and distance parameters, and stores the final shortest distance to any given node from the initially entered start-node.
 Once the end-node gets added to the "c_set" objects, that distance value can be considered as the shortest path from start-node to end-node
*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>


using namespace :: std;

//This is the class used to make the edge list graph. n1 is the start node, n2 is the end node, d is for distance, and rel is used to indicate the relevance of an edge
class table
{
	public:
		table():status(0), d(0), rel(0){}
		int n1;
		int n2;
		double d;
		int rel;
		int status;
};

//This class is used to store the closed set values
class c_set
{
	public:
		c_set():distance(0), node(0){}
		double distance;
		int node;
};

//This class is used for graphs
class graph
{
	public:
	int max=1225;
	double weight=0;
	double dens_r=rand()%11;
	double density=(30+dens_r)/100;
	int edges=round(max*density);
	table t[540];
	int current=0;
	//This function is used to generate a random graph, and is seeded from time to create a "truly random" graph 
	void make_graph()
	{
		for(int i=0 ; i<edges ; i++)
		{	
			int temp=i; 
			int n1=(rand()%50)+1;
			int n2=(rand()%50)+1;
			int weight_base=rand()%100;
			weight=(1+weight_base)/10.0;
			for(int j=0 ; j<=current ; j++)
				if((n1==t[j].n1 && n2==t[j].n2) || (n1==t[j].n2 && n2==t[j].n1) || (n1==n2))
				{
					i--;
					break;
				}
			if(temp==i)
			{
				t[current].n1=n1;
				t[current].n2=n2;
				t[current].d=weight;
				current++;
			}
		}
	}
	
	//This function prints the graph in edge list format. Uncomment the function to print the graph
	/*void print_graph()
	{
		cout<<"Start node\t"<<"End node\t"<<"Distance\t";
		for(int i=0 ; i<edges ; i++)
		{
			cout<<"\n"<<t[i].n1<<"\t\t"<<t[i].n2<<"\t\t"<<t[i].d;
		}
	}*/
};

//This function is used to swap the n1 and n2 values from objects of the class "table" 
void invert(table t[], int i)
{
	int temp=t[i].n1;
	t[i].n1=t[i].n2;
	t[i].n2=temp;
}

//This is the function that is used to pick the shortest edge from objects of class "table"
//This function takes into account the relevancy of the edges and finds out the shortest edge which has rel value set to 1
int shortest_path(int start, table t[], int edges)
{
	int small;
	for(int j=0 ; j<edges ; j++)
	{
		if(t[j].status==0 && t[j].rel==1)
		{	
			small=j;
			break;
		}
	}
	for(int i=0 ; i<edges ; i++)
		if(t[small].d>t[i].d && t[i].status==0 && t[i].rel==1)
			small=i;
	return small;
}

//This function is used to add the distance values of the starting node to the distance values of objects of class "table"
//This function essentially modifies the graph edge distance values into "real" distance values
void finalise(c_set closed_set[], int start, int edges, table t[])
{
	for(int i=0 ; i<edges ; i++)
	{
		if(t[i].n1==start && t[i].status==0)
		{	
			t[i].d+=closed_set[start-1].distance;
			t[i].rel=1;
		}
	}
}

//This function is used to check whether a particular node has already been added to the closed set or not
//This helps us decide whether the shortest path to a node has already been found or not
bool in_c_set(c_set closed_set[], int end)
{
	for(int i=0 ; i<50 ; i++)
	{
		if(closed_set[i].node==end)
			return true;
	}
	return false;
}

//This is the main function which accomplishes the addition of graph edge values to the closed set
//It keeps running recursively until the end node value doesnt get added to the closed set
void confirm(graph g1, int start, table t[], c_set closed_set[], int end)
{
	int done=0;
	int cont;
	for(int i=0 ; i<g1.edges; i++)
	{
		if(t[i].n2==start && t[i].status==0)
		{
			invert(t, i);
		}
	}	
	finalise(closed_set, start, g1.edges, t);
	int small=shortest_path(start, t, g1.edges);
	if(!in_c_set(closed_set, t[small].n2))
	{	
		closed_set[t[small].n2-1].distance=t[small].d;
		closed_set[t[small].n2-1].node=t[small].n2;
	}
	t[small].status=1;
	if(closed_set[end-1].distance!=0)
		done=1;
	if(small==g1.edges)
		cout<<"The start node is not connected to the end node in any way.";
	if(done==0 && small!=g1.edges)
		confirm(g1, t[small].n2, t, closed_set, end);
}

int main()
{
	srand(time(0));
	cout<<"Welcome to the Monte Carlo simulation.\n Here we are going to try to find the average shortest path in a 50 node graph with edge density ranging from 30 to 40 percent.\nWe are gonna do this by geneerating a random graph each time and try to find the distance from node 1 to node 2, then from node 1 to node 3, and so on.\nAfter all those distances are found out, we will simply find the mean of all the distance values.";
	int start=1, count=0;//count is the value in which we will go on storing the total distance of the nodes
	double total=49.00;
	for(int i=2 ; i<=50 ; i++)	
	{	
		graph g;
		g.make_graph();
		c_set closed_set[50];
		int end=i;
		closed_set[start-1].node=start;
		closed_set[start-1].distance=0;
		confirm(g, start, g.t, closed_set, end);
		//To see the closed set, uncomment the below comment.
		/*cout<<endl<<"Closed set"<<endl;
		for(int j=0 ; j<50 ; j++)
			cout<<closed_set[j].node<<"\t"<<closed_set[j].distance<<endl;
		cout<<"\n\n"<<closed_set[end-1].distance;*/
		count+=closed_set[end-1].distance;
		if(closed_set[end-1].distance==0)
			total-=1;
	}
	cout<<"\n\nAverage distance is "<<count/total<<endl;
}