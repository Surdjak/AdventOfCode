#include <map>
#include <string>

namespace day1 {
    class Dial {
    private:
        int range;
        int position;
        std::map<int, long long> encounteredStates;
    public:
        struct Instruction {
            int steps;
            Instruction(std::string s);
        };
        Dial(int range, int position = 50);
        void log_encountered_state(int index);
        void log_encountered_states();
        void jump(const Instruction& instr);
        void turn(const Instruction& instr);
        int getPosition() const;
        int getEncounterCount(int index) const;
    };
}
