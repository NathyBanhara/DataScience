#ifndef SAX_EVENT_CONSUMER_HPP

#define SAX_EVENT_CONSUMER_HPP

#include <iostream>
#include "json.hpp"
#include "queue.hpp"

using json = nlohmann::json;

class sax_event_consumer : public json::json_sax_t
{

    public:
    Queue queue = Queue();

    std::string name = "";

    bool null() override;
    bool boolean(bool val) override;
    bool number_integer(number_integer_t val) override;
    bool number_unsigned(number_unsigned_t val) override;
    bool number_float(number_float_t val, const string_t& s) override;
    bool string(string_t& val) override;
    bool start_object(std::size_t elements) override;
    bool end_object() override;
    bool start_array(std::size_t elements) override;
    bool end_array() override;
    bool key(string_t& val) override;
    bool binary(json::binary_t& val) override;
    bool parse_error(std::size_t position, const std::string& last_token, const json::exception& ex) override;
};

#endif