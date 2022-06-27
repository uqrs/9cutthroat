#include <u.h>
#include <libc.h>
#include "bone.h"
#include "player.h"

player *
mkplayer(int id, bone *(*strategy)(game *, player *, int *))
{
	player *new;

	new = calloc(1, sizeof(player));
	new->hand = nil;
	new->id = id;
	new->play = strategy;

	return new;
}

void
nukeplayer(player *plp)
{
	if (plp == nil)
		return;

	nukepile(plp->hand);
	free(plp);
}

