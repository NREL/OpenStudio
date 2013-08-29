#ifndef OPENSTUDIO_RUNMANAGER_LIB_TESTS_SIGNAL_LISTENER
#define OPENSTUDIO_RUNMANAGER_LIB_TESTS_SIGNAL_LISTENER

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

#endif

