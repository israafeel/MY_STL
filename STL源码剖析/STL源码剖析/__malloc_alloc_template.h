#include<stdlib.h>
typedef void(*handler_ptr)();
//Ò»¼¶ÄÚ´æÅäÖÃÆ÷
class __malloc_alloc_template{
private:
	static void * oom_malloc(size_t n);
	static void * oom_realloc(void * p, size_t n);
	static handler_ptr __malloc_alloc_oom_handler;
public:
	class __BAD_ALLOC{};
	static void * allocate(size_t n);
	static void deallocate(void *p);
	static void * reallocate(void *p, size_t new_sz);
	static handler_ptr set_malloc_handler(handler_ptr);
};
typedef __malloc_alloc_template malloc_alloc;