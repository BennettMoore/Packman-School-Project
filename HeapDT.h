// 
// file: HeapDT.h declares the interface to a Heap Data Type.
//
// @author CS @ RIT.EDU
// 
// // // // // // // // // // // // // // // // // // // // // // // // 

#ifndef HEAPDT_H
#define HEAPDT_H

/// <h2>HeapDT: Notes on the Heap Data Type</h2>
///
/// <b>The Heap type name is a pointer to a type that is opaque to clients.</b>
/// By convention, the public abstract data type name uses UpperMixedCase.
///
/// The HeapDT is a generic collection type.
/// While each item is stored as a <code>void *</code>, 
/// each item must be comparable for ordering based on an ordering function.
///
/// The functions documented are the public interface to the Heap data type.
/// These functions are the only way to operate on items in a heap instance.
///
/// Wherever a function has a precondition, and
/// wherever that precondition is violated by client code, and
/// wherever that precondition violation is detectable by the code,
/// then the function will assert failure and abort.
///
/// Similarly, wherever a function has a postcondition, and
/// wherever that postcondition is violated by client code,
/// then the function will assert failure and abort.

typedef struct heap_s * Heap;

/// hdt_create constructs an empty Heap with a given, initial capacity.
/// A Heap's capacity must be able to grow 
/// to accommodate an arbitrarily large number of items.
/// When the number of items reaches the capacity,
/// the heap must increase its capacity to enable further growth.
///
/// @param capacity : the maximum number of items the Heap can currently hold
///
/// @param cmp_fun : function determines the ordering operation of the Heap
///        and is applied to items in the Heap payload.
///        A function that implements '<' or '<=' produces a 'min-heap',
///        and one that implements '>' or '>=' produces a 'max-heap'.
///
/// @param print_item : function iterates through the heap and prints
///        each payload item in the Heap to the provided output stream.
///        The print function is largely for debugging.
///
/// @return a Heap instance, a pointer to the Heap created
///
/// @pre cmp_fun and print_item must not be NULL.
///
/// @post the Heap is not NULL (i.e. there was enough storage).
///
/// <h3>HeapDT Use Protocol Example Template</h3>
///
/// A client application includes the header file and
/// defines item comparison and item print functions for the payload type.
/// The use protocol uses the slash(/) surrounding arguments
/// to indicate these are application specific.
/// Here is the protocol:
///
/// <ul><li>
/// <pre>
/// // example constructs a heap instance using dynamic memory allocation.
/// Heap myheap = hdt_create( /capacity/, /compare/, /print/);
/// </pre>
/// </li><li>
/// <pre>
/// // example insertion call:
/// hdt_insert_item( myheap, /some item reference/ );
/// </pre>
/// </li><li>
/// <pre>
/// // use other heap functions to insert, remove, etc.
/// </pre>
/// </li><li>
/// <pre>
/// // before deletion, client must make the heap empty.
/// </pre>
/// </li><li>
/// <pre>
/// // delete and de-allocate a heap instance when finished.
/// void hdt_destroy( myheap ); // myheap will no longer be valid.
/// </pre>
/// </li></ul>
///

Heap hdt_create( size_t capacity
               , int (*cmp_fun)( const void * lhs, const void * rhs )
               , void (*print_item)( const void * item, FILE * outfp ) ) ;


/// hdt_destroy deletes all dynamic Heap storage.
///
/// @param a_heap : the subject Heap
///
/// @pre a_heap is a valid Heap data type reference.
/// @pre client has previously removed and deleted all items that
///      use dynamic memory allocation; otherwise memory will leak.
///
/// @post the a_heap reference is no longer valid.

void hdt_destroy( Heap a_heap );

///
/// @param a_heap : the subject Heap
/// @return the current number of active items in the Heap.
///
/// @pre a_heap is a valid Heap data type reference.

size_t hdt_size( Heap a_heap );

/// hdt_top peeks at the top and
/// returns an immutable pointer to the topmost item in the Heap.
///
/// @param a_heap : the subject Heap
/// @return an immutable pointer to the topmost item.
///
/// @pre a_heap is a non-empty, valid Heap data type instance.
///
/// @post the internal state of a_heap has not changed.

const void * hdt_top( const Heap a_heap );

/// hdt_remove_top removes the topmost item from the Heap.
///
/// @param a_heap : the subject Heap
/// @return a pointer to the removed item.
///
/// @pre a_heap is a non-empty, valid Heap data type instance.
///
/// @post client assumes ownership of the item removed.

void * hdt_remove_top( Heap a_heap );

/// hdt_insert_item inserts an item into the Heap.
///
/// @param a_heap : the subject Heap
/// @param item : the item to insert into the Heap
///
/// @pre a_heap is a valid Heap data type instance.
///
/// @post the Heap assumes (temporary) ownership/custody of the item, and
/// the client must not delete the memory while the Heap owns it.

void hdt_insert_item( Heap a_heap, const void * item );

/// hdt_print prints the Heap items onto the specified output file stream
/// using the Heap::print_item function
///
/// @param a_heap : the subject Heap
/// @param outfp : the output file stream
///
/// @pre a_heap is a valid Heap data type instance.
///
/// @post the internal state of a_heap has not changed.
/// @see Heap::print_item

void hdt_print( Heap a_heap, FILE * outfp );


#endif // HEAPDT_H

