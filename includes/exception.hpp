#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include <string>

class Exception: public std::exception
{
public:
    Exception(std::string message);

    const char* what() const throw() override;

private:
    std::string m_message;
};

#endif // EXCEPTION_HPP