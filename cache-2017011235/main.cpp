#include <iostream>
#include <string>
#include "Cache.h"
#include "util.h"

using namespace std;

float test(Cache &cache, string file_prefix, int log_ways, int log_offset, int wh, int wm, int rs) {
    cache.SetShape(log_ways, log_offset, rs);
    string input_name = "../trace/" + file_prefix + ".trace";
    int ways = log2val(log_ways);
    int block_size = log2val(log_offset);
    string whs = (wh == 0 ? "wb" : "wt");
    string wms = (wm == 0 ? "wa" : "wn");
    string rss;
    switch(rs) {
        case 0:
            rss = "rand";
            break;
        case 1:
            rss = "lru";
            break;
        case 2:
            rss = "bitree";
            break;
        case 3:
            rss = "p-lru";
            break;
        default:
            break;
    }
    string output_name = "../log/" + file_prefix + "_" + to_string(ways) + "_" + to_string(block_size) + "_"
            + whs + "_" + wms + "_" + rss + ".log";
    FILE *input_file, *output_file;
    input_file = fopen(input_name.data(), "r");
    output_file = fopen(output_name.data(), "w");
    fclose(output_file);
    output_file = fopen(output_name.data(), "a+");

    int total = 0, miss = 0;
    char method;
    uint64_t v_addr, tag;  // v_addr(64:17-log_ways]
    int index;  // v_addr(17-log_ways:log_offset]
    srandom(0);
    for (; !feof(input_file);) {
        if (file_prefix == "bodytrack_1m" || file_prefix == "canneal.uniq" || file_prefix == "streamcluster") {
            fscanf(input_file, "%lx\n", &v_addr);
            method = 'r';
        } else {
            fscanf(input_file, "%c %lx\n", &method, &v_addr);
        }
        tag = v_addr >> (17 - log_ways);
        index = (v_addr >> log_offset) & Mask[17 - log_ways - log_offset];
        if (method == 'r' || method == 'l') {
            if (cache.cacheGroup[index].Read(log_ways, tag, rs)) {
                fprintf(output_file, "Hit\n");
            } else {
                fprintf(output_file, "Miss\n");
                miss++;
            }
        } else if (method == 'w' || method == 's') {
            if (cache.cacheGroup[index].Write(log_ways, tag, wh, wm, rs)) {
                fprintf(output_file, "Hit\n");
            } else {
                fprintf(output_file, "Miss\n");
                miss++;
            }
        } else {}
        total++;
    }
    fclose(input_file);
    fclose(output_file);
    float miss_rate = (float) miss / (float) total;
    if (file_prefix.length() < 8)
        printf("%s\t\t%d\t%d\t%s\t%s\t%s\t%f\n", file_prefix.data(), ways, block_size, whs.data(), wms.data(), rss.data(), miss_rate);
    else
        printf("%s\t%d\t%d\t%s\t%s\t%s\t%f\n", file_prefix.data(), ways, block_size, whs.data(), wms.data(), rss.data(), miss_rate);
    return miss_rate;
}

void beautiful_print(string filename, int log_ways, int log_offset, int wh, int wm, int rs, float *miss_rate) {
    int block_size = log2val(log_offset);
    int ways = log2val(log_ways);
    string mapping;
    if (log_ways == 0) {
        mapping = "直接映射";
    } else if (log_ways < 17 - log_offset) {
        mapping = to_string(ways) + "-way组相联";
    } else {
        mapping = "全相联";
    }
    string wss;
    if (!wh && !wm) {
        wss = "写分配+写回";
    } else if (!wh && wm) {
        wss = "写不分配+写回";
    } else if (wh && !wm) {
        wss = "写分配+写直达";
    } else {
        wss = "写不分配+写直达";
    }
    string rss;
    switch(rs) {
        case 0:
            rss = "随机";
            break;
        case 1:
            rss = "LRU";
            break;
        case 2:
            rss = "二叉树";
            break;
        case 3:
            rss = "Protected LRU";
            break;
        default:
            break;
    }
    FILE *output_file = fopen(filename.data(), "a");
    fprintf(output_file, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f, ,%d,%s,%s,%s\n", miss_rate[0], miss_rate[1], miss_rate[2],
            miss_rate[3], miss_rate[4], miss_rate[5], miss_rate[6], miss_rate[7], miss_rate[8], miss_rate[9], block_size,
            mapping.data(), wss.data(), rss.data());
    fclose(output_file);
}

