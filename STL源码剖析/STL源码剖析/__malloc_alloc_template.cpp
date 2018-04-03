#include"__malloc_alloc_template.h"
handler_ptr __malloc_alloc_template::__malloc_alloc_oom_handler = NULL;

void * __malloc_alloc_template::allocate(size_t n){
	void * result = malloc(n);
	if (NULL == result)
		result = oom_malloc(n);
	return result;
}

void __malloc_alloc_template::deallocate(void *p){
	free(p);
}

void * __malloc_alloc_template::reallocate(void *p, size_t new_sz){
	void *result = realloc(p, new_sz);
	if (NULL == result)
		result = oom_realloc(p, new_sz);
	return result;
}

handler_ptr __malloc_alloc_template::set_malloc_handler(handler_ptr new_handler){
	handler_ptr old_handler = __malloc_alloc_oom_handler;
	__malloc_alloc_oom_handler = new_handler;
	return old_handler;
}

void * __malloc_alloc_template::oom_malloc(size_t n){
	handler_ptr malloc_handler = __malloc_alloc_oom_handler;
	void *result;
	for (;;){
		if (NULL == malloc_handler)
			throw __BAD_ALLOC();
		(*malloc_handler)();
		result = malloc(n);
		if (result)
			return result;
	}
}

void * __malloc_alloc_template::oom_realloc(void *p, size_t n){
	handler_ptr malloc_handler = __malloc_alloc_oom_handler;
	void *result;
	for (;;){
		if (NULL == malloc_handler)
			throw __BAD_ALLOC();
		(*malloc_handler)();
		result = realloc(p, n);
		if (result)
			return result;
	}
}