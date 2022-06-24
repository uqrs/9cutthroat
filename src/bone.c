#include <u.h>
#include <libc.h>
#include <stdio.h>
#include "bone.h"

int nsuits = SIX + 1;

pile *
mkpile()
{
	return calloc(1, sizeof(pile));
}

void
nukepile(pile *pp)
{
	bone *np, *next;

	if (pp == nil)
		return;

	for (np = pp->head; np != nil; np = next) {
		next = np->next;
		nukebone(np);
	}

	free(pp);
}

int
initpile(pile *pp)
{
	int i, j;

	if (pp == nil)
		return -1;

	for (i = 0; i < nsuits; i++)
		for (j = i; j < nsuits; j++)
			if (addbone(pp, mkbone(i, j), nil) < 0)
				return -1;

	return 0;
}

bone *
mkbone(int suit1, int suit2)
{
	bone *bp;

	if (suit1 < BLANK || suit1 > SIX || suit2 < BLANK || suit2 > SIX)
		return nil;

	bp = calloc(1, sizeof(bone));
	bp->suit1 = suit1;
	bp->suit2 = suit2;

	return bp;
}

void
nukebone(bone *bp)
{
	free(bp);
}

/* insert bp into pp->head after where */
int
addbone(pile *pp, bone *bp, bone *where)
{
	bone *np;

	if (pp == nil || bp == nil)
		return -1;

	if (pp->head == nil || where == nil) {
		bp->next = pp->head;
		pp->head = bp;
		pp->amount++;
		return 0;
	}

	for (np = pp->head; np != nil; np = np->next) {
		if (np == where) {
			bp->next = np->next;
			np->next = bp;
			pp->amount++;
			return 0;
		}
	}

	/* where not in list */
	return -1;
}

bone *
rmbone(pile *pp, bone *bp)
{
	bone *np;

	if (pp == nil || bp == nil)
		return nil;

	if (bp == pp->head) {
		pp->head = bp->next;
		bp->next = nil;

		pp->amount--;
		return bp;
	}

	for (np = pp->head; np != nil; np = np->next) {
		if (np->next == bp) {
			np->next = bp->next;
			bp->next = nil;

			pp->amount--;
			return bp;
		}
	}

	/* not in list */
	return nil;
}

bone *
flipbone(bone *bp)
{
	int swap;

	if (bp == nil)
		return nil;

	swap = bp->suit1;
	bp->suit1 = bp->suit2;
	bp->suit2 = swap;

	return bp;
}

/* check if playing bp to where in pp is a valid move (1), or not (0), or if the bone must be flipped (2) */
int
validmove(pile *pp, bone *bp, int where)
{
	bone *np;

	if (pp == nil || pp->head == nil || bp == nil || where < HEAD || where > TAIL)
		return -1;

	if (pp->head == nil) {
		return 1;
	} else if (where == HEAD) {
		if (bp->suit2 == pp->head->suit1)
			return 1;
		else if (bp->suit1 == pp->head->suit1)
			return 2;
	} else if (where == TAIL) {
		for (np = pp->head; np->next != nil; np = np->next)
			;
		if (bp->suit1 == np->suit2)
			return 1;
		else if (bp->suit2 == np->suit2)
			return 2;
	}

	return 0;
}

int
playbone(pile *ppfrom, pile *ppto, bone *bp, int where)
{
	bone *after;
	int exit;

	if (ppfrom == nil || ppto == nil || bp == nil || where < HEAD || where > TAIL)
		return -2;

	if ((exit = validmove(ppto, bp, where)) == 0)
		return -1;
	else if (exit == 2)
		flipbone(bp);

	if (where == HEAD)
		after = nil;
	else if (where == TAIL)
		for (after = ppto->head; after->next != nil; after = after->next)
			;

	return addbone(ppto, rmbone(ppfrom, bp), after);
}

bone *
findbone(pile *pp, int suit1, int suit2)
{
	bone *np;

	if (pp == nil || pp->head == nil || suit1 < BLANK || suit1 > SIX || suit2 < BLANK || suit2 > SIX)
		return nil;

	for (np = pp->head; np != nil; np = np->next) {
		if (suit1 == np->suit1 && suit2 == np->suit2)
			return np;
		np = flipbone(np);
		if (suit1 == np->suit1 && suit2 == np->suit2)
			return np;	
	}

	/* not in list */
	return nil;
}

bone *
findsbone(pile *pp, int suit, bone *prev)
{
	bone *np;

	if (pp == nil || suit < BLANK || suit > SIX)
		return nil;

	if (prev == nil)
		np = pp->head;
	else
		np = prev->next;

	for (; np != nil; np = np->next)
		if (np->suit1 == suit || np->suit2 == suit)
			return np;

	return nil;
}

bone *
getbone(pile *pp, int where)
{
	bone *np;

	if (pp == nil || where < HEAD || where > TAIL)
		return nil;

	if (where == HEAD)
		return pp->head;

	for (np = pp->head; np->next != nil; np = np->next)
		;

	return np;
}
