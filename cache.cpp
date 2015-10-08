#include "cache.h"
#include <stdio.h>
#include <iostream>

using namespace std;

CACHE::CACHE() {
	ICACHE = new CACHE_BASE(icache_way, icache_size, icache_line_size, icache_block_size);
	DCACHE = new CACHE_BASE(dcache_way, dcache_size, dcache_line_size, dcache_block_size);
	L2 = new CACHE_BASE(l2_way, l2_size, l2_line_size, l2_block_size);

	index_de = 0;
	tag_de = 0;
	block_de = 0;
	index_l2_de = 0;
	tag_l2_de = 0;
	block_l2_de = 0;

	write_l2_request = false;
	write_l2_index = 0;
	write_l2_tag = 0;

	block_tmp = new bool[DCACHE->get_block_num()];
	for(int i = 0; i < DCACHE->get_block_num(); i++){
		block_tmp[i] = false;
	}
	block_tmp_l2 = new bool[L2->get_block_num()];
	for(int i = 0; i < L2->get_block_num(); i++){
		block_tmp_l2[i] = false;
	}

	icache_access_times = 0;
	icache_miss_times = 0;
	icache_replacing_times = 0;
	dcache_access_times = 0;
	dcache_miss_times = 0;
	dcache_replacing_times = 0;
	l2_access_times = 0;
	l2_miss_times = 0;
	l2_replacing_times = 0;
}

CACHE::~CACHE() {
	ICACHE->~CACHE_BASE();
	DCACHE->~CACHE_BASE();
	L2->~CACHE_BASE();
	delete [] block_tmp;
	delete [] block_tmp_l2;
	ICACHE = NULL;
	DCACHE = NULL;
	L2 = NULL;
}

