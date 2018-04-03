#include"__sub_alloc_template.h"
template<bool threads>
char *__sub_alloc_template<threads>::start_free = NULL;
template<bool threads>
char *__sub_alloc_template<threads>::end_free = NULL;
template<bool threads>
size_t __sub_alloc_template<threads>::heap_size = 0;


template<bool threads>
void * __sub_alloc_template<threads>::refill(size_t n){
	int nobjs = __N_OBJECTS;
	char * chunk = chunk_alloc(n, nobjs);
	obj * volatile * my_free_list = free_list + FREELIST_INDEX(n);
	obj *result, *current_obj, *next_obj;
	if (1 == nobjs) return static_cast<void *>(chunk);
	result = static_cast<obj *>(chunk);
	*my_free_list = next_obj = static_cast<obj *>(chunk + n);
	for (int i = 1;; ++i){
		current_obj = next_obj;
		next_obj = static_cast<obj *>(static_cast<char *>(next_obj) + n);
		if (nobjs - 1 == i){
			current_obj->free_list_link = NULL;
			break;
		}
		else
			current_obj->free_list_link = next_obj;
	}
	return static_cast<void *>(result);
}

template<bool threads>
char * __sub_alloc_template<threads>::chunk_alloc(size_t size, int& nobjs){
	char *result;
	size_t total_bytes = size * nobjs;
	size_t bytes_left = end_free - start_free;
	if (bytes_left >= total_bytes){
		result = start_free;
		start_free += total_bytes;
		return result;
	}
	else if (bytes_left >= size){
		nobjs = bytes_left / size;
		total_bytes = size * nobjs;
		result = start_free;
		start_free += total_bytes;
		return result;
	}
	else{
		size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 1);
		if (bytes_left > 0){
			obj * my_free_list = free_list + FREELIST_INDEX(bytes_left);
			(static_cast<obj*>(start_free))->free_list_link = *my_free_list;
			*my_free_list = static_cast<obj*>(start_free);
		}
		start_free = static_cast<char *>(malloc(bytes_to_get));
		if (NULL == start_free){
			obj * my_free_list, *p;
			for (int i = size; i <= __MAX_BYTES; i += __ALIGN){
				my_free_list = free_list + FREELIST_INDEX(i);
				p = *my_free_list;
				if (NULL != p){
					*my_free_list = p->free_list_link;
					start_free = static<char *>p;
					end_free = start_free + i;
					return(chunk_alloc(size, nobjs));
				}
			}
			end_free = NULL;
			start_free = static_cast<char *>(malloc(bytes_to_get));
		}
		heap_size = bytes_to_get;
		end_free = start_free + heap_size;
		return (chunk_alloc(size, nobjs));
	}
}