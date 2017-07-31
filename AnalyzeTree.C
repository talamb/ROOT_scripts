/*
 *last updated: 6/4/17
 *Creator: Tim Lamb
 *From: https://root.cern.ch/using-macro-read-ttree
*/

#include "TFile.h"
#include "TTree.h"

void AnalyzeTree()
{
	Int_t totalSize = 0; //Sum of data Size (in bytes) of all events
	TH1F * testHist = 0; //pointer to the Histagram
	
	//open file
	//TFile *f = 0TFile::Open("http://lcg-heppkg.web.cern.ch/lcg-heppkg/ROOT/eventdata.root");
	
	//unable to open
	if(f==0){
		printf("Error: Cannot open http://lcg-heppkg.web.cern.ch/lcg-heppkg/ROOT/eventdata.root!\n");
		return;
	}

	//Create The Histogram
	testHist = new TH1F("hPosX", "Position in X", 20, -5, 5);
	testHist->Sumw2();
	
	//Create the tree reader and its data containers
	TTreeReader myReader("EventTree", f);

	TTreeReaderValue<Int_t> eventSize(myReader, "fEventSize");
	TTreeReaderArray<Double_t> particlesMomentum(myReader, "fParticles.fMomentum");
	TTreeReaderArray<Double_t> particlesPosX(myReader, "fParticles.fPosX");

	while(myReader.Next()){
		//Get the data from the current TTree entry by getting
		// the value from the connected reader (eventSize) :
		totalSize += *eventSize;
		
		for(Int_t i= particlesMomentum.GetSize() - 1 ; i >= 0; i--) {
			if(particlesMomentum[i] > 40){
				testHist->Fill(particlesPosX[i]);
			}
		}
	}

	Int_t sizeInMB = totalSize/1024/1024;
	printf("\nTotal size of all events %d MB\n", sizeInMB);

	testHist->Fit("pol2");
	
}
