#!/usr/bin/bash
stage=1
fea_type="sbnf3"
do_mvn=0;
feat_dir=/home/disk1/jyhou/feats/XiaoYing_STD
phone_num_dict_file=info/keyword_phone_num.txt

hmm_type="h2" #h1, h2
global_variance="1" #0, 1
transition_prob="0" #0, 1
#for x in keywords_20_60 keywords_60_100 keywords_native; # keywords_native_95_100
for x in keywords_60_100_50; # keywords_native_95_100
do
    for tempalte_num in 32;
    do
        for random_num in 4;
        do
        {
            for state_num_per_phone in 9; do
            {
                for mix_num in 1; do
                {
                    keyword_dir="$feat_dir/$x/"
                    model_dir="$feat_dir/hta_${x}_${tempalte_num}_${random_num}_${hmm_type}_${global_variance}_${transition_prob}_${state_num_per_phone}_${mix_num}/"
                    keyword_list_dir="${feat_dir}/list/"
                    keyword_list_basename="${x}_${tempalte_num}_${random_num}.list"
                    keyword_list_file="${keyword_list_dir}${keyword_list_basename}"
                    mkdir -p $model_dir

                    #keyword_list_file="keyword_debug.list"
                    if [ ! -f ${keyword_list_file} ]; then
                        echo "ERROR: can not find the keyword list file: $keyword_list_file"
                    fi

                    if [ $stage -le 1 ]; then
                        echo "./KMeans-GMM-HMM/train_keyword_model $keyword_dir ${keyword_list_file} $phone_num_dict_file $fea_type $do_mvn ${state_num_per_phone} ${mix_num} ${model_dir}"
                        ./KMeans-GMM-HMM/train_keyword_model $keyword_dir ${keyword_list_file} $phone_num_dict_file $fea_type $do_mvn ${state_num_per_phone} ${mix_num} ${model_dir}
                    fi
                }
                done
            }
            done
        } &
        done
        wait
    done
done

