#include "stdafx.h"
#include <list>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "Shadow.h"
using namespace std;

void createShadowOfPaper(PNGraph g, TInt paperId){
	int numPapers = 0;
	list<TNGraph::TNodeI> hiList;

	// Get the appropriate paper
	TNGraph::TNodeI paper = g->GetNI(paperId);

	//Iterate through nodes and get those with InMiNCit <= citations <= InMaxCit
	printf("Building node list...");
	for(TNGraph::TNodeI n = g->BegNI(); n != g->EndNI(); n++){
		if(orderByDate(paper, n)){
			//printf("%d\n", n.GetId());
			hiList.push_back(n);
			numPapers++;
		}
	}
	printf("Done.\n");
	printf("From paper %d, %d papers came after it\n", paperId, numPapers);

	// Sort List
	hiList.sort(orderByDate);

	//Create graph object
	TGnuPlot gp(TStr::Fmt("sh_%d_%d_%d", paperId, paper.GetInDeg(), time(NULL)), TStr::Fmt("Shadow ratio for paper %d (%d direct citations)", paperId, paper.GetInDeg()));

	// Iterate through the list, finding nodes that match
	list<int> cumulativeMatches;
	int currentMatches = 0;
	list<int> shadowListAccum;
	shadowListAccum.push_back(paperId);
	for(list<TNGraph::TNodeI>::iterator i = hiList.begin(); i != hiList.end(); i++){
		for(int e = 0; e < (*i).GetOutDeg(); e++){
			if(find(shadowListAccum.begin(), shadowListAccum.end(), (*i).GetOutNId(e)) != shadowListAccum.end()){
				currentMatches++;
				shadowListAccum.push_back((*i).GetId());
				break;
			}
		}
		cumulativeMatches.push_back(currentMatches);
		//printf("Current Matches is %d\n", currentMatches);
	}

	// Plot the graph
	printf("Plotting graph...\n");
	TFltPrV pv;
	int counter = 1;
	for(list<int>::iterator i = cumulativeMatches.begin(); i != cumulativeMatches.end(); i++){
		pv.Add(TFltPr((counter-0.0),((*i)-0.0)/counter));
		counter++;
	}
	gp.AddPlot(pv, gpwLines, TStr::Fmt("%d", paperId).CStr());
	gp.SetXYLabel("No. Papers Since", "Shadow ratio");
	gp.SavePng();
}