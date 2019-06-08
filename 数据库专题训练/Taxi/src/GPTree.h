#ifndef __GPT__
#define __GPT__

#include <vector>

void gpt_build();
int gpt_getsize();
int gpt_search(int S, int T);
int gpt_search_catch(int S, int T, int bound=0x3fffffff);
int gpt_find_path(int S, int T, std::vector<int> &order);

std::vector<int> gpt_KNN(int S, int K, const std::vector<int> &T);
std::vector<int> gpt_KNN_bound(int S, int K, const std::vector<int> &T, int bound=0x3fffffff);
std::vector<int> gpt_Range(int S, int R, const std::vector<int> &T);

#endif