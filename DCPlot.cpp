// DCPlot
#include "stdafx.h"
#include "DCPlot.h"
#include <list>
#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;

DCPlot::DCPlot(TStr filename, TStr title){
	m_gp = new TGnuPlot(filename, title);
}

TGnuPlot DCPlot::get(){
	return *m_gp;
}

void DCPlot::plotLineFromList(TIntV ilist, TInt columnWidth){
	TFltPrV pv;
	int count = 0;
	int sum = 0;
	list<TInt> window;
	for(TIntV::TIter i = ilist.BegI(); i != ilist.EndI(); i++){
		window.push_back(*i);
		sum += *i;
		if(count > columnWidth){
			TFlt x = count - columnWidth/2;
			TFlt y = (sum-0.0)/columnWidth;
			pv.Add(TFltPr(x,y));
			sum -= window.front();
			window.pop_front();
		}
		count++;
	}
	m_gp->AddPlot(pv, gpwLines, "");
}

void DCPlot::plotLinesFromList(TVec<TIntV> vilist, TInt columnWidth, bool withAverage){
	if(!withAverage){
		for(TVec<TIntV>::TIter x = vilist.BegI(); x != vilist.EndI(); x++){
			plotLineFromList(*x, columnWidth);
		}
	}
	else{
		int numElements = vilist.GetVal(0).Len();
		int numLines = vilist.Len();
		//Init Average Line
		TFltV avgLine(numElements);
		for(TFltV::TIter a = avgLine.BegI(); a != avgLine.EndI(); a++)
			*a = 0.0;
		//Plot Other Lines
		for(TVec<TIntV>::TIter j = vilist.BegI(); j != vilist.EndI(); j++){
			TFltV::TIter avg = avgLine.BegI();
			TFltPrV pv;
			int count = 0;
			int sum = 0;
			list<TInt> window;
			for(TIntV::TIter i = (*j).BegI(); i != (*j).EndI(); i++){
				window.push_back(*i);
				sum += *i;
				if(count > columnWidth){
					TFlt x = count - columnWidth/2;
					TFlt y = (sum-0.0)/columnWidth;
					if(count < numElements)
						*avg += y;
					pv.Add(TFltPr(x,y));
					sum -= window.front();
					window.pop_front();
				}
				count++;
				avg++;
			}
			m_gp->AddPlot(pv, gpwLines, "");
		}
		//Plot Average Line
		TFltPrV pv;
		TFlt i = 0.0;
		for(TFltV::TIter a = avgLine.BegI(); a != avgLine.EndI(); a++){
			if(i > columnWidth)
				pv.Add(TFltPr(i - columnWidth/2,*a/numLines));
			i += 1.0;
		}
		TInt p = m_gp->AddPlot(pv, gpwLines, "Mean Cumulative");
		m_gp->SetLineStyle(p, "lt 6 lw 3");
	}
}

void DCPlot::savePNG(TStr xLabel, TStr yLabel){
	m_gp->SetXYLabel(xLabel, yLabel);
	m_gp->SavePng();
}

DCPlot::~DCPlot(){
	delete m_gp;
}