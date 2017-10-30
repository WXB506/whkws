#!/usr/bin/bash
stage=1
core_num=20
feat_dir="/home/disk1/jyhou/feats/XiaoYing_STD" 
keyword_list_dir="./info/"
data_list_dir="/home/disk1/jyhou/feats/XiaoYing_STD/list/"

text_file="/home/disk1/jyhou/my_egs/swbd_xy_egs/info/text_fixed_tail_500"
syllable_num_file="info/keyword_syllable_num.txt" 

fea_type="sbnf3"
do_mvn=0;

#for x in keywords_20_60 keywords_60_100 keywords_native keywords_native-keywords_60_100;
hmm_type="h2" #h1, h2
transition_prob="0" #0, 1
for keyword_type in keywords_60_100_50
do
    for tempalte_num in 12 16 24 32;
    do
        for random_num in `seq 5`;
        do
            for global_variance in 1; do
                for state_num_per_phone in 9; do
                    for mix_num in 1; do
                        keyword_dir="$feat_dir/hta_${keyword_type}_${tempalte_num}_${random_num}_${hmm_type}_${global_variance}_${transition_prob}_${state_num_per_phone}_${mix_num}/"
                        keyword_list_basename="unigram.list"
                        keyword_list_file=${keyword_list_dir}${keyword_list_basename}
                        
                        #keyword_list_file="keyword_debug.list"
                        if [ ! -f ${keyword_list_file} ]; then
                            echo "ERROR: can not find the keyword list file: $keyword_list_file"
                        fi
                        
                        if [ $stage -le 1 ]; then
                            mkdir -p ./tmp
                            python script/split.py ${keyword_list_file} ./tmp/ ${core_num}
                            
                            for x in data_15_30 data_40_55 data_65_80;
                            do
                                test_dir="$feat_dir/$x/"
                                test_list_file="${data_list_dir}/${x}.list"
                                result_dir=${keyword_dir}hta_i_viterbi_${x}_${fea_type}/;
                                mkdir -p $result_dir
                                for i in `seq $core_num`; do
                                {
                                    #echo "./STD_v3/i_dtw_std $keyword_dir ./tmp/${keyword_list_basename}${i} $test_dir $test_list_file $fea_type $distance_type $do_mvn $result_dir"
                                          KMeans-GMM-HMM/i_viterbi_std $keyword_dir ./tmp/${keyword_list_basename}${i} $test_dir $test_list_file $fea_type $do_mvn $result_dir
                                } & 
                                done
                                wait
                            done
                        rm -r ./tmp
                        fi
                        
                        if [ $stage -le 2 ]; then
                            for x in data_15_30 data_40_55 data_65_80;
                            do
                               result_dir=${keyword_dir}hta_i_viterbi_${x}_${fea_type}/;
                               test_list_file="${data_list_dir}/${x}.list"
                               echo $result_dir
                               python ./script/evaluate.py $result_dir $keyword_list_file $test_list_file $text_file $syllable_num_file
                            done
                        fi
                    done
                done
            done
        done
    done
done
