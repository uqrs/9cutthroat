#include <u.h>
#include <libc.h>
#include <stdio.h>
#include "bone.h"
#include "game.h"
#include "player.h"
#include "strat.h"

void
playsolitaire(pile *boneyard)
{
	int id, lookid;
	int chosen;
	int exit;
	char *direction;
	bone *np;
	pile *field;

	direction = malloc(512);

	field = mkpile();

	addbone(field, rmbone(boneyard, findbone(boneyard, SIX, SIX)), nil);
	system("clear");

	while(boneyard->amount > 0) {
		id = 0;
		renderpile(field);

		print("\n\n");
		for (np = boneyard->head; np != nil; np = np->next, id++)
			print("{%d}	[%d|%d]\n", id, np->suit1, np->suit2);
		print("> ");

		scanf("%d", &chosen);

		if (chosen >= id || chosen < 0) {
			print("Illegal.\n");
			system("sleep 2");
		} else {
			lookid = 0;
			np = boneyard->head;
			while (lookid != chosen && np != nil) {
				np = np->next;
				lookid++;
			} 

			print("Play [%d|%d] Where? L/R\n", np->suit1, np->suit2);
			scanf("%s", direction);

			if (strcmp(direction, "L") == 0) {
				if ((exit = playbone(boneyard, field, np, HEAD)) < 0) {
					print("Illegal: %d\n", exit);
					system("sleep 2");
				}
			} else if (strcmp(direction, "R") == 0) {
				if ((exit = playbone(boneyard, field, np, TAIL)) < 0) {
					print("Illegal: %d\n", exit);
					system("sleep 2");
				}
			} else {
				print("What?\n");
				system("sleep 2");
			}
		}

		system("clear");
	}
}

void
main() {
	game *gp;
	player *plp;

	srand(time(0));
	gp = mkgame();
	initgame(gp);

	/* uncomment to play as player 0 */
	/* gp->players[0]->play = strat_real; */

	startgame(gp, 66);
	plp = playgame(gp, 0);

	if (plp == nil) {
		print("Oops...\n");
		nukegame(gp);
		exits("fail");
	}
	
	if (gp->state == WIN)
		print("VICTORY for player %d\n", plp->id);
	else if (gp->state == DRAW)
		print("DRAW.\n");
	else
		print("What?\n");

	nukegame(gp);

	exits(0);
}
