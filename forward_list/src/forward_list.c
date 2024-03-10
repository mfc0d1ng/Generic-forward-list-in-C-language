#include <stdio.h>
#include <stdlib.h>
#include "../include/forward_list.h"

static void Exit(const char* __func_name)
{
    printf("%s : Out of memory\n", __func_name); 
    exit(EXIT_FAILURE);
}


static void FWL_reset(Forward_List* __list)
{
    __list->start = NULL;
    __list->finish = NULL;
    __list->count = 0;
}


FWL_iterator FWL_advance(FWL_iterator __current, size_t __n)
{
    for(; __n; --__n)
    {
        if(__current)
        {
           __current = __current->next;
        }
        else
        {
            break;
        }
    }
    return __current;
}


FWL_iterator FWL_before_begin(Forward_List* __list)
{
    return (FWL_iterator) &__list->start;
}


FWL_iterator FWL_begin(Forward_List* __list)
{
    return __list->start;
}

FWL_iterator FWL_rbegin(Forward_List* __list)
{
    return __list->finish;
}

FWL_iterator FWL_end(Forward_List* __list)
{
    if(__list->finish)
    {
        return __list->finish->next;
    }
    else
    {
        return __list->finish;
    }
}

void* _FWL_front(Forward_List* __list)
{
    return FWL_begin(__list)->storage;
}

void* _FWL_back(Forward_List* __list)
{
    return FWL_rbegin(__list)->storage;
}


void FWL_pop_front(Forward_List* __list)
{
    FWL_pop_after(__list, FWL_before_begin(__list));
}


void FWL_pop_back(Forward_List* __list)
{
    FWL_pop_after(__list, FWL_advance(FWL_before_begin(__list), FWL_size(__list)-1));
}


static FWL_iterator FWL_pop_first_element(Forward_List* __list)
{
    Forward_List_Node* __temp = __list->start;
    __list->start = __list->start->next;
    if(!__list->start)
    {
        __list->finish = __list->start;
    }
    free(__temp);
    return __list->start;
}

static void FWL_pop_last_element(Forward_List* __list, FWL_iterator __position)
{
    free(__list->finish);
    __list->finish = __position;
    __position->next = NULL;
}

static FWL_iterator FWL_pop_next_element(FWL_iterator __position)
{
        Forward_List_Node* __temp = __position->next;
        __position->next = __position->next->next;
        free(__temp);
        return __position;
}


FWL_iterator FWL_pop_after(Forward_List* __list, FWL_iterator __position)
{
    if(!__position || !__position->next)
    {
        return NULL; 
    }
    Forward_List_Node* __ret = NULL;
    if(__position == FWL_before_begin(__list))
    {
        __ret = FWL_pop_first_element(__list);
    }
    else if(__position->next == FWL_rbegin(__list))
    {
        FWL_pop_last_element(__list, __position);
    }
    else if(__position->next->next)
    {
        __ret = FWL_pop_next_element(__position);
    }
    else {}
    --__list->count;
    return __ret;
}

static void FWL_init_list(Forward_List* __list, Forward_List_Node* __node)
{
    __node->next = NULL;
    __list->start = __node;
    __list->finish = __node;
}

static void FWL_insert_before_begin(Forward_List* __list, Forward_List_Node* __node)
{
    __node->next = __list->start;
    __list->start = __node;
}

static void __FWL_insert_after(Forward_List* __list, Forward_List_Node* __node, FWL_iterator __position)
{
    __node->next = __position->next;
    if(__node->next == NULL)
    {
        __list->finish = __node;
    }
    __position->next = __node;
}


FWL_iterator _FWL_insert_after(Forward_List* __list, FWL_iterator __position, void** __storage)
{
    Forward_List_Node* __node = (Forward_List_Node*) calloc(1, sizeof(Forward_List_Node*) + __list->size);
    if(__node == NULL)
    {
        Exit("_FWL_insert_after()");
    }
    else
    {
        if(__list->start == NULL)
        {
            FWL_init_list(__list, __node);
        }
        else
        {
            if(__position == FWL_before_begin(__list))
            {
                FWL_insert_before_begin(__list, __node);
            }
            else
            {
                __FWL_insert_after(__list, __node, __position);
            }
        }
    }
    *__storage = __node->storage;
    ++__list->count;
    return __node;
}


void FWL_splice_after_list(Forward_List* __list, FWL_iterator __position, Forward_List* __src_list)
{
    if(__position == NULL)
    {
        return;
    }
    if(FWL_empty(__list))
    {
        __list->start = __src_list->start;
        __list->finish = __src_list->finish;
    }
    else if(__position == FWL_before_begin(__list))
    {
        __src_list->finish->next = __list->start;
        __list->start = __src_list->start;
    }
    else if(__position == FWL_rbegin(__list))
    {
        __list->finish->next = __src_list->start;
        __list->finish = __src_list->finish;
    }
    else
    {
        __src_list->finish->next = __position->next;
        __position->next = __src_list->start;
    }

    __list->count += __src_list->count;

    FWL_reset(__src_list);
}


