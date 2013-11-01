
#include <isomodel/TimeFrame.hpp>
#include <cassert>

namespace openstudio {
namespace isomodel {

  TimeFrame::TimeFrame()
  {
    int i = 0,j = 0, dim;
    for(int I = 1; I <= 12;I++)
    {
      dim = monthLength(I);
      for(int J = 1;J<=dim;J++)
      {
        j++;
        for(int K=1;K<=24;K++)
        {
          assert(i < TIMESLICES);
          Hour[i] = K;
          Day[i] = J;
          Month[i] = I;
          YTD[i++] = j;
        }
      }		
    }
  }


  int TimeFrame::monthLength(int month)
  {
    switch(month)
    {
      case 2:
        return 28;
      case 9:
      case 4:
      case 6:
      case 11:
        return 30;
      default:
        return 31;
    }
  }
}
}
