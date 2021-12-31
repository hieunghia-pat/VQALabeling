#include "exception.hpp"

#include <exception>
#include <string>

Exception::Exception(std::string message)
    : m_message { message }
{

}

const char* Exception::what() const throw()
{
    return m_message.data();
}