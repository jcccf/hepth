#ifndef DCNPatent_H_
#define DCNPatent_H_

#include "stdafx.h"

class DCNodePatent{
	TNGraph::TNodeI m_node;
public:
	DCNodePatent(TNGraph::TNodeI n);
	friend bool operator== (DCNodePatent n1, DCNodePatent n2);
	friend bool operator!= (DCNodePatent n1, DCNodePatent n2);
	friend bool operator< (DCNodePatent n1, DCNodePatent n2);
	friend bool operator> (DCNodePatent n1, DCNodePatent n2);
	friend bool operator<= (DCNodePatent n1, DCNodePatent n2);
	friend bool operator>= (DCNodePatent n1, DCNodePatent n2);
	TNGraph::TNodeI get();
	~DCNodePatent();
};

#endif