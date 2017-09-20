#include <iostream>

#include "CHMM.h"
#include "infra.h"
#include "htkfile.h"
#include "dataset.h"
#include "feature.h"
#include "util.h"

#define STATE_PER_PHONE 3
#define MIX_NUM 2
using namespace std;

void TrainCHGMM (aslp_std::Feature *querys, 
     std::map< std::string, std::vector<int> > &uniq_query_map, 
     std::map< std::string, int > &phone_num_dict, string out_dir) {
   
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
        CHMM *chmm = new CHMM(phone_num*STATE_PER_PHONE, instances[0].width(), MIX_NUM);
        std::cout << it->first << std::endl;
        chmm->Train(instances, false); //here false means we don't tune the transition probs.
        std::cout << endl; 
        ofstream out(out_dir + it->first + ".mdl");
        out << *chmm;
        out.close();
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
    if(argc < 7) {
        cerr<<"USAGE: query_dir query_list_file phone_num_dict feature_type do_mvn result_dir" << endl;
        return EXIT_FAILURE;
    }
    
    std::string query_dir;
    std::string query_list_file;
    std::string feature_type;
    std::string out_dir;
    std::string phone_num_dict_file;
    int do_mvn;

    query_dir = string(argv[1]);
    query_list_file = string(argv[2]);
    phone_num_dict_file = string(argv[3]);
    feature_type = string(argv[4]);
    do_mvn = atoi(argv[5]);
    out_dir = string(argv[6]);

    StringVector query_list;
    query_list.read(query_list_file);

    
    int query_size = query_list.size();   
    // read test set
    aslp_std::Feature* querys = new aslp_std::Feature[query_size];
    
    ReadData(querys, query_dir, query_list, query_size, feature_type);
    if (do_mvn) {
        MVN(querys, query_size);
    }
    
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
    TrainCHGMM(querys, uniq_query_map, phone_num_dict, out_dir);    
    
    delete [] querys;
    return EXIT_SUCCESS;

    return 0;
}
