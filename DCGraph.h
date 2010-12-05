#ifndef DCGRAPH_H_
#define DCGRAPH_H_

#include "stdafx.h"
#include <list>
#include <algorithm>
using namespace std;

///<summary>Manages directed closures of PNGraphs</summary>
class DCGraph{
	PNGraph m_graph;
	list<TNGraph::TNodeI>* m_list;

public:
	DCGraph(PNGraph graph);
	DCGraph(PNGraph graph, bool (*filter)(TNGraph::TNodeI i));
	DCGraph(PNGraph graph, int minInDegree, int maxInDegree);
	
	///<summary>Gets a list of nodes of this graph</summary>
	list<TNGraph::TNodeI> getList();
	
	///<summary>Prints the nodes of this graph and their indegrees</summary>
	void printListInfo();

	///<summary>Gets the number of nodes of this graph</summary>
	int getListLength();
	
	~DCGraph();
};

#endif