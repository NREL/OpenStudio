#include "Error.hpp"

#include <iostream>

void error(QString mesg DECCFILELINE)
{
#ifdef NOFILELINE
    std::cout << mesg.toStdString() << std::endl;
#else
    std::cout << mesg.toStdString() << '(' << file << ',' << line << ")\n";
#endif
    exit(EXIT_FAILURE);
}

