#include<stdlib.h>
enum{ __N_OBJECTS = 20 };
enum{ __ALIGN = 8 };
enum{ __MAX_BYTES = 128 };
enum{ __NFREELISTS = __MAX_BYTES / __ALIGN };
template<bool threads>
class __sub_alloc_template{
private:
	union obj{
		union obj *free_list_link;
		char client_data[1];
	};
public:
	static void * allocate(size_t n){
		obj * my_free_list = free_list[FREELIST_INDEX(n)];
		obj * result = my_free_list;
		if (NULL == result){
			void *r = refill(ROUND_UP(n));
			return r;
		}
		my_free_list = result->free_list_link;
		return static_cast<void *>(result);
	}
	static void * deallocate(void *p, size_t n){
		obj * q = static_cast<obj *>(p);
		obj * my_free_list = free_list[FREELIST_INDEX(n)];
		q->free_list_link = *my_free_list;
		*my_free_list = q;
	}
	static void * reallocate(void *p, size_t old_sz, size_t new_sz);
private:
	static size_t ROUND_UP(size_t bytes){
		return (bytes + __ALIGN - 1) & ~(__ALIGN - 1);
	}
	static size_t FREELIST_INDEX(size_t bytes){
		return ((bytes + __ALIGN - 1) / __ALIGN - 1);
	}
	static char * chunk_alloc(size_t size, int& nobjs);
	static void * refill(size_t n);
	static void init_list(){
		for (int i = 0; i < __NFREELISTS; ++i)
			free_list[i] = NULL;
	}
private:
	static obj * free_list[__NFREELISTS];
	static char *start_free;
	static char *end_free;
	static size_t heap_size;
};