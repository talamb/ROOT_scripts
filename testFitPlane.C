/*
 *testFitPlane.C
 *Created on: 08/01/17
 *Last Updated: 08/01/17
 *Description:
 *Test for fitPlaneWave.C
 */

#include "TArrayF.h"
#include "TVector3.h"
#include "TMatrixF.h"
#include "TMath.h"
#include "TH2F.h"
#include <vector>


vector<TArrayF> predictVals(Int_t n, Float_t x_m [], Float_t y_m [], Float_t z_m [], vector<TArrayF> normCent);



void testFitPlane(){

//TODO Create test data without noise

//TODO run fitPlaneWave
//TODO draw actual vs. predicted for x, y, z

//TODO Create test data with noise

//TODO run fitPlaneWave
//TODO draw actual vs. predicted for x, y, z

}

vector<TArrayF> predictVals(Int_t n, Float_t x_m [], Float_t y_m [], Float_t z_m [], vector<TArrayF> normCent){
	vector<TArrayF> predicted;

	predicted.push_back(TArrayF(n));
	predicted.push_back(TArrayF(n));
	predicted.push_back(TArrayF(n));

	Float_t xCo = normCent[0][0];
	Float_t yCo = normCent[0][1];
	Float_t zCo = normCent[0][2];

	for(Int_t i = 0; i < n; ++i){
		//Save time calculating this once per itteration
		Double_t xComp = xCo * (xs[i]-normCent[1][0]);
		Double_t yComp = yCo * (ys[i]-normCent[1][1]);
		Double_t zComp = zCo * (zs[i]-normCent[1][2]);
			
		//X Predicted value
		predicted[0][i] = normCent[1][0] - ((yComp + zComp)/ xCo);
	
		//Y Predicted value
		predicted[1][i] = normCent[1][1] - ((xComp + zComp)/ yCo);

		//Z Predicted value
		predicted[2][i] = normCent[1][2] - ((xComp + yComp)/ zCo);
	}
}
