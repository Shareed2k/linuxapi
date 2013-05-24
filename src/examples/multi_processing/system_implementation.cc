/*
 * This file is part of the linuxapi project.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * The linuxapi package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * The linuxapi package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the GNU C Library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA.
 */

#include <stdlib.h>	// for EXIT_SUCCESS, exit(3)
#include <unistd.h>	// for fork(2), execl(2)
#include <sys/types.h>	// for waitpid(2)
#include <sys/wait.h>	// for waitpid(2)
#include <us_helper.h>	// for CHECK_NOT_M1()

/*
 * This example shows how to implement the system library function
 * I try to follow the system(3) manual entry semantics here...
 */

int system_clone(const char *command) {
	pid_t child=fork();
	if(child==-1) {
		return -1;
	}
	if(child) {
		// parent
		int status;
		pid_t child_that_got=waitpid(child, &status, 0);
		if(child_that_got==-1) {
			return -1;
		}
		return status;
	} else {
		// child
		// exec "sh -c command"
		// no need to check return code since if we get to
		// the line after execl it is sure that we have an
		// error
		execl("/bin/sh","/bin/sh","-c",command,NULL);
		// according to system(3) specifications
		exit(127);
	}
}

int main(int argc, char** argv, char** envp) {
	CHECK_NOT_M1(system_clone("ls -l"));
	return EXIT_SUCCESS;
}