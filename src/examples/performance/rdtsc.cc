/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * linuxapi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * linuxapi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with linuxapi. If not, see <http://www.gnu.org/licenses/>.
 */

#include <firstinclude.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>	// for EXIT_SUCCESS, EXIT_FAILURE
#include <unistd.h>	// for usleep(3)
#include <cpufreq.h>
#include <unistd.h>	// for sleep(3)
#include <err_utils.h>	// for CHECK_NOT_M1(), CHECK_ZERO()

/*
 * This is a demo which shows how, on i386 platforms, to read the rdtsc
 * register in order to get a high precision timer. Mind you, you must
 * be running on a machine that does not do frequency scaling in order
 * for this to work...
 *
 * TODO:
 * - add reading of the cpu frequency.
 * - add diffing the two values.
 * - add measuring absolute time via the cpu frequency.
 *
 * If you compare the results of this test with the cpu frequency information
 * provided by the OS you will find a slight difference.
 *
 * Where does this difference come from?
 * The sleep(3) call which is called here is handled by the OS using the PIC
 * interrupt handler clock while the RDTSC register that we call is from the
 * CPU internal clock. This means that we are comparing two different clocks!
 * In addition to all of this setting the fact that the CPU reports that it's
 * frequency is 600 MHz doesn't means that it ticks: 600,000,000 (600 million
 * times) per seconds exactly...
 *
 * EXTRA_LINK_FLAGS=-lcpufreq
 *
 * TODO:
 * - on multi core the RDTSC reports twice as long. Fix this...
 */
void long_task(void) {
	/*
	 * const int sleep_time=100;
	 * CHECK_NOT_M1(usleep(sleep_time));
	 */
	CHECK_ZERO(sleep(1));
	/*
	 * This doesn't really work since the compiler eliminates
	 * the entire loop when optimizing...:)
	 * const int count=1000000;
	 * for(int i=0;i<count;i++) {
	 * }
	 */
}

int main(int argc, char** argv, char** envp) {
	if(argc!=2) {
		fprintf(stderr, "%s: usage: %s [usecs]\n", argv[0], argv[0]);
		fprintf(stderr, "%s: example is 1000000 which means 1 second\n", argv[0]);
		return EXIT_FAILURE;
	}
	unsigned int usecs=atoi(argv[1]);
	printf("starting up...\n");

	printf("lets get the cpu and numa node via getcpu(2)...\n");
	int c, n;
	CHECK_NOT_M1(syscall(__NR_getcpu, &c, &n, NULL));
	printf("getcpu(2) says cpu is [%d] and numa node is [%d]...\n", c, n);

	printf("lets get the cpu via sched_getcpu(3)...\n");
	printf("sched_getcpu() is %d\n", sched_getcpu());

	printf("starting a sleep of 1 second...\n");
	ticks_t start=getticks();
	// CHECK_NOT_M1(usleep(usecs));
	unsigned long sum=0;
	for(unsigned int i=0; i<usecs; i++) {
		sum+=i*i;
	}
	ticks_t end=getticks();
	printf("finished...\n");
	printf("sum=%lu\n", sum);
	ticks_t diff=end - start;
	printf("start RDTSC is %llu\n", start);
	printf("end RDTSC is %llu\n", end);
	printf("diff RDTSC is %llu\n", diff);
	printf("time expired in micros is %u\n", get_mic_diff(start, end));

	// get_clk_tck is no good at giviing you the cpu frequency
	// it gives a 100 which is supposed to be the kernel resched timer
	// but a linux system means very little
	// (Ingo does dynamic ticks anyway...)
	printf("get_clk_tck() is %d\n", get_clk_tck());
	printf("sysconf(_SC_CLK_TCK) is %d\n", CHECK_NOT_M1(sysconf(_SC_CLK_TCK)));

	// this one requires parsing
	const char *cmd="cat /proc/cpuinfo | grep MH";
	printf("going to do command [%s]\n", cmd);
	CHECK_NOT_M1(system(cmd));

	// this one requires root (sudo)
	const char *cmd2="sudo cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq";
	printf("going to do command [%s]\n", cmd2);
	CHECK_NOT_M1(system(cmd2));

	// this one is ok for everyone
	const char *cmd3="cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq";
	printf("going to do command [%s]\n", cmd3);
	CHECK_NOT_M1(system(cmd3));

	// this one is ok for everyone
	const char *cmd4="cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq";
	printf("going to do command [%s]\n", cmd4);
	CHECK_NOT_M1(system(cmd4));

	// this is using libcpufreq (it actually does the about reading of the
	// file in /sys...)
	printf("cpufreq_get_freq_kernel(0) [%lu]\n", cpufreq_get_freq_kernel(0));
	// this is again using libcpufreq but will fail since it tries to access
	// the /sys file that requires root access)
	printf("cpufreq_get_freq_hardware(0) [%lu]\n", cpufreq_get_freq_hardware(0));
	// this is again using libcpufreq but will fail since it tries to access
	// the /proc file that requires root access)
	// printf("proc_get_freq_kernel(0) [%lu]\n",proc_get_freq_kernel(0));
	return EXIT_SUCCESS;
}
