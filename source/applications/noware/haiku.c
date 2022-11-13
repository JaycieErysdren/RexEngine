// ========================================================
//
// FILE:			/source/applications/noware/haiku.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			NOWARE
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Haiku
//
// LAST EDITED:		November 13th, 2022
//
// ========================================================

// Headers
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

// Random between high and low
int jrandom(int low, int high)
{
	return (rand() % (high - low + 1)) + low;
}

// Get 5 syllable sentence
char *haiku5(void)
{
	int r = jrandom(0, 31);

	switch (r)
	{
		case 1: return "rebel boat rocker";
		case 2: return "colostomy bag";
		case 3: return "walking in memphis";
		case 4: return "exploding midget";
		case 5: return "white cotton panties";
		case 6: return "nub technology";
		case 7: return "anonymous nub";
		case 8: return "more bunny justice";
		case 9: return "evil evil boy";
		case 10: return "you have been a fool";
		case 11: return "do not send me porn";
		case 12: return "i must escape it";
		case 13: return "those creamy white thighs";
		case 14: return "keeping us all cool";
		case 15: return "air conditioning";
		case 16: return "so what if I sweat";
		case 17: return "fresh air is so sweet";
		case 18: return "live mice sit on us";
		case 19: return "digital squirrel";
		case 20: return "liquid banana";
		case 21: return "peas and hominy";
		case 22: return "miscellaneous";
		case 23: return "gravity assist";
		case 24: return "random tuna head";
		case 25: return "some quick goat thinking";
		case 26: return "a velvet codpiece";
		case 27: return "let's tease the weasels";
		case 28: return "nice vomiting spell";
		case 29: return "a specific dick";
		case 30: return "the growing yodas";
		default: return "i have to call chuck";
	}
}

// Get 7 syllable sentence
char *haiku7(void)
{
	int r = jrandom(0, 18);

	switch (r)
	{
		case 1: return "jello nuclear bedtime story";
		case 2: return "crazy customizable";
		case 3: return "programmer's head on a stick";
		case 4: return "frictionless, massless mormon";
		case 5: return "put it in your report, bill";
		case 6: return "i don't need the influence";
		case 7: return "i'm coming out of the booth";
		case 8: return "in the middle of a storm";
		case 9: return "it tantalizes me so";
		case 10: return "you are my favorite thing";
		case 11: return "i leave the windows open";
		case 12: return "and fed you to the chickens";
		case 13: return "candy corn chicken fetus";
		case 14: return "the silhouette of a dog";
		case 15: return "guns exploding all around";
		case 16: return "a rising column of smoke";
		case 17: return "you don't jump like a monkey";
		default: return "the half-life of lime";
	}
}

// Program entry point
int main(int argc, char *argv[])
{
	srand(time(NULL));

	printf("\n  %s\n  %s\n  %s\n\n", haiku5(), haiku7(), haiku5());

	return EXIT_SUCCESS;
}
