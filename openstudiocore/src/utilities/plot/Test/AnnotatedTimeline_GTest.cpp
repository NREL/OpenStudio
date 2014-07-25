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

#include <gtest/gtest.h>

#include "../AnnotatedTimeline.hpp"
#include "../../data/Vector.hpp"
#include "../../data/TimeSeries.hpp"

using openstudio::AnnotatedTimeline;
using openstudio::Date;
using openstudio::Time;
using openstudio::path;
using openstudio::toPath;
using openstudio::randVector;
using openstudio::cumsum;
using openstudio::TimeSeries;

TEST(AnnotatedTimeline, StockPrices)
{
  // make two time series with random data
  unsigned N = 1000;
  TimeSeries t1(Date::currentDate(), Time(0,1), cumsum(randVector(-1,1.1,N), 100), "pesos");
  TimeSeries t2(Date::currentDate(), Time(0,1), cumsum(randVector(-1,1.1,N)), "dollars");

  AnnotatedTimeline::Ptr timeline = AnnotatedTimeline::create();
  timeline->addTimeSeries("Mexican Stocks", t1);
  timeline->addTimeSeries("US Stocks", t2);
  timeline->save(toPath("./StockPrices.htm"));
}
