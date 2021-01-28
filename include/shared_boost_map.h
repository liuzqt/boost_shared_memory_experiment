#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/flat_map.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/unordered_map.hpp>  //boost::unordered_map

namespace ipc = boost::interprocess;

class SharedMap {
  static constexpr auto SHM_NAME = "SharedMap";

  typedef int KeyType;
  typedef double MappedType;
  typedef std::pair<const int, double> ValueType;

  // Alias an STL compatible allocator of for the map.
  // This allocator will allow to place containers
  // in managed shared memory segments
  typedef ipc::allocator<ValueType, ipc::managed_shared_memory::segment_manager> ShmemAllocator;

  // Alias a map of ints that uses the previous STL-like allocator.
  // Note that the third parameter argument is the ordering function
  // of the map, just like with std::map, used to compare the keys.
  typedef boost::unordered_map<KeyType, MappedType, boost::hash<KeyType>, std::equal_to<KeyType>, ShmemAllocator> MyHashMap;

  ipc::managed_shared_memory segment;

 public:
  MyHashMap* mymap;
  
  SharedMap() : segment(ipc::open_or_create, SHM_NAME, 1024 * 1024 * 100) {
    mymap = segment.construct<MyHashMap>("MyHashMap")  // object name
            (3, boost::hash<KeyType>(), std::equal_to<KeyType>(),
             segment.get_allocator<ValueType>());  // allocator instance
  }

  ~SharedMap() {
    struct shm_remove {
      shm_remove() { ipc::shared_memory_object::remove(SHM_NAME); }
      ~shm_remove() { ipc::shared_memory_object::remove(SHM_NAME); }
    } remover;
  }
};