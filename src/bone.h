enum {
	BLANK,
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX
} suits;
int nsuits;

enum {
	HEAD=1,
	TAIL,
} ends;

typedef struct bone bone;
typedef struct bone {
	bone *next;
	int suit1, suit2;	/* suit1 "faces" to the left, or the HEAD of the list. suit2 to the right, or TAIL */
} bone;

typedef struct pile {
	bone *head;	/* head of list */
	int amount;	/* number of bones in list */
} pile;

pile *mkpile();
int initpile(pile *pp);
bone *mkbone(int suit1, int suit2);
int addbone(pile *pp, bone *bp, bone *where);
bone *rmbone(pile *pp, bone *bp);
bone *flipbone(bone *bp);
int validmove(pile *pp, bone *bp, int where);
int playbone(pile *ppfrom, pile *ppto, bone *bp, int where);
bone *findbone(pile *pp, int suit1, int suit2);
bone *findsbone(pile *pp, int suit, bone *prev);
bone *getbone(pile *pp, int where);
void nukebone(bone *bp);
void nukepile(pile *pp);
