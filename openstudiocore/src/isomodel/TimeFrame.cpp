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

#include "TimeFrame.hpp"
#include <cassert>

namespace openstudio {
namespace isomodel {

  TimeFrame::TimeFrame()
  {
    int i = 0,j = 0;
    for(int I = 1; I <= 12;I++)
    {
      int dim = monthLength(I);
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
