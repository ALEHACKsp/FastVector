#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

template<typename T>
class FVector
{
public:
using iterator = T*;
using const_iterator = const T*;
FVector(size_t capacity) : pointer( (T*)malloc( sizeof( T ) * capacity ) ), write( pointer ), end( pointer + capacity )
{
assert( capacity != 0 );
}

FVector( const FVector& ) = delete;
FVector( FVector&& ) = delete;

~FVector()
{
free( pointer );
}

FVector& operator=( const FVector& ) = delete;
FVector& operator=( FVector&& ) = delete;

bool empty() const { return pointer == write; }
size_t size() const { return write - pointer; }

T* data() { return pointer; }
const T* data() const { return pointer; };

T* begin() { return pointer; }
const T* begin() const { return pointer; }
T* end() { return write; }
const T* end() const { return write; }

T& front() { assert( !empty() ); return pointer[0]; }
const T& front() const { assert( !empty() ); return pointer[0]; }

T& back() { assert( !empty() ); return write[-1]; }
const T& back() const { assert( !empty() ); return write[-1]; }

T& operator[]( size_t idx ) { return pointer[idx]; }
const T& operator[]( size_t idx ) const { return pointer[idx]; }

T* push_next()
{
if( write == end ) AllocFunc();
return write++;
}

T* prepare_next()
{
if( write == end ) AllocFunc();
return write;
}

void commit_next()
{
write++;
}

void clear()
{
write = pointer;
}

void swap( FVector& vec )
{
const auto ptr1 = pointer;
const auto ptr2 = vec.pointer;
const auto write1 = write;
const auto write2 = vec.write;
const auto end1 = end;
const auto end2 = vec.end;

pointer = ptr2;
vec.pointer = ptr1;
write = write2;
vec.write = write1;
end = end2;
vec.end = end1;
}

__declspec(noinline) void AllocFunc()
{
const auto cap = ( end - pointer ) * 2;
const auto size = m_write - pointer;
T* ptr = (T*)malloc( sizeof( T ) * cap );
memcpy( ptr, pointer, size * sizeof( T ) );
free( pointer );
pointer = ptr;
write = pointer + size;
end = pointer + cap;
}

private:
T* pointer;
T* write;
T* end;
};
