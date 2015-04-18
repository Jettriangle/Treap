#include <iostream>
#include <cstdlib>
#include "tests.h"

int main() 
{
	Test test;
	if(test.Check(100, 0, 100))
		cout << "Accept\n";
	else
		cout << "Wrong answer\n";
    return 0;
}