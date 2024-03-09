/**
 *  @brief A generic data container with linear time access to elements,
 *  and fixed time insertion/deletion at any point in the sequence.
 *
 *  @headerfile forward_list
 *
 *  This is a @e singly @e linked %list.  Traversal up the
 *  %list requires linear time, but adding and removing elements (or
 *  @e nodes) is done in constant time, regardless of where the
 *  change takes place.  Unlike arrays, random-access iterators are 
 *  not provided, so subscripting ( @c [] ) access is not allowed.  
 *  For algorithms which only need sequential access, this lack makes 
 *  no difference.
 *
 *  Also the forward_list provides specialized algorithms %unique to 
 *  linked lists, such as splicing, sorting, and in-place reversal.
 *  
 *  Unlike the std::forward_list in C++ this implementation provides
 *  a method for adding elements at the end of the list.
 * 
 *  @author Mohamed fareed.
 */

#ifndef FORWARD_LIST
#define FORWARD_LIST

typedef unsigned char Forward_List_Generic;

struct Forward_List_Node
{
    struct Forward_List_Node* next;
    Forward_List_Generic storage[];
};

typedef struct Forward_List_Node  Forward_List_Node;
typedef struct Forward_List_Node* FWL_iterator;

struct Forward_List
{
    Forward_List_Node* start;
    Forward_List_Node* finish;
    size_t count;
    size_t size;
};

typedef struct Forward_List Forward_List;

/**
  * @brief  Initializes the %forward_list.
  * @param _Tp   Type of the data to be stored in the %forward_list.
  * @param  ...  The provided initializer list.
  *  
  * This function initializes the %forward_list with copies of
  * the elements in the initializer list.
  */
#define FWL_init(_Tp, ...) ({                        \
   _Tp __buffer[] = __VA_ARGS__;                     \
    size_t __size = sizeof(__buffer)/sizeof(_Tp);    \
    Forward_List __list = FWL_Init(sizeof(_Tp));     \
    for (size_t __i = 0; __i < __size; ++__i){       \
        FWL_push_back(_Tp, &__list, __buffer[__i]);  \
    }                                                \
    __list;                                          \
})

/**
 * @brief  Cast generic type into true data type.
 * @param  _Tp     The data type used to initialize 
 *                 the %forward_list.
 * @param  __iter  An Iterator into %forward_list.
 */
#define FWL_cast(_Tp, __iter)          (*(_Tp*)__iter->storage)

/**
 * @brief  Assigns an initializer_list to a %forward_list.
 * @param _Tp      The data type used to initialize 
 *                 the %forward_list.
 * @param  __list  Reference to %forward_list object.
 * @param  ...     An initializer_list.
 *
 * Replace the contents of the %forward_list with copies 
 * of the elements in the initializer_list.
 */
#define FWL_assign(_Tp, __list, ...)({                              \
   _Tp __buffer[] = __VA_ARGS__;                                    \
    size_t __size = sizeof(__buffer)/sizeof(_Tp);                   \
    if(!__size){                                                    \
        FWL_clear(__list);                                          \
    }else {                                                         \
        FWL_resize(__list, __size);                                 \
        _Tp* __buff = __buffer;                                     \
        for (FWL_iterator __it = FWL_begin(__list); __it != NULL;   \
                          __it = __it->next, ++__buff){             \
            FWL_cast(_Tp, __it) = *__buff;                          \
        }                                                           \
    }                                                               \
})

/**
 * @brief Moves the iterator a steps forward.
 * @param  __current Iterator into the %forward_list.
 * @param  __n       Number of the steps to move.
 * @return An iterator that points after @a n elements 
 *         from the @a current iterator.
*/
extern FWL_iterator FWL_advance(FWL_iterator __current, size_t __n);

/**
 * @param __list Points to %forward_list object.
 * 
 * Returns an iterator that points before the first element
 * in the %forward_list.  Iteration is done in ordinary element order.
 */
extern FWL_iterator FWL_before_begin(Forward_List* __list);

/**
 * @param __list Points to %forward_list object.
 * 
 * Returns a read/write iterator that points to the first element 
 * in the %forward_list. Iteration is done in ordinary element order.
 */
extern FWL_iterator FWL_begin(Forward_List* __list);

/**
 * @param __list Points to %forward_list object.
 * 
 * Returns a read/write reverse iterator that points to the last element 
 * in the %forward_list. Iteration is not possible due to the nature of
 * the %forward_list.
 */
extern FWL_iterator FWL_rbegin(Forward_List* __list);

