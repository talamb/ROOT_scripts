/*
 *fitPlaneWave.C
 *Created on 6/22/17
 *Last updated: 8/01/17
 *Description:
 *Takes in Event data and reduces the noise and returns the centroid and Normal of the Plane Wave.
 */

#include "TArrayF.h"
#include "TVector3.h"
#include "TMatrixF.h"
#include "TDecompSVD.h"
#include "TMath.h"
#include <vector>



/*============================================================================================
 * Function Declarations
 *============================================================================================
 */

vector<TArrayF> SVD(Float_t x_m[], Float_t y_m[], Float_t z_m[], vector<Int_t> indices, Int_t ignoreInd);
vector<TArrayF> itterFitPlaneWave(Int_t n, Float_t x_m[], Float_t y_m[], Float_t z_m[], Bool_t LUsech[]);
Float_t average(Int_t n, Float_t arr[], vector<Int_t> indices, Int_t ignoreInd);
Double_t angleBetween(TVector3 v1, TVector3 v2);

Double_t maxDeltaAngle = 0.005;
Int_t minNumOfHits = 3;

/*===================================================================================================
 * fitPlaneWave: Main that calls recusivePlaneWave and returns the result
 *===================================================================================================
 */ 
vector<TArrayF> fitPlaneWave(Int_t nchannl, Float_t x_m[], Float_t y_m[], Float_t z_m[], Float_t hittimes_ns[], Bool_t LUseCh[]){

	return itterFitPlaneWave(nchannl , x_m, y_m, z_m, LUseCh);
}

/*===================================================================================================
 * itterFitPlaneWave: Goes through different permutations to find the elements that best fit the wave and removes the outliers.
 *===================================================================================================
 */
vector<TArrayF> itterFitPlaneWave(Int_t n , Float_t x_m[], Float_t y_m[], Float_t z_m[], Bool_t LUseCh[]){
	
	vector<Int_t> indices;
	vector<TArrayF> BestFit;
	
	//Initial fill of indices
	for(Int_t i = 0; i < n; ++i){
		if(LUseCh[i]){
			indices.push_back(i);
		}
	}


	//Make sure there are at least 3 usable indices
	if(indices.size() < minNumOfHits){
		printf("fitPlaneWave needs at least three hits to work.");
		return {};
	}

	//Initially set originFit Plan Wave
	originFit = SVD(x_m, y_m, z_m, indices, -1);
	
	if(originFit.empty() || (originFit[1][0] == 0 && originFit[1][1] && BestFit[1][2])){
		printf("fitPlaneWave was unable to fit the data. Most likely the data is singular.");//TODO probably remove in the final
		return {};
	}

	Bool_t finished = false;

	//Set intial angle
	TVector3 originAngle(BestFit[1][0], BestFit[1][1], BestFit[1][2]);

	//recursiveFitPlaneWave loop
	while(!finished){

		//Initialze BestAngle
		Double_t maxAngle = 0.0f;

		//Initialize bestIndex.
		Int_t maxIndex = -1;


		//Find the index that can be gotten rid of
		for(Int_t i= (indices.size()-1); i>= 0; --i){
			//create Vector without the current index
			vector<TArrayF> svdResult = SVD(x_m, y_m, z_m, indices, i);
			TVector3 curItter(svdResult[1][0], svdResult[1][1], svdResult[1][2]);

			//Calculate angle between curItter and Original data
			Double_t deltaAngle = angleBetween(curItter, originAngle);


			if(maxIndex == -1){
				//set maxIndex and maxAngle
				maxIndex = i;
				maxAngle = deltaAngle;
			}
			else if(deltaAngle > maxAngle){
				//set best index and maxAngle
				maxIndex = i;
				maxAngle = deltaAngle;
			}

			
		}

		if(maxAngle <= maxDeltaAngle){
			//removes the i-th element from the vector
			indices.erase(indices.begin() + maxIndex);

			if(indices.size() <= 3){ finish = true;	}
		}
		else{
			//Angle threashold has been reached
			finished = true;
		}
	}

	//return the best fit planewave
	return SVD(x_m, y_m, z_m, indices, -1);
}

