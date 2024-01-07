/***************************************************************************
 *                                                                         *
 *    LIBDSK: General floppy and diskimage access library                  *
 *    Copyright (C) 2001  John Elliott <seasip.webmaster@gmail.com>            *
 *                                                                         *
 *    This library is free software; you can redistribute it and/or        *
 *    modify it under the terms of the GNU Library General Public          *
 *    License as published by the Free Software Foundation; either         *
 *    version 2 of the License, or (at your option) any later version.     *
 *                                                                         *
 *    This library is distributed in the hope that it will be useful,      *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU    *
 *    Library General Public License for more details.                     *
 *                                                                         *
 *    You should have received a copy of the GNU Library General Public    *
 *    License along with this library; if not, write to the Free           *
 *    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,      *
 *    MA 02111-1307, USA                                                   *
 *                                                                         *
 ***************************************************************************/

/* This program can display or change the serial number of a MicroDesign
 * floppy disc. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#ifdef HAVE_LIBGEN_H
# include <libgen.h>
#endif
#include "libdsk.h"
#include "utilopts.h"

#ifdef __PACIFIC__
# define AV0 "MD3SERIA"
#else
# ifdef HAVE_BASENAME
#  define AV0 (basename(argv[0]))
# else
#  define AV0 argv[0]
# endif
#endif

int do_md3(char *outfile, char *outtyp, char *outcomp, int forcehead, char *newserial);

int help(int argc, char **argv)
{
	fprintf(stderr, "Syntax: \n"
                "      %s { -serial <serialno> } { -type <type> } { -side <side> } dskimage\n",
			AV0);
	fprintf(stderr,"\nDefault type is autodetect.\n\n");
		
	fprintf(stderr, "eg: %s myfile.DSK\n"
                        "    %s -serial 1234567 myfile.DSK \n", AV0, AV0);
	return 1;
}


char *check_serial(char *arg, int *argc, char **argv)
{
        int n = find_arg(arg, *argc, argv);
	char *ser;

        if (n < 0) return NULL;
        excise_arg(n, argc, argv);
        if (n >= *argc)
        {
                fprintf(stderr, "Syntax error: use '%s <serialnumber>'\n", arg);
                exit(1);
        }
	ser = argv[n];
        excise_arg(n, argc, argv);
	return ser;
}






int main(int argc, char **argv)
{
	char *outtyp;
	char *outcomp;
	char *newser;
	int forcehead;
        int stdret;

        stdret = standard_args(argc, argv); if (!stdret) return 0;
	if (argc < 2) return help(argc, argv);

        ignore_arg("-itype", 2, &argc, argv);
        ignore_arg("-iside", 2, &argc, argv);
        ignore_arg("-icomp", 2, &argc, argv);
        ignore_arg("-otype", 2, &argc, argv);
        ignore_arg("-oside", 2, &argc, argv);
        ignore_arg("-ocomp", 2, &argc, argv);

	outtyp    = check_type  ("-type",   &argc, argv);
	outcomp   = check_type  ("-comp",   &argc, argv);
	newser    = check_serial("-serial", &argc, argv);
	forcehead = check_forcehead("-side", &argc, argv);	
        if (find_arg("--help",    argc, argv) > 0) return help(argc, argv);

	return do_md3(argv[1], outtyp, outcomp, forcehead, newser);
}


char *unscramble(unsigned char *s)
{
	static unsigned char buf[8];
	unsigned char b, *t;

	t = buf;	
	for (b = 7; b > 0; b--)
	{
		*t = (*s) ^ 0xA5 ^ b;
		++s;
		++t;	
	}
	return (char *)buf;
}

char *scramble(unsigned char *s)
{
	static unsigned char buf[8];
	unsigned char sbuf[8];
	unsigned char b, *t;

	sprintf((char *)sbuf, "%-7.7s", s);
	s = sbuf;
	t = buf;	
	for (b = 7; b > 0; b--)
	{
		*t = (*s) ^ 0xA5 ^ b;
		++s;
		++t;	
	}
	return (char *)buf;
}


int do_md3(char *outfile, char *outtyp, char *outcomp, int forcehead, char *newser)
{
	DSK_PDRIVER outdr = NULL;
	dsk_err_t e;
	DSK_GEOMETRY dg;
	dsk_pcyl_t c;
	dsk_phead_t h;
	dsk_psect_t s;
	unsigned char buf[256];
	
	e = dsk_open(&outdr, outfile, outtyp, outcomp);
	if (!e && forcehead >= 0) e = dsk_set_forcehead(outdr, forcehead);
	if (!e) e = dsk_getgeom(outdr, &dg);
	dg.dg_secsize = 256;	/* MD3 discs have 256-byte sectors in the
				 * copy-protection track */
	if (!e)
	{
		/* On all Creative Technology copy-protected discs observed
		 * so far, the copy-protection track has been the fourth
		 * track (logical track number 3):
		 *  - Single-sided pcw180: cylinder 3
		 *  - Double-sided pcw720 (SIDES_ALT): cylinder 1, head 1 */
		e = dg_lt2pt(&dg, 3, &c, &h);
	}
	if (!e)
	{
		/* The serial number has been in the first sector of this
		 * track on all discs observed so far. */
		s = 1;
		e = dsk_pread(outdr, &dg, buf, c, h, s);
	}
	if (!e) 	
	{
		unsigned int i;

                printf("Copy-protection sector from C%u/H%u/S%u:\n", c, h, s);
                /* The System and Version information mostly appear not
                 * significant for copy-protection; most programs run
                 * regardless of their value (exception: Tweak 2).
                 * MDMAKE type programs on master discs will display them
                 * to the user when describing what they are copying.
                 * The main program doesn't obviously use them at all. */
                printf("System:        %-12.12s\n", buf);
                printf("Version:       %-5.5s\n", buf+12);
                printf("Serial no:     %s\n", unscramble(buf+17));
		/* The bytes from 0x18 to 0x28 appear to be magic bytes
		 * used to verify the disc's validity and whether it is a
		 * master disc; they affect whether MDMAKE/INSTALL type
		 * programs working, and can differ between MD2-era and
		 * MD3-era master discs, at least. */
		printf("Magic:         ");
		for (i = 0x18; i < 0x29; i++)
		{
			printf("%02X ", buf[i]);
		}
		printf("\n");
                if (buf[0x29] != 0xE5) printf("Type code:     %c\n", buf[0x29]);
		else		       printf("No type code.\n");
		if (newser)
		{
			memcpy(buf+17, scramble((unsigned char *)newser), 7);
			e = dsk_pwrite(outdr, &dg, buf, c, h, s);
                	if (!e) printf("New serial no: %s\n", unscramble(buf+17));
		}
	}
	if (outdr) 
	{
		if (!e) e = dsk_close(&outdr); else dsk_close(&outdr);
	}
	if (e)
	{
		fprintf(stderr, "Could not get MD3 serial number from disc.\n");
		fprintf(stderr, "Possibly this disc does not have MD3 copy\n");
		fprintf(stderr, "protection.\n");
		fprintf(stderr, "%s\n", dsk_strerror(e));
		return 1;
	}
	return 0;
}

