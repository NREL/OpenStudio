/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef ISOMODEL_TIMEFRAME_HPP
#define ISOMODEL_TIMEFRAME_HPP

namespace openstudio {
namespace isomodel {

  class TimeFrame
  {
   public:
    static const int TIMESLICES = 8760;

    TimeFrame();

    static int monthLength(int month);
    int YTD[TIMESLICES], Hour[TIMESLICES], Day[TIMESLICES], Month[TIMESLICES];
  };
}  // namespace isomodel
}  // namespace openstudio
#endif
