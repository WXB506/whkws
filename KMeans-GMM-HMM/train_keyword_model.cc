#include <iostream>
#include <algorithm> 

#include "CHMM.h"
#include "infra.h"
#include "htkfile.h"
#include "dataset.h"
#include "feature.h"
#include "util.h"

using namespace std;

void TrainCHGMM (aslp_std::Feature *querys, 
     std::map< std::string, std::vector<int> > &uniq_query_map, int mix_num, string out_dir) {
    
    std::map<std::string, std::vector<int> >::iterator it;
    for (it=uniq_query_map.begin(); it != uniq_query_map.end(); it++) {
        std::vector<infra::matrix> instances;
        std::vector<int> frame_nums;

        for (int i = 0; i < (it->second).size(); i++) {
            instances.push_back(querys[(it->second)[i]].GetFeature());
            frame_nums.push_back(querys[(it->second)[i]].GetFeature().height());
        }
        // get the state number
        int max_frame_num = *max_element(frame_nums.begin(), frame_nums.end()); 
        int min_frame_num = *min_element(frame_nums.begin(), frame_nums.end());
        int state_num = (max_frame_num>min_frame_num*2) ? min_frame_num*2 : max_frame_num;
        state_num -= 1;
        CHMM *chmm = new CHMM(state_num, instances[0].width(), mix_num);
        std::cout << it->first << std::endl;
        chmm->Train(instances, false); //here false means we don't tune the transition probs.
        std::cout << endl; 
        ofstream out(out_dir + it->first + ".mdl");
        out << *chmm;
        out.close();
    }
}

void TrainCHGMM (aslp_std::Feature *querys, 
     std::map< std::string, std::vector<int> > &uniq_query_map, 
     std::map< std::string, int > &phone_num_dict, 
     int state_num_per_phone, int mix_num, string out_dir) {
   
    int phone_num=0; 
    std::map<std::string, int>::iterator it_phone_num; 
    std::map<std::string, std::vector<int> >::iterator it;
    for (it=uniq_query_map.begin(); it != uniq_query_map.end(); it++) {
        std::vector<infra::matrix> instances;
        for (int i = 0; i < (it->second).size(); i++) {
            instances.push_back(querys[(it->second)[i]].GetFeature());
        }
        it_phone_num = phone_num_dict.find(it->first);
        if(it_phone_num != phone_num_dict.end()){
            phone_num = it_phone_num->second;
        } else {
            std::cout << "can not file the phone num of keyword: " << it->first << std::endl;
            exit(1);
        }
        CHMM *chmm = new CHMM(phone_num*state_num_per_phone, instances[0].width(), mix_num);
        std::cout << it->first << std::endl;
        chmm->Train(instances, false); //here false means we don't tune the transition probs.
        std::cout << endl; 
        ofstream out(out_dir + it->first + ".mdl");
        out << *chmm;
        out.close();
    }
}

void TrainCHGMM (aslp_std::Feature *querys, 
     std::map< std::string, std::vector<int> > &uniq_query_map, 
     std::map< std::string, int > &phone_num_dict, 
     int state_num_per_phone, int mix_num, double *global_variance, string out_dir) {
   
    int phone_num=0; 
    std::map<std::string, int>::iterator it_phone_num; 
    std::map<std::string, std::vector<int> >::iterator it;
    int i,j;
    for (it=uniq_query_map.begin(); it != uniq_query_map.end(); it++) {
        std::vector<infra::matrix> instances;
        std::vector<int> frame_nums;

        for (int i = 0; i < (it->second).size(); i++) {
            instances.push_back(querys[(it->second)[i]].GetFeature());
            frame_nums.push_back(querys[(it->second)[i]].GetFeature().height());
        }
        int min_frame_num = *min_element(frame_nums.begin(), frame_nums.end());
        it_phone_num = phone_num_dict.find(it->first);
        if(it_phone_num != phone_num_dict.end()){
            phone_num = it_phone_num->second;
        } else {
            std::cout << "can not file the phone num of keyword: " << it->first << std::endl;
            exit(1);
        }
        int state_num = phone_num * state_num_per_phone;
        if (state_num >= min_frame_num*2) {
            state_num = min_frame_num*2 -1;
            std::cout << "info: min frame num less than the state num" << std::endl;
        }
        CHMM *chmm = new CHMM(state_num, instances[0].width(), mix_num);
        //set global variance
        for (i = 0; i < state_num; i++) {
            for (j = 0; j < mix_num; j++) {
                (chmm->GetStateModel(i))->setVariance(j, global_variance);
            }
        }

        if (it->first == "like") {
            int asdfasd=1;
        }
        std::cout << it->first << std::endl;
        chmm->Train(instances, false); //here false means we don't tune the transition probs.
        std::cout << endl; 
        ofstream out(out_dir + it->first + ".mdl");
        out << *chmm;
        out.close();
    }
}


