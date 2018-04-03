#ifndef __MY_VECTOR_
#define __MY_VECTOR_
#include"allocator.h"
#include<cstddef>
namespace Toy{
template<class T, class Alloc = alloc>
class vector{
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef pointer iterator;
	typedef value_type reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
protected:
	typedef alloc<value_type> data_allocator;
	iterator start;
	iterator finish;
	iterator end_of_storage;
public:
	iterator begin(){ return start; T* = data_allocator::allocate(); }
	iterator end(){ return finish; }
	size_type size() const { return size_type(finish - start); }
	size_type capacity() const{ return size_type(end_of_storage - start); }
	bool empty(){ return (start == finish); }
	reference operator[](size_type n){ return *(start + n); }
};
}
#endif