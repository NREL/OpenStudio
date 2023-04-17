/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/FuelFactors.hpp"
#include "../../model/FuelFactors_Impl.hpp"
#include "../../model/ScheduleConstant.hpp"

#include "../../model/OutputEnvironmentalImpactFactors.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/FuelFactors_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_FuelFactors) {

  ForwardTranslator ft;

  Model m;

  FuelFactors fuelFactors(m);

  auto makeSchedule = [&m](auto name) -> Schedule {
    ScheduleConstant sch(m);
    sch.setValue(1.0);
    sch.setName(name);
    return std::move(sch);
  };

  EXPECT_TRUE(fuelFactors.setExistingFuelResourceName("Electricity"));
  EXPECT_TRUE(fuelFactors.setSourceEnergyFactor(2.253));
  auto sch1 = makeSchedule("Schedule 1");
  EXPECT_TRUE(fuelFactors.setSourceEnergySchedule(sch1));
  EXPECT_TRUE(fuelFactors.setCO2EmissionFactor(168.33317));
  auto sch2 = makeSchedule("Schedule 2");
  EXPECT_TRUE(fuelFactors.setCO2EmissionFactorSchedule(sch2));
  EXPECT_TRUE(fuelFactors.setCOEmissionFactor(0.0420616));
  auto sch3 = makeSchedule("Schedule 3");
  EXPECT_TRUE(fuelFactors.setCOEmissionFactorSchedule(sch3));
  EXPECT_TRUE(fuelFactors.setCH4EmissionFactor(0.00139858));
  auto sch4 = makeSchedule("Schedule 4");
  EXPECT_TRUE(fuelFactors.setCH4EmissionFactorSchedule(sch4));
  EXPECT_TRUE(fuelFactors.setNOxEmissionFactor(0.410753));
  auto sch5 = makeSchedule("Schedule 5");
  EXPECT_TRUE(fuelFactors.setNOxEmissionFactorSchedule(sch5));
  EXPECT_TRUE(fuelFactors.setN2OEmissionFactor(0.00241916));
  auto sch6 = makeSchedule("Schedule 6");
  EXPECT_TRUE(fuelFactors.setN2OEmissionFactorSchedule(sch6));
  EXPECT_TRUE(fuelFactors.setSO2EmissionFactor(0.865731));
  auto sch7 = makeSchedule("Schedule 7");
  EXPECT_TRUE(fuelFactors.setSO2EmissionFactorSchedule(sch7));
  EXPECT_TRUE(fuelFactors.setPMEmissionFactor(0.0295827));
  auto sch8 = makeSchedule("Schedule 8");
  EXPECT_TRUE(fuelFactors.setPMEmissionFactorSchedule(sch8));
  EXPECT_TRUE(fuelFactors.setPM10EmissionFactor(0.018045));
  auto sch9 = makeSchedule("Schedule 9");
  EXPECT_TRUE(fuelFactors.setPM10EmissionFactorSchedule(sch9));
  EXPECT_TRUE(fuelFactors.setPM25EmissionFactor(0.0115377));
  auto sch10 = makeSchedule("Schedule 10");
  EXPECT_TRUE(fuelFactors.setPM25EmissionFactorSchedule(sch10));
  EXPECT_TRUE(fuelFactors.setNH3EmissionFactor(0.00110837));
  auto sch11 = makeSchedule("Schedule 11");
  EXPECT_TRUE(fuelFactors.setNH3EmissionFactorSchedule(sch11));
  EXPECT_TRUE(fuelFactors.setNMVOCEmissionFactor(0.00372332));
  auto sch12 = makeSchedule("Schedule 12");
  EXPECT_TRUE(fuelFactors.setNMVOCEmissionFactorSchedule(sch12));
  EXPECT_TRUE(fuelFactors.setHgEmissionFactor(3.36414e-06));
  auto sch13 = makeSchedule("Schedule 13");
  EXPECT_TRUE(fuelFactors.setHgEmissionFactorSchedule(sch13));
  EXPECT_TRUE(fuelFactors.setPbEmissionFactor(1.0));
  auto sch14 = makeSchedule("Schedule 14");
  EXPECT_TRUE(fuelFactors.setPbEmissionFactorSchedule(sch14));
  EXPECT_TRUE(fuelFactors.setWaterEmissionFactor(2.10074));
  auto sch15 = makeSchedule("Schedule 15");
  EXPECT_TRUE(fuelFactors.setWaterEmissionFactorSchedule(sch15));
  EXPECT_TRUE(fuelFactors.setNuclearHighLevelEmissionFactor(2.0));
  auto sch16 = makeSchedule("Schedule 16");
  EXPECT_TRUE(fuelFactors.setNuclearHighLevelEmissionFactorSchedule(sch16));
  EXPECT_TRUE(fuelFactors.setNuclearLowLevelEmissionFactor(3.0));
  auto sch17 = makeSchedule("Schedule 17");
  EXPECT_TRUE(fuelFactors.setNuclearLowLevelEmissionFactorSchedule(sch17));

  // Without the OutputEnvironmentalImpactFactors: not translated
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::FuelFactors);
    EXPECT_EQ(0u, idfObjs.size());
  }

  // Create one
  OutputEnvironmentalImpactFactors o(m);

  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::FuelFactors);
    ASSERT_EQ(1u, idfObjs.size());
    auto idfObject = idfObjs[0];

    EXPECT_EQ("Electricity", idfObject.getString(FuelFactorsFields::ExistingFuelResourceName).get());
    EXPECT_EQ(2.253, idfObject.getDouble(FuelFactorsFields::SourceEnergyFactor).get());
    EXPECT_EQ("Schedule 1", idfObject.getString(FuelFactorsFields::SourceEnergyScheduleName).get());
    EXPECT_EQ(168.33317, idfObject.getDouble(FuelFactorsFields::CO2EmissionFactor).get());
    EXPECT_EQ("Schedule 2", idfObject.getString(FuelFactorsFields::CO2EmissionFactorScheduleName).get());
    EXPECT_EQ(0.0420616, idfObject.getDouble(FuelFactorsFields::COEmissionFactor).get());
    EXPECT_EQ("Schedule 3", idfObject.getString(FuelFactorsFields::COEmissionFactorScheduleName).get());
    EXPECT_EQ(0.00139858, idfObject.getDouble(FuelFactorsFields::CH4EmissionFactor).get());
    EXPECT_EQ("Schedule 4", idfObject.getString(FuelFactorsFields::CH4EmissionFactorScheduleName).get());
    EXPECT_EQ(0.410753, idfObject.getDouble(FuelFactorsFields::NOxEmissionFactor).get());
    EXPECT_EQ("Schedule 5", idfObject.getString(FuelFactorsFields::NOxEmissionFactorScheduleName).get());
    EXPECT_EQ(0.00241916, idfObject.getDouble(FuelFactorsFields::N2OEmissionFactor).get());
    EXPECT_EQ("Schedule 6", idfObject.getString(FuelFactorsFields::N2OEmissionFactorScheduleName).get());
    EXPECT_EQ(0.865731, idfObject.getDouble(FuelFactorsFields::SO2EmissionFactor).get());
    EXPECT_EQ("Schedule 7", idfObject.getString(FuelFactorsFields::SO2EmissionFactorScheduleName).get());
    EXPECT_EQ(0.0295827, idfObject.getDouble(FuelFactorsFields::PMEmissionFactor).get());
    EXPECT_EQ("Schedule 8", idfObject.getString(FuelFactorsFields::PMEmissionFactorScheduleName).get());
    EXPECT_EQ(0.018045, idfObject.getDouble(FuelFactorsFields::PM10EmissionFactor).get());
    EXPECT_EQ("Schedule 9", idfObject.getString(FuelFactorsFields::PM10EmissionFactorScheduleName).get());
    EXPECT_EQ(0.0115377, idfObject.getDouble(FuelFactorsFields::PM25EmissionFactor).get());
    EXPECT_EQ("Schedule 10", idfObject.getString(FuelFactorsFields::PM25EmissionFactorScheduleName).get());
    EXPECT_EQ(0.00110837, idfObject.getDouble(FuelFactorsFields::NH3EmissionFactor).get());
    EXPECT_EQ("Schedule 11", idfObject.getString(FuelFactorsFields::NH3EmissionFactorScheduleName).get());
    EXPECT_EQ(0.00372332, idfObject.getDouble(FuelFactorsFields::NMVOCEmissionFactor).get());
    EXPECT_EQ("Schedule 12", idfObject.getString(FuelFactorsFields::NMVOCEmissionFactorScheduleName).get());
    EXPECT_EQ(3.36414e-06, idfObject.getDouble(FuelFactorsFields::HgEmissionFactor).get());
    EXPECT_EQ("Schedule 13", idfObject.getString(FuelFactorsFields::HgEmissionFactorScheduleName).get());
    EXPECT_EQ(1.0, idfObject.getDouble(FuelFactorsFields::PbEmissionFactor).get());
    EXPECT_EQ("Schedule 14", idfObject.getString(FuelFactorsFields::PbEmissionFactorScheduleName).get());
    EXPECT_EQ(2.10074, idfObject.getDouble(FuelFactorsFields::WaterEmissionFactor).get());
    EXPECT_EQ("Schedule 15", idfObject.getString(FuelFactorsFields::WaterEmissionFactorScheduleName).get());
    EXPECT_EQ(2.0, idfObject.getDouble(FuelFactorsFields::NuclearHighLevelEmissionFactor).get());
    EXPECT_EQ("Schedule 16", idfObject.getString(FuelFactorsFields::NuclearHighLevelEmissionFactorScheduleName).get());
    EXPECT_EQ(3.0, idfObject.getDouble(FuelFactorsFields::NuclearLowLevelEmissionFactor).get());
    EXPECT_EQ("Schedule 17", idfObject.getString(FuelFactorsFields::NuclearLowLevelEmissionFactorScheduleName).get());
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_FuelFactors) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  // Not there, Model shouldn't have it either
  {
    Model m = rt.translateWorkspace(w);
    EXPECT_FALSE(m.getOptionalUniqueModelObject<FuelFactors>());
  }

  OptionalWorkspaceObject _i_fuelFactors = w.addObject(IdfObject(IddObjectType::FuelFactors));
  ASSERT_TRUE(_i_fuelFactors);

  for (int i = 1; i <= 17; ++i) {
    OptionalWorkspaceObject _i_sch = w.addObject(IdfObject(IddObjectType::Schedule_Constant));
    ASSERT_TRUE(_i_sch);
    _i_sch->setName("Schedule " + std::to_string(i));
  }

  EXPECT_TRUE(_i_fuelFactors->setString(FuelFactorsFields::ExistingFuelResourceName, "Electricity"));
  EXPECT_TRUE(_i_fuelFactors->setDouble(FuelFactorsFields::SourceEnergyFactor, 2.253));
  EXPECT_TRUE(_i_fuelFactors->setString(FuelFactorsFields::SourceEnergyScheduleName, "Schedule 1"));
  EXPECT_TRUE(_i_fuelFactors->setDouble(FuelFactorsFields::CO2EmissionFactor, 168.33317));
  EXPECT_TRUE(_i_fuelFactors->setString(FuelFactorsFields::CO2EmissionFactorScheduleName, "Schedule 2"));
  EXPECT_TRUE(_i_fuelFactors->setDouble(FuelFactorsFields::COEmissionFactor, 0.0420616));
  EXPECT_TRUE(_i_fuelFactors->setString(FuelFactorsFields::COEmissionFactorScheduleName, "Schedule 3"));
  EXPECT_TRUE(_i_fuelFactors->setDouble(FuelFactorsFields::CH4EmissionFactor, 0.00139858));
  EXPECT_TRUE(_i_fuelFactors->setString(FuelFactorsFields::CH4EmissionFactorScheduleName, "Schedule 4"));
  EXPECT_TRUE(_i_fuelFactors->setDouble(FuelFactorsFields::NOxEmissionFactor, 0.410753));
  EXPECT_TRUE(_i_fuelFactors->setString(FuelFactorsFields::NOxEmissionFactorScheduleName, "Schedule 5"));
  EXPECT_TRUE(_i_fuelFactors->setDouble(FuelFactorsFields::N2OEmissionFactor, 0.00241916));
  EXPECT_TRUE(_i_fuelFactors->setString(FuelFactorsFields::N2OEmissionFactorScheduleName, "Schedule 6"));
  EXPECT_TRUE(_i_fuelFactors->setDouble(FuelFactorsFields::SO2EmissionFactor, 0.865731));
  EXPECT_TRUE(_i_fuelFactors->setString(FuelFactorsFields::SO2EmissionFactorScheduleName, "Schedule 7"));
  EXPECT_TRUE(_i_fuelFactors->setDouble(FuelFactorsFields::PMEmissionFactor, 0.0295827));
  EXPECT_TRUE(_i_fuelFactors->setString(FuelFactorsFields::PMEmissionFactorScheduleName, "Schedule 8"));
  EXPECT_TRUE(_i_fuelFactors->setDouble(FuelFactorsFields::PM10EmissionFactor, 0.018045));
  EXPECT_TRUE(_i_fuelFactors->setString(FuelFactorsFields::PM10EmissionFactorScheduleName, "Schedule 9"));
  EXPECT_TRUE(_i_fuelFactors->setDouble(FuelFactorsFields::PM25EmissionFactor, 0.0115377));
  EXPECT_TRUE(_i_fuelFactors->setString(FuelFactorsFields::PM25EmissionFactorScheduleName, "Schedule 10"));
  EXPECT_TRUE(_i_fuelFactors->setDouble(FuelFactorsFields::NH3EmissionFactor, 0.00110837));
  EXPECT_TRUE(_i_fuelFactors->setString(FuelFactorsFields::NH3EmissionFactorScheduleName, "Schedule 11"));
  EXPECT_TRUE(_i_fuelFactors->setDouble(FuelFactorsFields::NMVOCEmissionFactor, 0.00372332));
  EXPECT_TRUE(_i_fuelFactors->setString(FuelFactorsFields::NMVOCEmissionFactorScheduleName, "Schedule 12"));
  EXPECT_TRUE(_i_fuelFactors->setDouble(FuelFactorsFields::HgEmissionFactor, 3.36414e-06));
  EXPECT_TRUE(_i_fuelFactors->setString(FuelFactorsFields::HgEmissionFactorScheduleName, "Schedule 13"));
  EXPECT_TRUE(_i_fuelFactors->setDouble(FuelFactorsFields::PbEmissionFactor, 1.0));
  EXPECT_TRUE(_i_fuelFactors->setString(FuelFactorsFields::PbEmissionFactorScheduleName, "Schedule 14"));
  EXPECT_TRUE(_i_fuelFactors->setDouble(FuelFactorsFields::WaterEmissionFactor, 2.10074));
  EXPECT_TRUE(_i_fuelFactors->setString(FuelFactorsFields::WaterEmissionFactorScheduleName, "Schedule 15"));
  EXPECT_TRUE(_i_fuelFactors->setDouble(FuelFactorsFields::NuclearHighLevelEmissionFactor, 2.0));
  EXPECT_TRUE(_i_fuelFactors->setString(FuelFactorsFields::NuclearHighLevelEmissionFactorScheduleName, "Schedule 16"));
  EXPECT_TRUE(_i_fuelFactors->setDouble(FuelFactorsFields::NuclearLowLevelEmissionFactor, 3.0));
  EXPECT_TRUE(_i_fuelFactors->setString(FuelFactorsFields::NuclearLowLevelEmissionFactorScheduleName, "Schedule 17"));

  {
    Model m = rt.translateWorkspace(w);
    auto fuelFactorss = m.getConcreteModelObjects<openstudio::model::FuelFactors>();
    ASSERT_EQ(1, fuelFactorss.size());
    auto fuelFactors = fuelFactorss[0];

    EXPECT_EQ("Electricity", fuelFactors.existingFuelResourceName());
    EXPECT_EQ(2.253, fuelFactors.sourceEnergyFactor());
    ASSERT_TRUE(fuelFactors.sourceEnergySchedule());
    EXPECT_EQ("Schedule 1", fuelFactors.sourceEnergySchedule()->nameString());
    EXPECT_EQ(168.33317, fuelFactors.cO2EmissionFactor());
    ASSERT_TRUE(fuelFactors.cO2EmissionFactorSchedule());
    EXPECT_EQ("Schedule 2", fuelFactors.cO2EmissionFactorSchedule()->nameString());
    EXPECT_EQ(0.0420616, fuelFactors.cOEmissionFactor());
    ASSERT_TRUE(fuelFactors.cOEmissionFactorSchedule());
    EXPECT_EQ("Schedule 3", fuelFactors.cOEmissionFactorSchedule()->nameString());
    EXPECT_EQ(0.00139858, fuelFactors.cH4EmissionFactor());
    ASSERT_TRUE(fuelFactors.cH4EmissionFactorSchedule());
    EXPECT_EQ("Schedule 4", fuelFactors.cH4EmissionFactorSchedule()->nameString());
    EXPECT_EQ(0.410753, fuelFactors.nOxEmissionFactor());
    ASSERT_TRUE(fuelFactors.nOxEmissionFactorSchedule());
    EXPECT_EQ("Schedule 5", fuelFactors.nOxEmissionFactorSchedule()->nameString());
    EXPECT_EQ(0.00241916, fuelFactors.n2OEmissionFactor());
    ASSERT_TRUE(fuelFactors.n2OEmissionFactorSchedule());
    EXPECT_EQ("Schedule 6", fuelFactors.n2OEmissionFactorSchedule()->nameString());
    EXPECT_EQ(0.865731, fuelFactors.sO2EmissionFactor());
    ASSERT_TRUE(fuelFactors.sO2EmissionFactorSchedule());
    EXPECT_EQ("Schedule 7", fuelFactors.sO2EmissionFactorSchedule()->nameString());
    EXPECT_EQ(0.0295827, fuelFactors.pMEmissionFactor());
    ASSERT_TRUE(fuelFactors.pMEmissionFactorSchedule());
    EXPECT_EQ("Schedule 8", fuelFactors.pMEmissionFactorSchedule()->nameString());
    EXPECT_EQ(0.018045, fuelFactors.pM10EmissionFactor());
    ASSERT_TRUE(fuelFactors.pM10EmissionFactorSchedule());
    EXPECT_EQ("Schedule 9", fuelFactors.pM10EmissionFactorSchedule()->nameString());
    EXPECT_EQ(0.0115377, fuelFactors.pM25EmissionFactor());
    ASSERT_TRUE(fuelFactors.pM25EmissionFactorSchedule());
    EXPECT_EQ("Schedule 10", fuelFactors.pM25EmissionFactorSchedule()->nameString());
    EXPECT_EQ(0.00110837, fuelFactors.nH3EmissionFactor());
    ASSERT_TRUE(fuelFactors.nH3EmissionFactorSchedule());
    EXPECT_EQ("Schedule 11", fuelFactors.nH3EmissionFactorSchedule()->nameString());
    EXPECT_EQ(0.00372332, fuelFactors.nMVOCEmissionFactor());
    ASSERT_TRUE(fuelFactors.nMVOCEmissionFactorSchedule());
    EXPECT_EQ("Schedule 12", fuelFactors.nMVOCEmissionFactorSchedule()->nameString());
    EXPECT_EQ(3.36414e-06, fuelFactors.hgEmissionFactor());
    ASSERT_TRUE(fuelFactors.hgEmissionFactorSchedule());
    EXPECT_EQ("Schedule 13", fuelFactors.hgEmissionFactorSchedule()->nameString());
    EXPECT_EQ(1.0, fuelFactors.pbEmissionFactor());
    ASSERT_TRUE(fuelFactors.pbEmissionFactorSchedule());
    EXPECT_EQ("Schedule 14", fuelFactors.pbEmissionFactorSchedule()->nameString());
    EXPECT_EQ(2.10074, fuelFactors.waterEmissionFactor());
    ASSERT_TRUE(fuelFactors.waterEmissionFactorSchedule());
    EXPECT_EQ("Schedule 15", fuelFactors.waterEmissionFactorSchedule()->nameString());
    EXPECT_EQ(2.0, fuelFactors.nuclearHighLevelEmissionFactor());
    ASSERT_TRUE(fuelFactors.nuclearHighLevelEmissionFactorSchedule());
    EXPECT_EQ("Schedule 16", fuelFactors.nuclearHighLevelEmissionFactorSchedule()->nameString());
    EXPECT_EQ(3.0, fuelFactors.nuclearLowLevelEmissionFactor());
    ASSERT_TRUE(fuelFactors.nuclearLowLevelEmissionFactorSchedule());
    EXPECT_EQ("Schedule 17", fuelFactors.nuclearLowLevelEmissionFactorSchedule()->nameString());
  }
}
