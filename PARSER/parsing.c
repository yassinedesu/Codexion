/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yael-kha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 17:40:00 by yael-kha          #+#    #+#             */
/*   Updated: 2026/06/23 21:49:11 by yael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static long	ft_atol(char *str)
{
	int		i;
	long	res;
	long	neg;

	i = 0;
	neg = 1;
	res = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-')
	{
		neg *= -1;
		i++;
	}
	else if (str[i] == '+')
	{
		i++;
	}
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		res = (res * 10) + (str[i] - '0');
		i++;
	}
	return (res * neg);
}

static	int	input_validator(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc - 1)
	{
		j = 0;
		while (argv[i][j])
		{
			if (!((argv[i][j] >= '0' && argv[i][j] <= '9') || (argv[i][j] == '-' || argv[i][j] == '+')))
				return (-1);
			j++;
		}
		i++;
	}
	if (!argv[i] || (strcmp(argv[i], "fifo") != 0 && strcmp(argv[i], "edf") != 0))
		return (-1);
	return (0);
}

static int	check_sch(char *str)
{
	if (!str || (strcmp(str, "fifo") != 0 && strcmp(str, "edf") != 0))
		return (-1);
	return (0);
}

static char	*ft_strdup(char *src)
{
	int		i;
	int		len;
	char	*res;

	i = 0;
	if (!src)
		return (NULL);
	len = strlen(src);
	if(len != 3 && len != 4)
		return (NULL);
	res = (char *)malloc(len + 1);
	if (!res)
		return (NULL);
	while (src[i])
	{
		res[i] = src[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

t_input	*parsed_args(int argc, char **argv)
{
	t_input	*data;

	if (argc == 9)
	{
		data = (t_input *)malloc(sizeof(t_input));
		if (!data)
			return (NULL);
		data->number_of_coders = ft_atol(argv[1]);
		data->time_to_burnout = ft_atol(argv[2]);
		data->time_to_compile = ft_atol(argv[3]);
		data->time_to_debug = ft_atol(argv[4]);
		data->time_to_refactor = ft_atol(argv[5]);
		data->number_of_compiles_required = ft_atol(argv[6]);
		data->dongle_cooldown = ft_atol(argv[7]);
		data->scheduler = ft_strdup(argv[8]);
		if (data->number_of_coders <= 0 || data->time_to_burnout <= 0
			|| data->time_to_compile <= 0 || data->time_to_debug <= 0
			|| data->time_to_refactor <= 0
			|| data->number_of_compiles_required <= 0
			|| data->dongle_cooldown <= 0)
			return (free(data->scheduler), free(data), NULL);
	}
	else
		return (NULL);
	return (data);
}
