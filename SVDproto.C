/*
*Created By: Timothy A Lamb 6/20/17
*FileName: SVDproto.C
*LastUpdated: 6/20/17
***
*Short Decription:
* Takes Space-time doubles (x, y, z, t) from a TFile and reads them into an SVD object.
* Then it decomposes the data.
*/

#include "TFile.h"
#include "TTree.h"
#include "TMatrixD.h"
#include <vector>


void SVDproto(){/*TFile *f=0){

	//Test if File has been opened
	if(f==0){
		printf("Error: Could not open file. Please pass file: \"SVDproto.c(\"fileVariable\")\n");
		return;
	}

/*********************Create Matrix From data*****************************************/
/*	
	//TODO update for the format of the data file
	TTreeReader eventReader("EventTree", f);

	TTreeReaderArray<Double_t> xPosition(eventReader, "xPos");
	TTreeReaderArray<Double_t> yPosition(eventReader, "yPos");
	TTreeReaderArray<Double_t> zPosition(eventReader, "zPos");
	TTreeReaderArray<Double_t> timeVar(eventReader, "time");
	
	
	Int_t nrRows = (int) eventReader.GetEntries(true);
	const Int_t nrCols = 4;

	vector<Double_t> centroid (3, 0.0f);

	//Used to fill with unaltered data
	TMatrixD originalData(nrRows, nrCols);

	while(eventReader.Next()){
		for(Int_t i= xPosition.GetSize()-1; i >= 0; i--){
			//Assign Space-time values to Matrix
			originalData[i][0] = xPosition[i];
			originalData[i][1] = yPosition[i];
			originalData[i][2] = zPosition[i];
			originalData[i][3] = timeVar[i];

			//add Space-Time values to to centroid. We'll divide to get the average
			centroid[0] += xPosition[i];
			centroid[1] += yPosition[i];
			centroid[2] += zPosition[i];
			centroid[3] += timeVar[i];
		}
	}

*/

	//create a test wave 4cost + a, 4sint + a , 4tant + a, t	
	
	//create the SVD
	TDecompSVD svd(originalData);
	
	
	//try decomposing the data
	Bool_t ok = svd.Decompose();

	//find centroid of data
	centroid[0] /= nrRows;
	centroid[1] /= nrRows;
	centroid[2] /= nrRows;
	centroid[3] /= nrRows;

	//if data was decomposed do stuff. Else data is SINGULAR.
	if(ok){
		//TODO Do stuff with fSig and Centroid (my guess is  that if you take -1 fSig
		// and place it at the centoid that should point in the direction of object that
		// produced the cascade.

	}
	else{
		//Matrix is Singular
		printf("Data is Singular.\n");

	}


}
