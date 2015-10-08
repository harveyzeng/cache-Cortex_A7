#include "cache_base.h"
#include <iostream>

using namespace std;

CACHE_BASE::CACHE_BASE() {
	way_num = 0;
	cache_size = 0;
	cache_line_size = 0;
	block_size = 0;
	block_num = 0;
	index_num = 0;
	tag_bits_num = 0;
	index_bits_num = 0;
	block_bits_num = 0;
	offset_bits_num = 0;
	moesi_state = NULL;
	new_flag = NULL;
	tag = NULL;
}

CACHE_BASE::CACHE_BASE(int way_num_define, int cache_size_define, int cache_line_size_define, int block_size_define) {
	way_num = way_num_define;
	cache_size = cache_size_define;
	cache_line_size = cache_line_size_define;
	block_size = block_size_define;
	block_num = cache_line_size * 8 / block_size;
	index_num = cache_size * 1024 / (way_num * cache_line_size);
	offset_bits_num = 2;
	block_bits_num = log2(block_num);
	index_bits_num = log2(index_num);
	tag_bits_num = 32 - index_bits_num - block_bits_num - offset_bits_num;

	moesi_state = new int*[way_num];
	new_flag = new int*[way_num];
	tag = new int*[way_num];
	for(int i = 0; i < way_num; i++){
		moesi_state[i] = new int[index_num];
		new_flag[i] = new int[index_num];
		tag[i] = new int[index_num];
	}
	for(int i = 0; i < way_num; i++){
		for(int j = 0; j < index_num; j++){
			moesi_state[i][j] = 0;
			new_flag[i][j] = 0;
			tag[i][j] = 0;
		}
	}
}

CACHE_BASE::~CACHE_BASE() {
	for(int i = 0; i < way_num; i++){
		delete [] moesi_state[i];
		delete [] new_flag[i];
		delete [] tag[i];
		moesi_state[i] = NULL;
		new_flag[i] = NULL;
		tag[i] = NULL;
	}
	delete [] moesi_state;
	delete [] new_flag;
	delete [] tag;
	moesi_state = NULL;
	new_flag = NULL;
	tag = NULL;
}

int CACHE_BASE::get_way_num() {
	return way_num;
}

int CACHE_BASE::get_cache_size() {
	return cache_size;
}

int CACHE_BASE::get_cache_line_size() {
	return cache_line_size;
}

int CACHE_BASE::get_block_size() {
	return block_size;
}

int CACHE_BASE::get_block_num() {
	return block_num;
}

int CACHE_BASE::get_index_num() {
	return index_num;
}

int CACHE_BASE::get_tag_bits_num() {
	return tag_bits_num;
}

int CACHE_BASE::get_index_bits_num() {
	return index_bits_num;
}

int CACHE_BASE::get_block_bits_num() {
	return block_bits_num;
}

int CACHE_BASE::get_offset_bits_num() {
	return offset_bits_num;
}

int CACHE_BASE::get_moesi_state(int way, int index) {
	if(way >= way_num || way < 0){
		cout << "get MOESI state failed: way number invalid!" << endl;
		return 999;
	}
	else if(index >= index_num || index < 0){
		cout << "get MOESI state failed: index number invalid!" << endl;
		return 999;
	}
	else
		return moesi_state[way][index];
}

int CACHE_BASE::get_new_flag(int way, int index) {
	if(way >= way_num || way < 0){
		cout << "get new flag failed: way number invalid!" << endl;
		return 999;
	}
	else if(index >= index_num || index < 0){
		cout << "get new flag failed: index number invalid!" << endl;
		return 999;
	}
	else
		return new_flag[way][index];
}

int CACHE_BASE::get_tag(int way, int index) {
	if(way >= way_num || way < 0){
		cout << "get tag failed: way number invalid!" << endl;
		return 999;
	}
	else if(index >= index_num || index < 0){
		cout << "get tag failed: index number invalid!" << endl;
		return 999;
	}
	else
		return tag[way][index];
}

void CACHE_BASE::set_moesi_state(int way, int index, int moesi_state_num) {
	if(way >= way_num || way < 0)
		cout << "set MOESI state failed: way number invalid!" << endl;
	else if(index >= index_num || index < 0)
		cout << "set MOESI state failed: index number invalid!" << endl;
	else if(moesi_state_num >= 5 || moesi_state_num < 0)
		cout << "set MOESI state failed: MOESI state number invalid!" << endl;
	else
		moesi_state[way][index] = moesi_state_num;
}

void CACHE_BASE::set_new_flag(int way, int index, int new_flag_num) {
	if(way >= way_num || way < 0)
		cout << "set new flag failed: way number invalid!" << endl;
	else if(index >= index_num || index < 0)
		cout << "set new flag failed: index number invalid!" << endl;
	else if(new_flag_num >= way_num || new_flag_num < 0)
		cout << "set new flag failed: new flag number invalid!" << endl;
	else
		new_flag[way][index] = new_flag_num;
}

void CACHE_BASE::set_tag(int way, int index, int tag_num) {
	if(way >= way_num || way < 0)
		cout << "set tag failed: way number invalid!" << endl;
	else if(index >= index_num || index < 0)
		cout << "set tag failed: index number invalid!" << endl;
	else if(tag_num >= pow(2, tag_bits_num) || tag_num < 0)
		cout << "set tag failed: tag number invalid!" << endl;
	else
		tag[way][index] = tag_num;
}
