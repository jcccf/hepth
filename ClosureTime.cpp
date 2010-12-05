#include "stdafx.h"
#include <list>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "ClosureTime.h"
#include "DCGraph.h"
#include "DCPlot.h"
#include "DCNodePatent.h"
using namespace std;

	void createCitationsAgainstTimeCumulative(PNGraph g, TInt InMinCit, TInt InMaxCit){
		DCGraph dcg(g, InMinCit, InMaxCit);
		list<TNGraph::TNodeI> hiList = dcg.getList();
		int manyCitations = dcg.getListLength();
		printf("%d papers with more than %d citations\n", manyCitations, InMinCit);
		hiList.sort(orderByCitation);
		dcg.printListInfo();

		TGnuPlot gp(TStr::Fmt("ci_%04d_%04d_%d", InMinCit, InMaxCit, time(NULL)), TStr::Fmt("Average Citations against Time (Cumulative) (%d < x < %d Citations)", InMinCit,InMaxCit));

		TVec<TFlt> avg_mean(InMinCit);
		TVec<TIntV> avg_inlinks(manyCitations);
		TVec<TInt> n_citations(manyCitations);

		TVec<TIntV>::TIter c = avg_inlinks.BegI();
		TVec<TInt>::TIter c_citations = n_citations.BegI();
		for(list<TNGraph::TNodeI>::iterator n = hiList.begin(); n != hiList.end(); n++){
			printf("For Paper %07d...", (*n).GetId());

			(*c_citations) = (*n).GetInDeg();
			c_citations++;

			(*c) = TIntV((*n).GetInDeg());
			for(TIntV::TIter j = (*c).BegI(); j != (*c).EndI(); j++){
				(*j) = 0;
			}

			int curr_total = 0;
			int matches = 0;
			list<TNGraph::TNodeI> nodeList;

			//Traverse the edges and create a collection of InNodes
			for(int e = 0;e<(*n).GetInDeg();e++){
				TNGraph::TNodeI currNode = g->GetNI((*n).GetInNId(e));
				nodeList.push_back(currNode);
			}
			nodeList.sort(orderByDate);

			//Record number of in-links of each InNode
			TIntV::TIter d = (*c).BegI();
			for(list<TNGraph::TNodeI>::iterator n = nodeList.begin(); n!= nodeList.end(); n++){
				(*d) = (*n).GetInDeg();
				d++;
			}
			c++;
			printf("%4d out of %4d\n", matches, (*n).GetInDeg());
		}


		TIntV average_line(InMinCit);
		for(TIntV::TIter a = average_line.BegI(); a != average_line.EndI(); a++)
			(*a) = 0;
		//Get Number of Citations for Each Paper!
		for(TVec<TIntV>::TIter x = avg_inlinks.BegI(); x != avg_inlinks.EndI(); x++){
			// Plot each line
			TFltPrV pv;
			int cumulativeSum = 0;
			int i = 1;
			TIntV::TIter a = average_line.BegI();
			for(TIntV::TIter y = (*x).BegI(); y != (*x).EndI(); y++){
				if((a+1) != average_line.EndI()){
					(*a) += (*y);
					a++;
				}
				cumulativeSum += (*y);
				pv.Add(TFltPr((i-0.0), (cumulativeSum-0.0)/i));
				if(i == 1)
					gp.AddCmd(TStr::Fmt("set label \"%f\" at %d, %f", (cumulativeSum-0.0)/i, i, (cumulativeSum-0.0)/i).CStr());
				//printf("%f %f\n", (i-0.0), (cumulativeSum-0.0)/i);
				i++;
			}
			//system("PAUSE");
			gp.AddPlot(pv, gpwLines, "");
		}
		
		// Plot Mean Line
		TFltPrV pv;
		int i = 1;
		int cumulativeSum = 0;
		for(TIntV::TIter a = average_line.BegI(); a != average_line.EndI(); a++){
			cumulativeSum += (*a);
			pv.Add(TFltPr(i,(cumulativeSum-0.0)/(i*manyCitations)));
			i++;
		}
		TInt p = gp.AddPlot(pv, gpwLines, "Mean");
		gp.SetLineStyle(p, "lt 6 lw 3");

		gp.SetXYLabel("# of Citations over Time", "Average # of Citations of Citing Papers");
		gp.SavePng();
	}

	void createCitationsAgainstTimeSliding(PNGraph g, TInt ColumnWidth, TInt InMinCit, TInt InMaxCit){
		DCGraph dcg(g, InMinCit, InMaxCit);
		list<TNGraph::TNodeI> hiList = dcg.getList();
		int manyCitations = dcg.getListLength();
		printf("%d papers with more than %d citations\n", manyCitations, InMinCit);
		hiList.sort(orderByCitation);
		dcg.printListInfo();

		TVec<TFlt> avg_mean(InMinCit);
		TVec<TIntV> avg_inlinks(manyCitations);
		TVec<TInt> n_citations(manyCitations);

		TVec<TIntV>::TIter c = avg_inlinks.BegI();
		TVec<TInt>::TIter c_citations = n_citations.BegI();
		for(list<TNGraph::TNodeI>::iterator n = hiList.begin(); n != hiList.end(); n++){
			printf("For Paper %07d...", (*n).GetId());

			(*c_citations) = (*n).GetInDeg();
			c_citations++;

			(*c) = TIntV((*n).GetInDeg());
			for(TIntV::TIter j = (*c).BegI(); j != (*c).EndI(); j++){
				(*j) = 0;
			}

			int curr_total = 0;
			int matches = 0;
			list<TNGraph::TNodeI> nodeList;

			//Traverse the edges and create a collection of InNodes
			for(int e = 0;e<(*n).GetInDeg();e++){
				TNGraph::TNodeI currNode = g->GetNI((*n).GetInNId(e));
				nodeList.push_back(currNode);
			}
			nodeList.sort(orderByDate);
			
			//Record number of in-links of each InNode
			TIntV::TIter d = (*c).BegI();
			for(list<TNGraph::TNodeI>::iterator n = nodeList.begin(); n!= nodeList.end(); n++){
				(*d) = (*n).GetInDeg();
				d++;
			}
			c++;
			printf("%4d out of %4d\n", matches, (*n).GetInDeg());
		}

		DCPlot dp(TStr::Fmt("ci_sl_%04d_%04d_%d", InMinCit, InMaxCit, time(NULL)), TStr::Fmt("Average Citations against Time (%d < x < %d Citations) (Sliding Window)", InMinCit,InMaxCit));
		dp.plotLinesFromList(avg_inlinks, ColumnWidth, true);
		dp.savePNG("# of Citations over Time", "Average # of Citations of Citing Papers");
	}

	void createClosureKsAgainstTime(PNGraph g, TInt MaxK, TInt InMinCit, TInt InMaxCit){
		const char* descString = "citations";
		TGnuPlot gp(TStr::Fmt("dc_k_%s_%04d_%d", descString, InMinCit, time(NULL)), TStr::Fmt("Closure ratio against %s (%d < x < %d Citations)",descString, InMinCit,InMaxCit));

		TFltV avg_mean(InMinCit);
		TVec<TIntV> avg_mean_k(MaxK+1);
		for(TVec<TIntV>::TIter i = avg_mean_k.BegI(); i != avg_mean_k.EndI(); i++){
			(*i) = TIntV(InMinCit+1);
			for(TIntV::TIter j = (*i).BegI(); j != (*i).EndI(); j++){
				(*j) = 0;
			}
		}

		//Iterate through nodes and get those with >100 citations
		int manyCitations = 0;
		list<TNGraph::TNodeI> hiList;
		for(TNGraph::TNodeI n = g->BegNI(); n != g->EndNI(); n++){
			if(n.GetInDeg() >= InMinCit && n.GetInDeg() <= InMaxCit){
				//printf("%d\n", n.GetId());
				hiList.push_back(n);
				manyCitations++;
			}
		}
		printf("%d papers with more than %d citations\n", manyCitations, InMinCit);

		//Sort the nodes
		hiList.sort(orderByCitation);
		//Print for fun
		for(list<TNGraph::TNodeI>::iterator n = hiList.begin(); n != hiList.end(); n++){
			printf("Paper %d with %d citations\n", (*n).GetId(), (*n).GetInDeg());
		}

		list<TNGraph::TNodeI> nodeList;
		list<int> nodeListAccum;
		int total;
		for(list<TNGraph::TNodeI>::iterator n = hiList.begin(); n != hiList.end(); n++){
			int avg_pointer = 0;
			printf("For Paper %07d...", (*n).GetId());

			total = (*n).GetInDeg();
			nodeList.clear();
			nodeListAccum.clear();

			//Traverse the edges and create a collection of InNodes
			for(int e = 0;e<(*n).GetInDeg();e++){
				TNGraph::TNodeI currNode = g->GetNI((*n).GetInNId(e));
				nodeList.push_back(currNode);
			}
			nodeList.sort(orderByDate);

			//Check if any of currNode's OutDeg NIDs correspond to any of the previously accessed NIDs
			int n_count = 1;
			for(list<TNGraph::TNodeI>::iterator n = nodeList.begin(); n!= nodeList.end(); n++){
				int matches = 0;
				for(int e = 0; e < (*n).GetOutDeg(); e++){
					if(find(nodeListAccum.begin(), nodeListAccum.end(), (*n).GetOutNId(e)) != nodeListAccum.end()){
						matches++;
					}
				}
				if (matches > MaxK) matches = MaxK;
				if (matches > 0 && n_count <= InMinCit){
					//printf("%d matches in %d\n", matches, n_count);
					//system("PAUSE");
					avg_mean_k[matches][n_count]++;
				}
				n_count++;
				nodeListAccum.push_back((*n).GetId());
			}

			printf("processed %4d\n", (*n).GetInDeg());
		}

		for(int i = 1; i <= MaxK; i++){
			TFltPrV pv;
			int sum = 0;
			for(int j = 1; j <= InMinCit; j++){
				for(int k = MaxK; k >= i; k--)
					sum += avg_mean_k[k][j];
				pv.Add(TFltPr(j,(sum-0.0)/(j*manyCitations)));
			}
			gp.AddPlot(pv, gpwLines, TStr::Fmt("k = %d",i));
		}

		/*
		// Plot Mean Line
		TFltPrV pv;
		for(TInt i = 0;i<InMinCit;i++){
		pv.Add(TFltPr(i+1,avg_mean[i]/manyCitations));	
		}
		TInt p = gp.AddPlot(pv, gpwLines, "Mean");
		gp.SetLineStyle(p, "lt 6 lw 3");*/

		gp.SetXYLabel("Citations", "Closure ratio");
		gp.SavePng();
	}

	void createClosureKsAgainstTimeSingle(PNGraph g, TInt MaxK, TInt paperId){
		int manyCitations = 0;
		list<TNGraph::TNodeI> hiList;
		TNGraph::TNodeI paper = g->GetNI(paperId);
		TInt InMinCit = paper.GetInDeg();
		hiList.push_back(paper);
		manyCitations = 1;
		printf("%d papers with more than %d citations\n", manyCitations, InMinCit);

		TGnuPlot gp(TStr::Fmt("dc_k_paper%d_%d", paperId, time(NULL)), TStr::Fmt("Closure ratio against Citations (Paper %d with %d Citations)", paperId, InMinCit));

		TFltV avg_mean(InMinCit);
		TVec<TIntV> avg_mean_k(MaxK+1);
		for(TVec<TIntV>::TIter i = avg_mean_k.BegI(); i != avg_mean_k.EndI(); i++){
			(*i) = TIntV(InMinCit+1);
			for(TIntV::TIter j = (*i).BegI(); j != (*i).EndI(); j++){
				(*j) = 0;
			}
		}

		//Sort the nodes
		hiList.sort(orderByCitation);
		//Print for fun
		for(list<TNGraph::TNodeI>::iterator n = hiList.begin(); n != hiList.end(); n++){
			printf("Paper %d with %d citations\n", (*n).GetId(), (*n).GetInDeg());
		}

		list<TNGraph::TNodeI> nodeList;
		list<int> nodeListAccum;
		int total;
		for(list<TNGraph::TNodeI>::iterator n = hiList.begin(); n != hiList.end(); n++){
			int avg_pointer = 0;
			printf("For Paper %07d...", (*n).GetId());

			total = (*n).GetInDeg();
			nodeList.clear();
			nodeListAccum.clear();

			//Traverse the edges and create a collection of InNodes
			for(int e = 0;e<(*n).GetInDeg();e++){
				TNGraph::TNodeI currNode = g->GetNI((*n).GetInNId(e));
				nodeList.push_back(currNode);
			}
			nodeList.sort(orderByDate);

			//Check if any of currNode's OutDeg NIDs correspond to any of the previously accessed NIDs
			int n_count = 1;
			for(list<TNGraph::TNodeI>::iterator n = nodeList.begin(); n!= nodeList.end(); n++){
				int matches = 0;
				for(int e = 0; e < (*n).GetOutDeg(); e++){
					if(find(nodeListAccum.begin(), nodeListAccum.end(), (*n).GetOutNId(e)) != nodeListAccum.end()){
						matches++;
					}
				}
				if (matches > MaxK) matches = MaxK;
				if (matches > 0 && n_count <= InMinCit){
					//printf("%d matches in %d\n", matches, n_count);
					//system("PAUSE");
					avg_mean_k[matches][n_count]++;
				}
				n_count++;
				nodeListAccum.push_back((*n).GetId());
			}

			printf("processed %4d\n", (*n).GetInDeg());
		}

		for(int i = 1; i <= MaxK; i++){
			TFltPrV pv;
			int sum = 0;
			for(int j = 1; j <= InMinCit; j++){
				for(int k = MaxK; k >= i; k--)
					sum += avg_mean_k[k][j];
				pv.Add(TFltPr(j,(sum-0.0)/(j*manyCitations)));
			}
			gp.AddPlot(pv, gpwLines, TStr::Fmt("k = %d",i));
		}

		/*
		// Plot Mean Line
		TFltPrV pv;
		for(TInt i = 0;i<InMinCit;i++){
		pv.Add(TFltPr(i+1,avg_mean[i]/manyCitations));	
		}
		TInt p = gp.AddPlot(pv, gpwLines, "Mean");
		gp.SetLineStyle(p, "lt 6 lw 3");*/

		gp.SetXYLabel("Citations", "Closure ratio");
		gp.SavePng();
	}

	void createClosureKsAgainstTimeSliding(PNGraph g, TInt MaxK, TInt ColumnWidth, TInt InMinCit, TInt InMaxCit){
		TGnuPlot gp(TStr::Fmt("dc_k_sl_citations_%04d_%d", InMinCit, time(NULL)), TStr::Fmt("Closure ratio against Citations (%d < x < %d Citations) (Sliding Window)", InMinCit,InMaxCit));

		TFltV avg_mean(InMinCit);
		TVec<TIntV> avg_mean_k(MaxK+1);
		for(TVec<TIntV>::TIter i = avg_mean_k.BegI(); i != avg_mean_k.EndI(); i++){
			(*i) = TIntV(InMinCit+1);
			for(TIntV::TIter j = (*i).BegI(); j != (*i).EndI(); j++){
				(*j) = 0;
			}
		}

		DCGraph dcg(g, InMinCit, InMaxCit);
		list<TNGraph::TNodeI> hiList = dcg.getList();
		int manyCitations = dcg.getListLength();
		printf("%d papers with more than %d citations\n", manyCitations, InMinCit);
		hiList.sort(orderByCitation);
		dcg.printListInfo();

		list<TNGraph::TNodeI> nodeList;
		list<int> nodeListAccum;
		int total;
		for(list<TNGraph::TNodeI>::iterator n = hiList.begin(); n != hiList.end(); n++){
			int avg_pointer = 0;
			printf("For Paper %07d...", (*n).GetId());

			total = (*n).GetInDeg();
			nodeList.clear();
			nodeListAccum.clear();

			//Traverse the edges and create a collection of InNodes
			for(int e = 0;e<(*n).GetInDeg();e++){
				TNGraph::TNodeI currNode = g->GetNI((*n).GetInNId(e));
				nodeList.push_back(currNode);
			}
			nodeList.sort(orderByDate);

			//Check if any of currNode's OutDeg NIDs correspond to any of the previously accessed NIDs
			int n_count = 1;
			for(list<TNGraph::TNodeI>::iterator n = nodeList.begin(); n!= nodeList.end(); n++){
				int matches = 0;
				for(int e = 0; e < (*n).GetOutDeg(); e++){
					if(find(nodeListAccum.begin(), nodeListAccum.end(), (*n).GetOutNId(e)) != nodeListAccum.end()){
						matches++;
					}
				}
				if (matches > MaxK) matches = MaxK;
				if (matches > 0 && n_count <= InMinCit){
					//printf("%d matches in %d\n", matches, n_count);
					//system("PAUSE");
					avg_mean_k[matches][n_count]++;
				}
				n_count++;
				nodeListAccum.push_back((*n).GetId());
			}

			printf("processed %4d\n", (*n).GetInDeg());
		}

		for(int i = 1; i <= MaxK; i++){
			list<TFlt> sumList;
			TFlt sumListSum = 0;
			TFltPrV pv;
			FILE* file = fopen(TStr::Fmt("dc_k_sl_%d_%02d.txt", InMinCit, i).CStr(), "wt");
			int sum = 0;
			for(int j = 1; j <= InMinCit; j++){
				for(int k = MaxK; k >= i; k--)
					sum += avg_mean_k[k][j];
				sumList.push_back((sum-0.0)/(j*manyCitations));
				sumListSum += sumList.back();
				if(j >= ColumnWidth){
					TFlt x = j - ColumnWidth/2;
					TFlt y = sumListSum/ColumnWidth;
					pv.Add(TFltPr(x,y));
					if (j >= ColumnWidth + ColumnWidth/2)
						fprintf(file, TStr::Fmt("%f %f\n", x, y).CStr());
					sumListSum -= sumList.front();
					sumList.pop_front();
				}
			}
			fclose(file);
			gp.AddPlot(pv, gpwLines, TStr::Fmt("k = %d",i));
		}

		gp.SetXYLabel("Citations", "Closure ratio");
		gp.SavePng();
	}

	void createClosureKsAgainstTimeSlidingPatents(PNGraph g, TInt MaxK, TInt ColumnWidth, TInt InMinCit, TInt InMaxCit){
		TGnuPlot gp(TStr::Fmt("dc_k_sl_citations_%04d_%d", InMinCit, time(NULL)), TStr::Fmt("Closure ratio against Citations (%d < x < %d Citations) (Sliding Window)", InMinCit,InMaxCit));

		TFltV avg_mean(InMinCit);
		TVec<TIntV> avg_mean_k(MaxK+1);
		for(TVec<TIntV>::TIter i = avg_mean_k.BegI(); i != avg_mean_k.EndI(); i++){
			(*i) = TIntV(InMinCit+1);
			for(TIntV::TIter j = (*i).BegI(); j != (*i).EndI(); j++){
				(*j) = 0;
			}
		}

		DCGraph dcg(g, InMinCit, InMaxCit);
		list<TNGraph::TNodeI> hiList = dcg.getList();
		int manyCitations = dcg.getListLength();
		printf("%d papers with more than %d citations\n", manyCitations, InMinCit);
		hiList.sort(orderByCitation);
		dcg.printListInfo();

		list<TNGraph::TNodeI> nodeList;
		list<int> nodeListAccum;
		int total;
		for(list<TNGraph::TNodeI>::iterator n = hiList.begin(); n != hiList.end(); n++){
			int avg_pointer = 0;
			printf("For Paper %07d...", (*n).GetId());

			total = (*n).GetInDeg();
			nodeList.clear();
			nodeListAccum.clear();

			//Traverse the edges and create a collection of InNodes
			for(int e = 0;e<(*n).GetInDeg();e++){
				TNGraph::TNodeI currNode = g->GetNI((*n).GetInNId(e));
				nodeList.push_back(currNode);
			}
			nodeList.sort(orderByDatePatents);

			//Check if any of currNode's OutDeg NIDs correspond to any of the previously accessed NIDs
			int n_count = 1;
			for(list<TNGraph::TNodeI>::iterator n = nodeList.begin(); n!= nodeList.end(); n++){
				int matches = 0;
				for(int e = 0; e < (*n).GetOutDeg(); e++){
					if(find(nodeListAccum.begin(), nodeListAccum.end(), (*n).GetOutNId(e)) != nodeListAccum.end()){
						matches++;
					}
				}
				if (matches > MaxK) matches = MaxK;
				if (matches > 0 && n_count <= InMinCit){
					//printf("%d matches in %d\n", matches, n_count);
					//system("PAUSE");
					avg_mean_k[matches][n_count]++;
				}
				n_count++;
				nodeListAccum.push_back((*n).GetId());
			}

			printf("processed %4d\n", (*n).GetInDeg());
		}

		for(int i = 1; i <= MaxK; i++){
			list<TFlt> sumList;
			TFlt sumListSum = 0;
			TFltPrV pv;
			FILE* file = fopen(TStr::Fmt("dc_k_sl_%d_%02d.txt", InMinCit, i).CStr(), "wt");
			int sum = 0;
			for(int j = 1; j <= InMinCit; j++){
				for(int k = MaxK; k >= i; k--)
					sum += avg_mean_k[k][j];
				sumList.push_back((sum-0.0)/(j*manyCitations));
				sumListSum += sumList.back();
				if(j >= ColumnWidth){
					TFlt x = j - ColumnWidth/2;
					TFlt y = sumListSum/ColumnWidth;
					pv.Add(TFltPr(x,y));
					if (j >= ColumnWidth + ColumnWidth/2)
						fprintf(file, TStr::Fmt("%f %f\n", x, y).CStr());
					sumListSum -= sumList.front();
					sumList.pop_front();
				}
			}
			fclose(file);
			gp.AddPlot(pv, gpwLines, TStr::Fmt("k = %d",i));
		}

		gp.SetXYLabel("Citations", "Closure ratio");
		gp.SavePng();
	}

	
	void createClosureKsAgainstTimeSingleSliding(PNGraph g, TInt MaxK, TInt ColumnWidth, TInt paperId){
		
		//Iterate through nodes and get those with >100 citations
		int manyCitations = 1;
		TNGraph::TNodeI paper = g->GetNI(paperId);
		TInt InMinCit = paper.GetInDeg();
		list<TNGraph::TNodeI> hiList;
		hiList.push_back(paper);
		printf("%d papers with more than %d citations\n", manyCitations, InMinCit);

		TFltV avg_mean(InMinCit);
		TVec<TIntV> avg_mean_k(MaxK+1);
		for(TVec<TIntV>::TIter i = avg_mean_k.BegI(); i != avg_mean_k.EndI(); i++){
			(*i) = TIntV(InMinCit+1);
			for(TIntV::TIter j = (*i).BegI(); j != (*i).EndI(); j++){
				(*j) = 0;
			}
		}

		TGnuPlot gp(TStr::Fmt("dc_k_sl_paper%d_%04d_%d", paperId, InMinCit, time(NULL)), TStr::Fmt("Closure ratio against Citations (Paper %d with %d Citations) (Sliding Window)", paperId, InMinCit));
		
		//Sort the nodes
		hiList.sort(orderByCitation);
		//Print for fun
		for(list<TNGraph::TNodeI>::iterator n = hiList.begin(); n != hiList.end(); n++){
			printf("Paper %d with %d citations\n", (*n).GetId(), (*n).GetInDeg());
		}

		list<TNGraph::TNodeI> nodeList;
		list<int> nodeListAccum;
		int total;
		for(list<TNGraph::TNodeI>::iterator n = hiList.begin(); n != hiList.end(); n++){
			int avg_pointer = 0;
			printf("For Paper %07d...", (*n).GetId());

			total = (*n).GetInDeg();
			nodeList.clear();
			nodeListAccum.clear();

			//Traverse the edges and create a collection of InNodes
			for(int e = 0;e<(*n).GetInDeg();e++){
				TNGraph::TNodeI currNode = g->GetNI((*n).GetInNId(e));
				nodeList.push_back(currNode);
			}
			nodeList.sort(orderByDate);

			//Check if any of currNode's OutDeg NIDs correspond to any of the previously accessed NIDs
			int n_count = 1;
			for(list<TNGraph::TNodeI>::iterator n = nodeList.begin(); n!= nodeList.end(); n++){
				int matches = 0;
				for(int e = 0; e < (*n).GetOutDeg(); e++){
					if(find(nodeListAccum.begin(), nodeListAccum.end(), (*n).GetOutNId(e)) != nodeListAccum.end()){
						matches++;
					}
				}
				if (matches > MaxK) matches = MaxK;
				if (matches > 0 && n_count <= InMinCit){
					//printf("%d matches in %d\n", matches, n_count);
					//system("PAUSE");
					avg_mean_k[matches][n_count]++;
				}
				n_count++;
				nodeListAccum.push_back((*n).GetId());
			}

			printf("processed %4d\n", (*n).GetInDeg());
		}

		for(int i = 1; i <= MaxK; i++){
			list<TFlt> sumList;
			TFlt sumListSum = 0;
			TFltPrV pv;
			//FILE* file = fopen(TStr::Fmt("dc_k_sl_%d_%02d.txt", InMinCit, i).CStr(), "wt");
			int sum = 0;
			for(int j = 1; j <= InMinCit; j++){
				for(int k = MaxK; k >= i; k--)
					sum += avg_mean_k[k][j];
				sumList.push_back((sum-0.0)/(j*manyCitations));
				sumListSum += sumList.back();
				if(j >= ColumnWidth){
					TFlt x = j - ColumnWidth/2;
					TFlt y = sumListSum/ColumnWidth;
					pv.Add(TFltPr(x,y));
					//fprintf(file, TStr::Fmt("%f %f\n", x, y).CStr());
					sumListSum -= sumList.front();
					sumList.pop_front();
				}
			}
			//fclose(file);
			gp.AddPlot(pv, gpwLines, TStr::Fmt("k = %d",i));
		}

		gp.SetXYLabel("Citations", "Closure ratio");
		gp.SavePng();
	}
	
	void createClosureKsAgainstTimeSingleSlidingPatents(PNGraph g, TInt MaxK, TInt ColumnWidth, TInt paperId){
		
		//Iterate through nodes and get those with >100 citations
		int manyCitations = 1;
		TNGraph::TNodeI paper = g->GetNI(paperId);
		TInt InMinCit = paper.GetInDeg();
		list<TNGraph::TNodeI> hiList;
		hiList.push_back(paper);
		printf("%d papers with more than %d citations\n", manyCitations, InMinCit);

		TFltV avg_mean(InMinCit);
		TVec<TIntV> avg_mean_k(MaxK+1);
		for(TVec<TIntV>::TIter i = avg_mean_k.BegI(); i != avg_mean_k.EndI(); i++){
			(*i) = TIntV(InMinCit+1);
			for(TIntV::TIter j = (*i).BegI(); j != (*i).EndI(); j++){
				(*j) = 0;
			}
		}

		TGnuPlot gp(TStr::Fmt("dc_k_sl_patent%d_%04d_%d", paperId, InMinCit, time(NULL)), TStr::Fmt("Closure ratio against Citations (Paper %d with %d Citations) (Sliding Window)", paperId, InMinCit));
		
		//Sort the nodes
		hiList.sort(orderByCitation);
		//Print for fun
		for(list<TNGraph::TNodeI>::iterator n = hiList.begin(); n != hiList.end(); n++){
			printf("Paper %d with %d citations\n", (*n).GetId(), (*n).GetInDeg());
		}

		list<TNGraph::TNodeI> nodeList;
		list<int> nodeListAccum;
		int total;
		for(list<TNGraph::TNodeI>::iterator n = hiList.begin(); n != hiList.end(); n++){
			int avg_pointer = 0;
			printf("For Paper %07d...", (*n).GetId());

			total = (*n).GetInDeg();
			nodeList.clear();
			nodeListAccum.clear();

			//Traverse the edges and create a collection of InNodes
			for(int e = 0;e<(*n).GetInDeg();e++){
				TNGraph::TNodeI currNode = g->GetNI((*n).GetInNId(e));
				nodeList.push_back(currNode);
			}
			nodeList.sort(orderByDatePatents);

			//Check if any of currNode's OutDeg NIDs correspond to any of the previously accessed NIDs
			int n_count = 1;
			for(list<TNGraph::TNodeI>::iterator n = nodeList.begin(); n!= nodeList.end(); n++){
				int matches = 0;
				for(int e = 0; e < (*n).GetOutDeg(); e++){
					if(find(nodeListAccum.begin(), nodeListAccum.end(), (*n).GetOutNId(e)) != nodeListAccum.end()){
						matches++;
					}
				}
				if (matches > MaxK) matches = MaxK;
				if (matches > 0 && n_count <= InMinCit){
					//printf("%d matches in %d\n", matches, n_count);
					//system("PAUSE");
					avg_mean_k[matches][n_count]++;
				}
				n_count++;
				nodeListAccum.push_back((*n).GetId());
			}

			printf("processed %4d\n", (*n).GetInDeg());
		}

		for(int i = 1; i <= MaxK; i++){
			list<TFlt> sumList;
			TFlt sumListSum = 0;
			TFltPrV pv;
			FILE* file = fopen(TStr::Fmt("dc_k_sl_patent%d_%d_%02d.txt", paperId, InMinCit, i).CStr(), "wt");
			int sum = 0;
			for(int j = 1; j <= InMinCit; j++){
				for(int k = MaxK; k >= i; k--)
					sum += avg_mean_k[k][j];
				sumList.push_back((sum-0.0)/(j*manyCitations));
				sumListSum += sumList.back();
				if(j >= ColumnWidth){
					TFlt x = j - ColumnWidth/2;
					TFlt y = sumListSum/ColumnWidth;
					pv.Add(TFltPr(x,y));
					fprintf(file, TStr::Fmt("%f %f\n", x, y).CStr());
					sumListSum -= sumList.front();
					sumList.pop_front();
				}
			}
			fclose(file);
			gp.AddPlot(pv, gpwLines, TStr::Fmt("k = %d",i));
		}

		gp.SetXYLabel("Citations", "Closure ratio");
		gp.SavePng();
	}

	void createScatter(PNGraph g, TInt InMinCit, TInt InMaxCit){
		TGnuPlot gp(TStr::Fmt("sp_%04d_%04d_%d", InMinCit, InMaxCit, time(NULL)), TStr::Fmt("Scatter Plot of Closure ratio against papers (%d < x < %d Citations)", InMinCit,InMaxCit));

		list<TNGraph::TNodeI> nl;
		for(TNGraph::TNodeI n = g->BegNI(); n != g->EndNI(); n++){
			if(n.GetInDeg() >= InMinCit && n.GetInDeg() <= InMaxCit){ // TODO Do something about this
				nl.push_back(n);
			}
		}

		nl.sort(orderByCitation);

		list<TNGraph::TNodeI> nodeList;
		list<int> nodeListAccum;
		int nodeTotalCitations;
		int matches;
		TFltPrV pv;
		for(list<TNGraph::TNodeI>::iterator n = nl.begin(); n != nl.end(); n++){
			int avg_pointer = 0;
			printf("For Paper %07d...", (*n).GetId());

			nodeTotalCitations = (*n).GetInDeg();
			matches = 0;
			nodeList.clear();
			nodeListAccum.clear();

			//Traverse the edges and create a collection of InNodes
			for(int e = 0;e<(*n).GetInDeg();e++){
				TNGraph::TNodeI currNode = g->GetNI((*n).GetInNId(e));
				nodeList.push_back(currNode);
			}
			nodeList.sort(orderByDate);

			//Check if any of currNode's OutDeg NIDs correspond to any of the previously accessed NIDs
			for(list<TNGraph::TNodeI>::iterator n = nodeList.begin(); n!= nodeList.end(); n++){
				for(int e = 0; e < (*n).GetOutDeg(); e++){
					if(find(nodeListAccum.begin(), nodeListAccum.end(), (*n).GetOutNId(e)) != nodeListAccum.end()){
						matches++;
						break;
					}
				}
				nodeListAccum.push_back((*n).GetId());
			}

			pv.Add(TFltPr(nodeTotalCitations,(matches-0.0)/nodeTotalCitations));

			printf("%4d out of %4d\n", matches, nodeTotalCitations);
		}

		int p = gp.AddPlot(pv, gpwPoints, "");
		gp.SetYRange(0, 1);
		//gp.SetXRange(InMinCit, InMaxCit);
		gp.SetLineStyle(p, "lt 1 lw 1 pt 1");
		gp.SetXYLabel("Papers (# Citations)", "Closure ratio");
		gp.SavePng();
	}

	void createClosureAgainstPapers(PNGraph g, TInt ColumnWidth, TBool IsSliding, TInt InMinCit, TInt InMaxCit){
		const char* slidString = (IsSliding) ? "_sliding" : "";
		const char* slidString2 = (IsSliding) ? "Sliding window width" : "Column width";
		TGnuPlot gp(TStr::Fmt("hs%s_%04d_%04d_%04d_%d", slidString, InMinCit, InMaxCit, ColumnWidth, time(NULL)), TStr::Fmt("Histogram of Closure ratio against papers (%d < x < %d Citations) with %s %d", InMinCit,InMaxCit, slidString2, ColumnWidth));

		list<TNGraph::TNodeI> nl;
		for(TNGraph::TNodeI n = g->BegNI(); n != g->EndNI(); n++){
			if(n.GetInDeg() >= (InMinCit+1) && n.GetInDeg() <= InMaxCit){ // TODO Do something about this
				nl.push_back(n);
			}
		}

		nl.sort(orderByCitation);

		list<TNGraph::TNodeI> nodeList;
		list<int> nodeListAccum;
		int nodeTotalCitations;
		int matches;
		TFlt intervalAccum = 0.0;
		int intervalElements = 0;
		int intervalUntil = InMinCit;
		list<TTriple<TInt, TInt, TFlt>> slideList;
		int slideCurrent = 0;
		int slideCount = 0;
		TFlt slideSum = 0;
		TFltPrV pv;
		if(IsSliding)
			pv.Add(TFltPr(InMinCit+((ColumnWidth-0.0)/2)-1.5, 0));
		for(list<TNGraph::TNodeI>::iterator n = nl.begin(); n != nl.end(); n++){
			int avg_pointer = 0;
			printf("For Paper %07d...", (*n).GetId());

			nodeTotalCitations = (*n).GetInDeg();
			matches = 0;
			nodeList.clear();
			nodeListAccum.clear();

			//Traverse the edges and create a collection of InNodes
			for(int e = 0;e<(*n).GetInDeg();e++){
				TNGraph::TNodeI currNode = g->GetNI((*n).GetInNId(e));
				nodeList.push_back(currNode);
			}
			nodeList.sort(orderByDate);



			//Check if any of currNode's OutDeg NIDs correspond to any of the previously accessed NIDs
			for(list<TNGraph::TNodeI>::iterator n = nodeList.begin(); n!= nodeList.end(); n++){
				for(int e = 0; e < (*n).GetOutDeg(); e++){
					if(find(nodeListAccum.begin(), nodeListAccum.end(), (*n).GetOutNId(e)) != nodeListAccum.end()){
						matches++;
						break;
					}
				}
				nodeListAccum.push_back((*n).GetId());
			}

			if(!IsSliding){
				while(nodeTotalCitations > intervalUntil){
					/*printf("Until %d, %f %d %f", intervalUntil, intervalAccum, intervalElements, intervalAccum/intervalElements);
					system("PAUSE");*/
					if (intervalAccum > 0)
						pv.Add(TFltPr(intervalUntil-5, intervalAccum/intervalElements));
					intervalUntil += ColumnWidth;
					intervalAccum = 0;
					intervalElements = 0;
				}
				if(nodeTotalCitations > 0)
					intervalAccum += (matches-0.0)/nodeTotalCitations;
				intervalElements++;

			}
			else{
				while(nodeTotalCitations > intervalUntil){
					// Remove all points in accumulator that are less than intervalUntil-ColumnWidth
					while( (int) slideList.size() > 0 && (*(slideList.begin())).Val1 < (intervalUntil - ColumnWidth) ){
						slideList.pop_front();
					}
					// Sum the counts in the list
					for(list<TTriple<TInt, TInt, TFlt>>::iterator x = slideList.begin(); x != slideList.end(); x++){
						slideCount += (*x).Val2;
						slideSum += (*x).Val3;
					}
					// Plot
					//printf("Adding %f %f\n", intervalUntil-((ColumnWidth-0.0)/2)-0.5, slideSum/slideCount);
					//system("PAUSE");
					if(intervalUntil-InMinCit >= ColumnWidth && slideSum > 0)
						pv.Add(TFltPr(intervalUntil-((ColumnWidth-0.0)/2)-0.5, slideSum/slideCount));
					intervalUntil++;
					slideCount = 0;
					slideSum = 0;
				}
				// Create a new list item if it doesn't exist
				if(nodeTotalCitations > slideCurrent){
					slideList.push_back(TTriple<TInt,TInt,TFlt>(nodeTotalCitations,0,0.0));
				}
				// Update list
				slideCurrent = nodeTotalCitations;
				list<TTriple<TInt, TInt, TFlt>>::iterator liEnd = slideList.end();
				liEnd--;
				(*liEnd).Val2++;
				(*liEnd).Val3 += (matches-0.0)/nodeTotalCitations;
			}

			printf("%4d out of %4d\n", matches, nodeTotalCitations);
		}

		if(!IsSliding){
			pv.Add(TFltPr(intervalUntil-5, intervalAccum/intervalElements));
			pv.Add(TFltPr(intervalUntil, intervalAccum/intervalElements));
		}
		else{
			//printf("Sliding!!!");
			//system("PAUSE");
			while(InMaxCit > intervalUntil){
				// Remove all points in accumulator that are less than intervalUntil-ColumnWidth
				while( (int) slideList.size() > 0 && (*(slideList.begin())).Val1 < (intervalUntil - ColumnWidth) ){
					slideList.pop_front();
				}
				// Sum the counts in the list
				for(list<TTriple<TInt, TInt, TFlt>>::iterator x = slideList.begin(); x != slideList.end(); x++){
					slideCount += (*x).Val2;
					slideSum += (*x).Val3;
				}
				// Plot
				//printf("Adding %f %f\n", intervalUntil-((ColumnWidth-0.0)/2)-0.5, slideSum/slideCount);
				//system("PAUSE");
				if(intervalUntil-InMinCit >= ColumnWidth && slideSum > 0)
					pv.Add(TFltPr(intervalUntil-((ColumnWidth-0.0)/2)-0.5, slideSum/slideCount));
				intervalUntil++;
				slideCount = 0;
				slideSum = 0;
			}
			pv.Add(TFltPr(intervalUntil-((ColumnWidth-0.0)/2)+0.5, 0));
		}

		int p = gp.AddPlot(pv, gpwSteps, "");
		gp.SetYRange(0, 1);
		//gp.SetXRange(InMinCit, InMaxCit);
		gp.SetLineStyle(p, "lt 1 lw 1 pt 1");
		gp.SetXYLabel("Papers (# Citations)", "Closure ratio");
		gp.SavePng();
	}

	void createClosureAgainstTime(PNGraph g, TBool CAdjustTime, TInt InMinCit, TInt InMaxCit){
		const char* descString = (CAdjustTime) ? "time" : "citations";
		TGnuPlot gp(TStr::Fmt("dc_%s_%04d_%d", descString, InMinCit, time(NULL)), TStr::Fmt("Closure ratio against %s (%d < x < %d Citations)",descString, InMinCit,InMaxCit));

		TVec<TFlt> avg_mean(InMinCit);

		DCGraph dcg(g, InMinCit, InMaxCit);
		list<TNGraph::TNodeI> hiList = dcg.getList();
		int manyCitations = dcg.getListLength();
		printf("%d papers with %d < x < %d citations\n", manyCitations, InMinCit, InMaxCit);

		//Sort the nodes
		hiList.sort(orderByCitation);
		//Print for fun
		for(list<TNGraph::TNodeI>::iterator n = hiList.begin(); n != hiList.end(); n++){
			printf("Paper %d with %d citations\n", (*n).GetId(), (*n).GetInDeg());
		}

		list<TNGraph::TNodeI> nodeList;
		list<int> nodeListAccum;
		int total;
		int curr_total;
		int matches;
		for(list<TNGraph::TNodeI>::iterator n = hiList.begin(); n != hiList.end(); n++){
			int avg_pointer = 0;
			printf("For Paper %07d...", (*n).GetId());

			total = (*n).GetInDeg();
			curr_total = 0;
			matches = 0;
			nodeList.clear();
			nodeListAccum.clear();

			//Traverse the edges and create a collection of InNodes
			for(int e = 0;e<(*n).GetInDeg();e++){
				TNGraph::TNodeI currNode = g->GetNI((*n).GetInNId(e));
				nodeList.push_back(currNode);
			}
			nodeList.sort(orderByDate);

			TFltPrV pv;

			//Check if any of currNode's OutDeg NIDs correspond to any of the previously accessed NIDs
			for(list<TNGraph::TNodeI>::iterator n = nodeList.begin(); n!= nodeList.end(); n++){
				for(int e = 0; e < (*n).GetOutDeg(); e++){
					if(find(nodeListAccum.begin(), nodeListAccum.end(), (*n).GetOutNId(e)) != nodeListAccum.end()){
						matches++;
						break;
					}
				}
				curr_total++;
				//printf("%.4f\n", (matches-0.0)/curr_total);

				TFlt x = (CAdjustTime) ? (curr_total-0.0)/total : curr_total;
				TFlt y = (matches-0.0)/curr_total;
				pv.Add(TFltPr(x,y));
				nodeListAccum.push_back((*n).GetId());

				if(CAdjustTime){
					if(x > (avg_pointer-0.0)/InMinCit)
						avg_mean[avg_pointer++] += y;
				}
				else{
					if(avg_pointer < InMinCit)
						avg_mean[avg_pointer++] += y;
				}
			}

			gp.AddPlot(pv, gpwLines, "");

			printf("%4d out of %4d\n", matches, (*n).GetInDeg());
		}

		// Plot Mean Line
		TInt meanAdjust = (CAdjustTime) ? InMinCit : 1;
		TFltPrV pv;
		for(TInt i = 0;i<InMinCit;i++){
			pv.Add(TFltPr((i+1-0.0)/meanAdjust,avg_mean[i]/manyCitations));	
		}
		TInt p = gp.AddPlot(pv, gpwLines, "Mean");
		gp.SetLineStyle(p, "lt 6 lw 3");

		if(CAdjustTime)
			gp.SetXYLabel("Time", "Closure ratio");
		else
			gp.SetXYLabel("Citations", "Closure ratio");
		gp.SavePng();
	}

	void createClosureAgainstTimePatents(PNGraph g, TBool CAdjustTime, TInt InMinCit, TInt InMaxCit){
		int manyCitations = 0; // Number of papers
		list<TNGraph::TNodeI> hiList; // List of papers
		TVec<TFlt> avg_mean(InMinCit); // Closure ratio mean when a paper got x citations
		const char* descString = (CAdjustTime) ? "time" : "citations";
		
		//Iterate through nodes and get those with InMiNCit <= citations <= InMaxCit
		printf("Building node list...");
		for(TNGraph::TNodeI n = g->BegNI(); n != g->EndNI(); n++){
			if(n.GetInDeg() >= InMinCit && n.GetInDeg() <= InMaxCit){
				//printf("%d\n", n.GetId());
				hiList.push_back(n);
				manyCitations++;
			}
		}
		printf("Done.\n");
		printf("%d papers with more than %d citations\n", manyCitations, InMinCit);

		//Sort the nodes
		printf("Sorting node list...");
		hiList.sort(orderByCitation);
		printf("Done.\n");

		//Create graph object
		TGnuPlot gp(TStr::Fmt("dcp_%s_%04d_%d", descString, InMinCit, time(NULL)), TStr::Fmt("Closure ratio against %s (%d < x < %d Citations) for total %d Citations",descString, InMinCit,InMaxCit, manyCitations));

		//Print node list
		/*for(list<TNGraph::TNodeI>::iterator n = hiList.begin(); n != hiList.end(); n++){
			printf("Paper %d with %d citations\n", (*n).GetId(), (*n).GetInDeg());
		}*/

		//Get closure ratio for each paper
		for(list<TNGraph::TNodeI>::iterator n = hiList.begin(); n != hiList.end(); n++){
			printf("For Paper %07d...", (*n).GetId());
			int avg_pointer = 0;
			int total = (*n).GetInDeg();
			int curr_total = 0;
			int matches = 0;
			list<TNGraph::TNodeI> nodeList;
			list<int> nodeListAccum;

			//Traverse the edges and create a collection of InNodes and sort
			for(int e = 0;e<(*n).GetInDeg();e++){
				TNGraph::TNodeI currNode = g->GetNI((*n).GetInNId(e));
				nodeList.push_back(currNode);
			}
			nodeList.sort(orderByDatePatents);

			//TFltPrV pv;

			//Check if any of currNode's OutDeg NIDs correspond to any of the previously accessed NIDs
			for(list<TNGraph::TNodeI>::iterator n = nodeList.begin(); n!= nodeList.end(); n++){
				for(int e = 0; e < (*n).GetOutDeg(); e++){
					if(find(nodeListAccum.begin(), nodeListAccum.end(), (*n).GetOutNId(e)) != nodeListAccum.end()){
						matches++;
						break;
					}
				}
				curr_total++;
				//printf("%.4f\n", (matches-0.0)/curr_total);

				TFlt x = (CAdjustTime) ? (curr_total-0.0)/total : curr_total;
				TFlt y = (matches-0.0)/curr_total;
				//pv.Add(TFltPr(x,y));
				nodeListAccum.push_back((*n).GetId());

				if(CAdjustTime){
					if(x > (avg_pointer-0.0)/InMinCit)
						avg_mean[avg_pointer++] += y;
				}
				else{
					if(avg_pointer < InMinCit)
						avg_mean[avg_pointer++] += y;
				}
			}

			//gp.AddPlot(pv, gpwLines, "");
			
			printf("%4d out of %4d\n", matches, (*n).GetInDeg());
		}

		// Plot Mean Line
		printf("Plotting mean line...");
		TInt meanAdjust = (CAdjustTime) ? InMinCit : 1;
		TFltPrV pv;
		for(TInt i = 0;i<InMinCit;i++){
			pv.Add(TFltPr((i+1-0.0)/meanAdjust,avg_mean[i]/manyCitations));	
		}
		TInt p = gp.AddPlot(pv, gpwLines, "Mean");
		gp.SetLineStyle(p, "lt 6 lw 3");
		printf("Done.\n");

		if(CAdjustTime)
			gp.SetXYLabel("Time", "Closure ratio");
		else
			gp.SetXYLabel("Citations", "Closure ratio");
		gp.SavePng();
	}


