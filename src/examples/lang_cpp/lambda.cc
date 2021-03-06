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
#include <iostream>	// for std::cout, std::endl
#include <stdlib.h>	// for EXIT_SUCCESS
#include <vector>
#include <algorithm>

/*
 * Examples of lambdas from C++0x standard...
 *
 * EXTRA_COMPILE_FLAGS=-std=c++11
 */

int main(int argc, char** argv, char** envp) {
	auto f=[] { std::cout << "Hello, Lambdasn" << std::endl; };
	f();
	std::vector<int> v;
	for(int i=0; i<10; i++) {
		v.push_back(i);
	}
	auto f2=[] (double x){ return x*x*12+x*4+7; };
	std::transform(v.begin(), v.end(), v.begin(), f2);
	return EXIT_SUCCESS;
}
