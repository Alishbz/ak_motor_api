/****************************************************************************************
* ALI SAHBAZ
*
*
* FIFO Queue
*
* Date          : 02.03.2024
* By            : Ali Sahbaz
* e-mail        : ali_sahbaz@outlook.com
*/
#pragma once

#include <cstring> 

template <typename TYPE , size_t SIZE>
class circular_queue {
    TYPE  data_buffer[SIZE];
    unsigned int read_ptr;
    unsigned int write_ptr;

public:

    circular_queue() :read_ptr(0), write_ptr(0) {
        std::memset(data_buffer, 0, sizeof(data_buffer));
    }

    void clear() {
        read_ptr = 0;
        write_ptr = 0;
        std::memset(data_buffer, 0, sizeof(data_buffer));
    }

    // false -> not empty
    bool is_empty() const {
        if (read_ptr != write_ptr) {
            return false;
        }
        return true;
    }

    size_t get_unread_size()const {
        return (write_ptr >= read_ptr) ? (write_ptr - read_ptr) : (SIZE - read_ptr + write_ptr);
    }

    void write(const TYPE& value) {
        data_buffer[write_ptr] = value;
        write_ptr = (write_ptr + 1) % SIZE;
    }

    TYPE read() {
        if (is_empty() == false) {
            TYPE out = data_buffer[read_ptr];
            read_ptr = (read_ptr + 1) % SIZE;
            return out;
        }
        return TYPE{};
    }
};