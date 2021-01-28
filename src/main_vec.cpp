#include <chrono>
#include <iostream>
#include <vector>

#include "shared_vec.h"

template <typename V>
void bench(V& vec) {
  int size = 1000000;
  vec.reserve(size);

  for (int i = 0; i < size; ++i) {
    vec.push_back(double(i));
  }
  std::vector<int> query;
  for (int i = 0; i < size; ++i) {
    query.emplace_back(rand() % size);
  }
  auto t1 = std::chrono::high_resolution_clock::now();
  std::vector<double> ans;
  ans.reserve(size);
  for (int q : query) {
    double res = vec.at(q);
    ans.push_back(res);
  }
  auto t2 = std::chrono::high_resolution_clock::now();
  std::cout << "query size " << ans.size() << " time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
}

int main() {
  std::vector<double> vec;
  SharedVec shared_vec_holder;
  bench(vec);
  bench(*shared_vec_holder.myvec);
  return 0;
}