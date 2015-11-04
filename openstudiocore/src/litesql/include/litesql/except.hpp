/* LiteSQL 
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */

#ifndef _litesql_except_hpp
#define _litesql_except_hpp
#include <iostream>
#include <string>
#include <exception>
#include "litesql/utils.hpp"
/** \file except.hpp
    contains litesql's exception classes */
namespace litesql {
/** base class for exceptions */
	class Except : public std::exception  {
private:
    std::string msg;
public: 
    Except(std::string m) throw()  : msg(m) {}
    virtual ~Except(void) throw() {}
	virtual const char* what() const throw() {
		return msg.c_str();
	}
    friend std::ostream &operator<<(std::ostream &os, const Except &e) {
        os << e.msg;
        return os;
    }
};
/** exception thrown when a record is not found */    
class NotFound : public Except {
public:
    NotFound(std::string s="") : Except("NotFound: "+s) {}
};
/** exception thrown when database cannot be accessed */
class DatabaseError : public Except {
public:
    DatabaseError(std::string m) : Except("DatabaseError: "+m) {}
};
/** exception thrown when SQL statement cannot be executed */
class SQLError : public Except {
public:
    SQLError(std::string m) : Except("SQLError: "+m) {}
};
/** exception thrown when backend produces internal error */
class InternalError : public Except {
public:
    InternalError(std::string m) : Except("InternalError: " +m) {}
};
/** exception thrown when backend cannot allocate memory */
class MemoryError : public Except {
public:
    MemoryError(std::string m) : Except("Allocation failed: "+m){}
};
/** exception thrown when database (disk) is full */
class InsertionError : public Except {
public:
    InsertionError(std::string m) : Except("Database full: "+m){}
};
/** exception thrown when none of other exceptions match */
class UnknownError : public Except {
    // handles the rest
public:
    UnknownError(std::string m) : Except("UnknownError: "+m){}
};


}
#endif 