void CACHE::decode(bool* addr, bool instruction) {
/*
	bool addr_b[32];
	for(int i = 0; i < 8; i++){
		switch(addr[i]) {
			case '0':
				addr_b[4*i]   = 0;
				addr_b[4*i+1] = 0;
				addr_b[4*i+2] = 0;
				addr_b[4*i+3] = 0;
				break;
			case '1':
				addr_b[4*i]   = 0;
				addr_b[4*i+1] = 0;
				addr_b[4*i+2] = 0;
				addr_b[4*i+3] = 1;
				break;
			case '2':
				addr_b[4*i]   = 0;
				addr_b[4*i+1] = 0;
				addr_b[4*i+2] = 1;
				addr_b[4*i+3] = 0;
				break;
			case '3':
				addr_b[4*i]   = 0;
				addr_b[4*i+1] = 0;
				addr_b[4*i+2] = 1;
				addr_b[4*i+3] = 1;
				break;
			case '4':
				addr_b[4*i]   = 0;
				addr_b[4*i+1] = 1;
				addr_b[4*i+2] = 0;
				addr_b[4*i+3] = 0;
				break;
			case '5':
				addr_b[4*i]   = 0;
				addr_b[4*i+1] = 1;
				addr_b[4*i+2] = 0;
				addr_b[4*i+3] = 1;
				break;
			case '6':
				addr_b[4*i]   = 0;
				addr_b[4*i+1] = 1;
				addr_b[4*i+2] = 1;
				addr_b[4*i+3] = 0;
				break;
			case '7':
				addr_b[4*i]   = 0;
				addr_b[4*i+1] = 1;
				addr_b[4*i+2] = 1;
				addr_b[4*i+3] = 1;
				break;
			case '8':
				addr_b[4*i]   = 1;
				addr_b[4*i+1] = 0;
				addr_b[4*i+2] = 0;
				addr_b[4*i+3] = 0;
				break;
			case '9':
				addr_b[4*i]   = 1;
				addr_b[4*i+1] = 0;
				addr_b[4*i+2] = 0;
				addr_b[4*i+3] = 1;
				break;
			case 'A':
				addr_b[4*i]   = 1;
				addr_b[4*i+1] = 0;
				addr_b[4*i+2] = 1;
				addr_b[4*i+3] = 0;
				break;
			case 'B':
				addr_b[4*i]   = 1;
				addr_b[4*i+1] = 0;
				addr_b[4*i+2] = 1;
				addr_b[4*i+3] = 1;
				break;
			case 'C':
				addr_b[4*i]   = 1;
				addr_b[4*i+1] = 1;
				addr_b[4*i+2] = 0;
				addr_b[4*i+3] = 0;
				break;
			case 'D':
				addr_b[4*i]   = 1;
				addr_b[4*i+1] = 1;
				addr_b[4*i+2] = 0;
				addr_b[4*i+3] = 1;
				break;
			case 'E':
				addr_b[4*i]   = 1;
				addr_b[4*i+1] = 1;
				addr_b[4*i+2] = 1;
				addr_b[4*i+3] = 0;
				break;
			case 'F':
				addr_b[4*i]   = 1;
				addr_b[4*i+1] = 1;
				addr_b[4*i+2] = 1;
				addr_b[4*i+3] = 1;
				break;
			default:
				addr_b[4*i]   = 0;
				addr_b[4*i+1] = 0;
				addr_b[4*i+2] = 0;
				addr_b[4*i+3] = 0;
				break;
		}
	}

	//initialize
	index_de = 0;
	tag_de = 0;
	block_de = 0;
	index_l2_de = 0;
	tag_l2_de = 0;
	block_l2_de = 0;

	//L1 cache decode
	if(instruction == true){//instruction stream
		for(int i = 0; i < ICACHE->get_block_bits_num(); i++){
			block_de = block_de
					+ addr_b[ICACHE->get_tag_bits_num() + ICACHE->get_index_bits_num() + i]
					* pow(2, (ICACHE->get_block_bits_num() - i - 1));
		}
		for(int i = 0; i < ICACHE->get_index_bits_num(); i++){
			index_de = index_de
					+ addr_b[ICACHE->get_tag_bits_num() + i]
					* pow(2, (ICACHE->get_index_bits_num() - i - 1));
		}
		for(int i = 0; i < ICACHE->get_tag_bits_num(); i++){
			tag_de = tag_de
					+ addr_b[i]
					* pow(2, (ICACHE->get_tag_bits_num() - i - 1));
		}
		cout << "instruction address:" << endl;
		cout << "	" << addr << endl;
		cout << "	";
		for(int i = 0; i < 32; i++)
			cout << addr_b[i];
		cout << endl;
		//cout << "	tag: " << tag_de << endl;
		//cout << "	index: " << index_de << endl;
		//cout << "	block: " << block_de << endl;
	}
	else{//data stream
		for(int i = 0; i < DCACHE->get_block_bits_num(); i++){
			block_de = block_de
				+ addr_b[DCACHE->get_tag_bits_num() + DCACHE->get_index_bits_num() + i]
				* pow(2, (DCACHE->get_block_bits_num() - i - 1));
		}
		for(int i = 0; i < DCACHE->get_index_bits_num(); i++){
			index_de = index_de
				+ addr_b[DCACHE->get_tag_bits_num() + i]
				* pow(2, (DCACHE->get_index_bits_num() - i - 1));
		}
		for(int i = 0; i < DCACHE->get_tag_bits_num(); i++){
			tag_de = tag_de
				+ addr_b[i]
				* pow(2, (DCACHE->get_tag_bits_num() - i - 1));
		}
		cout << "data address:" << endl;
		cout << "	" << addr << endl;
		cout << "	";
		for(int i = 0; i < 32; i++)
			cout << addr_b[i];
		cout << endl;
		//cout << "	tag: " << tag_de << endl;
		//cout << "	index: " << index_de << endl;
		//cout << "	block: " << block_de << endl;
	}

	//L2 cache decode
	for(int i = 0; i < L2->get_block_bits_num(); i++){
		block_l2_de = block_l2_de
				+ addr_b[L2->get_tag_bits_num() + L2->get_index_bits_num() + i]
				* pow(2, (L2->get_block_bits_num() - i - 1));
	}
	for(int i = 0; i < L2->get_index_bits_num(); i++){
		index_l2_de = index_l2_de
				+ addr_b[L2->get_tag_bits_num() + i]
				* pow(2, (L2->get_index_bits_num() - i - 1));
	}
	for(int i = 0; i < L2->get_tag_bits_num(); i++){
		tag_l2_de = tag_l2_de
				+ addr_b[i]
				* pow(2, (L2->get_tag_bits_num() - i - 1));
	}
	//cout << "	L2_tag: " << tag_l2_de << endl;
	//cout << "	L2_index: " << index_l2_de << endl;
	//cout << "	L2_block: " << block_l2_de << endl;
*/

	//initialize
	index_de = 0;
	tag_de = 0;
	block_de = 0;
	index_l2_de = 0;
	tag_l2_de = 0;
	block_l2_de = 0;

	//L1 cache decode
	if(instruction == true){//instruction stream
		for(int i = 0; i < ICACHE->get_block_bits_num(); i++){
			block_de = block_de
					+ addr[ICACHE->get_offset_bits_num()
						+ ICACHE->get_block_bits_num() - i - 1]
					* pow(2, (ICACHE->get_block_bits_num() - i - 1));
		}
		for(int i = 0; i < ICACHE->get_index_bits_num(); i++){
			index_de = index_de
					+ addr[ICACHE->get_offset_bits_num()
						+ ICACHE->get_block_bits_num()
						+ ICACHE->get_index_bits_num() - i - 1]
					* pow(2, (ICACHE->get_index_bits_num() - i - 1));
		}
		for(int i = 0; i < ICACHE->get_tag_bits_num(); i++){
			tag_de = tag_de
					+ addr[ICACHE->get_offset_bits_num()
						+ ICACHE->get_block_bits_num()
						+ ICACHE->get_index_bits_num()
						+ ICACHE->get_tag_bits_num() - i - 1]
					* pow(2, (ICACHE->get_tag_bits_num() - i - 1));
		}
/*
		cout << "instruction address:" << endl;
		cout << "	";
		for(int i = 0; i < 32; i++){
			cout << addr[31-i];
		}
		cout << endl;
		cout << "	tag: " << tag_de << endl;
		cout << "	index: " << index_de << endl;
		cout << "	block: " << block_de << endl;
*/
	}
	else{//data stream
		for(int i = 0; i < DCACHE->get_block_bits_num(); i++){
			block_de = block_de
				+ addr[DCACHE->get_offset_bits_num()
					+ DCACHE->get_block_bits_num() - i - 1]
				* pow(2, (DCACHE->get_block_bits_num() - i - 1));
		}
		for(int i = 0; i < DCACHE->get_index_bits_num(); i++){
			index_de = index_de
				+ addr[DCACHE->get_offset_bits_num()
					+ DCACHE->get_block_bits_num()
					+ DCACHE->get_index_bits_num() - i - 1]
				* pow(2, (DCACHE->get_index_bits_num() - i - 1));
		}
		for(int i = 0; i < DCACHE->get_tag_bits_num(); i++){
			tag_de = tag_de
				+ addr[DCACHE->get_offset_bits_num()
					+ DCACHE->get_block_bits_num()
					+ DCACHE->get_index_bits_num()
					+ DCACHE->get_tag_bits_num() - i - 1]
				* pow(2, (DCACHE->get_tag_bits_num() - i - 1));
		}
/*
		cout << "data address:" << endl;
		cout << "	";
		for(int i = 0; i < 32; i++){
			cout << addr[31-i];
		}
		cout << endl;
		cout << "	tag: " << tag_de << endl;
		cout << "	index: " << index_de << endl;
		cout << "	block: " << block_de << endl;
*/
	}

	//L2 cache decode
	for(int i = 0; i < L2->get_block_bits_num(); i++){
		block_l2_de = block_l2_de
				+ addr[L2->get_offset_bits_num()
					+ L2->get_block_bits_num() - i - 1]
				* pow(2, (L2->get_block_bits_num() - i - 1));
	}
	for(int i = 0; i < L2->get_index_bits_num(); i++){
		index_l2_de = index_l2_de
				+ addr[L2->get_offset_bits_num()
					+ L2->get_block_bits_num()
					+ L2->get_index_bits_num() - i - 1]
				* pow(2, (L2->get_index_bits_num() - i - 1));
	}
	for(int i = 0; i < L2->get_tag_bits_num(); i++){
		tag_l2_de = tag_l2_de
				+ addr[L2->get_offset_bits_num()
					+ L2->get_block_bits_num()
					+ L2->get_index_bits_num()
					+ L2->get_tag_bits_num() - i - 1]
				* pow(2, (L2->get_tag_bits_num() - i - 1));
	}
/*
	cout << "	L2_tag: " << tag_l2_de << endl;
	cout << "	L2_index: " << index_l2_de << endl;
	cout << "	L2_block: " << block_l2_de << endl;
*/
}

