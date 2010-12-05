#include "stdafx.h"
#include <list>
#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;

bool orderByCitation (TNGraph::TNodeI n1, TNGraph::TNodeI n2){
	if(n1.GetInDeg() < n2.GetInDeg())
		return true;
	return false;
}

bool orderByDatePatents(TNGraph::TNodeI n1, TNGraph::TNodeI n2){
	if(n1.GetId() < n2.GetId())
		return true;
	return false;
}

bool orderByDate (TNGraph::TNodeI n1, TNGraph::TNodeI n2){
	int d1 = n1.GetId();
	int d2 = n2.GetId();
	int y1 = d1 / 100000;
	int r1 = d1 - y1 * 100000;
	int y2 = d2 / 100000;
	int r2 = d2 - y2 * 100000;

	//Convert to year
	y1 = (y1 >= 30) ? y1 + 1900: y1 + 2000;
	y2 = (y2 >= 30) ? y2 + 1900: y2 + 2000;

	if(y1 < y2)
		return true;
	else if(y1 > y2)
		return false;
	else
		if(r1 < r2)
			return true;
		else
			return false;
}