void GlobalVariance(aslp_std::Feature *features, int feature_size, double *global_variance) {
    int dim = features[0].GetFeature().width();
    double *mean = new double[dim];
    double *vars = new double[dim]; 
    double vector_num = 0;
    int i, j, k;
    for (i = 0; i < dim; i++) {
        mean[i] = 0;
        vars[i] = 0;
    }
    for (i = 0; i < feature_size; i++) {
        infra::matrix feature = features[i].GetFeature();
        //cout << feature.height() << " " << feature.width() << endl;;
        for (j = 0; j < feature.height(); j++) {
            for (k = 0; k < dim; k++) {
                mean[k] += feature(j,k);
                vars[k] += (feature(j,k)*feature(j,k));
                vector_num += 1;
            }
        }
    }

    for (i = 0; i < dim; i++) {
        mean[i] /= vector_num;
        vars[i] = vars[i]/vector_num - mean[i] * mean[i];
        global_variance[i] = vars[i];
        std::cout << mean[i] << " " << vars[i] << std::endl;
    }

    delete mean;
    delete vars;    
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

void build_phone_num_dict(std::string phone_num_dict_file, std::map< std::string, int > *phone_num_dict) {
    std::string query;
    int num;
    ifstream in(phone_num_dict_file);
    while (in >> query) {
        in >> num;
        //std::cout << query << " " << num  << std::endl;
        phone_num_dict->insert(make_pair(query, num));
    }

}

int main(int argc, char *argv[]) {    
    if(argc < 9) {
        cerr<<"USAGE: query_dir query_list_file phone_num_dict feature_type do_mvn state_num_per_phone mix_num result_dir" << endl;
        return EXIT_FAILURE;
    }
    
    std::string query_dir;
    std::string query_list_file;
    std::string feature_type;
    std::string out_dir;
    std::string phone_num_dict_file;
    int do_mvn;
    int state_num_per_phone;
    int mix_num;

    query_dir = string(argv[1]);
    query_list_file = string(argv[2]);
    phone_num_dict_file = string(argv[3]);
    feature_type = string(argv[4]);
    do_mvn = atoi(argv[5]);
    state_num_per_phone = atoi(argv[6]);
    mix_num = atoi(argv[7]);
    out_dir = string(argv[8]);

    StringVector query_list;
    query_list.read(query_list_file);

    
    int query_size = query_list.size();   
    // read test set
    aslp_std::Feature* querys = new aslp_std::Feature[query_size];
    
    ReadData(querys, query_dir, query_list, query_size, feature_type);
    NormalizeFeature(querys, query_size);    
    std::vector<std::string> query_id_splits;
    std::map< std::string, std::vector<int> > uniq_query_map;
    for (int i=0; i < query_size; i++) {
        std::string query_id = querys[i].GetFeatureId();
        query_id_splits = aslp_std::Split(query_id, "_");
        string keyword = query_id_splits[0];
        if (uniq_query_map.count(keyword) > 0) {
            uniq_query_map[keyword].push_back(i);
        } else {
            std::vector<int> instance_ids;
            uniq_query_map.insert(std::pair< std::string, std::vector<int> >(keyword, instance_ids) );
            uniq_query_map[keyword].push_back(i);
        }
    }

    std::map< std::string, int > phone_num_dict;
    build_phone_num_dict(phone_num_dict_file, &phone_num_dict);

    int dim = querys[0].GetFeature().width();

    double *global_variance = new double[dim];
    GlobalVariance(querys, query_size, global_variance);
//    for (int i = 0; i< dim; i++) {
//      global_variance[i] = 1;
//    }      
    TrainCHGMM(querys, uniq_query_map, phone_num_dict, state_num_per_phone, mix_num, global_variance, out_dir);
    
    delete [] querys;
    return EXIT_SUCCESS;

    return 0;
}
