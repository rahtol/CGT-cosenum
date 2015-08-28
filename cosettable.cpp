#include <crtdbg.h>
#include <stdio.h>
#include <memory.h>
#include "cosettable.h"

/*****************************************************************************/

CCosetTable::CCosetTable (int cardinalityX, int M)
{
	this->k = 2*cardinalityX; /* #A = 2*#X */
	this->M = M;

	p = new int [M+1];
	memset(p, 0, (M+1)*sizeof(int));

	chi = new int [k*M];
	memset(chi, 0, k*M*sizeof(int));

	q = new int [M+1];
	memset(q, 0, (M+1)*sizeof(int));
	nq = 0;

	n = 1;
	p [1] = 1;
};

/*****************************************************************************/

CCosetTable::~CCosetTable ()
{
	delete [] p;
	delete [] chi;
	delete [] q;
};

/*****************************************************************************/

int CCosetTable::getchi(int alpha, int x) 
{
	_ASSERT(alpha>0 && alpha<=n);
	_ASSERT(x>0 && x<=k);

	return chi[(alpha-1)*k+(x-1)]; 
};

/*****************************************************************************/

void CCosetTable::setchi(int alpha, int x, int beta) 
{ 
	_ASSERT(alpha>0 && alpha<=n);
	_ASSERT(x>0 && x<=k);
	_ASSERT(beta>=0 && beta<=n);

	chi[(alpha-1)*k+(x-1)] = beta; 
};

/*****************************************************************************/

void CCosetTable::push (int alpha) 
{ 
	q [nq++] = alpha; 
};

/*****************************************************************************/

int CCosetTable::pop () 
{
	_ASSERT(nq>0);

	return q [--nq];
};

/*****************************************************************************/

void CCosetTable::define (int alpha, int x)
{
	_ASSERT (getchi (alpha, x) == 0);

	if(n>=M)
	{
		throw "Aborted.";
	};

	n++;
	p[n] = n;
	setchi (alpha, x, n);
	setchi (n, inv(x), alpha);
};

/*****************************************************************************/

int CCosetTable::scan (int alpha, int *w)
{
	int r = w[0]; /* length(w) */
	int i = 1;
	int j = r;
	int f = alpha;
	int b= alpha;
	int fxi;
	int bxj;
	int rc = 0;

	/* Scan forwards */
	while ((i <= r) && ((fxi=getchi(f,w[i])) != 0))
	{
		f = fxi;
		i++;
	};

	if (i > r) 
	{
		if (f != alpha)
		{
			coincidence (f, alpha);
			rc = 1;
		}
		else
		{
			rc = 2;
		};
	}
	else
	{

		/* Forward scan was incomplete. Scan backwards. */
		while ((j >= i) && ((bxj = getchi(b, inv(w[j]))) != 0))
		{
			b = bxj;
			j--;
		};

		if (j < i)
		{
			coincidence (f, b);
			rc = 3;
		}
		else
		{
			if (j == i)
			{
				/* Deduction */
				setchi (f, w[i], b);
				setchi (b, inv(w[i]), f);
				rc = 4;
			}
			else
			{
				/* j>i, scan is incomplete and yields no information */
				rc = 5;
			}
		}
	};
	
	return rc;
};

/*****************************************************************************/

void CCosetTable::scanandfill (int alpha, int *w)
{
	int r = w[0]; /* length(w) */
	int i = 1;
	int j = r;
	int f = alpha;
	int b = alpha;
	int fxi;
	int bxj;
	int cont = 1;

	while (cont != 0) 
	{
		cont = 0;
		/* Scan forwards */
		while ((i <= r) && ((fxi=getchi(f,w[i])) != 0))
		{
			f = fxi;
			i++;
		};

		if (i > r) 
		{
			if (f != alpha)
			{
				coincidence (f, alpha);
			};
		}
		else
		{

			/* Forward scan was incomplete. Scan backwards. */
			while ((j >= i) && ((bxj = getchi(b, inv(w[j]))) != 0))
			{
				b = bxj;
				j--;
			};

			if (j < i)
			{
				coincidence (f, b);
			}
			else
			{
				if (j == i)
				{
					/* Deduction */
					setchi (f, w[i], b);
					setchi (b, inv(w[i]), f);
				}
				else
				{
					/* j>i, scan is incomplete -> new definition and continue */
					define (f, w[i]);
					cont = 1;
				}
			}
		}
	}
};

/*****************************************************************************/

void CCosetTable::coincidence (int alpha, int beta)
{
	merge (alpha, beta);

	while (nq != 0)
	{
		int gamma = pop();
		_ASSERT (p[gamma] < gamma); /* not a live coset any more */

		for (int x=1; x<=k; x++)
		{
			int delta = getchi (gamma, x);
			if (delta != 0)
			{
				setchi (delta, inv(x), 0); /* undefine delta^(x^-1) */
				int mu = rep (gamma);
				int nu = rep (delta);
				int mux = getchi (mu, x);
				if (mux != 0)
				{
					merge (nu, mux);
				}
				else
				{
					int nuinvx = getchi (nu, inv(x));
					if (nuinvx != 0)
					{
						merge (mu, nuinvx);
					}
					else
					{
						setchi (mu, x, nu);
						setchi (nu, inv(x), mu);
					}
				}
			}
		}
	}
};

/*****************************************************************************/

int CCosetTable::rep (int kappa)
{
	int lambda = kappa;
	int rho = p [lambda];
	while (lambda != rho) {
		lambda = rho;
		rho = p [lambda];
	};

	/* now perform path compression */
	int mu = kappa;
	rho = p [mu];
	while (lambda != rho) {
		int tmp = p [mu];
		p [mu] = lambda;
		mu = rho;
		rho = tmp;
	};

	return lambda;
};

/*****************************************************************************/

void CCosetTable::merge (int kappa, int lambda)
{
	int fi = rep (kappa);
	int psi = rep (lambda);
	if (fi != psi)
	{
		int mu = (fi < psi ? fi : psi);  /* Min(fi,psi) */
		int nu = (fi < psi ? psi : fi);  /* Max(fi,psi) */
		p[nu] = mu;
		push (nu);
	}
};

/*****************************************************************************/

bool CCosetTable::cosetenumerationR (int **R, int**Y)
{
	try {

	for (int **pY = Y; (*pY) != NULL; pY++)
	{
		scanandfill (1, *pY);
	};

	for (int alpha = 1; alpha <= n; alpha++)
	{
		for (int **pR = R; ((*pR) != NULL) && (p[alpha] == alpha) ; pR++)
		{
			scanandfill (alpha, *pR);
		};

		if(p[alpha] == alpha)
		{
			for (int x=1; x<=k; x++)
			{
				if(getchi(alpha,x)==0)
				{
					define(alpha,x);
				}
			}
		}
	};

		return true;
	}
	catch (char *msg)
	{
		return false;
	}
};

/*****************************************************************************/

int CCosetTable::noLiveCosets ()
{
	int nLive = 0;

	for (int alpha = 1; alpha <= n; alpha++)
	{
		if(p[alpha] == alpha) nLive++;
	};

	return nLive;
};

/*****************************************************************************/

void CCosetTable::compress ()
{
};

/*****************************************************************************/

void CCosetTable::print()
{
	printf ("n=%d, k=%d, live=%d\n", n, k, noLiveCosets());
};

/*****************************************************************************/
