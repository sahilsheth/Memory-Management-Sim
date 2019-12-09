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

struct process
{
    int memoryNum;
    int startTime;
    int endTime;
    int numBlocks;
    vector<int> memoryBlocks;
};
