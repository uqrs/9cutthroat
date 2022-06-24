#include <u.h>
#include <libc.h>
#include "bone.h"
#include "player.h"
#include "game.h"
#include "strat.h"

game *
mkgame()
{
	return calloc(1, sizeof(game));
}

void
nukegame(game *gp)
{
	int i;

	if (gp == nil)
		return;

	for (i = 0; i < MAXPLAYERS; i++)
		nukeplayer(gp->players[i]);

	nukepile(gp->yard);
	free(gp);
}

int
initgame(game *gp)
{
	int i, j, k, r, handsize;
	bone *np;

	if (gp == nil)
		return -1;

	gp->field = mkpile();
	gp->yard = mkpile();
	if (initpile(gp->yard) < 0) {
		nukepile(gp->yard);
		return -1;
	}

	handsize = gp->yard->amount / 4;

	gp->nplayers = MAXPLAYERS;
	for (i = 0; i < MAXPLAYERS; i++) {
		gp->players[i] = mkplayer(i, strat_rng);
		gp->players[i]->hand = mkpile();
		for (j = 0; j < handsize; j++) {
			r = nrand(gp->yard->amount);

			for (np = gp->yard->head, k = 0; np != nil && k < r; np = np->next, k++)
				;
			addbone(gp->players[i]->hand, rmbone(gp->yard, np), nil);
		}
	}

	gp->state = NEW;

	return 0;
}

int
nextturn(game *gp)
{
	if (gp == nil)
		return -1;

	gp->nturns++;
	gp->turn = (gp->turn + 1) % gp->nplayers;

	return gp->turn;
}

int
canplay(player *plp, pile *pp)
{
	bone *bp;

	for (bp = plp->hand->head; bp != nil; bp = bp->next)
		if (validmove(pp, bp, HEAD) || validmove(pp, bp, TAIL))
			return 1;
	return 0;
}

int
startgame(game *gp, int start)
{
	bone *bp;
	int i;

	if (gp == nil || ((start > gp->nplayers || start < 0) && start != 66))
		return -1;

	gp->state = PLAYING;

	if (start == 66) {
		for (i = 0; i < gp->nplayers; i++) {
			if ((bp = findbone(gp->players[i]->hand, SIX, SIX)) != nil) {
				gp->turn = i;
				playbone(gp->players[i]->hand, gp->field, bp, HEAD);
				gp->playedsuits[SIX] = 2;

				nextturn(gp);

				return 0;
			}
		}
	}

	gp->turn = start;

	return 0;
}

player *
playgame(game *gp, int nturns)
{
	player *plp, *candidate;
	bone *np, *head, *tail;
	int i, lowest, draw, total, *where;

	if (gp == nil || nturns < 0)
		return nil;

	if (gp->state != PLAYING)
		return nil;

	if (nturns == 0)
		nturns = 10000;

	where = calloc(1, sizeof(int));

	for(i = 0; i < nturns; i++) {
		plp = gp->players[gp->turn];

		if (canplay(plp, gp->field) == 0) {
			print("Player %d PASS!\n", plp->id);
			nextturn(gp);
			continue;
		}

		np = plp->play(gp, plp, where);

		if (where == nil || *where < HEAD || *where > TAIL
			|| playbone(plp->hand, gp->field, np, *where) < 0) {
			i--;
			continue;
		}

		print("Player %d plays [%d:%d]\n", plp->id, np->suit1, np->suit2);

		gp->playedsuits[np->suit1]++;
		gp->playedsuits[np->suit2]++;

		if (plp->hand->amount == 0)
			goto win;

		head = getbone(gp->field, HEAD);
		tail = getbone(gp->field, TAIL);

		if (head->suit1 == tail->suit2 && gp->playedsuits[head->suit1] == nsuits+1)
			goto block;

		nextturn(gp);
	}

	return plp;

win:
	gp->state = WIN;

	return plp;

block:
	lowest = 1000000;
	draw = 0;
	candidate = nil;

	for (i = 0, plp = gp->players[i]; i < gp->nplayers; plp = gp->players[++i]) {
		total = 0;
		for (np = plp->hand->head; np != nil; np = np->next)
			total += np->suit1 + np->suit2;

		print("Player %d has %d\n", plp->id, total);

		if (total < lowest) {
			draw = 0;
			candidate = plp;
			lowest = total;
		} else if (total == lowest) {
			draw = 1;
		}
	}

	if (draw == 1)
		gp->state = DRAW;
	else
		gp->state = WIN;

	return candidate;
}