bool CACHE::icache_access(int index, int tag){
	icache_access_times++;

	//find the target line
	bool hit = false;
	for(int i = 0; i < ICACHE->get_way_num(); i++){
		if(ICACHE->get_moesi_state(i, index) == 2 && ICACHE->get_tag(i, index) == tag){//clean state & equal tag
			hit = true;
		}
	}
	if(hit == false){
		//cout << "	MISS!!!" << endl;
		icache_miss_times++;
	}
	else{
		//cout << "	HIT!!!" << endl;
	}
	return hit;
}

void CACHE::icache_update(int index, int tag){
	//find invalid way
	int invalid_way = ICACHE->get_way_num() + 1;
	for(int i = 0; i < ICACHE->get_way_num(); i++){
		if(ICACHE->get_moesi_state(i, index) == 0){//invalid state
			invalid_way = i;
		}
	}

	//update cache line
	if(invalid_way >= ICACHE->get_way_num()){//no invalid way-->replacement
		//cout << "I-cache replacement occurs!" << endl;
		icache_replacing_times++;
		for(int i = 0; i < ICACHE->get_way_num(); i++){
			if(ICACHE->get_new_flag(i, index) == 0){//replace the oldest way
				ICACHE->set_moesi_state(i, index, 2);
				ICACHE->set_new_flag(i, index, ICACHE->get_way_num()-1);
				ICACHE->set_tag(i, index, tag);
			}
			else{
				//update other way's new flag
				ICACHE->set_new_flag(i, index, ICACHE->get_new_flag(i, index)-1);
			}
		}
	}
	else{//linefill
		for(int i = 0; i < ICACHE->get_way_num(); i++){
			if(i == invalid_way){//linefill the invalid way
				ICACHE->set_moesi_state(i, index, 2);
				ICACHE->set_new_flag(i, index, ICACHE->get_way_num()-1);
				ICACHE->set_tag(i, index, tag);
			}
			else{//update other way's new flag
				if(ICACHE->get_moesi_state(i, index) == 2){
					ICACHE->set_new_flag(i, index, ICACHE->get_new_flag(i, index)-1);
				}
			}
		}
	}
}

