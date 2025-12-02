#include <iostream>
#include <cmath>
#include "day1/dial.h"

using namespace day1;

Dial::Instruction::Instruction(std::string s) {
    // First character is either 'L' or 'R'
    char direction = s[0];
    // The rest is the number of steps
    int magnitude = std::stoi(s.substr(1));
    steps = (direction == 'L') ? -magnitude : magnitude;
}

Dial::Dial(int range, int position) 
    : range(range), position(position) 
{
    for (int i = 0; i < range; ++i) 
    {
        encounteredStates[i] = 0;
    }
    encounteredStates[position] = 1;
}

void Dial::log_encountered_state(int index)
{
    std::cout << index << ": " << encounteredStates[index] << std::endl;
}

void Dial::log_encountered_states() 
{
    for (int i = 0; i < range; ++i) 
    {
        log_encountered_state(i);
    }
}

void Dial::jump(const Instruction& instr)
{
    position = (position + instr.steps) % range;
    if (position < 0) 
    {
        position += range;
    }
    encounteredStates[position]++;
}

void Dial::turn(const Instruction& instr) 
{
    for (int i = 0; i < std::abs(instr.steps); ++i) 
    {
        position += (instr.steps > 0) ? 1 : -1;
        if (position >= range) 
        {
            position = 0;
        } 
        else if (position < 0) 
        {
            position = range - 1;
        }
        encounteredStates[position]++;
    }
}

int Dial::getPosition() const 
{
    return position;
}

int Dial::getEncounterCount(int index) const 
{
    auto it = encounteredStates.find(index);
    if (it != encounteredStates.end()) 
    {
        return it->second;
    }
    return 0;
}
