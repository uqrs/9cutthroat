#define MAXPLAYERS 4

enum {
	EMPTY,	/* game object just created */
	NEW,	/* game ready to play */
	PLAYING,	/* game in progress */
	WIN,		/* game resulted in victory */
	DRAW,	/* game resulted in a draw */
} states;

typedef struct player player;		/* (player.h:0/struct player {/) */
typedef struct game {
	pile *yard, *field;			/* boneyard and playfield */
	player *players[MAXPLAYERS];	/* player objects participating */
	int nplayers;				/* number of players in game.players */
	int nturns;					/* total number of played turns */
	int turn;					/* index of player currently playing */
	int playedsuits[SIX + 1];		/* amount of every suit on the field */
	int state;					/* see enum states */
} game;

game *mkgame();
int initgame();
int startgame(game *gp, int start);
player *playgame(game *gp, int nturns);
void nukegame(game *gp);