/**
 * @param __list Points to %forward_list object.
 * 
 * Returns a read/write iterator that points one past the last element 
 * in the %forward_list. Iteration is not possible.
 */
extern FWL_iterator FWL_end(Forward_List* __list);

/* Generic _FWL_front() */
extern void* _FWL_front(Forward_List* __list);

/**
 * @param _Tp     The data type used to initialize 
 *                the %forward_list.
 * @param __list  Reference to %forward_list object.
 * @return        a read/write access to the first element 
 *                in non-empty %forward_list.
 */
#define FWL_front(_Tp, __list)({  \
    *(_Tp*) _FWL_front(__list);   \
})

/* Generic _FWL_back() */
extern void* _FWL_back(Forward_List *__list);

/**
 * @param _Tp     The data type used to initialize 
 *                the %forward_list.
 * @param __list  Reference to %forward_list object.
 * @return        a read/write access to the last element 
 *                in non-empty %forward_list.
 */
#define FWL_back(_Tp, __list)({   \
    *(_Tp*) _FWL_back(__list);    \
})

/**
 * @brief  Add data to the end of the %forward_list.
 * @param _Tp      The data type used to initialize 
 *                 the %forward_list.
 * @param  __list  Reference to %forward_list object.
 * @param  ...     Data to be added.
 *
 * This is a typical stack operation.  The function creates an
 * element at the end of the %forward_list. and assigns the given 
 * data to it. Due to the nature of a %forward_list. this operation
 * can be done in constant time, and does not invalidate iterators
 * and references.
 */
#define FWL_push_back(_Tp, __list, ...) ({                          \
    FWL_insert_after(_Tp, __list, FWL_rbegin(__list), __VA_ARGS__); \
})

/**
 * @brief  Add data to the front of the %forward_list.
 * @param _Tp      The data type used to initialize 
 *                 the %forward_list.
 * @param  __list  Reference to %forward_list object.
 * @param  ...     Data to be added.
 *
 * This is a typical stack operation.  The function creates an
 * element at the front of the %forward_list and assigns the given 
 * data to it.  Due to the nature of a %forward_list this operation 
 * can be done in constant time, and does not invalidate iterators 
 * and references.
 */
#define FWL_push_front(_Tp, __list, ...) ({                               \
    FWL_insert_after(_Tp, __list, FWL_before_begin(__list), __VA_ARGS__); \
})

/**
 * @brief  Removes first element.
 * @param __list Points to %forward_list object.
 *
 * This is a typical stack operation.  It shrinks the %forward_list
 * by one.  Due to the nature of a %forward_list this operation can 
 * be done in constant time, and only invalidates iterators/references
 * to the element being removed.
 *
 * Note that no data is returned, and if the first element's data
 * is needed, it should be retrieved before FWL_pop_front() is
 * called.
 */
extern void FWL_pop_front(Forward_List* __list);

/**
 * @brief  Removes last element.
 * @param __list Points to %forward_list object.
 *
 * This is a typical stack operation.  It shrinks the %forward_list
 * by one.  Due to the nature of a forward_list this operation is so
 * slow if it's frequently used specially with larger lists. user must
 * consider using a doubly linked list if this operation is heavily
 * required.
 *
 * Note that no data is returned, and if the first element's data
 * is needed, it should be retrieved before FWL_pop_front() is
 * called.
 */
extern void FWL_pop_back(Forward_List* __list);

/**
 * @brief  Removes the element pointed to by the iterator following position.
 * @param  __list       Points to %forward_list object.
 * @param  __position   An iterator pointing before the element to be erased.
 * @return An iterator pointing to the element following the one that was erased, 
 *          or end() if no such element exists.
 * 
 * This function will erase the element pointed to by the iterator following 
 * position and thus shorten the %forward_list by one. 
 *  
 * This function only erases the element, and that if the element is itself
 * a pointer, the pointed-to memory is not touched in any way. Managing the 
 * pointer is the user's responsibility.
 */
extern FWL_iterator FWL_pop_after(Forward_List* __list, FWL_iterator __position);

/* Generic _FWL_insert_after() */
extern FWL_iterator _FWL_insert_after(Forward_List* __list, FWL_iterator __position, void** __storage);

/**
 * @brief Inserts given value into %forward_list after specified iterator.
 * @param _Tp           The data type used to initialize 
 *                      the %forward_list.
 * @param  __list       Reference to a %forward_list object.
 * @param  __position   An iterator into the %forward_list.
 * @param ...           Data to be inserted.
 * @return An iterator that points to the inserted data.
 * 
 * This function will insert a copy of the given value after the specified location.
 * Due to the nature of a %forward_list this operation can be done in constant time,
 * and does not invalidate iterators and references.
*/
#define FWL_insert_after(_Tp, __list, __position, ...)({                             \
   _Tp* __storage = NULL;                                                            \
    FWL_iterator __ret = _FWL_insert_after(__list, __position, (void**)& __storage); \
    *__storage = (_Tp)__VA_ARGS__;                                                   \
    __ret;                                                                           \
})