int main(int argc, char *argv[]) {
    Cache cache;
    if (argc == 7) {
        string file_prefix = string(argv[1]);
        int log_ways = atoi(argv[2]);
        int log_offset = atoi(argv[3]);
        int wh = atoi(argv[4]);
        int wm = atoi(argv[5]);
        int rs = atoi(argv[6]);
        test(cache, file_prefix, log_ways, log_offset, wh, wm, rs);
        return 0;
    } else if (argc == 2) {
        string experiment = string(argv[1]);
        if (experiment == "replace") {
            string filename = "../replace.csv";
            FILE *output_file = fopen(filename.data(), "w");
            fprintf(output_file, "astar,bodytrack_1m,bzip2,canneal.uniq,gcc,mcf,perlbench,streamcluster,swim,twolf, ,块大小,映射方式,写策略,替换策略\n");
            fclose(output_file);
            for (int rs = 0; rs <= 3; ++rs) {
                float miss_rate[10];
                miss_rate[0] = test(cache, "astar", 3, 3, 0, 0, rs);
                miss_rate[1] = test(cache, "bodytrack_1m", 3, 3, 0, 0, rs);
                miss_rate[2] = test(cache, "bzip2", 3, 3, 0, 0, rs);
                miss_rate[3] = test(cache, "canneal.uniq", 3, 3, 0, 0, rs);
                miss_rate[4] = test(cache, "gcc", 3, 3, 0, 0, rs);
                miss_rate[5] = test(cache, "mcf", 3, 3, 0, 0, rs);
                miss_rate[6] = test(cache, "perlbench", 3, 3, 0, 0, rs);
                miss_rate[7] = test(cache, "streamcluster", 3, 3, 0, 0, rs);
                miss_rate[8] = test(cache, "swim", 3, 3, 0, 0, rs);
                miss_rate[9] = test(cache, "twolf", 3, 3, 0, 0, rs);
                beautiful_print(filename, 3, 3, 0, 0, rs, miss_rate);
            }
        }
        return 0;
    }
    string filename = "../ans.csv";
    FILE *output_file = fopen(filename.data(), "w");
    fprintf(output_file, "astar,bodytrack_1m,bzip2,canneal.uniq,gcc,mcf,perlbench,streamcluster,swim,twolf, ,块大小,映射方式,写策略,替换策略\n");
    fclose(output_file);
    for (int log_offset = 3; log_offset <= 6; ++log_offset) {
        if (log_offset == 4) continue;
        for (int log_ways = 0; log_ways <= 17 - log_offset; ++log_ways) {
            if (log_ways == 0 || log_ways == 2 || log_ways == 3 || log_ways == 17 - log_offset) {
                float miss_rate[10];
                miss_rate[0] = test(cache, "astar", log_ways, log_offset, 0, 0, 1);
                miss_rate[1] = test(cache, "bodytrack_1m", log_ways, log_offset, 0, 0, 1);
                miss_rate[2] = test(cache, "bzip2", log_ways, log_offset, 0, 0, 1);
                miss_rate[3] = test(cache, "canneal.uniq", log_ways, log_offset, 0, 0, 1);
                miss_rate[4] = test(cache, "gcc", log_ways, log_offset, 0, 0, 1);
                miss_rate[5] = test(cache, "mcf", log_ways, log_offset, 0, 0, 1);
                miss_rate[6] = test(cache, "perlbench", log_ways, log_offset, 0, 0, 1);
                miss_rate[7] = test(cache, "streamcluster", log_ways, log_offset, 0, 0, 1);
                miss_rate[8] = test(cache, "swim", log_ways, log_offset, 0, 0, 1);
                miss_rate[9] = test(cache, "twolf", log_ways, log_offset, 0, 0, 1);
                beautiful_print(filename, log_ways, log_offset, 0, 0, 1, miss_rate);
            }
        }
    }
    for (int rs = 0; rs <= 2; ++rs) {
        float miss_rate[10];
        miss_rate[0] = test(cache, "astar", 3, 3, 0, 0, rs);
        miss_rate[1] = test(cache, "bodytrack_1m", 3, 3, 0, 0, rs);
        miss_rate[2] = test(cache, "bzip2", 3, 3, 0, 0, rs);
        miss_rate[3] = test(cache, "canneal.uniq", 3, 3, 0, 0, rs);
        miss_rate[4] = test(cache, "gcc", 3, 3, 0, 0, rs);
        miss_rate[5] = test(cache, "mcf", 3, 3, 0, 0, rs);
        miss_rate[6] = test(cache, "perlbench", 3, 3, 0, 0, rs);
        miss_rate[7] = test(cache, "streamcluster", 3, 3, 0, 0, rs);
        miss_rate[8] = test(cache, "swim", 3, 3, 0, 0, rs);
        miss_rate[9] = test(cache, "twolf", 3, 3, 0, 0, rs);
        beautiful_print(filename, 3, 3, 0, 0, rs, miss_rate);
    }
    for (int wm = 0; wm <= 1 ; ++wm) {
        for (int wh = 0; wh <=1 ; ++wh) {
            float miss_rate[10];
            miss_rate[0] = test(cache, "astar", 3, 3, wh, wm, 1);
            miss_rate[1] = test(cache, "bodytrack_1m", 3, 3, wh, wm, 1);
            miss_rate[2] = test(cache, "bzip2", 3, 3, wh, wm, 1);
            miss_rate[3] = test(cache, "canneal.uniq", 3, 3, wh, wm, 1);
            miss_rate[4] = test(cache, "gcc", 3, 3, wh, wm, 1);
            miss_rate[5] = test(cache, "mcf", 3, 3, wh, wm, 1);
            miss_rate[6] = test(cache, "perlbench", 3, 3, wh, wm, 1);
            miss_rate[7] = test(cache, "streamcluster", 3, 3, wh, wm, 1);
            miss_rate[8] = test(cache, "swim", 3, 3, wh, wm, 1);
            miss_rate[9] = test(cache, "twolf", 3, 3, wh, wm, 1);
            beautiful_print(filename, 3, 3, wh, wm, 1, miss_rate);
        }
    }
    return 0;
}
