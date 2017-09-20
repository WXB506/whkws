import numpy as np
import sys
import random

def build_keyword_dict(keyword_list_file):
    keyword_dict={}
    for line in open(keyword_list_file).readlines():
        keyword = line.strip().split("_")[0]
        if not  keyword_dict.has_key(keyword):
            keyword_dict[keyword]=[]
        keyword_dict[keyword].append(line.strip())
    return keyword_dict
        
if __name__=="__main__":
    if (len(sys.argv) < 4):
        print("USAGE:python %s keyword_list_file train_list_file dev_list_file"%sys.argv[0])
        exit(1)

    keyword_dict = build_keyword_dict(sys.argv[1])
    fid_tr = open(sys.argv[2], "w")
    fid_cv = open(sys.argv[3], "w")
    for keyword in keyword_dict.keys():
        random.shuffle(keyword_dict[keyword])
        for i in range(2):
            fid_cv.writelines(keyword_dict[keyword][i]+"\n")

        for i in range(2, len(keyword_dict[keyword])):
            fid_tr.writelines(keyword_dict[keyword][i]+"\n")

    fid_tr.close()
    fid_cv.close()
