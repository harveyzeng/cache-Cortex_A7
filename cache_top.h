#include "cache.h"

#ifndef CACHE_TOP_H
#define CACHE_TOP_H

using namespace std;

class CACHE_TOP{
	private:
		CACHE *c1;

		//cache mode
		bool read_allocate_mode;

		//read allocate mode
		bool linefilling;
		bool read_allocate_entering;
		int linefill_count;
		int current_tag;
		int current_index;
		int current_tag_l2;
		int current_index_l2;
	public:
		CACHE_TOP();
		~CACHE_TOP();
		void memory_access(bool* addr, bool instruction, bool write);
};
#endif
