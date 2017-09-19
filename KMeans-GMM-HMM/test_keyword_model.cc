#include <iostream>

#include "CHMM.h"
#include "infra.h"
#include "htkfile.h"
#include "dataset.h"
#include "feature.h"
#include "util.h"

using namespace std;
void get_acoustic_cost(const CHMM &chmm, const Feature &features
        std::vector< infra::matrix > *cost_mats) {
    for 
}

void MVN(aslp_std::Feature* features, int feature_size) {
    for (int i=0; i < feature_size; i++) {
        features[i].DoMVN();
    }
}

void ReadData(aslp_std::Feature* features, string feature_dir, StringVector &feature_list, int feature_list_size, string feature_type) {
    for (int i=0; i < feature_list_size; i++) {
        features[i].ReadData(feature_dir, feature_list[i], feature_type);
    }
}

int main(int argc, char *argv[]) {
    if(argc < 8) {
        cerr<<"USAGE: model_dir model test_dir test_list feature_type do_mvn result_dir" << endl;
        return EXIT_FAILURE;
    }
    
    std::string model_dir;
    std::string model_file;
    std::string test_dir;
    std::string test_list_file;
    std::string feature_type;
    int do_mvn;
    std::string out_dir;

    model_dir = string(argv[1]);
    model_file = string(argv[2]);
    test_dir = string(argv[3]);
    test_list_file = string(argv[4])
    feature_type = string(argv[4]);
    do_mvn = atoi(argv[5]);
    out_dir = string(argv[6]);
    
    StringVector test_list;
    test_list.read(test_list_file);

    
    int test_size = test_list.size();   
    // read test set
    aslp_std::Feature* tests = new aslp_std::Feature[test_size];
    
    ReadData(tests, test_dir, test_list, test_size, feature_type);
    if (do_mvn) {
        MVN(tests, test_size);
    }

    // read model 
    ifstream in(model_dir+model_file);
    CHMM chmm;
    chmm << in;
    
}
