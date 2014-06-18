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
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"

#include "../../model/ExteriorLights.hpp"
#include "../../model/ExteriorLights_Impl.hpp"
#include "../../model/ExteriorLightsDefinition.hpp"
#include "../../model/ExteriorLightsDefinition_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/ScheduleConstant_Impl.hpp"

#include <utilities/idd/Zone_FieldEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture,ExteriorLights)
{
  Model model;

  ExteriorLightsDefinition exteriorLightsDefinition(model);
  exteriorLightsDefinition.setDesignLevel(2303.0);

  ScheduleConstant schedule(model);
  schedule.setValue(1.0);

  ExteriorLights exteriorLights(exteriorLightsDefinition,schedule);
  exteriorLights.setControlOption("AstronomicalClock");

  ForwardTranslator trans;
  Workspace workspace = trans.translateModel(model);

}

