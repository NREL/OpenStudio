/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../PhotovoltaicPerformanceSimple.hpp"
#include "../Schedule.hpp"
#include "../ScheduleCompact.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, PhotovoltaicPerformanceSimple_Instantiate) {
  auto result = false;
  auto value = 0.1;

  Model model;

  PhotovoltaicPerformanceSimple photovoltaicPerformanceSimple(model);

  EXPECT_EQ("Fixed", photovoltaicPerformanceSimple.conversionEfficiencyInputMode());
  EXPECT_TRUE(photovoltaicPerformanceSimple.fixedEfficiency());
  EXPECT_EQ(0.12, photovoltaicPerformanceSimple.fixedEfficiency().get());
  EXPECT_EQ(photovoltaicPerformanceSimple.fractionOfSurfaceAreaWithActiveSolarCells(),0.89);
  EXPECT_TRUE(photovoltaicPerformanceSimple.isfractionOfSurfaceAreaWithActiveSolarCellsDefaulted());
  result = photovoltaicPerformanceSimple.setFractionOfSurfaceAreaWithActiveSolarCells(value);
  EXPECT_TRUE(result);
  EXPECT_TRUE(photovoltaicPerformanceSimple.fractionOfSurfaceAreaWithActiveSolarCells());
  EXPECT_EQ(photovoltaicPerformanceSimple.fractionOfSurfaceAreaWithActiveSolarCells(), value);
  photovoltaicPerformanceSimple.resetFractionOfSurfaceAreaWithActiveSolarCells();
  EXPECT_EQ(photovoltaicPerformanceSimple.fractionOfSurfaceAreaWithActiveSolarCells(), 0.89);

  result = photovoltaicPerformanceSimple.setFixedEfficiency(value);
  EXPECT_TRUE(result);
  EXPECT_EQ("Fixed", photovoltaicPerformanceSimple.conversionEfficiencyInputMode());
  EXPECT_TRUE(photovoltaicPerformanceSimple.fixedEfficiency());
  EXPECT_EQ(photovoltaicPerformanceSimple.fixedEfficiency().get(), value);
  photovoltaicPerformanceSimple.resetFixedEfficiency();
  EXPECT_EQ("Fixed", photovoltaicPerformanceSimple.conversionEfficiencyInputMode());
  EXPECT_TRUE(photovoltaicPerformanceSimple.fixedEfficiency());
  EXPECT_EQ(0.12, photovoltaicPerformanceSimple.fixedEfficiency().get());

  EXPECT_FALSE(photovoltaicPerformanceSimple.efficiencySchedule());
  ScheduleCompact scheduleCompact(model);
  result = photovoltaicPerformanceSimple.setEfficiencySchedule(scheduleCompact);
  EXPECT_TRUE(result);
  EXPECT_EQ("Scheduled", photovoltaicPerformanceSimple.conversionEfficiencyInputMode());
  EXPECT_TRUE(photovoltaicPerformanceSimple.efficiencySchedule());
  photovoltaicPerformanceSimple.resetEfficiencySchedule();
  EXPECT_FALSE(photovoltaicPerformanceSimple.efficiencySchedule());
  EXPECT_EQ("Fixed", photovoltaicPerformanceSimple.conversionEfficiencyInputMode());
  EXPECT_TRUE(photovoltaicPerformanceSimple.fixedEfficiency());
  EXPECT_EQ(0.12, photovoltaicPerformanceSimple.fixedEfficiency().get());

}