bool CACHE::dcache_access(int index, int tag, bool write) {
	dcache_access_times++;

	//find the target line
	bool hit = false;
	for(int i = 0; i < DCACHE->get_way_num(); i++){
		if(DCACHE->get_moesi_state(i, index) == 2 || DCACHE->get_moesi_state(i, index) == 4){//exclusive or modified state
			if(DCACHE->get_tag(i, index) == tag){//check tag number
				hit = true;
				if(write == true)
					DCACHE->set_moesi_state(i, index, 4);//change to modified state
			}
		}
	}

	if(hit == false){
		//cout << "	MISS!!!" << endl;
		dcache_miss_times++;
	}
	else{
		//cout << "	HIT!!!" << endl;
	}
	return hit;
}

void CACHE::dcache_update(int index, int tag, bool write) {
	//find invalid way
	int invalid_way = DCACHE->get_way_num() + 1;
	for(int i = 0; i < DCACHE->get_way_num(); i++){
		if(DCACHE->get_moesi_state(i, index) == 0){//invalid state
			invalid_way = i;
		}
	}

	//update cache line
	if(invalid_way > DCACHE->get_way_num()){//no invalid way-->replacement
		//cout << "D-cache replacement occurs!" << endl;
		dcache_replacing_times++;
		for(int i = 0; i < DCACHE->get_way_num(); i++){
			if(DCACHE->get_new_flag(i, index) == 0){//replace the oldest way
				if(DCACHE->get_moesi_state(i, index) == 4){//if the way is in modified state, write back to L2
					//write back request
					write_l2_request = true;
					//caculate the line address
					unsigned int line_addr =  (unsigned int)DCACHE->get_tag(i, index)
								* (unsigned int)pow(2, (32 - DCACHE->get_tag_bits_num()))
								+ (unsigned int)index
								* (unsigned int)pow(2, (DCACHE->get_offset_bits_num() + DCACHE->get_block_bits_num()));
					//decode the address to L2 form
					write_l2_index =  (int)(line_addr % (unsigned int)(pow(2, (32 - L2->get_tag_bits_num()))))
							/ (int)(pow(2, (L2->get_block_bits_num() + L2->get_offset_bits_num())));
					//cout << write_l2_index << endl;
					write_l2_tag = (int)(line_addr / (unsigned int)(pow(2, (32 - L2->get_tag_bits_num()))));
					//cout << write_l2_tag << endl;
				}
				if(write == true)
					DCACHE->set_moesi_state(i, index, 4);//set to modified state
				else
					DCACHE->set_moesi_state(i, index, 2);//set to exclusive state
				DCACHE->set_new_flag(i, index, DCACHE->get_way_num()-1);
				DCACHE->set_tag(i, index, tag);
			}
			else{//update other way's new flag
				DCACHE->set_new_flag(i, index, DCACHE->get_new_flag(i, index)-1);
			}
		}
	}
	else{//linefill
		for(int i = 0; i < DCACHE->get_way_num(); i++){
			if(i == invalid_way){//linefill the invalid way
				if(write == true)
					DCACHE->set_moesi_state(i, index, 4);//set to modified state
				else
					DCACHE->set_moesi_state(i, index, 2);//set to exclusive state
				DCACHE->set_new_flag(i, index, DCACHE->get_way_num()-1);
				DCACHE->set_tag(i, index, tag);
			}
			else{//update other way's new flag
				if(DCACHE->get_moesi_state(i, index) == 2 || DCACHE->get_moesi_state(i, index) == 4){
					DCACHE->set_new_flag(i, index, DCACHE->get_new_flag(i, index)-1);
				}
			}
		}
	}
}

