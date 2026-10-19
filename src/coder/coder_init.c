#include "codexion.h"

#include <stdlib.h>


int	init_coders(t_simulation *sim)
{
	    if (sim == NULL)
	        return (1);
    if (sim->config.coder_count <= 0)
        return (1);
    if (sim->coders != NULL)
        return (1);
    if (sim->dongles == NULL)
        return (1);
    sim->coders = malloc(sizeof(t_coder) * sim->config.coder_count);
    if (sim->coders == NULL)
        return (1);
    int i = 0;
    while (i < sim->config.coder_count)
    {
        sim->coders[i].id = i;
        sim->coders[i].last_compile_start = sim->start_time;
        sim->coders[i].compile_count = 0;
        sim->coders[i].left_dongle = &sim->dongles[i];
        sim->coders[i].right_dongle = &sim->dongles[(i + 1) % sim->config.coder_count];
        sim->coders[i].simulation = sim;
        if (pthread_mutex_init(&sim->coders[i].mutex, NULL) != 0)
        {
            int j = 0;
            while (j < i)
            {
                pthread_mutex_destroy(&sim->coders[j].mutex);
                j++;
            }
            free(sim->coders);
            sim->coders = NULL;
            return (1);
        }
        i++;
    }
    
    return (0);
}

void	destroy_coders(t_simulation *sim)
{
	    if (sim == NULL)
	        return ;
    if (sim->coders != NULL)
    {
        int i = 0;
        while (i < sim->config.coder_count)
        {
            pthread_mutex_destroy(&sim->coders[i].mutex);
            i++;
        }
        free(sim->coders);
        sim->coders = NULL;
    }
}
