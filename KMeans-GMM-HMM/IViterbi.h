#include <iostream>
#include <string>
#include <vector>

#include "CHMM.h"
#include "infra.h"

float VertibeDecode(infra::vector init_p, infra::matrix A, infra::matrix acoustic_cost, std::pair<int, int> &area);

float SegmentationByFillerReestimation(float epsilon, const fst::VectorFst<fst::StdArc> &fst, DecodableInterface *decodable, std::pair<int, int> &area);
