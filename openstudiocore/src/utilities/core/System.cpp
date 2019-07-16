/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "System.hpp"
#include "Application.hpp"

#include <boost/thread.hpp>

#include <boost/numeric/ublas/lu.hpp>


namespace openstudio{

#ifdef _WINDOWS

  #define _WIN32_WINNT 0x0500

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
