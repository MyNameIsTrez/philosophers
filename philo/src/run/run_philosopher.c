/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   run_philosopher.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: sbos <sbos@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/08 16:12:40 by sbos          #+#    #+#                 */
/*   Updated: 2022/11/08 16:12:40 by sbos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	run_single_philosopher_edgecase(t_philosopher *philosopher)
{
	mutex_lock(&philosopher->data->running_program_mutex);
	if (!philosopher->data->running_program) // TODO: Is this one necessary?
	{
		mutex_unlock(&philosopher->data->running_program_mutex);
		return ;
	}
	mutex_unlock(&philosopher->data->running_program_mutex);

	mutex_lock(philosopher->left_fork);

	print_event(EVENT_FORK, philosopher);

	while (true)
	{
		mutex_lock(&philosopher->data->running_program_mutex);
		if (!philosopher->data->running_program)
		{
			mutex_unlock(&philosopher->data->running_program_mutex);
			break ;
		}
		mutex_unlock(&philosopher->data->running_program_mutex);
		usleep(LOOP_USLEEP);
	}

	mutex_unlock(philosopher->left_fork);
}

static void	run_regular_philosopher(t_philosopher *philosopher)
{
	if (philosopher->index % 2 == 1)
		precise_sleep(philosopher, philosopher->time_of_last_meal, philosopher->data->time_to_eat / 2);

	while (true)
	{
		mutex_lock(&philosopher->data->running_program_mutex);
		if (!philosopher->data->running_program) // TODO: Should this check be done after every action below?
		{
			mutex_unlock(&philosopher->data->running_program_mutex);
			break ;
		}
		mutex_unlock(&philosopher->data->running_program_mutex);

		if (philosopher->index % 2 == 0)
		{
			mutex_lock(philosopher->left_fork);

			print_event(EVENT_FORK, philosopher);

			mutex_lock(philosopher->right_fork);

			print_event(EVENT_FORK, philosopher);
		}
		else
		{
			mutex_lock(philosopher->right_fork);

			print_event(EVENT_FORK, philosopher);

			mutex_lock(philosopher->left_fork);

			print_event(EVENT_FORK, philosopher);
		}

		// TODO: This probably shouldn't happen when the philosopher hasn't eaten but is just quitting because another phil died.
		philosopher->times_eaten++;
		if (philosopher->times_eaten == philosopher->data->times_to_eat)
		{
			mutex_lock(&philosopher->data->philosophers_eating_mutex);
			philosopher->data->philosophers_eating--;
			mutex_unlock(&philosopher->data->philosophers_eating_mutex);
		}

		print_event(EVENT_EAT, philosopher);

		mutex_lock(&philosopher->time_of_last_meal_mutex);
		philosopher->time_of_last_meal = get_time_ms();
		mutex_unlock(&philosopher->time_of_last_meal_mutex);

		precise_sleep(philosopher, philosopher->time_of_last_meal, philosopher->data->time_to_eat);

		// TODO: Does the unlocking need to occur in the same order as the locking? If so, then this won't work:

		mutex_unlock(philosopher->left_fork);
		mutex_unlock(philosopher->right_fork);

		print_event(EVENT_SLEEP, philosopher);

		t_time	time_of_last_sleep;

		time_of_last_sleep = get_time_ms();

		// mutex_lock(&philosopher->data->printf_mutex);
		// printf("time to sleep: %zu\n", philosopher->data->time_to_sleep);
		// mutex_unlock(&philosopher->data->printf_mutex);

		precise_sleep(philosopher, time_of_last_sleep, philosopher->data->time_to_sleep);

		// mutex_lock(&philosopher->data->printf_mutex);
		// printf("%zu after sleeping\n", philosopher->index + 1);
		// mutex_unlock(&philosopher->data->printf_mutex);

		print_event(EVENT_THINK, philosopher);

		if (philosopher->data->philosopher_count % 2 == 1)
			precise_sleep(philosopher, get_time_ms(), philosopher->data->time_to_eat / 2);
	}
}

void	*run_philosopher(void *arg)
{
	t_philosopher	*philosopher;

	philosopher = arg;

	while (true)
	{
		mutex_lock(&philosopher->data->running_philosophers_mutex);
		if (philosopher->data->running_philosophers)
		{
			mutex_unlock(&philosopher->data->running_philosophers_mutex);
			break ;
		}
		mutex_unlock(&philosopher->data->running_philosophers_mutex);

		usleep(LOOP_USLEEP);
	}

	if (philosopher->data->philosopher_count == 1)
		run_single_philosopher_edgecase(philosopher);
	else
		run_regular_philosopher(philosopher);

	return (NULL);
}