bool CACHE::l2_access(int index, int tag, bool write) {
	l2_access_times++;
	//cout << tag << endl;
	//getchar();

	//find the target line
	bool hit = false;
	for(int i = 0; i < L2->get_way_num(); i++){
		if(L2->get_moesi_state(i, index) == 2 || L2->get_moesi_state(i, index) == 4){//exclusive or modified state
			if(L2->get_tag(i, index) == tag){//check tag number
				hit = true;
				if(write == true)
					L2->set_moesi_state(i, index, 4);//change to modified state
			}
		}
	}
	if(hit == false){
		//cout << "	L2 MISS!!!" << endl;
		l2_miss_times++;
	}
	else{
		//cout << "	L2 HIT!!!" << endl;
	}
	return hit;
}

void CACHE::l2_update(int index, int tag, bool write) {
	//find invalid way
	int invalid_way = L2->get_way_num() + 1;
	for(int i = 0; i < L2->get_way_num(); i++){
		if(L2->get_moesi_state(i, index) == 0){//invalid state
			invalid_way = i;
		}
	}

	//update cache line
	if(invalid_way >= L2->get_way_num()){//no invalid way-->replacement
		//cout << "L2-cache replacement occurs!" << endl;
		l2_replacing_times++;
		for(int i = 0; i < L2->get_way_num(); i++){
			if(L2->get_new_flag(i, index) == 0){//replace the oldest way
				if(write == true)
					L2->set_moesi_state(i, index, 4);//set to modified state
				else
					L2->set_moesi_state(i, index, 2);//set to exclusive state
				L2->set_new_flag(i, index, L2->get_way_num()-1);
				L2->set_tag(i, index, tag);
			}
			else{//update other way's new flag
				L2->set_new_flag(i, index, L2->get_new_flag(i, index)-1);
			}
		}
	}
	else{//linefill
		for(int i = 0; i < L2->get_way_num(); i++){
			if(i == invalid_way){//linefill the invalid way
				if(write == true)
					L2->set_moesi_state(i, index, 4);//set to modified state
				else
					L2->set_moesi_state(i, index, 2);//set to exclusive state
				L2->set_new_flag(i, index, L2->get_way_num()-1);
				L2->set_tag(i, index, tag);
			}
			else{//update other way's new flag
				if(L2->get_moesi_state(i, index) == 2 || L2->get_moesi_state(i, index) == 4){
					L2->set_new_flag(i, index, L2->get_new_flag(i, index)-1);
				}
			}
		}
	}
}

