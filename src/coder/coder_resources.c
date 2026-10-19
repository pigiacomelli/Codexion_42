#include "codexion.h"

static void	order_dongles(t_coder *coder, t_dongle **first,
		t_dongle **second)
{
	if (coder->left_dongle->id < coder->right_dongle->id)
	{
		*first = coder->left_dongle;
		*second = coder->right_dongle;
	}
	else
	{
		*first = coder->right_dongle;
		*second = coder->left_dongle;
	}
}

int	acquire_dongle_pair(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder == NULL || coder->left_dongle == NULL
		|| coder->right_dongle == NULL
		|| coder->left_dongle == coder->right_dongle)
		return (1);
	order_dongles(coder, &first, &second);
	if (acquire_dongle(coder, first) != 0)
		return (1);
	if (acquire_dongle(coder, second) != 0)
	{
		release_dongle(coder, first);
		return (1);
	}
	return (0);
}

void	release_dongle_pair(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder == NULL || coder->left_dongle == NULL
		|| coder->right_dongle == NULL
		|| coder->left_dongle == coder->right_dongle)
		return ;
	order_dongles(coder, &first, &second);
	release_dongle(coder, second);
	release_dongle(coder, first);
}
