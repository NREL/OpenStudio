/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#ifndef UTILITIES_CORE_SINGLETON_HPP
#define UTILITIES_CORE_SINGLETON_HPP

// Warning: If T's constructor throws, instance() will return a null reference.

namespace openstudio{
  
  // Ripped from boost http://www.boost.org/doc/libs/1_42_0/libs/pool/doc/implementation/singleton.html
  // T must be: no-throw default constructible and no-throw destructible
  template <typename T>
  class Singleton
  {
    private:
      struct object_creator
      {
        // This constructor does nothing more than ensure that instance()
        //  is called before main() begins, thus creating the static
        //  T object before multithreading race issues can come up.
        object_creator() { Singleton<T>::instance(); }
        inline void do_nothing() const {  }
      };
      static object_creator create_object;

      Singleton();

    public:

      typedef T object_type;

      // If, at any point (in user code), Singleton<T>::instance()
      //  is called, then the following function is instantiated.
      static object_type & instance()
      {
        // This is the object that we return a reference to.
        // It is guaranteed to be created before main() begins because of
        //  the next line.
        static object_type obj;

        // The following line does nothing else than force the instantiation
        //  of Singleton<T>::create_object, whose constructor is
        //  called before main() begins.
        create_object.do_nothing();

        return obj;
      }
  };
  template <typename T>
  typename Singleton<T>::object_creator
  Singleton<T>::create_object;

}; // openstudio

#endif // UTILITIES_CORE_SINGLETON_HPP


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