/**
 *  @brief  Inserts the contents of an initializer_list into
 *          %forward_list after the specified iterator.
 *  @param _Tp           The data type used to initialize 
 *                       the %forward_list.
 *  @param  __list       Reference to a %forward_list object.
 *  @param  __position   An iterator into the %forward_list.
 *  @param  ...          An initializer_list.
 *  @return  An iterator pointing to the last inserted element
 *           or @a __position if @a __list is empty.
 *
 *  This function will insert copies of the data in the
 *  initializer_list into the %forward_list after the
 *  location specified by @a __position.
 *
 *  This operation is linear in the number of elements inserted
 *  and does not invalidate iterators and references.
 */
#define FWL_insert_after_ilist(_Tp, __list, __position, ...)({        \
    FWL_iterator __pos = __position;                                  \
   _Tp __buffer[] = __VA_ARGS__;                                      \
    size_t __size = sizeof(__buffer)/sizeof(_Tp);                     \
    for (size_t __i = 0; __i < __size; ++__i){                        \
        __pos = FWL_insert_after(_Tp, __list, __pos, __buffer[__i]);  \
    }                                                                 \
    __pos;                                                            \
})

/**
 *  @brief  Insert contents of another %forward_list.
 *  @param  __list      Points to %forward_list object.
 *  @param  __position  Iterator referencing the element to insert after.
 *  @param  __src_list  Source list.
 *
 *  The elements of @a src_list are inserted in @a list after the element
 *  referenced by @a position in constant time.  @a src_list becomes an 
 *  empty list.
 */
extern void FWL_splice_after_list(Forward_List* __list, FWL_iterator __position, Forward_List* __src_list);

/**
 *  @brief  Insert element from another %forward_list.
 *  @param  __list      Points to %forward_list object.
 *  @param  __position  Iterator referencing the element to insert after.
 *  @param  __src_list  Source list.
 *  @param  __i         Iterator referencing the element before the element
 *                      to move.
 *
 *  Removes the element after the element referenced by @a i in @a src_list
 *  and inserts it into @a list after @a position.
 */
extern void FWL_splice_after_element(Forward_List* __list, FWL_iterator __position, Forward_List* __src_list, FWL_iterator __i);

/**
 *  @brief  Insert range from another %forward_list.
 *  @param  __list      Points to %forward_list object.
 *  @param  __position  Iterator referencing the element to insert after.
 *  @param  __src_list  Source list.
 *  @param  __before    Iterator referencing before the start of range
 *                      in source list.
 *  @param  __last      Iterator referencing the end of range in source list.
 *
 *  Removes elements in the range (__before,__last) in @a src_list and 
 *  inserts them in @a list after @a __position in constant time.
 */
extern void FWL_splice_after_range(Forward_List* __list, FWL_iterator __position, Forward_List* __src_list, FWL_iterator __before, 
                                                                                                            FWL_iterator __last);

/**
 * @brief  Removes a range of elements.
 * @param  __list   Points to %forward_list object.
 * @param  __before An iterator pointing before the first element to be erased.
 * @param  __last   An iterator pointing to one past the last element to be erased.
 * @return An iterator pointing to the element pointed to by @a last
 *         prior to erasing (or end()).
 *
 * This function will erase the elements in the range @a [first,last) and 
 * shorten the %forward_list accordingly.
 * 
 * Note if the elements themselves are pointers, the pointed-to memory is not
 * touched in any way. Managing the pointer is the user's responsibility.
 */
extern FWL_iterator FWL_erase_after(Forward_List* __list, FWL_iterator __before, FWL_iterator __last);

/**
 * @brief  Returns true if the %forward_list is empty.
 * @param  __list   Points to %forward_list object.
 */
extern int FWL_empty(Forward_List* __list);

/**
 * @brief  Returns the number of elements in the %forward_list.
 * @param  __list   Points to %forward_list object.
 */
extern size_t FWL_size(Forward_List* __list);

/**
 * @brief  copy the %forward_list.
 * @param _Tp       The data type used to initialize 
 *                  the %forward_list.
 * @param  __list   Reference to %forward_list object.
 * @return          Deep copy of a %forward_list object.
 */
