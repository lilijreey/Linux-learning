/*
 * =====================================================================================
 *
 *       Filename:  tracers-6.6.4.cpp
 *
 *    Description:  a Tracers in 6.6.4
 *
 *        Created:  12/11/2011 09:37:16 PM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */
//#include	<unistd.h>
//#include	<stdlib.h>
//#include	<string.h>
//#include	<zhao/tools.h>
#include	<iostream>
#include	<algorithm>

class SortTracer {
private:
	int value ;
	int gen ;
	static long n_created;
	static long n_destroyed ;
	static long n_assigned ;
	static long n_compared;
	static long n_max_live;

	static void update_max_live() {
		if (n_created - n_destroyed > n_max_live) {
			n_max_live = n_created - n_destroyed ;
		}
	}

public:
	static long creat() {
		return n_created ;
	}
	static long des() {
		return n_destroyed;
	}
	static long assig() {
		return n_assigned;
	}
	static long compare() {
		return n_compared;
	}
	static long max_live() {
		return n_max_live;
	}

	SortTracer(int v = 0) :value(v), gen(1) {
		++n_created ;
		update_max_live();
		std::cerr << "SortTracer #" << n_created
			      << ", created generation " << gen
				  << " (total: " << n_created - n_destroyed
				  << ')' << std::endl ;

	}

	SortTracer(SortTracer const &b) 
		: value(b.value),  gen(b.gen + 1) {
		++n_created ;
		update_max_live() ;
		std::cerr << "SortTracer #" << n_created
			      << ", copied as generation " << gen
				  << " (total: " << n_created - n_destroyed
				  << ')' << std::endl ;
	}

	~SortTracer() {
		++n_destroyed ;
		update_max_live();
		std::cerr << "SortTracer generation " << gen
			      << " destroyed (total: "
				  << n_created - n_destroyed << ')' << std::endl ;
	}

	SortTracer& operator=(SortTracer const &b) {
		++n_assigned ;
		std::cerr << "SortTracer generation #" << n_assigned 
			      << " generation " << gen 
				  << " = " << b.gen
				  << ')' << std::endl ;
		value = b.value ;
		return *this ;
	}

	friend bool operator<(SortTracer const &a, SortTracer const &b) {
		++n_compared ;
		std::cerr << "SortTracer comparison #" << n_compared 
				  << " (generation " << a.gen
				  << " < " << b.gen
				  << ')' << std::endl ;
		return a.value < b.value ;
	}
	int val() const {
		return value ;
	}
};


long SortTracer::n_created = 0 ;
long SortTracer::n_destroyed = 0 ;
long SortTracer::n_max_live=0;
long SortTracer::n_assigned = 0 ;
long SortTracer::n_compared = 0 ;
int main() {
	SortTracer intput[] = {7, 3, 5, 6, 4, 2, 0, 1, 9, 8} ;

	for (int i = 0; i < 10; ++i) {
		std::cerr << intput[i].val() << ' ' ;
	}
	std::cerr << std::endl ;
	long created_start = SortTracer::creat() ;
	long max_live_start = SortTracer::max_live() ;
	long assig_start = SortTracer::assig() ;
	long compare_start = SortTracer::compare() ;

	std::cerr << "---[ Start std::sort() ] --------------------------------" << std::endl ;
	std::sort<>(&intput[0], &intput[9]+1) ;
	std::cerr << "--- End std::sort() ] -----------------------------------" << std::endl;

	for (int i=0; i <10; ++i) {
		std::cerr << intput[i].val() << ' ' ;
	}

	std::cerr << "\n\n" ;
	std::cerr << "std::sort() of 10 SortTracer's"
		      << " was performed by: " << std::endl
			  << SortTracer::creat() - created_start 
			  << " temporary tracers" << std::endl << ' '
			  << "up to "
			  << SortTracer::max_live()
			  << " tracers at the same time ("
			  << max_live_start << " before)" << std::endl << ' '
			  << SortTracer::assig() - assig_start 
			  << " assignments" << std::endl << ' '
			  << SortTracer::compare() - compare_start ;

	return 0 ;
}