static Forward_List_Node* FWL_unlink_node(Forward_List* __src_list, FWL_iterator __i)
{
    Forward_List_Node* __node = NULL;
    if(__i->next)
    {
        __node = __i->next;
    }
    else
    {
        return NULL;
    }

    if(__i == FWL_before_begin(__src_list))
    {
        __src_list->start = __src_list->start->next;
        if(__src_list->start == FWL_end(__src_list))
        {
            __src_list->finish = __src_list->start;
        }
    }
    else if(__i->next == FWL_rbegin(__src_list))
    {
        __src_list->finish = __i;
        __src_list->finish->next = NULL;
    }
    else
    {
        if(__i->next)
        {
            __i->next = __i->next->next;
        }
    }
    --__src_list->count;
    return __node;
}


static void FWL_splice_node(Forward_List* __list, FWL_iterator __position, Forward_List_Node* __node)
{
    if(!__node)
    {
        return;
    }
    if(__list->start == NULL)
    {
        __node->next = NULL;
        __list->start =  __node;
        __list->finish = __node;
    }
    else if(__position == FWL_before_begin(__list))
    {
        __node->next = __list->start;
        __list->start = __node;
    }
    else if(__position == __list->finish)
    {
        __node->next = NULL;
        __list->finish->next = __node;
        __list->finish = __node;
    }
    else
    {
        __node->next = __position->next;
        __position->next = __node;
    }
    ++__list->count;
}


void FWL_splice_after_element(Forward_List* __list, FWL_iterator __position, Forward_List* __src_list, FWL_iterator __i)
{
    if(!__position || !__i)
    {
        return;
    }
    FWL_splice_node(__list, __position, FWL_unlink_node(__src_list, __i));
}


void FWL_splice_after_range(Forward_List* __list, FWL_iterator __position, Forward_List* __src_list, FWL_iterator __before, 
                                                                                                     FWL_iterator __last)
{
    if(FWL_empty(__src_list) || !__before || __before == __last)
    {
        return;
    }
    while (__before->next != __last)
    {
        FWL_splice_after_element(__list, __position, __src_list, __before);
        __position = __position->next;
    }
}


FWL_iterator FWL_erase_after(Forward_List* __list, FWL_iterator __before, FWL_iterator __last)
{
    FWL_iterator _iter = __before;
    if(_iter == FWL_before_begin(__list))
    {
        while (__list->start != __last)
        {
            FWL_pop_after(__list, _iter);
        }
    }
    else
    {
        while (_iter->next != __last)
        {
            FWL_pop_after(__list, _iter);
        }
    }
    return __last;
}


