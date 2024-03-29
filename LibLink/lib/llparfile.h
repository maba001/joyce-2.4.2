/* liblink: Simulate parallel / serial hardware for emulators

    Copyright (C) 2002  John Elliott <seasip.webmaster@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/* Simulating a parallel port using a file */
typedef struct ll_parfile
{
	LL_DEVICE	 llpf_super;
	FILE		*llpf_fp;
	int		 llpf_latch;
	unsigned	 llpf_selected;
} LL_PARFILE;


/* Open connection */
ll_error_t llpf_open(LL_PDEV self);
/* Close connection */
ll_error_t llpf_close(LL_PDEV self);
/* Flush data */
ll_error_t llpf_flush(LL_PDEV self);
/* Sending data */
ll_error_t llpf_send(LL_PDEV self, unsigned char b);
/* Sending data */
ll_error_t llpf_send_byte(LL_PDEV self, unsigned char b);
/* Setting control lines */
ll_error_t llpf_sctl(LL_PDEV self, unsigned b);
/* Reading control lines */
ll_error_t llpf_rctl_poll(LL_PDEV self, unsigned *b);
 


