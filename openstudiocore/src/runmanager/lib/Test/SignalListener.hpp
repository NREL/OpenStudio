/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef RUNMANAGER_LIB_TEST_SIGNALLISTENER_HPP
#define RUNMANAGER_LIB_TEST_SIGNALLISTENER_HPP

#include <QObject>
#include <gtest/gtest.h>
#include <QThread>

namespace openstudio
{
namespace runmanager 
{
namespace test
{
  class Test_Listener : public QObject
  {
    Q_OBJECT;

    public:

      bool slot_fired;

      Test_Listener()
        : slot_fired(false)
      {
      }

    public slots:
      void listen()
      {
        slot_fired = true;
        static volatile bool slotrunning = false;
        ASSERT_FALSE(slotrunning);
        slotrunning=true;
        for (int i = 0; i < 10; ++i)
        {
          QThread::yieldCurrentThread(); // make sure we yield and give another thread a chance to hop in here
        }

        slotrunning = false;

      }

  };
}
}
}

#endif // RUNMANAGER_LIB_TEST_SIGNALLISTENER_HPP

