model_dir=keyword_model/
model=actually.mdl
test_dir=/home/disk1/jyhou/feats/XiaoYing_STD/keywords_60_100/
test_list=./list/keywords_60_100_10_1.list.cv
feature_type=sbnf1
do_mvn=1
result_dir=loglikes/

echo "./KMeans-GMM-HMM/test_keyword_model $model_dir $model $test_dir $test_list $feature_type $do_mvn $result_dir"
     ./KMeans-GMM-HMM/test_keyword_model $model_dir $model $test_dir $test_list $feature_type $do_mvn $result_dir
