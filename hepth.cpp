#include "stdafx.h"
#include "ClosureTime.h"
#include "Shadow.h"

int main(int argc, char* argv[]) {

	Env = TEnv(argc, argv, TNotify::StdNotify);
	Env.PrepArgs(TStr::Fmt("DCGraphInfo. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
	TExeTm ExeTm;

	const TStr patentsFile = "cit-Patents.txt";
	const TStr hepthFile = "Cit-HepTh.txt";
	const TStr hepphFile = "Cit-HepPh.txt";
	
	const TStr InFNm = Env.GetIfArgPrefixStr("-i:", hepthFile, "Input file");
	const TInt InMinCit = Env.GetIfArgPrefixInt("-cmin:", 500, "Minimum Citations");
	const TInt InMaxCit = Env.GetIfArgPrefixInt("-cmax:", 1000, "Maximum Citations");

	PNGraph g = TNGraph::New();

	Try
		g = TSnap::LoadEdgeList<PNGraph>(InFNm);
	printf("\nInput file has %d nodes, %d edges\n", g->GetNodes(), g->GetEdges());

	//createClosureAgainstTime(g, false, 500, 9999);

	//Patents
	//createClosureKsAgainstTimeSingleSlidingPatents(g, 30, 100, 4723129);
	//createClosureKsAgainstTimeSingleSlidingPatents(g, 30, 100, 4463359);
	//createClosureKsAgainstTimeSingleSlidingPatents(g, 30, 100, 4740796);
	//createClosureKsAgainstTimeSingleSlidingPatents(g, 30, 100, 4345262);
	//createClosureKsAgainstTimeSingleSlidingPatents(g, 30, 100, 4558333);
	//createClosureKsAgainstTimeSingleSlidingPatents(g, 30, 100, 4313124);
	//createClosureKsAgainstTimeSingleSlidingPatents(g, 30, 100, 4683195);
	//createClosureKsAgainstTimeSingleSlidingPatents(g, 30, 100, 4459600);
	//createClosureKsAgainstTimeSingleSlidingPatents(g, 30, 100, 4683202);
	//createClosureKsAgainstTimeSlidingPatents(g, 30, 100, 500, 9999);

	//HepPH
	/*createClosureKsAgainstTimeSingleSliding(g, 30, 100, 9803315);
	createClosureKsAgainstTimeSingleSliding(g, 30, 100, 9804398);
	createClosureKsAgainstTimeSingleSliding(g, 30, 100, 9407339);
	createClosureKsAgainstTimeSingleSliding(g, 30, 100, 9512380);
	createClosureKsAgainstTimeSingleSliding(g, 30, 100, 9606399);
	createClosureKsAgainstTimeSingleSliding(g, 30, 100, 9807344);*/

	//HepTH
	//createClosureKsAgainstTimeSingle(g, 30, 9711200);
	/*createClosureKsAgainstTimeSingleSliding(g, 30, 100, 9711200);
	createClosureKsAgainstTimeSingleSliding(g, 30, 100, 9802150);
	createClosureKsAgainstTimeSingleSliding(g, 30, 100, 9802109);
	createClosureKsAgainstTimeSingleSliding(g, 30, 100, 9407087);
	createClosureKsAgainstTimeSingleSliding(g, 30, 100, 9610043);
	createClosureKsAgainstTimeSingleSliding(g, 30, 100, 9510017);
	createClosureKsAgainstTimeSingleSliding(g, 30, 100, 9908142);
	createClosureKsAgainstTimeSingleSliding(g, 30, 100, 9503124);
	createClosureKsAgainstTimeSingleSliding(g, 30, 100, 9906064);
	createClosureKsAgainstTimeSingleSliding(g, 30, 100, 9408099);*/
	//createCitationsAgainstTime(g, 500, 9999);
	//createCitationsAgainstTimeSliding(g, 100, 500, 9999);
	//createClosureKsAgainstTimeSliding(g, 30, 100, 1000, 9999);
	//createClosureKsAgainstTime(g, 10, 100, 9999);
	//createClosureKsAgainstTime(g, 15, 500, 9999);
	//createClosureAgainstTime(g, false, InMinCit, InMaxCit);
	//createClosureAgainstTime(g, false, 100, 120);
	//createClosureAgainstTime(g, false, 500, 9999);
	//createScatter(g, 100, 200);
	//createClosureAgainstPapers(g, 20, false, 0, 2500);
	//createClosureAgainstPapers(g, 50, false, 0, 2500);
	//createClosureAgainstPapers(g, 150, true, 0, 2500);
	//createClosureAgainstPapers(g, 200, true, 0, 2500);
	//createClosureAgainstPapers(g, 100, true, 0, 2500);
	//createClosureAgainstTimePatents(g, false, 500, 9999);
	//createClosureAgainstTimePatents(g, false, 50, 100);
	//createShadowOfPaper(g, 9802109);
	//createShadowOfPaper(g, 9802150);
	//createShadowOfPaper(g, 9711200);
	//createShadowOfPaper(g, 9704104);
	//createShadowOfPaper(g, 9703031);
	//createShadowOfPaper(g, 9908141);
	//createShadowOfPaper(g, 9602022);
	//createShadowOfPaper(g, 9601029);
	//createShadowOfPaper(g, 9611050);
	
	Catch
		printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
	system("PAUSE");
	return 0;
}
	