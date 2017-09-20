#include "IViterbi.h"

float subsequence_DTW_c4(const infra::matrix &dist, infra::vector &area){
    unsigned long height = dist.height();
    unsigned long width = dist.width();
    infra::matrix cost(height, width);
    infra::matrix length(height, width);
    infra::matrix path(height, width);
    infra::matrix s_point(height, width);

    cost(0, 0) = dist(0, 0);
    length(0, 0) = 1;
    path(0, 0) = -1;
    s_point(0, 0) = 0;
    int i, j;
    double cost_0, cost_1, cost_2;
    //first colum
    for (i=1; i < height; i++) {
        cost(i, 0) = BIG_FLT;
        length(i, 0) = 1;
        path(i, 0) = -1;
        s_point(i, 0) = j;
    }
    //first row
    for (j=1; j < width; j++) {
        cost(0, j) = cost(0, j-1) + dist(0, j);
        length(0, j) = j+1;
        path(0, j) = 1;
        s_point(0, j) = j;
    }

    for (i = 1; i < height; i++) {
        for (j = 1; j < width; j++) {
            cost_1 = cost(i-1, j-1);
            cost_2 = cost(i, j-1);
            if(j < i) {
                length(i, j) = j + 1;
                path(i, j) = -1;
                s_point(i, j) = s_point(i-1, j-1);
                cost(i, j) = cost_1 + BIG_FLT;
                continue;
            }
            if (cost_1 <= cost_2) {
                length(i, j) = j + 1;
                path(i, j) = 2;
                s_point(i, j) = s_point(i-1, j-1);
                cost(i, j) = cost_1 + dist(i, j);
            } else if (cost_2 <= cost_1) {
                length(i, j) = j + 1;
                path(i, j) = 1;
                s_point(i, j) = s_point(i, j-1);
                cost(i, j) = cost_2 + dist(i, j);
            } else {
                std::cout << "WARNNING: we do not find the min value of DTW distance" << std::endl;
            }
        }
    }
    // here we get the spotting area and calculate the epsilon
    i = height-1;
    j = width-1;
    while(path(i, j) > 0.5  &&  path(i, j) < 1.5) {
        j--;
    }
    int s = int(s_point(i, j));
    area(0) = s+1;
    area(1) = j-1;
    return (cost(i, j)-cost(0, s)-dist(i, j))/(length(i, j)-length(0, s)-1);
}
float I_DTW(float epsilon, const infra::matrix &dist, infra::vector &area) {
    unsigned long height = dist.height();
    unsigned long width = dist.width();

    int i,j;
    int counter=0;
    float best_epsilon = BIG_FLT;

    //make new distance matrix for subsequence dtw
    infra::matrix dist_tmp(height+2, width);
    for (i = 1; i < height+1; i++) {
        dist_tmp.row(i) = dist.row(i-1);
    }

    while ((best_epsilon - epsilon) > 1e-5f || best_epsilon < epsilon) {
        best_epsilon = epsilon;
        for(j = 0; j < width; j++) {
            dist_tmp(0, j) = epsilon;
            dist_tmp(height + 1, j) = epsilon;
        }
        epsilon = subsequence_DTW_c4(dist_tmp, area);
        //std::cout << "iteration: " << counter << ", epsilon: " << epsilon << ", start frame: " << area(0) << ", end frame: " << area(1) << std::endl;
        /*if (epsilon > 1) {
            std::cout << "too short utterances: (" << width  << " vs "
                      << height << ")" << std::endl;
            break;
        }*/
        counter++;
        if (counter > MAX_ITERATION) {
            std::cout << "exceed maximum iterations: " << counter << std::endl;
            std::cout << "precision: " << best_epsilon - epsilon << std::endl;
            break;
        }
    }
    //std::cout << std::endl;
    return epsilon;
}
