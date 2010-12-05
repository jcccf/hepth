#ifndef comparisons
#define comparisons

// Comparator for ordering citations by number of papers it is cited by (in-degree)
bool orderByCitation (TNGraph::TNodeI n1, TNGraph::TNodeI n2);

bool orderByDatePatents(TNGraph::TNodeI n1, TNGraph::TNodeI n2);

// Comparator for ordering citations by date
bool orderByDate (TNGraph::TNodeI n1, TNGraph::TNodeI n2);

#endif