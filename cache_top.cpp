#include "cache_top.h"
#include <stdio.h>
#include <iostream>

using namespace std;

CACHE_TOP::CACHE_TOP() {
	c1 = new CACHE();
	//c1->get_info();
	read_allocate_mode = false;
	linefilling = false;
	read_allocate_entering = false;
	linefill_count = 0;
	current_tag = -1;
	current_index = -1;
	current_tag_l2 = -1;
	current_index_l2 = -1;
}

CACHE_TOP::~CACHE_TOP() {
	c1->get_result();
	c1->~CACHE();
}

void CACHE_TOP::memory_access(bool* addr, bool instruction, bool write) {
	c1->decode(addr, instruction);
	
	//exit read allocate
	if(read_allocate_mode == true){
		if(write == true){//write
			//write burst that is not a full cache line
			if(read_allocate_entering == true){
			}
			else{
				if(c1->get_tag_l2_de() == current_tag_l2 && c1->get_index_l2_de() == current_index_l2){
				}
				else{
					if(c1->check_block_tmp_l2() == false){
						read_allocate_mode = false;
						linefilling = false;
						read_allocate_entering = false;
						linefill_count = 0;
						current_tag = 0;
						current_index = 0;
						current_tag_l2 = 0;
						current_index_l2 = 0;
						c1->clear_block_tmp();
						c1->clear_block_tmp_l2();
						//cout << "clear block!" << endl;
						//cout << "************************" << endl;
						//cout << "Exit read allocate mode!" << endl;
						//cout << "************************" << endl;
					}
				}
			}
		}
		else{//read
			//read the line currently being written-->do later
		}
	}

	//cache methodology
	bool hit = false;
	if(instruction == true){//instruction stream
		if(write == true){//write
			cout << "Error: I-cache Read Only!!!" << endl;
			getchar();
		}
		else{//read
			hit = c1->icache_access(c1->get_index_de(), c1->get_tag_de());
			if(hit == true){//read hit
			}
			else{//read miss
				c1->icache_update(c1->get_index_de(), c1->get_tag_de());
				hit = c1->l2_access(c1->get_index_l2_de(), c1->get_tag_l2_de(), false);
				if(hit == true){//L2 hit
				}
				else{//L2 miss
					c1->l2_update(c1->get_index_l2_de(), c1->get_tag_l2_de(), false);
				}
			}
		}
	}
	else{//data stream
		if(read_allocate_mode == true){
			if(write == true){//write in read allocate mode
				hit = c1->dcache_access(c1->get_index_de(), c1->get_tag_de(), true);
				if(hit == true){//write hit
				}
				else{//write miss
					hit = c1->l2_access(c1->get_index_l2_de(), c1->get_tag_l2_de(), true);
					if(hit == true){//L2 hit
						//read allocate mode policy
						if(read_allocate_entering == true){
							read_allocate_entering = false;
							current_tag_l2 = c1->get_tag_l2_de();
							current_index_l2 = c1->get_index_l2_de();
							c1->clear_block_tmp_l2();
							//cout << "clear block!" << endl;
							c1->set_block_tmp_l2(c1->get_block_l2_de(), true);
							//cout << "set block: " << c1->get_block_l2_de() << endl;
						}
						else{
							c1->set_block_tmp_l2(c1->get_block_l2_de(), true);
							//cout << "set block: " << c1->get_block_l2_de() << endl;
						}
						
					}
					else{//L2 miss
						//read allocate mode policy
						if(read_allocate_entering == true){
							read_allocate_entering = false;
							current_tag_l2 = c1->get_tag_l2_de();
							current_index_l2 = c1->get_index_l2_de();
							c1->clear_block_tmp_l2();
							//cout << "clear block!" << endl;
							c1->set_block_tmp_l2(c1->get_block_l2_de(), true);
							//cout << "set block: " << c1->get_block_l2_de() << endl;
						}
						else{
							current_tag_l2 = c1->get_tag_l2_de();
							current_index_l2 = c1->get_index_l2_de();
							c1->clear_block_tmp_l2();
							//cout << "clear block!" << endl;
							c1->set_block_tmp_l2(c1->get_block_l2_de(), true);
							//cout << "set block: " << c1->get_block_l2_de() << endl;
						}

						//cache policy
						c1->l2_update(c1->get_index_l2_de(), c1->get_tag_l2_de(), true);
					}
				}
			}
			else{//read in read allocate mode
				hit = c1->dcache_access(c1->get_index_de(), c1->get_tag_de(), false);
				if(hit == true){//read hit
				}
				else{//read miss
					c1->dcache_update(c1->get_index_de(), c1->get_tag_de(), false);
					if(c1->get_write_l2_request() == true){//L1 write request handling
						c1->set_request_down();
						hit = c1->l2_access(c1->get_write_l2_index(), c1->get_write_l2_tag(), true);
						if(hit == true){//L2 hit
						}
						else{//L2 miss
							c1->l2_update(c1->get_write_l2_index(), c1->get_write_l2_tag(), true);
						}
					}
					hit = c1->l2_access(c1->get_index_l2_de(), c1->get_tag_l2_de(), false);
					if(hit == true){//L2 hit
						//read the line currently being written
						if(c1->get_index_l2_de() == current_index_l2 && c1->get_tag_l2_de() == current_tag_l2){
							read_allocate_mode = false;
							linefilling = false;
							read_allocate_entering = false;
							linefill_count = 0;
							current_tag = 0;
							current_index = 0;
							current_tag_l2 = 0;
							current_index_l2 = 0;
							c1->clear_block_tmp();
							c1->clear_block_tmp_l2();
							//cout << "************************" << endl;
							//cout << "Exit read allocate mode!" << endl;
							//cout << "************************" << endl;
						}
					}
					else{//L2 miss
						c1->l2_update(c1->get_index_l2_de(), c1->get_tag_l2_de(), false);
					}
				}
			}
		}
		else{
			if(write == true){//write in normal mode
				hit = c1->dcache_access(c1->get_index_de(), c1->get_tag_de(), true);
				if(hit == true){//write hit
					//read allocate mode entering policy
					if(linefilling == true && c1->get_index_de() == current_index && c1->get_tag_de() == current_tag){//same line
						c1->set_block_tmp(c1->get_block_de(), true);
					}
					else{//different line
						linefilling = false;
						linefill_count = 0;
						current_tag = 0;
						current_index = 0;
						c1->clear_block_tmp();
					}

					//linefill count check
					if(c1->check_block_tmp() == true){
						linefill_count++;
						//cout << "********************" << endl;
						//cout << "linefill count + 1 !" << endl;
						//cout << "********************" << endl;
					}

					//read allocate mode check
					if(read_allocate_mode == false && linefill_count == 3){
							read_allocate_mode = true;
							linefilling = false;
							read_allocate_entering = true;
							linefill_count = 0;
							current_tag = 0;
							current_index = 0;
							current_tag_l2 = 0;
							current_index_l2 = 0;
							c1->clear_block_tmp();
							c1->clear_block_tmp_l2();
							//cout << "*************************" << endl;
							//cout << "Enter read allocate mode!" << endl;
							//cout << "*************************" << endl;
					}
				}
				else{//write miss
					//read allocate mode entering policy
					if(c1->check_block_tmp() == true){//check pass
						linefilling = true;
						current_tag = c1->get_tag_de();
						current_index = c1->get_index_de();
						c1->clear_block_tmp();
						c1->set_block_tmp(c1->get_block_de(), true);
					}
					else{//check fail
						linefilling = true;
						linefill_count = 0;
						current_tag = c1->get_tag_de();
						current_index = c1->get_index_de();
						c1->clear_block_tmp();
						c1->set_block_tmp(c1->get_block_de(), true);
					}

					//cache policy
					c1->dcache_update(c1->get_index_de(), c1->get_tag_de(), true);
					if(c1->get_write_l2_request() == true){
						c1->set_request_down();
						hit = c1->l2_access(c1->get_write_l2_index(), c1->get_write_l2_tag(), true);
						if(hit == true){//L2 hit
						}
						else{//L2 miss
							c1->l2_update(c1->get_write_l2_index(), c1->get_write_l2_tag(), true);
						}
					}
					hit = c1->l2_access(c1->get_index_l2_de(), c1->get_tag_l2_de(), false);
					if(hit == true){//L2 hit
					}
					else{//L2 miss
						c1->l2_update(c1->get_index_l2_de(), c1->get_tag_l2_de(), false);
					}
				}
			}
			else{//read in normal mode
				hit = c1->dcache_access(c1->get_index_de(), c1->get_tag_de(), false);
				if(hit == true){//read hit
					linefilling = false;
					linefill_count = 0;
					current_tag = 0;
					current_index = 0;
					c1->clear_block_tmp();
				}
				else{//read miss
					//read allocate mode entering policy
					if(c1->check_block_tmp() == true){//check pass
						linefilling = true;
						current_tag = c1->get_tag_de();
						current_index = c1->get_index_de();
						c1->clear_block_tmp();
						c1->set_block_tmp(c1->get_block_de(), true);
					}
					else{//check fail
						linefilling = true;
						linefill_count = 0;
						current_tag = c1->get_tag_de();
						current_index = c1->get_index_de();
						c1->clear_block_tmp();
						c1->set_block_tmp(c1->get_block_de(), true);
					}

					//cache policy
						c1->dcache_update(c1->get_index_de(), c1->get_tag_de(), false);
						if(c1->get_write_l2_request() == true){
							c1->set_request_down();
							hit = c1->l2_access(c1->get_write_l2_index(), c1->get_write_l2_tag(), true);
							if(hit == true){//L2 hit
							}
							else{//L2 miss
								c1->l2_update(c1->get_write_l2_index(), c1->get_write_l2_tag(), true);
							}
						}
						hit = c1->l2_access(c1->get_index_l2_de(), c1->get_tag_l2_de(), false);
						if(hit == true){//L2 hit
						}
						else{//L2 miss
							c1->l2_update(c1->get_index_l2_de(), c1->get_tag_l2_de(), false);
						}
				}
			}
		}
	}
}
