#include "cache_base.h"
#include<math.h>

#ifndef CACHE_H
#define CACHE_H

#define icache_way (2)
#define icache_size (32)//KBs
#define icache_line_size (32)//Bytes
#define icache_block_size (32)//bits

#define dcache_way (4)
#define dcache_size (32)//KBs
#define dcache_line_size (64)//Bytes
#define dcache_block_size (32)//bits

#define l2_way (8)
#define l2_size (256)//KBs
#define l2_line_size (64)//Bytes
#define l2_block_size (32)//bits

using namespace std;

class CACHE{
	private:
		CACHE_BASE *ICACHE;
		CACHE_BASE *DCACHE;
		CACHE_BASE *L2;

		//address decode
		int index_de;
		int tag_de;
		int block_de;
		int index_l2_de;
		int tag_l2_de;
		int block_l2_de;
		
		//write L2 request
		bool write_l2_request;
		int write_l2_index;
		int write_l2_tag;

		//read allocate mode
		bool *block_tmp;
		bool *block_tmp_l2;

		//cache message
		int icache_access_times;
		int icache_miss_times;
		int icache_replacing_times;
		int dcache_access_times;
		int dcache_miss_times;
		int dcache_replacing_times;
		int l2_access_times;
		int l2_miss_times;
		int l2_replacing_times;
		
	public:
		CACHE();
		~CACHE();

		void decode(bool* addr, bool instruction);

		bool icache_access(int index, int tag);
		void icache_update(int index, int tag);
		bool dcache_access(int index, int tag, bool write);
		void dcache_update(int index, int tag, bool write);
		bool l2_access(int index, int tag, bool write);
		void l2_update(int index, int tag, bool write);

		int get_tag_de();
		int get_index_de();
		int get_block_de();
		int get_tag_l2_de();
		int get_index_l2_de();
		int get_block_l2_de();

		bool get_write_l2_request();
		void set_request_down();
		int get_write_l2_index();
		int get_write_l2_tag();

		void set_block_tmp(int block, bool value);
		bool get_block_tmp(int block);
		bool check_block_tmp();
		void clear_block_tmp();
		void set_block_tmp_l2(int block, bool value);
		bool get_block_tmp_l2(int block);
		bool check_block_tmp_l2();
		void clear_block_tmp_l2();

		void get_info();
		void get_result();
};
#endif
