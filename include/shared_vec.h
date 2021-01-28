#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

namespace ipc = boost::interprocess;

class SharedVec {
  // Define an STL compatible allocator of ints that allocates from the managed_shared_memory.
  // This allocator will allow placing containers in the segment
  typedef ipc::allocator<int, ipc::managed_shared_memory::segment_manager> ShmemAllocator;

  // Alias a vector that uses the previous STL-like allocator so that allocates
  // its values from the segment
  typedef ipc::vector<int, ShmemAllocator> MyVector;
  ipc::managed_shared_memory segment;
  ShmemAllocator alloc_inst;

 public:
  MyVector* myvec;

  SharedVec() : segment(ipc::open_or_create, "MySharedMemory", 1024 * 1024 * 100), alloc_inst(segment.get_segment_manager()) {
    myvec = segment.construct<MyVector>("MyVector")(alloc_inst);
  }

  ~SharedVec() {
    struct shm_remove {
      shm_remove() { ipc::shared_memory_object::remove("MySharedMemory"); }
      ~shm_remove() { ipc::shared_memory_object::remove("MySharedMemory"); }
    } remover;
  }
};