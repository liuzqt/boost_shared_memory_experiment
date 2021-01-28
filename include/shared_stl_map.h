#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <unordered_map>

namespace ipc = boost::interprocess;

class SharedSTLMap {
    static constexpr auto SHM_NAME = "SharedSTLMap";

    template <typename T> using Alloc = ipc::allocator<T, ipc::managed_shared_memory::segment_manager>;
    template <typename K, typename V, typename KH = std::hash<K>, typename KEq = std::equal_to<K> >
    using HashMap = std::unordered_map<K, V, KH, KEq, Alloc<std::pair<const K, V>> >;

  ipc::managed_shared_memory segment;

 public:
  HashMap<int, double> * mymap;
  
  SharedSTLMap() : segment(ipc::open_or_create, SHM_NAME, 1024 * 1024 * 100) {
    mymap = segment.construct<HashMap<int, double>>("MyHashMap")(segment.get_segment_manager());  // allocator instance
  }

  ~SharedSTLMap() {
    struct shm_remove {
      shm_remove() { ipc::shared_memory_object::remove(SHM_NAME); }
      ~shm_remove() { ipc::shared_memory_object::remove(SHM_NAME); }
    } remover;
  }
};