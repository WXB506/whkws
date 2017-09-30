#include <iostream>
#include <math.h>

#include "CHMM.h"
#include "infra.h"
#include "htkfile.h"
#include "dataset.h"
#include "feature.h"
#include "util.h"
#include "IViterbi.h"

using namespace std;
void get_acoustic_cost(CHMM &chmm, aslp_std::Feature *features, int lenght,
        std::vector< infra::matrix > *cost_mats) {
    int i, j, k;
    int state_num = chmm.GetStateNum();
    int dim = chmm.GetStateModel(0)->GetDimNum();
    assert(state_num > 0);
    assert(dim = features[0].GetFeature().width());
    
    infra::matrix *seq;
    std::vector< GMM * > gmms;
    for ( k = 0; k < state_num; k++ ) {
        gmms.push_back(chmm.GetStateModel(k));
    }

    for (i = 0; i < lenght; i++) {
        seq = &(features[i].GetFeature());
        infra::matrix cost_mat(state_num, seq->height());
        (*cost_mats).push_back(cost_mat);
        for (j = 0; j < seq->height(); j++) {
            // prepare feature for GMM
            double log_prob;
            double *sample = new double[dim];
            for (k = 0; k < dim; k++) {
                sample[k] = (*seq)(j, k);
            }
            for (k = 0; k < state_num; k++) {
                log_prob = log(gmms[k]->GetProbability(sample));
                (*cost_mats)[i](k, j) = -log_prob;
            }
        }
    }
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

void NormalizeFeature(aslp_std::Feature* features, int feature_size) {
    for (int i=0; i < feature_size; i++) {
        features[i].DoNormalizeFeature();
    }
}

void ReadModel(CHMM* models, string model_dir, StringVector &model_list, int model_num, string suffix) {
    for (int i=0; i < model_num; i++) {
        ifstream in(model_dir + model_list[i] + "." + suffix);
        in >> models[i];
        models[i].LengthNormalization();
        in.close();
    }
}

int main(int argc, char *argv[]) {
    if(argc < 8) {
        cerr<<"USAGE: model_dir model_list_file test_dir test_list feature_type do_mvn result_dir" << endl;
        return EXIT_FAILURE;
    }
    
    std::string model_dir;
    std::string model_list_file;
    std::string test_dir;
    std::string test_list_file;
    std::string feature_type;
    int do_mvn;
    std::string out_dir;

    model_dir = string(argv[1]);
    model_list_file = string(argv[2]);
    test_dir = string(argv[3]);
    test_list_file = string(argv[4]);
    feature_type = string(argv[5]);
    do_mvn = atoi(argv[6]);
    out_dir = string(argv[7]);
    
    StringVector model_list;
    model_list.read(model_list_file);
    StringVector test_list;
    test_list.read(test_list_file);

    int model_num = model_list.size();
    int test_size = test_list.size();   
    // read test set
    aslp_std::Feature* tests = new aslp_std::Feature[test_size];
    CHMM *models = new CHMM[model_num];
    
    ReadModel(models, model_dir, model_list, model_num, "mdl");
    
    ReadData(tests, test_dir, test_list, test_size, feature_type);
    if (do_mvn) {
        MVN(tests, test_size);
    }
    NormalizeFeature(tests, test_size);    
    // score for one model
    for (int i = 0; i < model_num; i++) {
        string query_id = model_list[i];
        ofstream ofs((out_dir + query_id + ".RESULT").c_str());
        std::vector< infra::matrix > *cost_mats = new std::vector< infra::matrix >;
        get_acoustic_cost(models[i], tests, test_size, cost_mats);
        for (int j = 0; j < test_size; j++) {
            infra::vector area(2);
            float score = I_DTW(0, (*cost_mats)[j], area);
            ofs << score << " " << area(0) << " " << area(1) << endl;
        }
        ofs.close();
        cost_mats->clear();
    }
    //

    /*
    for (int i = 0; i < test_size; i++) {
        string test_id = tests[i].GetFeatureId();
        ofstream out(out_dir + test_id + ".loglike");   
        for (int j = 0; j < (*cost_mats)[i].height(); j++) {
            for (int k = 0; k < (*cost_mats)[i].width(); k++) {
                out << -(*cost_mats)[i](j, k) << " ";
            }
            out << endl;
        }
        out.close();
    }  
    */  
}
