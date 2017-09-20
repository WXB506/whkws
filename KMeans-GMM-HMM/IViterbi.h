#include <iostream>
#include <string>
#include <vector>

#include "CHMM.h"
#include "infra.h"

#define BIG_FLT 9999999
#define MAX_ITERATION 10  

float VertibeDecode(infra::vector init_p, infra::matrix A, infra::matrix acoustic_cost, std::pair<int, int> &area);

float SegmentationByFillerReestimation(std::pair<int, int> &area);

//local constraint (i,j) comes from {(i-1, j-1), (i, j-1)}
float subsequence_DTW_c4(const infra::matrix &dist, infra::vector &area);

float I_DTW(float epsilon, const infra::matrix &dist, infra::vector &area);

