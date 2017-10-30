#!/usr/bin/bash
stage=1
fea_type="sbnf1"
if [ $fea_type = "sbnf1" ]; then
    distance_type="cosine"
    do_mvn=1;
fi

feat_dir=/home/disk1/jyhou/feats/XiaoYing_STD
phone_num_dict_file=info/keyword_phone_num.txt
#for x in keywords_20_60 keywords_60_100 keywords_native; # keywords_native_95_100
for x in keywords_60_100_50; # keywords_native_95_100
do
    keyword_dir="$feat_dir/$x/"
    model_dir="./keyword_model/"
    mkdir -p $model_dir
    keyword_list_dir="/home/disk1/jyhou/feats/XiaoYing_STD/list/"
    keyword_list_basename="${x}_32_5.list.tr"
    keyword_list_file="${keyword_list_dir}${keyword_list_basename}"

    #keyword_list_file="keyword_debug.list"
    if [ ! -f ${keyword_list_file} ]; then
        echo "ERROR: can not find the keyword list file: $keyword_list_file"
    fi

    if [ $stage -le 1 ]; then
        echo "./KMeans-GMM-HMM/train_keyword_model $keyword_dir ${keyword_list_file} $phone_num_dict_file $fea_type $do_mvn $model_dir"
              ./KMeans-GMM-HMM/train_keyword_model $keyword_dir ${keyword_list_file} $phone_num_dict_file $fea_type $do_mvn 9 1 $model_dir
    fi

done

