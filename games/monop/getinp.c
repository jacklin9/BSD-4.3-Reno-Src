/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that: (1) source distributions retain this entire copyright
 * notice and comment, and (2) distributions including binaries display
 * the following acknowledgement:  ``This product includes software
 * developed by the University of California, Berkeley and its contributors''
 * in the documentation or other materials provided with the distribution
 * and in all advertising materials mentioning features or use of this
 * software. Neither the name of the University nor the names of its
 * contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef lint
static char sccsid[] = "@(#)getinp.c	5.3 (Berkeley) 6/1/90";
#endif /* not lint */

# include	<stdio.h>
# include	<ctype.h>

# define	reg	register

# define	LINE	70

static char	buf[257];

getinp(prompt, list)
char	*prompt, *list[]; {

	reg int	i, n_match, match;
	char	*sp;
	int	plen;


	for (;;) {
inter:
		printf(prompt);
		for (sp = buf; (*sp=getchar()) != '\n'; )
			if (*sp == -1)	/* check for interupted system call */
				goto inter;
			else if (sp != buf || *sp != ' ')
				sp++;
		if (buf[0] == '?' && buf[1] == '\n') {
			printf("Valid inputs are: ");
			for (i = 0, match = 18; list[i]; i++) {
				if ((match+=(n_match=strlen(list[i]))) > LINE) {
					printf("\n\t");
					match = n_match + 8;
				}
				if (*list[i] == '\0') {
					match += 8;
					printf("<RETURN>");
				}
				else
					printf(list[i]);
				if (list[i+1])
					printf(", ");
				else
					putchar('\n');
				match += 2;
			}
			continue;
		}
		*sp = '\0';
		for (sp = buf; *sp; sp++)
			if (isupper(*sp))
				*sp = tolower(*sp);
		for (i = n_match = 0; list[i]; i++)
			if (comp(list[i])) {
				n_match++;
				match = i;
			}
		if (n_match == 1)
			return match;
		else if (buf[0] != '\0')
			printf("Illegal response: \"%s\".  Use '?' to get list of valid answers\n", buf);
	}
}

static
comp(s1)
char	*s1; {

	reg char	*sp, *tsp, c;

	if (buf[0] != '\0')
		for (sp = buf, tsp = s1; *sp; ) {
			c = isupper(*tsp) ? tolower(*tsp) : *tsp;
			tsp++;
			if (c != *sp++)
				return 0;
		}
	else if (*s1 != '\0')
		return 0;
	return 1;
}
