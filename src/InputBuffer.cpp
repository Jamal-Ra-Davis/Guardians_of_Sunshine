#include "../headers/InputBuffer.h"

InputBuffer::InputBuffer()
{
	int temp[18] = {
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

	max_list_size = 18;
	for (int i=0; i<max_list_size; i++)
	{
		data[i] = temp[i];
	}


	halfMatch = false;
    midMatch = false;
    fullMatch = false;
}
void InputBuffer::printOutput()
{
	printf("Combo Move:\t");
	for (int i=0; i<max_list_size; i++)
	{
		printf("%d, ", data[i]);
	}
	printf("\nInput:\t");
	std::list<int>::iterator iter;
    for (iter=inputs.begin(); iter != inputs.end();  ++iter)
	{
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
    while (inputs.size() > max_list_size)
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
                if (i < max_list_size)
                    fullMatch_ = false;
                if (i < (3*max_list_size)/4)
                    midMatch_ = false;
                if (i < max_list_size/2)
                    halfMatch_ = false;
            }
            i++;
        }
		if (i < max_list_size/2)
            halfMatch_ = false;
        if (i < (3*max_list_size)/4)
            midMatch_ = false;
        if (i < max_list_size)
            fullMatch_ = false;

        if (halfMatch_)
            halfMatch = true;
        if (midMatch_)
            midMatch = true;
        if (fullMatch_)
            fullMatch = true;
	}
}
