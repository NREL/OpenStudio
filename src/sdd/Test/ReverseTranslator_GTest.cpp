/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "SDDFixture.hpp"

#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Facility.hpp"
#include "../../model/Facility_Impl.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/SimulationControl.hpp"
#include "../../model/SimulationControl_Impl.hpp"
#include "../../model/RunPeriod.hpp"
#include "../../model/RunPeriod_Impl.hpp"
#include "../../model/YearDescription.hpp"
#include "../../model/YearDescription_Impl.hpp"
#include "../../model/RunPeriodControlSpecialDays.hpp"
#include "../../model/RunPeriodControlSpecialDays_Impl.hpp"

#include "../../utilities/core/Optional.hpp"

#include <resources.hxx>

#include <sstream>

TEST_F(SDDFixture, ReverseTranslator_load) {

  openstudio::path p = resourcesPath() / openstudio::toPath("simxml/OffLrg-ThermalEnergyStorage_StoragePriority-ap.xml");

  ASSERT_TRUE(openstudio::filesystem::exists(p)) << "Path '" << p << "' doesn't exist.";

  openstudio::sdd::ReverseTranslator rt;
  boost::optional<openstudio::model::Model> _m = rt.loadModel(p);

  EXPECT_TRUE(_m);
}
