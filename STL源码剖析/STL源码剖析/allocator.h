#pragma once
#include"__malloc_alloc_template.h"
#include"__sub_alloc_template.h"
#include<cstddef>	    // for ptrdiff_t size_t
namespace Toy
{
	template<class T, bool threads = false>
	class alloc{
	public:
		typedef T value_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
	public:
		static pointer allocate(size_t n){
			return (n == 0) ? NULL : static_cast<pointer>(_allocate( n * sizeof(T)));
		}
		static pointer allocate(){
			return static_cast<pointer>(_allocate(sizeof(T)));
		}
		static void deallocate(void *p, size_t n){
			if (NULL != p && n != 0)
				_deallocate(p, n * sizeof(T));
			else
				throw __POINTER_ERROR();
		}
		static void deallocate(void *p){
			if (NULL != p)
				_deallocate(p, sizeof(T));
			else
				throw __POINTER_ERROR();
		}
	private:
		class __POINTER_ERROR{};
		static void * _allocate(size_t n){
#ifndef __UNUSE_SUB_ALLOC
			if (n <= static_cast<size_t>(__MAX_BYTES))
				return __sub_alloc_template<threads>::allocate(n);
			else
#endif
				return malloc_alloc::allocate(n);
		}
		static void _deallocate(void *p, size_t n){
#ifndef __UNUSE_SUB_ALLOC
			if (n <= static_cast<size_t>(__MAX_BYTES))
				__sub_alloc_template<threads>::deallocate(p, n);
			else
#endif
				malloc_alloc::deallocate(p);
		}
	};	
}
