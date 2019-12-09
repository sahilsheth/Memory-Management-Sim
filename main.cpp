#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "memoryBlock.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;





void readFile(vector<process> &);
//void printMemory(vector<memoryBlock> &memBlocks, int &memorySize, int &pageSize, int &memoryLeft);
void printMemory(vector<memoryBlock> &memBlocks, int &memorySize, int &pageSize);
void print(vector<int> &);
void printResults(vector<process> &, vector<int> &, int &, int &);
void find(vector<process> &, vector<int> &);
bool addToMem(vector<memoryBlock> &, int &, int &, process &);
void createMem(vector<memoryBlock> &, int &, int &);
void removeFromMemoryMap(vector<memoryBlock> &, int &, int &, int &);
int check(vector<memoryBlock> &, int &, int &, int &);


              
void readFile(vector<process> &processing)
{
    ifstream readingFile;
    readingFile.open("in1.txt");
    process p;
    int processes;
    readingFile >> processes;
    for(int i = 0; i < processes; i++)
    {
        readingFile >> p.memoryNum;
        readingFile >> p.startTime;
        readingFile >> p.endTime;
        readingFile >> p.numBlocks;
        vector<int> tempBlock;
        int blocks;
        for(int j = 0; j < p.numBlocks; j++)
        {
            readingFile >> blocks;
            tempBlock.push_back(blocks);
        }
        
        p.memoryBlocks = tempBlock;
        processing.push_back(p);
    }
    readingFile.close();
}

void printMemory(vector<memoryBlock> &memBlocks, int &memorySize, int &pageSize)
{
    
    int begin = 0;
    int finish = 0;
    int countMem = 0;
    cout << "Memory Map: " << endl;
    
    
    while (countMem < (memorySize / pageSize))
    {
        if (!memBlocks[countMem].blockAvailable)
        {
            if (begin != 0)
            {
                finish = countMem - 1;
                cout << "            " << begin * pageSize << "-" << ((finish + 1) * pageSize) - 1 << ": Free frame(s)" <<endl;
                begin = 0;
            }
            
            cout << "            " << memBlocks[countMem].startingBlock << "-" << memBlocks[countMem].endingBlock << ": Processes "
            << memBlocks[countMem].numberofProcess << " , Pages " << memBlocks[countMem].numberofPages << endl;
        }
        else if (begin == 0)
        {
            begin = countMem;
        }
        
        countMem++;
    }
    
    if (begin != 0)
    {
        finish = countMem - 1;
        cout << "     " << begin * pageSize << "-" << ((finish + 1) * pageSize) - 1 << ": Free frame(s)" <<endl;
    }
    
    cout << endl;
}

void print(vector<int> & printing)
{
    for (int i = 0; i < printing.size(); i++)
        cout << printing[i] << " ";
}


void printResults(vector<process> &lists, vector<int> &timing, int &sizeofMem, int &sizeofPage)
{
    vector<int> input;
    bool line;
    process q;
    vector<memoryBlock> memMap;
    
    //Create the Memory Map
    createMem(memMap, sizeofMem, sizeofPage);
    
    for (int i = 0; i < timing.size(); ++i)
    {
        line = true;
        
        //cout << "t = " << timing[i] << ": ";
        
     
        for (int j = 0; j < lists.size(); ++j)
        {
            if (lists[j].startTime == timing[i])
            {
                input.push_back(lists[j].memoryNum);
                
                if (!line)
                {
                    cout << "    ";
                }
                
                cout << "Process " << lists[j].memoryNum << " arrives" << endl;
                cout << "Input: [ ";
                print(input);
                cout << "]" << endl;
                
                line = false;
            }
            else if (lists[j].startTime + lists[j].endTime == timing[i])
            {
                if (!line)
                {
                    cout << "       ";
                }
                
                cout << "Process " << lists[j].memoryNum << " completes" << endl;
                removeFromMemoryMap(memMap, sizeofMem, sizeofPage, lists[j].memoryNum);
                printMemory(memMap, sizeofMem, sizeofPage);
                line = false;
            }
        }
        
        
        while (input.size() != 0)
        {
       
            q = lists[input.front() - 1];
            
            if (addToMem(memMap, sizeofMem, sizeofPage, q) )
            {
                cout << "Memory Map moves Process " << q.memoryNum << " to memory" << endl;
                
                input.erase(input.begin());
                
                cout << " Input: [ ";
                print(input);
                cout << "]" << endl;
                
                
                printMemory(memMap, sizeofMem, sizeofPage);
            }
            else
            {
                cout << "\nNo free space!\n\n";
                break;
            }
        }
    }
}

