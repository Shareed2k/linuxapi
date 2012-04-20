/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

	The linuxapi package is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	The linuxapi package is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with the GNU C Library; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
	02111-1307 USA.
*/

#include<firstinclude.h>
#include<stdio.h> // for printf(3), fileno(3), stdin, stdout, stderr
#include<stdlib.h> // EXIT_SUCCESS
#include<unistd.h> // getdtablesize(2), sysconf(3)
#include<sys/resource.h> // for getrlimit(2)
#include<us_helper.h> // for CHECK_NOT_M1()

/*
* This shows the default open files in linux.
* Many ways to find this out...
*
* NOTES:
* - getdtablesize() returns the MAXIMUM number of open files, not the current
* number of open files.
*/

int main(int argc,char** argv,char** envp) {
	int dsize;
	CHECK_NOT_M1(dsize=getdtablesize());
	printf("getdtablesize() returned %d\n",dsize);
	long ldsize;
	CHECK_NOT_M1(ldsize=sysconf(_SC_OPEN_MAX));
	printf("sysconf(_SC_OPEN_MAX) returned %ld\n",ldsize);
	struct rlimit rlim;
	CHECK_NOT_M1(getrlimit(RLIMIT_NOFILE,&rlim));
	printf("rlim_cur is %ld\n",rlim.rlim_cur);
	printf("rlim_max is %ld\n",rlim.rlim_max);
	printf("fileno(stdin) is %d\n",fileno(stdin));
	printf("fileno(stdout) is %d\n",fileno(stdout));
	printf("fileno(stderr) is %d\n",fileno(stderr));
	return EXIT_SUCCESS;
}