#ifndef CACHE_BASE_H
#define CACHE_BASE_H

#include<math.h>

using namespace std;

class CACHE_BASE{
	private:
		int way_num;
		int cache_size; //KB
		int cache_line_size; //Byte
		int block_size; //bit
		int block_num;
		int index_num;
		int tag_bits_num;
		int index_bits_num;
		int block_bits_num;
		int offset_bits_num;
		int **moesi_state;
		int **new_flag;
		int **tag;
	public:
		CACHE_BASE();
		CACHE_BASE(int way_num_define, int cache_size_define, int cache_line_size_define, int block_size_define);
		~CACHE_BASE();
		int get_way_num();
		int get_cache_size();
		int get_cache_line_size();
		int get_block_size();
		int get_block_num();
		int get_index_num();
		int get_tag_bits_num();
		int get_index_bits_num();
		int get_block_bits_num();
		int get_offset_bits_num();
		int get_moesi_state(int way, int index);
		int get_new_flag(int way, int index);
		int get_tag(int way, int index);
		void set_moesi_state(int way, int index, int moesi_state_num);
		void set_new_flag(int way, int index, int new_flag_num);
		void set_tag(int way, int index, int tag_num);
};
#endif