int FWL_empty(Forward_List* __list)
{
    if(__list->start == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


size_t FWL_size(Forward_List* __list)
{
    return __list->count;
}


void FWL_sort(Forward_List* __list, int (*__compare)(const void *, const void *))
{
    if (__list->start == FWL_end(__list) || __list->start->next == FWL_end(__list))
    {
        return;
    }

    if(__compare == NULL)
    {
        return;
    }

    Forward_List_Node** head = &__list->start;
    Forward_List_Node* start1 = NULL;
    Forward_List_Node* end1 = NULL;
    Forward_List_Node* start2 = NULL;
    Forward_List_Node* end2 = NULL; 
    Forward_List_Node* prevend = NULL;
    Forward_List_Node* temp = NULL;

    Forward_List_Node* __temp = NULL;
    Forward_List_Node* astart = NULL;
    Forward_List_Node* aend = NULL;
    Forward_List_Node* bstart = NULL;
    Forward_List_Node* bendnext = NULL;

    size_t counter = 0;
    int isFirstIter = 0;

    for (size_t gap = 1; gap < __list->count; gap = gap * 2) 
    {
        start1 = *head;
        for (; start1;) 
        {
 
            /* If this is first iteration */
            isFirstIter = 0;
            if (start1 == *head)
            {
                isFirstIter = 1;
            }
 
            /* First part for merging */
            counter = gap;
            end1 = start1;
            for (; --counter && end1->next;)
            {
                end1 = end1->next;
            }
 
            /* Second part for merging */
            start2 = end1->next;
            if (!start2)
            {
                break;
            }
            
            counter = gap;
            end2 = start2;
            for (; --counter && end2->next;)
            {
                end2 = end2->next;
            }
 
            /* To store for next iteration. */
            temp = end2->next;

            /* ===begin merge=== */

            if (__compare(start1->storage, start2->storage)) 
            {
                __temp = start1;
                start1 = start2;
                start2 = __temp;

                __temp = end1;
                end1 = end2;
                end2 = __temp;
            }

            astart = start1;
            aend = end1;
            bstart = start2;
            bendnext = end2->next;
            for (; astart != aend && bstart != bendnext;)
            {
                if (__compare(astart->next->storage, bstart->storage))
                {
                    __temp = bstart->next;
                    bstart->next = astart->next;
                    astart->next = bstart;
                    bstart = __temp;
                }
                astart = astart->next; 
            }

            if(astart == aend)
            {
                astart->next = bstart;
            }
            else
            {
                end2 = end1;
            }

            /* ===end merge=== */
            
            /* Update head for first iteration, else append after previous list */
            if (isFirstIter)
            {
                *head = start1;
            }
            else
            {
                prevend->next = start1;
            }
 
            prevend = end2;
            start1 = temp;
        }
        prevend->next = start1;
    }
    __list->finish = end2;
}


void _FWL_remove(Forward_List* __list, const void* __valuePtr, int (*__compare)(const void *, const void *))
{
    if(__list->start == NULL)
    {
        return;
    }
    Forward_List_Node* __it = __list->start;
    while (__it->next != NULL)
    {
        if(__compare(__valuePtr, __it->next->storage))
        {
            __it = FWL_pop_after(__list, __it);
        }
        else
        {
            __it = __it->next;
        }
        if(__it == NULL)
        {
            break;
        }
    }
    if(__compare(__valuePtr, __list->start->storage))
    {
        FWL_pop_front(__list);
    }
}


void FWL_remove_if(Forward_List* __list, int (*__predicate)(const void *))
{
    if(__list->start == NULL)
    {
        return;
    }
    Forward_List_Node* __it = __list->start;
    while (__it->next != NULL)
    {
        if(__predicate(__it->next->storage))
        {
            __it = FWL_pop_after(__list, __it);
        }
        else
        {
            __it = __it->next;
        }
        if(__it == NULL)
        {
            break;
        }
    }
    if(__predicate(__list->start->storage))
    {
        FWL_pop_front(__list);
    }
}


void FWL_unique(Forward_List* __list, int (*__compare)(const void *, const void *))
{
    if(!__list->start || !__list->start->next)
    {
        return;
    }
    Forward_List_Node* __it = __list->start;
    while (__it->next != NULL)
    {
        if(__compare(__it->storage, __it->next->storage))
        {
            __it = FWL_pop_after(__list, __it);
        }
        else
        {
            __it = __it->next;
        }
        if(__it == NULL)
        {
            break;
        }
    }
}


void FWL_reverse(Forward_List* __list)
{
    Forward_List_Node* current = FWL_begin(__list);
    if(!current || !current->next)
    {
        return;
    }
    Forward_List_Node* start = current;
    Forward_List_Node* prev = NULL;
    Forward_List_Node* next = NULL;
    while (current != NULL)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    __list->start = prev;
    __list->finish = start;
}


static void FWL_truncate(Forward_List* __list, FWL_iterator __curr, FWL_iterator __prev)
{
    Forward_List_Node* __temp = NULL;
    while (__curr != NULL)
    {
        __temp = __curr;
        __curr = __curr->next;
        free(__temp);
        --__list->count;
    }
    __list->finish = __prev;
    __list->finish->next = NULL;
}

static void FWL_shrink_list(Forward_List* __list, size_t __n)
{
    size_t __i = 0;
    FWL_iterator __prev = NULL;
    for (FWL_iterator __it = FWL_begin(__list); __it != NULL; __it = __it->next, ++__i)
    {
        if(__i == __n)
        {
            FWL_truncate(__list, __it, __prev);
            break;
        }
        __prev = __it;
    }
}

static void FWL_extend_list(Forward_List* __list, size_t __n)
{
    void* temp = NULL;
    if(FWL_empty(__list))
    {
        while (__list->count < __n)
        {
            _FWL_insert_after(__list, FWL_before_begin(__list), &temp);
        }
    }
    else
    {
        while (__list->count < __n)
        {
            _FWL_insert_after(__list, FWL_rbegin(__list), &temp);
        }
    }
}


void FWL_resize(Forward_List* __list, size_t __n)
{
    if(__n == FWL_size(__list))
    {
        return;
    }
    if(__n == 0)
    {
        FWL_clear(__list);
    }
    else if(FWL_size(__list) > __n)
    {
        FWL_shrink_list(__list, __n);
    }
    else
    {
        FWL_extend_list(__list, __n);
    }
}


void FWL_swap(Forward_List* __list1, Forward_List* __list2)
{
    if(__list1->size != __list2->size)
    {
        printf("%s", "FWL_swap(): swap failed\n");
        exit(EXIT_FAILURE);
    }
    Forward_List __temp = *__list1;
    *__list1 = *__list2;
    *__list2 = __temp;
}


void FWL_clear(Forward_List* __list)
{
    if(FWL_empty(__list))
    {
        return;
    }
    Forward_List_Node* __temp = NULL;
    for (Forward_List_Node* __it = FWL_begin(__list); __it != NULL; )
    {
        __temp = __it;
        __it = __it->next;
        free(__temp);
    }
    FWL_reset(__list);
}


Forward_List FWL_Init(size_t __size)
{
    Forward_List temp = {.start = NULL, .finish = NULL, 
                         .count = 0,   .size = __size};
    return temp;
}

