#include "TArrayD.h"
#include "TMath.h"
#include "TRandom.h"

void CreateTestData(Double_t t){

	TRandom *interference = new TRandom();

	for(Int_t i = xx.GetSize() -1; i>= 0; --i){
		/*Double_t interfer1 = interference->Gaus(0, 2);
		Double_t interfer2 = interference->Gaus(0, 1);
		Double_t interfer3 = interference->Gaus(0, 2);
		if(i%13==0){
			xx[i] = 4 * TMath::Tan(TMath::Pi() * t) + interfer1;
			yy[i] = 4 * TMath::Tan(TMath::Pi() * t) + interfer2;
			zz[i] = 4 * TMath::Tan(TMath::Pi() * t) + interfer3;
		}
		else if (i%7==0){
			xx[i] = 4 * TMath::Tan(TMath::Pi() * t) + interfer2 + interfer3;
			yy[i] = 4 * TMath::Tan(TMath::Pi() * t) + interfer1 + 2;
			zz[i] = 4 * TMath::Tan(TMath::Pi() * t) + interfer2 + interfer1;
		}
		else if (i%2== 0){
			xx[i] = 4 * TMath::Tan(TMath::Pi() * t) + interfer2;
			yy[i] = 4 * TMath::Tan(TMath::Pi() * t) + interfer2;
			zz[i] = 4 * TMath::Tan(TMath::Pi() * t) + interfer2;
		}
		else{
			xx[i] = 4 * TMath::Tan(TMath::Pi() * t);
			yy[i] = 4 * TMath::Tan(TMath::Pi() * t);
			zz[i] = 4 * TMath::Tan(TMath::Pi() * t);
		}*/
		xx[i] = 4 * TMath::Cos(TMath::Pi() * t);
		yy[i] = 4 * TMath::Sin(TMath::Pi() * t);
		zz[i] = 4 * TMath::Tan(TMath::Pi() * t);

	}
}
