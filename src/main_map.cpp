#include <chrono>
#include <ext/pb_ds/assoc_container.hpp>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "shared_boost_map.h"
#include "shared_stl_map.h"

template <typename M>
void bench(M& m) {
  int size = 1000000;
  // Insert data in the map
  for (int i = 0; i < size; ++i) {
    m.insert({i, double(i)});
  }
  std::vector<int> query;
  for (int i = 0; i < size; ++i) {
    query.emplace_back(rand() % size);
  }
  std::vector<double> ans;
  ans.reserve(size);
  auto t1 = std::chrono::high_resolution_clock::now();
  for (int q : query) {
    double res = m.find(q)->second;
    ans.push_back(res);
  }
  auto t2 = std::chrono::high_resolution_clock::now();
  std::cout << "query size " << ans.size() << " time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
}

int main() {
  std::unordered_map<int, double> m1;
  boost::unordered_map<int, double> m2;
  __gnu_pbds::cc_hash_table<
      int, double, std::hash<int>, std::equal_to<int>, __gnu_pbds::direct_mask_range_hashing<>,
      __gnu_pbds::hash_standard_resize_policy<__gnu_pbds::hash_exponential_size_policy<>, __gnu_pbds::hash_load_check_resize_trigger<>, true>>
      m3;
  SharedMap m_holder;
  SharedSTLMap stl_m_holder;
  std::cout << "\nbench stl map" << std::endl;
  bench(m1);
  std::cout << "\nbench boost map" << std::endl;
  bench(m2);
  std::cout << "\nbench pbds map" << std::endl;
  bench(m3);
  std::cout << "\nbench boost map in shared mem" << std::endl;
  bench(*m_holder.mymap);
  std::cout << "\nbench stl map in shared mem" << std::endl;
  bench(*stl_m_holder.mymap);
  return 0;
}