int CACHE::get_index_de() {
	return index_de;
}

int CACHE::get_tag_de() {
	return tag_de;
}

int CACHE::get_block_de() {
	return block_de;
}

int CACHE::get_index_l2_de() {
	return index_l2_de;
}

int CACHE::get_tag_l2_de() {
	return tag_l2_de;
}

int CACHE::get_block_l2_de() {
	return block_l2_de;
}

bool CACHE::get_write_l2_request() {
	return write_l2_request;
}

void CACHE::set_request_down() {
	write_l2_request = false;
}

int CACHE::get_write_l2_index() {
	return write_l2_index;
}

int CACHE::get_write_l2_tag() {
	return write_l2_tag;
}

void CACHE::set_block_tmp(int block, bool value) {
	if(block >= DCACHE->get_block_num() || block < 0){
		cout << "Error: invalid block number!!!" << endl;
	}
	else{
		block_tmp[block] = value;
	}
}

bool CACHE::get_block_tmp(int block) {
	if(block >= DCACHE->get_block_num() || block < 0){
		cout << "Error: invalid block number!!!" << endl;
		return false;
	}
	else{
		return block_tmp[block];
	}
}

bool CACHE::check_block_tmp() {
	bool check_pass;
	check_pass = true;
	for(int i = 0; i < DCACHE->get_block_num(); i++){
		if(block_tmp[i] == false){
			check_pass = false;
		}
	}
	return check_pass;
}

void CACHE::clear_block_tmp() {
	for(int i = 0; i < DCACHE->get_block_num(); i++){
		block_tmp[i] = false;
	}
}

void CACHE::set_block_tmp_l2(int block, bool value) {
	if(block >= L2->get_block_num() || block < 0){
		cout << "Error: invalid block number!!!" << endl;
	}
	else{
		block_tmp_l2[block] = value;
	}
}

bool CACHE::get_block_tmp_l2(int block) {
	if(block >= L2->get_block_num() || block < 0){
		cout << "Error: invalid block number!!!" << endl;
		return false;
	}
	else{
		return block_tmp_l2[block];
	}
}

bool CACHE::check_block_tmp_l2() {
	bool check_pass;
	check_pass = true;
	for(int i = 0; i < L2->get_block_num(); i++){
		if(block_tmp_l2[i] == false){
			check_pass = false;
			//cout << "check fail: " << i << endl;
		}
	}
	return check_pass;
}

void CACHE::clear_block_tmp_l2() {
	for(int i = 0; i < L2->get_block_num(); i++){
		block_tmp_l2[i] = false;
	}
}

