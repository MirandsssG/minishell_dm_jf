#include "../minishell.h"

void	cleanup(t_data *data)
{
	if (data->env_list)
		free_env_list(data->env_list);
}