#define FWL_copy(_Tp, __list)({                          \
    Forward_List __cp = FWL_init(_Tp, {});               \
    for(FWL_iterator __it = FWL_begin(__list),           \
                     __iend = FWL_end(__list);           \
         __it != __iend; __it = __it->next){             \
        FWL_push_back(_Tp, &__cp, FWL_cast(_Tp, __it));  \
    }                                                    \
    __cp;                                                \
})

/** 
 * @brief  Sort the elements according to comparison function.
 * @param  __list     Points to %forward_list object.
 * @param  __compare  Comparison function.
 *  
 * This function Sorts the elements according to comparison function.
 * Equivalent elements remain in list order.
 * 
 * I don't own the sort algorithm, You can check it via the link below
 * https://www.geeksforgeeks.org/iterative-merge-sort-for-linked-list/
 */
extern void FWL_sort(Forward_List* __list, int (*__compare)(const void *, const void *));

/* Generic  _FWL_remove() */
extern void _FWL_remove(Forward_List* __list, const void* __valuePtr, int (*__compare)(const void *, const void *));

/** 
 * @brief  Removes all elements equal to value according to comparison function.
 * @param _Tp           The data type used to initialize 
 *                      the %forward_list.
 * @param  __list       Reference to %forward_list object.
 * @param  __compare    Comparison function.
 * @param  ...          Value to be removed.
 *
 * Removes every element in the list equal to value according to comparison 
 * function. Remaining elements stay in %forward_list order.
 *
 * Note that this function only erases the elements, and that if the
 * elements themselves are pointers, the pointed-to memory is not 
 * touched in any way. Managing the pointer is the user's responsibility.
 */ 
#define FWL_remove(_Tp, __list, __compare, ...)({  \
   _Tp __value = (_Tp)__VA_ARGS__;                 \
    _FWL_remove(__list, &__value, __compare);      \
})

/**
 *  @brief  Removes all elements satisfying a predicate.
 *  @param  __predicate  Unary predicate function.
 *
 *  Removes every element in the %forward_list for which 
 *  the predicate returns true. Remaining elements stay in 
 *  the %forward_list order.
 *  
 *  Note that this function only erases the elements, and that if 
 *  the elements themselves are pointers, the pointed-to memory is
 *  not touched in any way.  Managing the pointer is the user's
 *  responsibility.
 */
extern void FWL_remove_if(Forward_List* __list, int (*__predicate)(const void *));

/**
 * @brief  Removes consecutive duplicate elements according to comparison function.
 * @param  __list     Points to %forward_list object.
 * @param  __compare  Comparison function.
 *
 * Removes consecutive duplicate elements according to comparison function.
 * For each consecutive set of elements with the same value, removes all but
 * one element. Remaining elements stay in list order.
 * 
 * Note that this function only erases the elements, and that if the elements 
 * themselves are pointers, the pointed-to memory is not touched in any way.
 * Managing the pointer is the user's responsibility.
 */
extern void FWL_unique(Forward_List* __list, int (*__compare)(const void *, const void *));

/**
 * @brief  Reverse the elements in %forward_list.
 * @param  __list   Points to %forward_list object.
 * 
 * This function will reverse the order of elements in the %forward_list.
 * in linear time.
 */
extern void FWL_reverse(Forward_List* __list);

/**
 * @brief Resizes the %forward_list to the specified number of elements.
 * @param  __list  Points to %forward_list object.
 * @param  __n     Number of elements the %forward_list should contain.
 * 
 * This function will %resize the %forward_list to the specified number
 * of elements. If the number is smaller than the %forward_list's current
 * number of elements the %forward_list is truncated, otherwise the 
 * %forward_list is extended.
*/
extern void FWL_resize(Forward_List* __list, size_t __n);

/**
 * @brief  Swap contents of two %forward_lists.
 * @param  __list1   Points to the first %forward_list object.
 * @param  __list2   Points to the second %forward_list object.
 */
extern void FWL_swap(Forward_List* __list1, Forward_List* __list2);

/**
 * @brief  Erases all the elements.
 * @param  __list   Points to %forward_list object.
 * 
 * Note that this function only erases the elements, and
 * that if the elements themselves are pointers, the pointed-to
 * memory is not touched in any way. Managing the pointer is the
 * user's responsibility.
 */
extern void FWL_clear(Forward_List* __list);

/* Truncates the %forward_list. */
extern void FWL_truncate(Forward_List* __list, FWL_iterator __curr, 
                                               FWL_iterator __prev);

/* Initializes the %forward_list. */
extern Forward_List FWL_Init(size_t);

#endif


