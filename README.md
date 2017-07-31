# ROOT_scripts
### A collection scripts for CERN's ROOT Framework

## AnalyzeTree.C

This is just an implementation of the Tree tutorial on the CERN website.

## SVDproto.C

Prototype of SVD. Mostly nonfunctional, but good example of using a tree.

## NormCent.C

Original Implementation of SVD. In the current state it will take in three TArrayD's and fills a file "SVDData.root" with a tree "SVDData" with the actual value vs. the predicted value. TArrayD's are expected to be the same size and should be at least 3 elements long (otherwise the matrix will be [singular](https://en.wikipedia.org/wiki/Invertible_matrix))

### CreateTestData.C

Generates test data for NormCent.C. Requires three environment TArrayD's (xx, yy, zz).

## fitPlaneWave.C

The current script for calculating SVD. It has the added function recursizeFitPlaneWave that helps reduce noise from the data. This script also runs on 3 Float_t arrays that contain the cartesian coordinates, a Float_t array of flags that indicate weather to use the corresponding cooridnate, and an Int_t that indicates the length of the arrays. 

