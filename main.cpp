#include "cache_top.h"
#include <iostream>
#include <stdio.h>
#include <bitset>

using namespace std;

int main(int argc, char *argv[]){

	CACHE_TOP a1;
	//unsigned int in_decimal;
	bool in_bool[32];

	unsigned int in_decimal1 = 2852356608; //AA038200
	unsigned int in_decimal2 = 2852618752; //AA078200
	unsigned int in_decimal3 = 2852880896; //AA0B8200
	unsigned int in_decimal4 = 2853143040; //AA0F8200
	unsigned int in_decimal5 = 2853405184; //AA138200
	unsigned int in_decimal6 = 2853667328; //AA178200
	unsigned int in_decimal7 = 2853929472; //AA1B8200
	unsigned int in_decimal8 = 2854191616; //AA1F8200
	unsigned int in_decimal9 = 2854453760; //AA238200
	unsigned int in_decimal10 = 2854715904;//AA278200

	cout << "1" << endl;
	bitset<32> in_binary1(in_decimal1);
	for(int i = 0; i < 32; i++){
		in_bool[i] = in_binary1[i];
	}
	a1.memory_access(in_bool, 0, 1);
	//getchar();

	cout << "2" << endl;
	bitset<32> in_binary2(in_decimal2);
	for(int i = 0; i < 32; i++){
		in_bool[i] = in_binary2[i];
	}
	a1.memory_access(in_bool, 0, 1);
	//getchar();

	cout << "3" << endl;
	bitset<32> in_binary3(in_decimal3);
	for(int i = 0; i < 32; i++){
		in_bool[i] = in_binary3[i];
	}
	a1.memory_access(in_bool, 0, 1);
	//getchar();

	cout << "4" << endl;
	bitset<32> in_binary4(in_decimal4);
	for(int i = 0; i < 32; i++){
		in_bool[i] = in_binary4[i];
	}
	a1.memory_access(in_bool, 0, 1);
	//getchar();

	cout << "5" << endl;
	bitset<32> in_binary5(in_decimal5);
	for(int i = 0; i < 32; i++){
		in_bool[i] = in_binary5[i];
	}
	a1.memory_access(in_bool, 0, 1);
	//getchar();

	cout << "6" << endl;
	bitset<32> in_binary6(in_decimal6);
	for(int i = 0; i < 32; i++){
		in_bool[i] = in_binary6[i];
	}
	a1.memory_access(in_bool, 0, 1);
	//getchar();

	cout << "7" << endl;
	bitset<32> in_binary7(in_decimal7);
	for(int i = 0; i < 32; i++){
		in_bool[i] = in_binary7[i];
	}
	a1.memory_access(in_bool, 0, 1);
	//getchar();

	cout << "8" << endl;
	bitset<32> in_binary8(in_decimal8);
	for(int i = 0; i < 32; i++){
		in_bool[i] = in_binary8[i];
	}
	a1.memory_access(in_bool, 0, 1);
	//getchar();

	cout << "9" << endl;
	bitset<32> in_binary9(in_decimal9);
	for(int i = 0; i < 32; i++){
		in_bool[i] = in_binary9[i];
	}
	a1.memory_access(in_bool, 0, 1);
	//getchar();

	cout << "10" << endl;
	bitset<32> in_binary10(in_decimal10);
	for(int i = 0; i < 32; i++){
		in_bool[i] = in_binary10[i];
	}
	a1.memory_access(in_bool, 0, 1);
	//getchar();

/*
	for(int i = 0; i < 48; i++){
		bitset<32> in_binary(in_decimal);
		bool in_bool[32];
		for(int j = 0; j < 32; j++){
			in_bool[j] = in_binary[j];
		}
		a1.memory_access(in_bool, 0, 1);
		in_decimal = in_decimal + 4;
	}
*/
	return 0;
}
