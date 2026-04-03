/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaghzal <tlaghzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 06:30:00 by tlaghzal          #+#    #+#             */
/*   Updated: 2026/03/28 06:30:00 by tlaghzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include "../ft_malloc.h"

#define NUM_THREADS 5
#define ALLOCS_PER_THREAD 100

typedef struct {
    int thread_id;
    int success;
    int fail;
} thread_data_t;

void *thread_func(void *arg)
{
    thread_data_t *data = (thread_data_t *)arg;
    void *ptrs[ALLOCS_PER_THREAD];
    int i;

    data->success = 0;
    data->fail = 0;

    for (i = 0; i < ALLOCS_PER_THREAD; i++)
    {
        size_t size = (i + 1) * 10;
        ptrs[i] = ft_malloc(size);
        if (!ptrs[i])
        {
            data->fail++;
            continue;
        }

        // Write pattern based on thread ID
        memset(ptrs[i], data->thread_id, size);

        // Verify pattern to ensure no corruption from other threads
        unsigned char *check = (unsigned char *)ptrs[i];
        int valid = 1;
        for (size_t j = 0; j < size; j++)
        {
            if (check[j] != (unsigned char)data->thread_id)
            {
                valid = 0;
                break;
            }
        }

        if (valid)
            data->success++;
        else
            data->fail++;

        // Free half of them immediately to test ft_free concurrency
        if (i % 2 == 0)
        {
            ft_free(ptrs[i]);
            ptrs[i] = NULL;
        }
    }

    // Free the remaining allocations
    for (i = 0; i < ALLOCS_PER_THREAD; i++)
    {
        if (ptrs[i] != NULL)
        {
            ft_free(ptrs[i]);
            ptrs[i] = NULL;
        }
    }

    return NULL;
}

int main(void)
{
    pthread_t threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];
    int i;
    int total_success = 0;
    int total_fail = 0;

    printf("Starting multi-threaded ft_malloc test with %d threads...\n", NUM_THREADS);

    for (i = 0; i < NUM_THREADS; i++)
    {
        thread_data[i].thread_id = i;
        if (pthread_create(&threads[i], NULL, thread_func, &thread_data[i]) != 0)
        {
            perror("pthread_create");
            return 1;
        }
    }

    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
        total_success += thread_data[i].success;
        total_fail += thread_data[i].fail;
    }

    printf("Test completed.\n");
    printf("Total allocations verified: %d\n", total_success);
    printf("Total failures: %d\n", total_fail);

    // Test free_all with some remaining allocations
    void *leak1 = ft_malloc(100);
    void *leak2 = ft_malloc(200);
    if (leak1) memset(leak1, 'A', 100);
    if (leak2) memset(leak2, 'B', 200);
    
    printf("Calling free_all() to clean up remaining allocations...\n");
    free_all();

    if (total_fail == 0)
    {
        printf("\n✅ SUCCESS: All tests passed!\n");
        return 0;
    }
    else
    {
        printf("\n❌ FAILURE: %d tests failed.\n", total_fail);
        return 1;
    }
}