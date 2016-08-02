#pragma once

#include <SDL2/SDL.h>
#include <list>

class InputBuffer
{
	private:
		std::list<int> inputs;
		int max_list_size;
    	int data[18];

		bool halfMatch;
		bool midMatch;
		bool fullMatch;		

	public:
		enum Keys{UP, DOWN, LEFT, RIGHT, JUMP, SPIN};
		InputBuffer();
		
		bool foundHalfMatch() {return halfMatch;}
		bool foundMidMatch() {return midMatch;}
		bool foundFullMatch() {return fullMatch;}
	
		void printOutput();
		void reset();
		void add(int val);
		void checkLists();
};
