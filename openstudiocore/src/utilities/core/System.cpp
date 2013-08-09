/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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



#include <utilities/core/System.hpp>
#include "Application.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>



namespace openstudio{

#ifdef _WINDOWS

  #define _WIN32_WINNT 0x0500

  #define NOMINMAX

  #include <windows.h>

  /// return the amount of time that the system has been idle
  boost::optional<Time> System::systemIdleTime()
  {
    boost::optional<Time> result;

    LASTINPUTINFO lastInput;
    lastInput.cbSize = sizeof(LASTINPUTINFO);
    if (GetLastInputInfo(&lastInput)){
      // current system up time in MS, lastInput is referenced to this
      // maximum value is 49.7 days, how telling.....
      DWORD tickCount = GetTickCount(); 

      // idle milliseconds
      unsigned idleMS = boost::lexical_cast<unsigned>(tickCount-lastInput.dwTime); 
      
      // use integer division like a pro
      result = Time(0,0,0,idleMS/1000);
    }
    return result;
  }


#else

  /// return the amount of time that the system has been idle
  boost::optional<Time> System::systemIdleTime()
  {
    return boost::optional<Time>();
  }

#endif


  void System::msleep(int msecs)
  {
    boost::posix_time::ptime start = boost::posix_time::microsec_clock::universal_time();

    int remainingtime = msecs;
    while ( remainingtime > 0 )
    {
      bool didwork = openstudio::Application::instance().processEvents(remainingtime);
      remainingtime = msecs - (boost::posix_time::microsec_clock::universal_time() - start).total_milliseconds();
//      std::cout << "time " << msecs << " remainingtime " << remainingtime << std::endl;

      int timetosleep = 0;

      if (remainingtime > 0)
      {
        if (didwork)
        {
          timetosleep = 1; // it was working, let it keep processing after a yield
        } else {
          timetosleep = remainingtime<10?remainingtime:10;
        }
      }

#ifdef _WINDOWS
        Sleep(timetosleep);
#else
        usleep(timetosleep * 1000);
#endif
    }

    openstudio::Application::instance().processEvents(); // process any outstanding events
  }

  unsigned System::numberOfProcessors()
  {
    unsigned numberOfProcessors = boost::thread::hardware_concurrency();
    if (numberOfProcessors < 1){
      numberOfProcessors = 1;
    }
    return numberOfProcessors;
  }


  void System::testExceptions1()
  {
    try {
      std::cout << "testExceptions1: Test 1" << std::endl;
      throw std::exception();
    } catch (const std::exception &) {
    }

    try {
      std::cout << "testExceptions1: Test 2" << std::endl;
      throw std::exception();
    } catch (...) {
    }
  }

  void System::testExceptions2()
  {
    try {
      std::cout << "testExceptions2: Test 1" << std::endl;
      throw std::runtime_error("test");
    } catch (const std::runtime_error &) {
    }

    try {
      std::cout << "testExceptions2: Test 2" << std::endl;
      throw std::runtime_error("test");
    } catch (const std::exception &) {
    }

    try {
      std::cout << "testExceptions2: Test 3" << std::endl;
      throw std::runtime_error("test");
    } catch (...) {
    }
   }

  void System::testExceptions3()
  {
    class MyException : public std::runtime_error
    {
      public:
        MyException() : std::runtime_error("exception")
        {}

        virtual ~MyException() throw() 
        {}
    };

    try {
      std::cout << "testExceptions3: Test 1" << std::endl;
      throw MyException();
    } catch (const MyException &) {
    }

    try {
      std::cout << "testExceptions3: Test 2" << std::endl;
      throw MyException();
    } catch (const std::runtime_error &) {
    }

    try {
      std::cout << "testExceptions3: Test 3" << std::endl;
      throw MyException();
    } catch (const std::exception &) {
    }

    try {
      std::cout << "testExceptions3: Test 4" << std::endl;
      throw MyException();
    } catch (...) {
    }
  }

  void System::testExceptions4()
  {
    class MyException : public std::runtime_error
    {
      public:
        MyException() : std::runtime_error("exception")
        {}

        virtual ~MyException() throw() 
        {}
    };

    class MyException2 : public MyException
    {
      public:
        MyException2() 
        {}

        virtual ~MyException2() throw() 
        {}
    };


    try {
      std::cout << "testExceptions4: Test 1" << std::endl;
      throw MyException2();
    } catch (const MyException2 &) {
    }

    try {
      std::cout << "testExceptions4: Test 2" << std::endl;
      throw MyException2();
    } catch (const MyException &) {
    }

    try {
      std::cout << "testExceptions4: Test 3" << std::endl;
      throw MyException2();
    } catch (const std::runtime_error &) {
    }

    try {
      std::cout << "testExceptions4: Test 4" << std::endl;
      throw MyException2();
    } catch (const std::exception &) {
    }

    try {
      std::cout << "testExceptions4: Test 5" << std::endl;
      throw MyException2();
    } catch (...) {
    }
  }


} // openstudio
