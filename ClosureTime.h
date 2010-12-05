#ifndef closure_time
#define closure_time

#include "Comparisons.h"

void createClosureAgainstTimePatents(PNGraph g, TBool CAdjustTime, TInt InMinCit, TInt InMaxCit);

// Plots the average number of citations against citations over time
// for papers with between InMinCit and InMaxCit citations
void createCitationsAgainstTime(PNGraph g, TInt InMinCit, TInt InMaxCit);

// Plots the closure ratio against either citations over time OR adjusted time for papers
// for papers with between InMinCit and InMaxCit citations
void createClosureAgainstTime(PNGraph g, TBool CAdjustTime, TInt InMinCit, TInt InMaxCit);

// Plots the k closure ratio against citations over time
// with between InMinCit and InMaxCit citations
void createClosureKsAgainstTime(PNGraph g, TInt MaxK, TInt InMinCit, TInt InMaxCit);
void createClosureKsAgainstTimeSliding(PNGraph g, TInt MaxK, TInt ColumnWidth, TInt InMinCit, TInt InMaxCit);
void createClosureKsAgainstTimeSingle(PNGraph g, TInt MaxK, TInt paperId);
void createClosureKsAgainstTimeSingleSliding(PNGraph g, TInt MaxK, TInt ColumnWidth, TInt paperId);
void createClosureKsAgainstTimeSingleSlidingPatents(PNGraph g, TInt MaxK, TInt ColumnWidth, TInt paperId);
void createClosureKsAgainstTimeSlidingPatents(PNGraph g, TInt MaxK, TInt ColumnWidth, TInt InMinCit, TInt InMaxCit);

// Plots the final closure ration for each paper, arranged in order of # of citations
// between InMinCit and InMaxCit citations
void createScatter(PNGraph g, TInt InMinCit, TInt InMaxCit);

void createClosureAgainstPapersCumulative(PNGraph g, TInt ColumnWidth, TBool IsSliding, TInt InMinCit, TInt InMaxCit);
void createCitationsAgainstTimeSliding(PNGraph g, TInt ColumnWidth, TInt InMinCit, TInt InMaxCit);

#endif