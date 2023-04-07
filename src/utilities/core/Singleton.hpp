/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