void CACHE::get_info() {
	cout << "================================================" << endl;
	cout << "I-cache info:" << endl;
	cout << "	way num:         " << ICACHE->get_way_num() << endl;
	cout << "	size:            " << ICACHE->get_cache_size() << " KB" << endl;
	cout << "	line size:       " << ICACHE->get_cache_line_size() << " Byte" << endl;
	cout << "	block size:      " << ICACHE->get_block_size() << " bit" << endl;
	cout << "	tag bits num:    " << ICACHE->get_tag_bits_num() << endl;
	cout << "	index bits num:  " << ICACHE->get_index_bits_num() << endl;
	cout << "	block bits num:  " << ICACHE->get_block_bits_num() << endl;
	cout << "	offset bits num: " << ICACHE->get_offset_bits_num() << endl;
	cout << "D-cache info:" << endl;
	cout << "	way num:         " << DCACHE->get_way_num() << endl;
	cout << "	size:            " << DCACHE->get_cache_size() << " KB" << endl;
	cout << "	line size:       " << DCACHE->get_cache_line_size() << " Byte" << endl;
	cout << "	block size:      " << DCACHE->get_block_size() << " bit" << endl;
	cout << "	tag bits num:    " << DCACHE->get_tag_bits_num() << endl;
	cout << "	index bits num:  " << DCACHE->get_index_bits_num() << endl;
	cout << "	block bits num:  " << DCACHE->get_block_bits_num() << endl;
	cout << "	offset bits num: " << DCACHE->get_offset_bits_num() << endl;
	cout << "L2-cache info:" << endl;
	cout << "	way num:         " << L2->get_way_num() << endl;
	cout << "	size:            " << L2->get_cache_size() << " KB" << endl;
	cout << "	line size:       " << L2->get_cache_line_size() << " Byte" << endl;
	cout << "	block size:      " << L2->get_block_size() << " bit" << endl;
	cout << "	tag bits num:    " << L2->get_tag_bits_num() << endl;
	cout << "	index bits num:  " << L2->get_index_bits_num() << endl;
	cout << "	block bits num:  " << L2->get_block_bits_num() << endl;
	cout << "	offset bits num: " << L2->get_offset_bits_num() << endl;
	cout << "================================================" << endl;
}

void CACHE::get_result() {
	double icache_hit_ratio;
	double dcache_hit_ratio;
	double l2_hit_ratio;

	if(icache_access_times == 0)
		icache_hit_ratio = 0.0;
	else
		icache_hit_ratio = 100 * ((double)icache_access_times - (double)icache_miss_times) / (double)icache_access_times;
	if(dcache_access_times == 0)
		dcache_hit_ratio = 0.0;
	else
		dcache_hit_ratio = 100 * ((double)dcache_access_times - (double)dcache_miss_times) / (double)dcache_access_times;
	if(l2_access_times == 0)
		l2_hit_ratio = 0.0;
	else
		l2_hit_ratio = 100 * ((double)l2_access_times - (double)l2_miss_times) / (double)l2_access_times;
	cout << "================================================" << endl;
	cout << "I-cache result:" << endl;
	cout << "	access times:    " << icache_access_times << endl;
	cout << "	miss times:      " << icache_miss_times << endl;
	cout << "	replacing times: " << icache_replacing_times << endl;
	cout << "	hit ratio:       " << icache_hit_ratio << "%" << endl;
	cout << "D-cache result:" << endl;
	cout << "	access times:    " << dcache_access_times << endl;
	cout << "	miss times:      " << dcache_miss_times << endl;
	cout << "	replacing times: " << dcache_replacing_times << endl;
	cout << "	hit ratio:       " << dcache_hit_ratio << "%" << endl;
	cout << "L2-cache result:" << endl;
	cout << "	access times:    " << l2_access_times << endl;
	cout << "	miss times:      " << l2_miss_times << endl;
	cout << "	replacing times: " << l2_replacing_times << endl;
	cout << "	hit ratio:       " << l2_hit_ratio << "%" << endl;
	cout << "================================================" << endl;
}
