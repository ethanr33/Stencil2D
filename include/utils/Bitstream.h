
#include <deque>
#include <cstdint>

class Bitstream {
    private:
        // Push to front, pop from back
        std::deque<bool> bitstream;
    public:
        void push_byte(uint8_t byte);
        bool pop();
        uint64_t pop_num(uint8_t amount, bool is_msb_first=false);

        bool is_empty() const;
};