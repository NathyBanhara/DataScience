#include "json.hpp"
#include "queue.hpp"
#include "sax_event_consumer.hpp"

using json = nlohmann::json;


bool sax_event_consumer::null()
{
    this->queue.push(queue.createStruct("null", "null"));
    return true;
}
 
bool sax_event_consumer::boolean(bool val)
{
    this->queue.push(queue.createStruct(to_string(val), "boolean"));
    return true;
}
 
bool sax_event_consumer::number_integer(number_integer_t val)
{
    this->queue.push(queue.createStruct(to_string(val), "integer"));
    return true;
}

bool sax_event_consumer::number_unsigned(number_unsigned_t val)
{
    this->queue.push(queue.createStruct(to_string(val), "integer"));
    return true;
}

bool sax_event_consumer::number_float(number_float_t val, const string_t& s)
{
    this->queue.push(queue.createStruct(to_string(val), "double"));
    return true;
}
 
bool sax_event_consumer::string(string_t& val)
{
    this->queue.push(queue.createStruct(val, "string"));
    return true;
}

bool sax_event_consumer::start_object(std::size_t elements)
{
    return true;
}

bool sax_event_consumer::end_object()
{
    return true;
}

bool sax_event_consumer::start_array(std::size_t elements)
{
    this->queue.push(queue.createStruct(this->name, "array"));
    return true;
}
 
bool sax_event_consumer::end_array()
{
    this->queue.push(queue.createStruct("end_array", ""));
    return true;
}
 
bool sax_event_consumer::key(string_t& val)
{
    this->name = val;
    return true;
}

bool sax_event_consumer::binary(json::binary_t& val)
{
    return true;
}

bool sax_event_consumer::parse_error(std::size_t position, const std::string& last_token, const json::exception& ex)
{
    //events.push_back("parse_error(position=" + std::to_string(position) + ", last_token=" + last_token + ",\n            ex=" + std::string(ex.what()) + ")");
    return false;
}