#pragma once

#include <SDL2/SDL.h>
#include <list>

#define COMBO_LENGTH 18

class InputBuffer
{
	private:
		std::list<int> inputs;
		int max_list_size;
    	int data[COMBO_LENGTH];

		bool halfMatch;
		bool midMatch;
		bool fullMatch;		

	public:
		static int data_test[COMBO_LENGTH];
		enum Keys{UP, DOWN, LEFT, RIGHT, JUMP, SPIN};
		InputBuffer();
	
		int* getComboData() {return data;}
		int getComboLength() {return COMBO_LENGTH;}	
		bool foundHalfMatch() {return halfMatch;}
		bool foundMidMatch() {return midMatch;}
		bool foundFullMatch() {return fullMatch;}
	
		void printOutput();
		void reset();
		void add(int val);
		void checkLists();
};
