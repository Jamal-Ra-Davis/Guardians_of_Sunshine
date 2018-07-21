#include "../headers/InputBuffer.h"

int InputBuffer::data[COMBO_LENGTH] = {
                        UP,
                        DOWN,
                        LEFT,
                        LEFT,
                        RIGHT,
                        RIGHT,
                        DOWN,
                        SPIN,
                        DOWN,
                        UP,
                        LEFT,
                        RIGHT,
                        LEFT,
                        DOWN,
                        SPIN,
                        UP,
                        DOWN,
                        JUMP
                    };

InputBuffer::InputBuffer()
{
/*
	int temp[COMBO_LENGTH] = {
                        UP,
                        DOWN,
                        LEFT,
                        LEFT,
                        RIGHT,
                        RIGHT,
                        DOWN,
                        SPIN,
                        DOWN,
                        UP,
                        LEFT,
                        RIGHT,
                        LEFT,
                        DOWN,
                        SPIN,
                        UP,
                        DOWN,
                        JUMP
                    };
*/
	//max_list_size = COMBO_LENGTH;//max_list_size looks like a constant. can probably remove and just use COMBO_LENGTH
	//for (int i=0; i<COMBO_LENGTH; i++)
	//{
	//	data[i] = temp[i];
	//}


	halfMatch = false;
    midMatch = false;
    fullMatch = false;
}
const char* InputBuffer::getKeyName(int key)
{
	switch (key)
	{
		case UP:
			return "UP";
		case DOWN:
			return "DOWN";
		case LEFT:
			return "LEFT";
		case RIGHT:
			return "RIGHT";
		case JUMP:
			return "JUMP";
		case SPIN:
			return "SPIN";
		default:
			return "Invalid Key";
	}
}
void InputBuffer::printOutput(bool text)
{
	printf("Combo Move:\t");
	for (int i=0; i<COMBO_LENGTH; i++)
	{
		if (text)
			printf("%6s, ", getKeyName(data[i]));
		else
			printf("%d, ", data[i]);
	}
	printf("\n");
	printf("Input:     \t");
	std::list<int>::iterator iter;
    for (iter=inputs.begin(); iter != inputs.end();  ++iter)
	{
		if (text)
			printf("%6s, ", getKeyName(*iter));
		else
			printf("%d, ", *iter);
	}
	printf("\n");
}
void InputBuffer::reset()
{
	halfMatch = false;
    midMatch = false;
    fullMatch = false;

	while (inputs.size() > 0)
    {
        inputs.pop_front();
    }
}
void InputBuffer::add(int val)
{
	inputs.push_back(val);
    while (inputs.size() > COMBO_LENGTH)
    {
        inputs.pop_front();
    }
}
void InputBuffer::checkLists()
{
	halfMatch = false;
    midMatch = false;
    fullMatch = false;

	std::list<int>::iterator iter;
    for (iter=inputs.begin(); iter != inputs.end();  ++iter)
    {
		bool halfMatch_ = true;
        bool midMatch_ = true;
        bool fullMatch_ = true;
        std::list<int>::iterator sub_iter;
        int i=0;
        for (sub_iter=iter; sub_iter != inputs.end(); ++sub_iter)
        {
            int val = *sub_iter;
            if (val != data[i])
            {
                if (i < COMBO_LENGTH)
                    fullMatch_ = false;
                if (i < (3*COMBO_LENGTH)/4)
                    midMatch_ = false;
                if (i < COMBO_LENGTH/2)
                    halfMatch_ = false;
            }
            i++;
        }
		if (i < COMBO_LENGTH/2)
            halfMatch_ = false;
        if (i < (3*COMBO_LENGTH)/4)
            midMatch_ = false;
        if (i < COMBO_LENGTH)
            fullMatch_ = false;

        if (halfMatch_)
            halfMatch = true;
        if (midMatch_)
            midMatch = true;
        if (fullMatch_)
            fullMatch = true;
	}
}
