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

#include <boost/numeric/ublas/matrix.hpp> 
#include <boost/numeric/ublas/matrix_sparse.hpp> 
#include <boost/numeric/ublas/io.hpp> 
#include <boost/numeric/ublas/matrix_proxy.hpp> 
#include <boost/numeric/ublas/triangular.hpp> 
#include <boost/numeric/ublas/lu.hpp> 

#include <cassert>

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

  void System::testExceptions5()
  {
    using namespace boost::numeric::ublas; 
    using namespace std; 

    struct BreakUBlas
    {
      static bool invertMatrix(matrix<double>& orig, matrix<double>& inverted) { 
        typedef permutation_matrix<std::size_t> pmatrix; 
        matrix<double> A(orig); 
        pmatrix pm(A.size1()); 

        // perform LU-factorization 
        int res = lu_factorize(A,pm); 
        if( res != 0 ) return false; 

        inverted.assign(identity_matrix<double>(A.size1())); 
        lu_substitute(A, pm, inverted); 
  
        return true; 
      }; 

      static void breakIt() { 
        double data[6][6] = {{15, 29700, 472042, 7.8021e+06, 1.32426e+08, 2.29091e+09}, 
          {29700,1.32426e+08,2.29091e+09,4.01989e+10,7.13142e+11,1.27611e+13}, 
          {472042,2.29091e+09,4.01989e+10,7.13142e+11,1.27611e+13,2.29941e+14}, 
          {7.8021e+06,4.01989e+10,7.13142e+11,1.27611e+13,2.29941e+14,4.16694e+15}, 
          {1.32426e+08,7.13142e+11,1.27611e+13,2.29941e+14,4.16694e+15,7.58705e+16}, 
          {2.29091e+09,1.27611e+13,2.29941e+14,4.16694e+15,7.58705e+16,1.38694e+18}}; 

        matrix<double> a(6, 6);
        for (unsigned i = 0; i < a.size1 (); ++i)
          for (unsigned j = 0; j < a.size2 (); ++j)
            a(i, j) = data[i][j];

        matrix<double> b(a); 
        invertMatrix(a, b); 
      } 
    };

    try {
      std::cout << "testExceptions5: Test 1" << std::endl;
      BreakUBlas::breakIt();
      assert(!"Exception Not Thrown");
    } catch (boost::numeric::ublas::internal_logic &) {
    }

    try {
      std::cout << "testExceptions5: Test 2" << std::endl;
      BreakUBlas::breakIt();
      assert(!"Exception Not Thrown");
    } catch (std::logic_error &) {
    }

    try {
      std::cout << "testExceptions5: Test 3" << std::endl;
      BreakUBlas::breakIt();
      assert(!"Exception Not Thrown");
    } catch (std::exception &) {
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