void removeFromMemoryMap(vector<memoryBlock> &mapping, int &memSize, int &pageSize, int &r)
{
    for (int i = 0; i < mapping.size(); i++)
    {
        if (mapping[i].numberofProcess == r)
        {
            mapping[i].blockAvailable = true;
        }
    }
}


bool addToMem(vector<memoryBlock> &memoriesMap, int &memoriesSize, int &pagesSize, process &s)
{
    
    double sizeofBlocks;
    int insize;
    
    int blockStart = -1;
    bool adding;
    int numbPage = 1;
    
    for (int i = 0; i < s.numBlocks; i++)
    {
        sizeofBlocks = s.memoryBlocks[i] / pagesSize;
        
        for (int j = i; j < s.numBlocks; ++j)
        {
            sizeofBlocks += (s.memoryBlocks[j] / pagesSize);
        }
        
        insize = (int) sizeofBlocks;
        blockStart = check(memoriesMap, memoriesSize, pagesSize, insize);
        
        if (blockStart == -1)
        {
            insize = (int) sizeofBlocks;
            blockStart = check(memoriesMap, memoriesSize, pagesSize, insize);
        }
        
        adding = false;
        
        if (blockStart > -1)
        {
            adding = true;
            
            for (int j = blockStart; j < (blockStart + ((s.memoryBlocks[i] + 99) / pagesSize)); j++)
            {
                memoriesMap[j].numberofProcess = s.memoryNum;
                memoriesMap[j].numberofPages = numbPage;
                memoriesMap[j].blockAvailable = false;
                numbPage++;
            }
        }
    }
    
    return adding;
}




void createMem(vector<memoryBlock> &memories, int &mem, int &page)
{
    memoryBlock t;
    
    for (int i = 0; i < (mem / page); ++i)
    {
        t.startingBlock = i * page;
        t.endingBlock = ((i + 1) * page) - 1;
        t.numberofProcess = -1;
        t.numberofPages = -1;
        t.blockAvailable = true;
        memories.push_back(t);
    }
}




void find(vector<process> &listing, vector<int> &times)
{
    bool finding;
    bool found;
    
    for (int i = 0; i < listing.size(); i++)
    {
        finding = false;
        found = false;
        
        for (int j = 0; j < times.size(); j++)
        {
            if (listing[i].startTime == times[j])
            {
                finding = true;
            }
            
            if (listing[i].startTime + listing[i].endTime == times[j])
            {
                found = true;
            }
        }
        
        if (!finding)
        {
            times.push_back(listing[i].startTime);
        }
        
        if (!found)
        {
            times.push_back(listing[i].startTime + listing[i].endTime);
        }
    }
    
    sort(times.begin(), times.end());
}

int check(vector<memoryBlock> &memoMap, int &memoSize, int &pagesSize, int &blocksSize)
{
    int free = 0;
    
    for (int i = 0; i < memoMap.size(); i++)
    {
        if (memoMap[i].blockAvailable)
        {
            free++;
        }
        else
        {
            free = 0;
        }
        
        if (free == blocksSize)
        {
            
            return (i + 1 - blocksSize);
        }
    }
    
    return -1;
}



int main(int argc, char** argv)
{
    int sizeofPages = 0;
    int sizeofMemory = 0;
    
    if(argc == 3)
    {
        sizeofMemory = atoi(argv[1]);
        sizeofPages = atoi(argv[2]) * 100;

    }
    else
    {
        cout<< "Memory Size(Kbytes): ";
        cin>> sizeofMemory;
        cout<< "Page Size (1:100, 2:200, 3:400): ";
        cin>> sizeofPages;
        sizeofPages = sizeofPages * 100;
    }
    
    //Variables
    vector<process> listofProcesses;
    vector<int> memTime;
    
    if (sizeofPages == 300)
    {
        sizeofPages = sizeofPages + 100;
    }
    
    readFile(listofProcesses);
    find(listofProcesses, memTime);
    printResults(listofProcesses, memTime, sizeofMemory, sizeofPages);
    return 0;
    
}
