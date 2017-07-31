/*
 *NormCent.C
 *Created on 6/22/17
 *Last updated: 7/19/17
 *Description:
 *takes in 3 double arrays (x, y z) and finds the centroid and normal vector
 * to the data.
 */

#include "TObject.h"
#include "TArrayD.h"
#include "TMatrixD.h"
#include "TDecompSVD.h"

void NormCent(TArrayD xs, TArrayD ys, TArrayD zs){

	//Setup variables
	TMatrixD originalData(xs.GetSize(), 3);

	TArrayD Centroid(3);

	Bool_t ok;
	
	//set Centroid
	Centroid[0] = (xs.GetSum())/(xs.GetSize());
	Centroid[1] = (ys.GetSum())/(ys.GetSize());
	Centroid[2] = (zs.GetSum())/(zs.GetSize());

	//set Matrix
	for(Int_t i = xs.GetSize()-1; i>= 0; --i){
		originalData[i][0] = xs[i];
		originalData[i][1] = ys[i];
		originalData[i][2] = zs[i];
	}

	TDecompSVD DecompMatr(originalData);

	ok = DecompMatr.Decompose();

	printf("Centroid of the data is: (%f, %f, %f)\n", Centroid[0], Centroid[1], Centroid[2]);

	if(ok){
		TMatrixD fV = DecompMatr.GetV();
		//TDecompSVD stores the smallest EigenValue at the end of fSig which is the index row
		//of the row with the smalles Eigenvector
		Int_t indexOfMin = DecompMatr.GetSig().GetNoElements() -1;
		
		//Set up writting to the file		
		TFile *Data = new TFile("SVDData.root", "UPDATE");
		TTree *DataTree = (TTree *) Data->Get("SVDData");
		TBranch * xBranch = DataTree->GetBranch("xData");
		TBranch * yBranch = DataTree->GetBranch("yData");
		TBranch * zBranch = DataTree->GetBranch("zData");
	
		//Set Up Branches
		Double_t xVals[2];
		Double_t yVals[2];
		Double_t zVals[2];

		xBranch->SetAddress(xVals);
		yBranch->SetAddress(yVals);
		zBranch->SetAddress(zVals);


		//Get the coeficients to plane equation
		Double_t xCo = fV[indexOfMin][0];
		Double_t yCo = fV[indexOfMin][1];
		Double_t zCo = fV[indexOfMin][2];

		printf("The Normal of the best fit plane is (%f, %f, %f)\n", xCo, yCo, zCo);
		
		//Calculate the predicted value and Fill the tree with (actual, predicted)
		for(Int_t i = xs.GetSize()-1; i >= 0; --i){
			//Use this to store the current delta values
			Double_t deltaX = 0.0f;
			Double_t deltaY = 0.0f;
			Double_t deltaZ = 0.0f;

			//Save time calculating this once per itteration
			Double_t xComp = xCo * (xs[i]-Centroid[0]);
			Double_t yComp = yCo * (ys[i]-Centroid[1]);
			Double_t zComp = zCo * (zs[i]-Centroid[2]);
			
			//solve for predicted vaule
			deltaX = Centroid[0] - ((yComp + zComp)/ xCo);
			deltaY = Centroid[1] - ((xComp + zComp)/ yCo);
			deltaZ = Centroid[2] - ((xComp + yComp)/ zCo);
		
			
			//(Actual, Predicted)
			xVals[0] = xs[i];
			yVals[0] = ys[i];
			zVals[0] = zs[i];

			xVals[1] = deltaX;
			yVals[1] = deltaY;
			zVals[1] = deltaZ;
			
			//Add to SVDData.root files
			DataTree->Fill();
		}

		Data->Write(0, TObject::kWriteDelete, 0);
		Data->Close();
		delete Data;	

	}
	else{
		printf(" and the data is Singluar.\n");
	}
	
} 
