#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <queue>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;



struct memoryBlock
{
    int startingBlock;
    int endingBlock;
    int numberofPages;
    int numberofProcess;
    bool blockAvailable;
};
