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

#include "ll.h"

/* Write a new value. */
ll_error_t ll_isend(LL_PDEV self, unsigned char value)
{
	if (!self || !self->lld_clazz) return LL_E_BADPTR;

	if (self->lld_clazz->llv_send)
	{
		return (*self->lld_clazz->llv_send)(self, value);
	}
	return LL_E_NOTIMPL;
}


/* Set control lines. */
ll_error_t ll_isctl(LL_PDEV self, unsigned value)
{
	if (!self || !self->lld_clazz) return LL_E_BADPTR;

	if (self->lld_clazz->llv_sctl)
	{
		return (*self->lld_clazz->llv_sctl)(self, value);
	}
	return LL_E_NOTIMPL;
}




