// DCNodePatent
#include "stdafx.h"
#include "DCNodePatent.h"
using namespace std;

DCNodePatent::DCNodePatent(TNGraph::TNodeI n){
	m_node = n;
}

DCNodePatent::~DCNodePatent(){
	//Do nothing
}

TNGraph::TNodeI DCNodePatent::get(){
	return m_node;
}

bool operator< (DCNodePatent n1, DCNodePatent n2){
	return n1.m_node.GetId() < n2.m_node.GetId();
}

bool operator> (DCNodePatent n1, DCNodePatent n2){
	return n1.m_node.GetId() > n2.m_node.GetId();
}

bool operator== (DCNodePatent n1, DCNodePatent n2){
	return n1.m_node.GetId() == n2.m_node.GetId();
}

bool operator<= (DCNodePatent n1, DCNodePatent n2){
	return n1.m_node.GetId() <= n2.m_node.GetId();
}

bool operator>= (DCNodePatent n1, DCNodePatent n2){
	return n1.m_node.GetId() >= n2.m_node.GetId();
}