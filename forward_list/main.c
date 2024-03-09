#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/forward_list.h"

int compare(const void *__x, const void *__y)
{
    return strcmp(*(char **)__x, *(char **)__y) > 0;
}

int main(void)
{
    printf("\n %s\n\n", "---<(Examples)>--- ");

    /* Initializes the %forward_list 'test' */
    Forward_List test = FWL_init(int, {0, 1, 2, 3, 4});

    /* Displays the data stored in the %forward_list 'test' */
    for(FWL_iterator it = FWL_begin(&test); it != FWL_end(&test); it = it->next)
    {
        printf(" %i -> ", *(int *)it->storage);
    }
    puts("NULL\n");

    /* Erases all the elements in the %forward_list 'test' */
    FWL_clear(&test);


    /* Initializes the %forward_list 'test1' */
    Forward_List test1 = FWL_init(float, {1.5, 2});

    /* Add the values of variable i to the end of the %forward_list 'test1' */
    for (float i = 2.5; i <= 3.5; i+=0.5)
    {
        FWL_push_back(float, &test1, i);
    }

    /* Displays the data stored in the %forward_list 'test1' */
    for(FWL_iterator it = FWL_begin(&test1); it != FWL_end(&test1); it = it->next)
    {
        printf(" %.1f -> ", *(float *)it->storage);
    }
    puts("NULL\n");

    /* Erases all the elements in %forward_list 'test1' */
    FWL_clear(&test1);


    /* Initializes the %forward_list 'test2' */
    Forward_List test2 = FWL_init(char, {'A', 'E'});

    FWL_iterator it2 = FWL_begin(&test2);

    /* Inserts all the characters from 'B' to 'D' into the %forward_list 'test2'
       After the first element 'A'. */
    for (char i = 'B'; i < 'E'; i++)
    {
        it2 = FWL_insert_after(int, &test2, it2, i);
    }

    /* Displays the data stored in the %forward_list 'test2' */
    for(FWL_iterator it = FWL_begin(&test2); it != FWL_end(&test2); it = it->next)
    {
        printf(" '%c' -> ", *(char *)it->storage);
    }
    puts("NULL\n");

    /* Erases all the elements in %forward_list 'test2' */
    FWL_clear(&test2);


    /* Initializes the %forward_list 'test3' */
    Forward_List test3 = FWL_init(char*, {"Battle", "C", "Apple", "Camel", "B", "A"});

    /* Sorts the elements in the %forward_list 'test3' */
    FWL_sort(&test3, compare);

    /* Displays the data stored in the %forward_list 'test3' */
    for(FWL_iterator it = FWL_begin(&test3); it != FWL_end(&test3); it = it->next)
    {
        printf(" \"%s\" -> ", *(char **)it->storage);
    }
    puts("NULL");

    /* Erases all the elements in %forward_list 'test3' */
    FWL_clear(&test3);

    return EXIT_SUCCESS;
}