/*==================================================================================================
 * SVD : takes in arrays of points and indices to use and returns the Centroid and Best Fit Normal
 *==================================================================================================
 */

vector<TArrayF> SVD(Float_t x_m[], Float_t y_m[], Float_t z_m[], vector<Int_t> indices, Int_t ignoreInd){

	//We keep the ignoreInd is still a part of the array
	Int_t n = indices.size()-1;

	//Make sure there enough points NOTE: recusivePlaneWave should catch this. 
	if(n < minNumOfHits){
		printf("Must have at least 3 points for SVD to work");//TODO possibly remove in the final
		return {};
	}

	//Setup variables
	TMatrixF originalData(n, 3);
	vector<TArrayF> results;
	
	results.push_back(TArrayF(3));
	results.push_back(TArrayF(3));

	Bool_t ok;
	
	//Calculate Centroid and store in the first element of results
	results[0][0] = average(n, x_m, indices, ignoreInd);
	results[0][1] = average(n, y_m, indices, ignoreInd);
	results[0][2] = average(n, z_m, indices, ignoreInd);

	//Fill the matrix with the important values
	for(Int_t i = n; i>= 0; --i){
		if(i == ignoreInd) {
			continue;
		}
		originalData[i][0] = x_m[indices[i]];
		originalData[i][1] = y_m[indices[i]];
		originalData[i][2] = z_m[indices[i]];
	}


	//Actual Workhorse of SVD. DecomMatr will calculate and store the SVD of the data
	TDecompSVD DecompMatr(originalData);

	//If Decomposition was sucessful: ok= true ? ok= False
	ok = DecompMatr.Decompose();

	
	//TODO Remove in Final	
	printf("Centroid of the data is: (%f, %f, %f)\n", results[0][0], results[0][1], results[0][2]);


	if(ok){
		TMatrixD fV = DecompMatr.GetV();
		//TDecompSVD stores the smallest EigenValue at the end of fSig which is the index row
		//of the row with the smalles Eigenvector
		Int_t indexOfMin = DecompMatr.GetSig().GetNoElements() -1;


		//Get the coeficients (normal) to plane equation save to results[1]
		results[1][0] = fV[indexOfMin][0];
		results[1][1] = fV[indexOfMin][1];

		//TODO (ASK TO MAKE SURE) Force upward moving particles.		
		if(fV[indexOfMin][2] < 0){
			results[1][2] = -1.0f * fV[indexOfMin][2];
		}
		else {
			results[1][2] = fV[indexOfMin][2];
		}

		printf("The Normal of the best fit plane is (%f, %f, %f)\n", results[1][0], results[1][0], results[1][2]); //TODO Remove in final

	}
	else{
		
		//The data is singluar set normal as <0,0,0>
		printf(" and the data is Singluar.\n");//TODO remove in final
		results[1][0] = 0;
		results[1][1] = 0;
		results[1][2] = 0;
	}
	
	return results;
} 

/*================================================================================================
 * average: takes in an array, a list of indices, and the size of the indices array. Returns the average of
 *	 the arr that are a part of the indices
 *=================================================================================================
 */

Float_t average(Int_t n, Float_t arr[], vector<Int_t> indices, Int_t ignoreInd){

	Float_t total = 0.0f;

	for(Int_t i = 0; i < n; ++i){
		if(i == ignoreInd){
			continue;
		}
		total += arr[indices[i]];
	}

	return total/((Float_t) n);
}

/*==================================================================================================
 * angleBetween: takes in to TVector3 and returns the angle between the two vectors
 *==================================================================================================
 */

Double_t angleBetween(TVector3 v1, TVector3 v2){
	//theta = Acos( (v1 dot v2) / (Magnitude(v1) * Magnitude(v2))
	return TMath::ACos( (v1.Dot(v2)) / (v1.Mag() * v2.Mag()) );
}
