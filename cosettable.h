/*
 * words from A* to be supplied as int[len+1] with len=int[0] and w[i]=int[i]
 */

class CCosetTable {
public:
	int *p; // p : [1..n] -> [1..n] where p[a]=a for all live cosets a and p[x]<x for merged cosets
	int *chi; //chi : [1..n] x A -> [0..n] defines action of coset alpha on x el A, 0=undefined
	int n; // number of allocated cosets, -N-O-T- live cosets !!!
	int M; // coset limit
	int k; // cardinality of A = X united X^-1 = 2 * cardinality of X
	int *q; // queue of cosets that have been merged and are waiting for update in chi
	int nq; // number of entries in queue q

public:
	CCosetTable (int cardinalityX, int M);
	~CCosetTable ();

	int getchi(int alpha, int x);
	void setchi(int alpha, int x, int beta);
	void push (int alpha);
	int pop ();

	void define (int alpha, int x);
	int scan (int alpha, int *w);
	void scanandfill (int alpha, int *w);
	void coincidence (int alpha, int beta);
	int rep (int kappa);
	void merge (int kappa, int lambda);

	bool cosetenumerationR (int **R, int**Y);
	void compress ();

	int noLiveCosets ();
	void print();

};

inline int inv (int x) { return ((x%2) == 0 ? x-1 : x+1); };
