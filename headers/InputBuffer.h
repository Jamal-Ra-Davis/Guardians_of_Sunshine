#pragma once

#include <SDL2/SDL.h>
#include <list>

#define COMBO_LENGTH 18

class InputBuffer
{
	private:
		std::list<int> inputs;
		//int max_list_size;
    	//int data[COMBO_LENGTH];

		bool halfMatch;
		bool midMatch;
		bool fullMatch;		

	public:
		static int data[COMBO_LENGTH];
		enum Keys{UP, DOWN, LEFT, RIGHT, JUMP, SPIN, NUM_KEYS};
		InputBuffer();
	
		int* getComboData() {return data;}
		int getComboLength() {return COMBO_LENGTH;}	
		bool foundHalfMatch() {return halfMatch;}
		bool foundMidMatch() {return midMatch;}
		bool foundFullMatch() {return fullMatch;}
		const char* getKeyName(int key);
	
		void printOutput(bool text=true);
		void reset();
		void add(int val);
		void checkLists();
};
