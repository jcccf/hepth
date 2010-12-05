// DCGraph
#include "stdafx.h"
#include "DCGraph.h"
using namespace std;

DCGraph::DCGraph(PNGraph g){
	m_graph = g;
	m_list = new list<TNGraph::TNodeI>;
	for(TNGraph::TNodeI n = m_graph->BegNI(); n != m_graph->EndNI(); n++){
		m_list->push_back(n);
	}
}

DCGraph::DCGraph(PNGraph g, bool (*filter)(TNGraph::TNodeI i)){
	m_graph = g;
	m_list = new list<TNGraph::TNodeI>;
	for(TNGraph::TNodeI n = m_graph->BegNI(); n != m_graph->EndNI(); n++){
		if(filter(n))
			m_list->push_back(n);
	}
}

DCGraph::DCGraph(PNGraph g, int minInDegree, int maxInDegree){
	m_graph = g;
	m_list = new list<TNGraph::TNodeI>;
	for(TNGraph::TNodeI n = m_graph->BegNI(); n != m_graph->EndNI(); n++){
		if(n.GetInDeg() >= minInDegree && n.GetInDeg() <= maxInDegree)
			m_list->push_back(n);
	}
}

DCGraph::~DCGraph(){
	delete m_list;
}

list<TNGraph::TNodeI> DCGraph::getList(){
	return *m_list;
}

int DCGraph::getListLength(){
	return m_list->size();
}

void DCGraph::printListInfo(){
	printf("===\n");
	for(list<TNGraph::TNodeI>::iterator n = m_list->begin(); n != m_list->end(); n++){
		printf("Node %d has %d citations\n", (*n).GetId(), (*n).GetInDeg());
	}
	printf("===\n");
}