#ifndef DCPLOT_H_
#define DCPLOT_H_

#include "stdafx.h"
#include <list>
#include <algorithm>
using namespace std;

///<summary>Manages plots of directed closures of PNGraphs</summary>
class DCPlot{
	TGnuPlot* m_gp;
public:
	DCPlot(TStr filename, TStr title);

	TGnuPlot get();

	///<summary>Plots integer points on a graph. If the list contains {3,6,9}, the points (0,3) (1,6) and (2,9) are plotted.</summary>
	///<param name="columnWidth">The width of the sliding window to use.</param>
	void plotLineFromList(TIntV ilist, TInt columnWidth = 100);
	
	///<summary>Plots lists of integer points on a graph.</summary>
	///<param name="columnWidth">The width of the sliding window to use.</param>
	///<param name="withAverage">Whether to plot a mean line.</param>
	void plotLinesFromList(TVec<TIntV> vilist, TInt columnWidth = 100, bool withAverage = false);

	void savePNG(TStr xLabel, TStr yLabel);

	~DCPlot();
};

#endif