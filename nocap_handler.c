/*-
 * Copyright (c) 2023 Dell EMC
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/param.h>
#include <sys/dnv.h>
#include <sys/nv.h>
#include <sys/procdesc.h>

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "netdumpd.h"

/*
 * The handler capability lets us invoke a script upon completion (successful or
 * otherwise) of a netdump. The script is executed with cwd set to the dumpdir.
 */

int
netdump_handler(const char *reason, const char *ip,
    const char *hostname, const char *infofile, const char *corefile)
{
	nvlist_t *nvl;
	int error, pd;

	nvl = nvlist_create(0);
	nvlist_add_string(nvl, "cmd", "exec_handler");
	nvlist_add_string(nvl, "reason", reason);
	nvlist_add_string(nvl, "ip", ip);
	nvlist_add_string(nvl, "hostname", hostname);
	nvlist_add_string(nvl, "infofile", infofile);
	nvlist_add_string(nvl, "corefile", corefile);
	// TODO: not sure how to remove this call, will try commenting out
	// nvl = cap_xfer_nvlist(cap, nvl);
	// if (nvl == NULL)
	// 	return (-1);

	error = (int)dnvlist_get_number(nvl, "error", 0);
	pd = dnvlist_take_descriptor(nvl, "procdesc", -1);
	nvlist_destroy(nvl);

	if (error != 0) {
		if (pd != -1) {
			(void)close(pd);
			pd = -1;
		}
		errno = error;
	}
	return (pd);
}
