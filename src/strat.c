#include <u.h>
#include <libc.h>
#include <stdio.h>
#include "bone.h"
#include "player.h"
#include "game.h"
#include "strat.h"

void
renderpile(pile *pp)
{
	bone *np;

	if (pp == nil)
		return;

	for (np = pp->head; np != nil; np = np->next)
		print("[%d|%d]", np->suit1, np->suit2);
	print("\n");

	return;
}

void
renderhand(pile *pp)
{
	bone *np;
	int id;

	if (pp == nil)
		return;

	for (np = pp->head, id = 0; np != nil; np = np->next, id++)
		print("{%d}	[%d:%d]\n", id, np->suit1, np->suit2);
}

void
renderamounts(game *gp)
{
	int i;

	if (gp == nil)
		return;

	for (i = 0; i < gp->nplayers; i++)
		print("Player %d has %d bones.\n", i, gp->players[i]->hand->amount);
}

bone *
strat_real(game *gp, player *self, int *where)
{
	int i, id, lookid, chosen;
	char *direction;
	bone *np;

	if (gp == nil || self == nil || where == nil)
		return nil;

	direction = malloc(512);

	for (;;) {
		chosen = 0;
		for (i = 0; i < gp->nplayers; i++)
			if (gp->players[i] == self)
				break;

		print("YOU ARE PLAYER %d\n\n", i);
		renderpile(gp->field);
		print("\n");
		renderamounts(gp);
		print("\n");
		renderhand(self->hand);

		for (np = self->hand->head, id = 0; np != nil; np = np->next, id++)
			;

		print("> ");

		scanf("%d", &chosen);

		if (chosen >= id || chosen < 0) {
			print("Illegal num\n");
			system("sleep 2");
		} else {	
			lookid = 0;
			np = self->hand->head;
			while (lookid != chosen && np != nil) {
				np = np->next;
				lookid++;
			} 
	
			print("Play [%d|%d] Where? L/R\n", np->suit1, np->suit2);
			scanf("%s", direction);
	
			if (strcmp(direction, "L") == 0) {
				free(direction);
				*where = HEAD;

				return np;
			} else if (strcmp(direction, "R") == 0) {
				free(direction);
				*where = TAIL;

				return np;
			} else {
				print("What?\n");
				system("sleep 2");
			}
		}
	}

	return nil;
}
/*
int
strat_real(game *gp, player *self)
{
	int i, id, lookid, chosen, exit, where;
	char *direction;
	bone *np;

	direction = malloc(512);

	where = 0;

	for (;;) {
		for (i = 0; i < gp->nplayers; i++)
			if (gp->players[i] == self)
				break;

		print("YOU ARE PLAYER %d\n\n", i);
		renderpile(gp->field);
		print("\n");
		renderamounts(gp);
		print("\n");
		renderhand(self->hand);

		for (np = self->hand->head, id = 0; np != nil; np = np->next, id++)
			;

		print("> ");

		scanf("%d", &chosen);

		if (chosen >= id || chosen < 0) {
			print("Illegal num\n");
			system("sleep 2");
		} else {	
			lookid = 0;
			np = self->hand->head;
			while (lookid != chosen && np != nil) {
				np = np->next;
				lookid++;
			} 
	
			print("Play [%d|%d] Where? L/R\n", np->suit1, np->suit2);
			scanf("%s", direction);
	
			if (strcmp(direction, "L") == 0) {
				where = HEAD;
				if ((exit = playbone(self->hand, gp->field, np, HEAD)) < 0) {
					print("Illegal.\n", exit);
					system("sleep 2");
				} else {
					goto endt;
				}
			} else if (strcmp(direction, "R") == 0) {
				where = TAIL;
				if ((exit = playbone(self->hand, gp->field, np, TAIL)) < 0) {
					print("Illegal.\n", exit);
					system("sleep 2");
				} else {
					goto endt;
				}
			} else if (strcmp(direction, "P") == 0) {
				print("Okay, I'll take your word for it.\n");
				system("sleep 2");
				goto endt;
			} else {
				print("What?\n");
				system("sleep 2");
			}
		}
	}

	return HEAD;

endt:
	system("clear");
	free(direction);

	return where;

}
*/

bone *
strat_rng(game *gp, player *self, int *where)
{
	bone *np;
	int rng, i;

	if (gp == nil || self == nil || where == nil)
		return nil;

	for (;;) {
		rng = nrand(self->hand->amount);
		np = self->hand->head;

		for (i = 0; i < rng; i++)
			np = np->next;

		if (validmove(gp->field, np, HEAD) > 0)
			*where = HEAD;
		else if (validmove(gp->field, np, TAIL) > 0)
			*where = TAIL;
		else
			continue;

		return np;
	}
}
/*
int
strat_rng(game *gp, player *self)
{
	bone *np;
	int rng, i;

	for (;;) {
		rng = nrand(self->hand->amount);
		np = self->hand->head;

		for (i = 0; i < rng; i++)
			np = np->next;

		if (playbone(self->hand, gp->field, np, HEAD) == 0) {
			return HEAD;
		} else if (playbone(self->hand, gp->field, np, TAIL) == 0) {
			return TAIL;
		}
	}
}*/
