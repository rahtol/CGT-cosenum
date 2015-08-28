// cosenum.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "cosettable.h"
#include <time.h>

void example51 ()
{
	int x3[] = {3, 1, 1, 1};
	int y3[] = {3, 3, 3, 3};
	int xiyixy[] = {4, 2, 4, 1, 3};
	int x[] = {1, 1};

	CCosetTable ct (2, 100);
	ct.scan (1, x);
	ct.define (1, 3);
	ct.define (1, 4);
	ct.scan (1, y3);
	ct.scan (1, xiyixy);
	ct.scan (2, xiyixy);

	ct.print ();

	int *(R[]) = {x3, y3, xiyixy, 0};
	int *Y[] = {x, NULL};
	CCosetTable ct2 (2, 10);
	ct2.cosetenumerationR (R, Y);
	ct2.print ();
//	CCosetTable ct2c(ct2); // compress 
};

void example54()
{
	int abci[] = {3,1,3,6};
	int bcdi[] = {3,3,5,8};
	int cdei[] = {3,5,7,10};
	int deai[] = {3,7,9,2};
	int eabi[] = {3,9,1,4};
	int a[] = {1,1};
	int *(R[]) = {abci, bcdi, cdei, deai, eabi, NULL};
	int *Y[] = {a, NULL};
	int *Yt[] = {NULL};

	CCosetTable ct54 (5, 200);
	ct54.cosetenumerationR (R, Y);
	ct54.print ();

	CCosetTable ct54b (5, 200);
	ct54b.cosetenumerationR (R, Yt);
	ct54b.print ();

};

void exampleM12()
{
	int a11[] = {11, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; 
	int c2[] = {2, 3, 3}; 
	int d2[] = {2, 5, 5}; 
	int acacac[] = {6, 1, 3, 1, 3, 1, 3}; 
	int adadad[] = {6, 1, 5, 1, 5, 1, 5}; 
	int cd10[] = {20, 3, 5, 3, 5, 3, 5, 3, 5, 3, 5, 3, 5, 3, 5, 3, 5, 3, 5, 3, 5}; 
	int a2cdcdadicidici[] = {11, 1, 1, 3, 5, 3, 5, 1, 6, 4, 6, 4};

	int *(R[]) = {a11, c2, d2, acacac, adadad, cd10, a2cdcdadicidici, NULL};
	int *Y[] = {NULL};

	CCosetTable ctM12 (3, 800000);
	if (ctM12.cosetenumerationR (R, Y))
	{
		printf("Ok.\n");
		ctM12.print ();
	}
	else
	{
		printf("#### Error ####\n");
	};

};

int _tmain(int argc, _TCHAR* argv[])
{

	clock_t start, finish;
	double  duration;

	start = clock();
	exampleM12();
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("duration=%5.3fs\n", duration);

	return 0;
}

