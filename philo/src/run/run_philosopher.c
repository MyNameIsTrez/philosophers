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

static void	run_single_philosopher_edgecase(t_philosopher *philosopher,
				t_data *data)
{
	mutex_lock(&data->running_program_mutex);
	if (!data->running_program) // TODO: Is this one necessary?
	{
		mutex_unlock(&data->running_program_mutex);
		return ;
	}
	mutex_unlock(&data->running_program_mutex);
	mutex_lock(philosopher->left_fork);
	print_event(EVENT_FORK, philosopher, data);
	while (true)
	{
		mutex_lock(&data->running_program_mutex);
		if (!data->running_program)
		{
			mutex_unlock(&data->running_program_mutex);
			break ;
		}
		mutex_unlock(&data->running_program_mutex);
		usleep(LOOP_USLEEP);
	}
	mutex_unlock(philosopher->left_fork);
}

static void	run_regular_philosopher(t_philosopher *philosopher, t_data *data)
{
	if (philosopher->index % 2 == 1)
		precise_sleep(philosopher->time_of_last_meal, data->time_to_eat / 2, data);

	while (true)
	{
		mutex_lock(&data->running_program_mutex);
		if (!data->running_program) // TODO: Should this check be done after every action below?
		{
			mutex_unlock(&data->running_program_mutex);
			break ;
		}
		mutex_unlock(&data->running_program_mutex);

		if (philosopher->index % 2 == 0)
		{
			mutex_lock(philosopher->left_fork);

			print_event(EVENT_FORK, philosopher, data);

			mutex_lock(philosopher->right_fork);

			print_event(EVENT_FORK, philosopher, data);
		}
		else
		{
			mutex_lock(philosopher->right_fork);

			print_event(EVENT_FORK, philosopher, data);

			mutex_lock(philosopher->left_fork);

			print_event(EVENT_FORK, philosopher, data);
		}

		// TODO: This probably shouldn't happen when the philosopher hasn't eaten but is just quitting because another phil died.
		philosopher->times_eaten++;
		if (philosopher->times_eaten == data->times_to_eat)
		{
			mutex_lock(&data->philosophers_eating_mutex);
			data->philosophers_eating--;
			mutex_unlock(&data->philosophers_eating_mutex);
		}

		print_event(EVENT_EAT, philosopher, data);

		mutex_lock(&philosopher->time_of_last_meal_mutex);
		philosopher->time_of_last_meal = get_time_ms();
		mutex_unlock(&philosopher->time_of_last_meal_mutex);

		precise_sleep(philosopher->time_of_last_meal, data->time_to_eat, data);

		// TODO: Does the unlocking need to occur in the same order as the locking? If so, then this won't work:

		mutex_unlock(philosopher->left_fork);
		mutex_unlock(philosopher->right_fork);

		print_event(EVENT_SLEEP, philosopher, data);

		// mutex_lock(&data->printf_mutex);
		// printf("time to sleep: %zu\n", data->time_to_sleep);
		// mutex_unlock(&data->printf_mutex);

		precise_sleep(get_time_ms(), data->time_to_sleep, data);

		// mutex_lock(&data->printf_mutex);
		// printf("%zu after sleeping\n", philosopher->index + 1);
		// mutex_unlock(&data->printf_mutex);

		print_event(EVENT_THINK, philosopher, data);

		if (data->philosopher_count % 2 == 1)
			precise_sleep(get_time_ms(), data->time_to_eat / 2, data);
	}
}

void	*run_philosopher(void *_philosopher)
{
	t_philosopher	*philosopher;
	t_data			*data;

	philosopher = _philosopher;
	data = philosopher->data;
	while (true)
	{
		mutex_lock(&data->running_philosophers_mutex);
		if (data->running_philosophers)
		{
			mutex_unlock(&data->running_philosophers_mutex);
			break ;
		}
		mutex_unlock(&data->running_philosophers_mutex);
		usleep(LOOP_USLEEP);
	}
	if (data->philosopher_count == 1)
		run_single_philosopher_edgecase(philosopher, data);
	else
		run_regular_philosopher(philosopher, data);
	return (NULL);
}
