
#include "utils/Bitstream.h"

void Bitstream::push_byte(uint8_t byte) {
    for (int i = 0; i < 8; i++) {
        this->bitstream.push_front(byte & 0x1);
        byte = byte >> 1;
    }
}

bool Bitstream::pop() {
    bool back = this->bitstream.back();
    this->bitstream.pop_back();
    return back;
}

uint64_t Bitstream::pop_num(uint8_t amount, bool is_msb_first) {
    uint64_t res = 0;

    for (int i = 0; i < amount; i++) {
        if (is_msb_first) {
            res = (res << 1) | this->pop();
        } else {
            res = (this->pop() << i) | res;
        }
    }

    return res;
}

bool Bitstream::is_empty() const {
    return this->bitstream.size() == 0;
}
