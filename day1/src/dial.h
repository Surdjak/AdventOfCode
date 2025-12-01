#include <map>
#include <iostream>
#include <cmath>

class Dial {
private:
    int range;
    int position;
    std::map<int, long long> encounteredStates;
public:
    struct Instruction {
        int steps;

        Instruction(std::string s) {
            // First character is either 'L' or 'R'
            char direction = s[0];
            // The rest is the number of steps
            int magnitude = std::stoi(s.substr(1));
            steps = (direction == 'L') ? -magnitude : magnitude;
        }
    };

    Dial(int range, int position = 50) 
        : range(range), position(position) 
    {
        for (int i = 0; i < range; ++i) 
        {
            encounteredStates[i] = 0;
        }
        encounteredStates[position] = 1;
    }

    void log_encountered_state(int index)
    {
        std::cout << index << ": " << encounteredStates[index] << std::endl;
    }

    void log_encountered_states() 
    {
        for (int i = 0; i < range; ++i) 
        {
            log_encountered_state(i);
        }
    }

    void jump(const Instruction& instr)
    {
        position = (position + instr.steps) % range;
        if (position < 0) 
        {
            position += range;
        }
        encounteredStates[position]++;
    }

    void turn(const Instruction& instr) 
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

    int getPosition() const 
    {
        return position;
    }

    int getEncounterCount(int index) const 
    {
        auto it = encounteredStates.find(index);
        if (it != encounteredStates.end()) 
        {
            return it->second;
        }
        return 0;
    }
};