/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_SINGLETON_HPP
#  define UTILITIES_CORE_SINGLETON_HPP

// Warning: If T's constructor throws, instance() will return a null reference.

namespace openstudio {

// Ripped from boost http://www.boost.org/doc/libs/1_42_0/libs/pool/doc/implementation/singleton.html
// T must be: no-throw default constructible and no-throw destructible
template <typename T>
class Singleton
{
 public:
  Singleton() = delete;
  using object_type = T;

  // If, at any point (in user code), Singleton<T>::instance()
  //  is called, then the following function is instantiated.
  static object_type& instance() {
    static object_type obj;
    return obj;
  }
};
}  // namespace openstudio

#endif  // UTILITIES_CORE_SINGLETON_HPP

/* EXAMPLE USAGE

How to use this class?

myclass.hpp

#ifndef MYCLASS_HPP
#define MYCLASS_HPP

#include "singleton.hpp"

class MyClassSingleton
{

friend class Singleton<MyClass>;

public:

     void doSomething() { std::cout << "Something"; }

private:

     MyClass();

};

typedef Singleton<MyClassSingleton> MyClass;

#endif

main.cpp

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <ostream>

#include "myclass.hpp"

void test()
{
   MyClass::instance().doSomething();
}

int main(int argc, char* []argv)
{
    boost::thread thread1(&test);
    boost::thread thread2(&test);
    thread1.join();
    thread2.join();
    return 0;
}

*/
