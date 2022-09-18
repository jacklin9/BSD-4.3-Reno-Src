/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted provided
 * that: (1) source distributions retain this entire copyright notice and
 * comment, and (2) distributions including binaries display the following
 * acknowledgement:  ``This product includes software developed by the
 * University of California, Berkeley and its contributors'' in the
 * documentation or other materials provided with the distribution and in
 * all advertising materials mentioning features or use of this software.
 * Neither the name of the University nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef lint
static char sccsid[] = "@(#)nova.c	5.4 (Berkeley) 6/1/90";
#endif /* not lint */

# include	"trek.h"

/*
**  CAUSE A NOVA TO OCCUR
**
**	A nova occurs.  It is the result of having a star hit with
**	a photon torpedo.  There are several things which may happen.
**	The star may not be affected.  It may go nova.  It may turn
**	into a black hole.  Any (yummy) it may go supernova.
**
**	Stars that go nova cause stars which surround them to undergo
**	the same probabilistic process.  Klingons next to them are
**	destroyed.  And if the starship is next to it, it gets zapped.
**	If the zap is too much, it gets destroyed.
*/

nova(x, y)
int	x, y;
{
	register int		i, j;
	register int		se;

	if (Sect[x][y] != STAR || Quad[Ship.quadx][Ship.quady].stars < 0)
		return;
	if (ranf(100) < 15)
	{
		printf("Spock: Star at %d,%d failed to nova.\n", x, y);
		return;
	}
	if (ranf(100) < 5)
		return (snova(x, y));
	printf("Spock: Star at %d,%d gone nova\n", x, y);

	if (ranf(4) != 0)
		Sect[x][y] = EMPTY;
	else
	{
		Sect[x][y] = HOLE;
		Quad[Ship.quadx][Ship.quady].holes += 1;
	}
	Quad[Ship.quadx][Ship.quady].stars -= 1;
	Game.kills += 1;
	for (i = x - 1; i <= x + 1; i++)
	{
		if (i < 0 || i >= NSECTS)
			continue;
		for (j = y - 1; j <= y + 1; j++)
		{
			if (j < 0 || j >= NSECTS)
				continue;
			se = Sect[i][j];
			switch (se)
			{

			  case EMPTY:
			  case HOLE:
				break;

			  case KLINGON:
				killk(i, j);
				break;

			  case STAR:
				nova(i, j);
				break;

			  case INHABIT:
				kills(i, j, -1);
				break;

			  case BASE:
				killb(i, j);
				Game.killb += 1;
				break;

			  case ENTERPRISE:
			  case QUEENE:
				se = 2000;
				if (Ship.shldup)
					if (Ship.shield >= se)
					{
						Ship.shield -= se;
						se = 0;
					}
					else
					{
						se -= Ship.shield;
						Ship.shield = 0;
					}
				Ship.energy -= se;
				if (Ship.energy <= 0)
					lose(L_SUICID);
				break;

			  default:
				printf("Unknown object %c at %d,%d destroyed\n",
					se, i, j);
				Sect[i][j] = EMPTY;
				break;
			}
		}
	}
	return;
}