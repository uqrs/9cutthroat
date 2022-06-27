typedef struct game game;	/* (game.h:0/struct game {/) */
typedef struct player player;
typedef struct player {
	pile *hand;						/* player hand */
	int id;							/* player ID */
	bone *(*play)(game *, player *, int *);	/* player 'strategy'; strat.c */
} player;

player *mkplayer(int id, bone *(*strategy)(game *, player *, int *));
void nukeplayer(player *plp);
