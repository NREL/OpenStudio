/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/ShadowCalculation.hpp"
#include "../../model/ShadowCalculation_Impl.hpp"

#include "../../model/ThermalZone.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/ShadowCalculation_FieldEnums.hxx>
#include <utilities/idd/OS_ShadowCalculation_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ShadowCalculation) {

  ForwardTranslator ft;

  // Create a model
  Model m;

  // Get the unique object
  ShadowCalculation sc = m.getUniqueModelObject<ShadowCalculation>();

  // Check all cases where a single output request is True so we know we assigned the fields correctly
  auto boolToString = [](bool b) { return b ? "Yes" : "No";};

  for (int i = 0; i < 3; ++i) {
    bool status[] = {false, false, false};
    status[i] = true;
    bool outputExternal = status[0];
    bool disableSelfShadingWithin = status[1];
    bool disableSelfShadingFrom = status[2];
    EXPECT_TRUE(sc.setOutputExternalShadingCalculationResults(outputExternal));
    EXPECT_TRUE(sc.setDisableSelfShadingWithinShadingZoneGroups(disableSelfShadingWithin));
    EXPECT_TRUE(sc.setDisableSelfShadingFromShadingZoneGroupstoOtherZones(disableSelfShadingFrom));

    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::ShadowCalculation);
    ASSERT_EQ(1u, idfObjs.size());

    WorkspaceObject idf_sc(idfObjs[0]);

    EXPECT_EQ(boolToString(outputExternal), idf_sc.getString(ShadowCalculationFields::OutputExternalShadingCalculationResults).get());
    EXPECT_EQ(boolToString(disableSelfShadingWithin), idf_sc.getString(ShadowCalculationFields::DisableSelfShadingWithinShadingZoneGroups).get());
    EXPECT_EQ(boolToString(disableSelfShadingFrom), idf_sc.getString(ShadowCalculationFields::DisableSelfShadingFromShadingZoneGroupstoOtherZones).get());
  }

  // Test all fields
  EXPECT_TRUE(sc.setShadingCalculationMethod("PixelCounting"));
  EXPECT_TRUE(sc.setShadingCalculationUpdateFrequencyMethod("Timestep"));
  EXPECT_TRUE(sc.setShadingCalculationUpdateFrequency(1));
  EXPECT_TRUE(sc.setMaximumFiguresInShadowOverlapCalculations(200));
  EXPECT_TRUE(sc.setPolygonClippingAlgorithm("ConvexWeilerAtherton"));
  EXPECT_TRUE(sc.setPixelCountingResolution(499));
  EXPECT_TRUE(sc.setSkyDiffuseModelingAlgorithm("DetailedSkyDiffuseModeling"));
  EXPECT_TRUE(sc.setOutputExternalShadingCalculationResults(true));
  EXPECT_TRUE(sc.setDisableSelfShadingWithinShadingZoneGroups(false));
  EXPECT_TRUE(sc.setDisableSelfShadingFromShadingZoneGroupstoOtherZones(true));

  {
    Workspace w = ft.translateModel(m);
    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::ShadowCalculation);
    ASSERT_EQ(1u, idfObjs.size());

    WorkspaceObject idf_sc(idfObjs[0]);

    EXPECT_EQ("PixelCounting", idf_sc.getString(ShadowCalculationFields::ShadingCalculationMethod).get());
    EXPECT_EQ("Timestep", idf_sc.getString(ShadowCalculationFields::ShadingCalculationUpdateFrequencyMethod).get());
    EXPECT_EQ(1, idf_sc.getInt(ShadowCalculationFields::ShadingCalculationUpdateFrequency).get());
    EXPECT_EQ(200, idf_sc.getInt(ShadowCalculationFields::MaximumFiguresinShadowOverlapCalculations).get());
    EXPECT_EQ("ConvexWeilerAtherton", idf_sc.getString(ShadowCalculationFields::PolygonClippingAlgorithm).get());
    EXPECT_EQ(499, idf_sc.getInt(ShadowCalculationFields::PixelCountingResolution).get());
    EXPECT_EQ("DetailedSkyDiffuseModeling", idf_sc.getString(ShadowCalculationFields::SkyDiffuseModelingAlgorithm).get());
    EXPECT_EQ(boolToString(true), idf_sc.getString(ShadowCalculationFields::OutputExternalShadingCalculationResults).get());
    EXPECT_EQ(boolToString(false), idf_sc.getString(ShadowCalculationFields::DisableSelfShadingWithinShadingZoneGroups).get());
    EXPECT_EQ(boolToString(true), idf_sc.getString(ShadowCalculationFields::DisableSelfShadingFromShadingZoneGroupstoOtherZones).get());
    EXPECT_EQ(0u, idf_sc.numExtensibleGroups());
  }

  // TEST With defaults
  sc.resetMaximumFiguresInShadowOverlapCalculations();
  sc.resetPolygonClippingAlgorithm();
  sc.resetShadingCalculationUpdateFrequency();
  sc.resetShadingCalculationUpdateFrequencyMethod();
  sc.resetSkyDiffuseModelingAlgorithm();
  {
    Workspace w = ft.translateModel(m);
    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::ShadowCalculation);
    ASSERT_EQ(1u, idfObjs.size());

    WorkspaceObject idf_sc(idfObjs[0]);

    EXPECT_EQ("PixelCounting", idf_sc.getString(ShadowCalculationFields::ShadingCalculationMethod).get());
    EXPECT_EQ("Periodic", idf_sc.getString(ShadowCalculationFields::ShadingCalculationUpdateFrequencyMethod).get());
    EXPECT_EQ(20, idf_sc.getInt(ShadowCalculationFields::ShadingCalculationUpdateFrequency).get());
    EXPECT_EQ(15000, idf_sc.getInt(ShadowCalculationFields::MaximumFiguresinShadowOverlapCalculations).get());
    EXPECT_EQ("SutherlandHodgman", idf_sc.getString(ShadowCalculationFields::PolygonClippingAlgorithm).get());
    EXPECT_EQ(499, idf_sc.getInt(ShadowCalculationFields::PixelCountingResolution).get());
    EXPECT_EQ("SimpleSkyDiffuseModeling", idf_sc.getString(ShadowCalculationFields::SkyDiffuseModelingAlgorithm).get());
    EXPECT_EQ(boolToString(true), idf_sc.getString(ShadowCalculationFields::OutputExternalShadingCalculationResults).get());
    EXPECT_EQ(boolToString(false), idf_sc.getString(ShadowCalculationFields::DisableSelfShadingWithinShadingZoneGroups).get());
    EXPECT_EQ(boolToString(true), idf_sc.getString(ShadowCalculationFields::DisableSelfShadingFromShadingZoneGroupstoOtherZones).get());
    EXPECT_EQ(0u, idf_sc.numExtensibleGroups());
  }

  // TEST for the Shading Zone Groups
  ThermalZone z1(m);
  ThermalZone z2(m);
  ThermalZone z3(m);
  EXPECT_TRUE(sc.addShadingZoneGroup({z1, z2}));
  EXPECT_TRUE(sc.addShadingZoneGroup({z3}));
  EXPECT_EQ(2u, sc.numberofShadingZoneGroups());
  {
    Workspace w = ft.translateModel(m);
    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::ShadowCalculation);
    ASSERT_EQ(1u, idfObjs.size());

    WorkspaceObject idf_sc(idfObjs[0]);

    EXPECT_EQ("PixelCounting", idf_sc.getString(ShadowCalculationFields::ShadingCalculationMethod).get());
    EXPECT_EQ("Periodic", idf_sc.getString(ShadowCalculationFields::ShadingCalculationUpdateFrequencyMethod).get());
    EXPECT_EQ(20, idf_sc.getInt(ShadowCalculationFields::ShadingCalculationUpdateFrequency).get());
    EXPECT_EQ(15000, idf_sc.getInt(ShadowCalculationFields::MaximumFiguresinShadowOverlapCalculations).get());
    EXPECT_EQ("SutherlandHodgman", idf_sc.getString(ShadowCalculationFields::PolygonClippingAlgorithm).get());
    EXPECT_EQ(499, idf_sc.getInt(ShadowCalculationFields::PixelCountingResolution).get());
    EXPECT_EQ("SimpleSkyDiffuseModeling", idf_sc.getString(ShadowCalculationFields::SkyDiffuseModelingAlgorithm).get());
    EXPECT_EQ(boolToString(true), idf_sc.getString(ShadowCalculationFields::OutputExternalShadingCalculationResults).get());
    EXPECT_EQ(boolToString(false), idf_sc.getString(ShadowCalculationFields::DisableSelfShadingWithinShadingZoneGroups).get());
    EXPECT_EQ(boolToString(true), idf_sc.getString(ShadowCalculationFields::DisableSelfShadingFromShadingZoneGroupstoOtherZones).get());
    ASSERT_EQ(2u, idf_sc.numExtensibleGroups());
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_ShadowCalculation) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::None, IddFileType::EnergyPlus);

  // Not there, Model shouldn't have it either
  Model m = rt.translateWorkspace(w);
  EXPECT_FALSE(m.getOptionalUniqueModelObject<ShadowCalculation>());

  OptionalWorkspaceObject _i_sc = w.addObject(IdfObject(IddObjectType::ShadowCalculation));
  ASSERT_TRUE(_i_sc);

  auto boolToString = [](bool b) { return b ? "Yes" : "No";};

  // RT with all defaults
  {
    Model m = rt.translateWorkspace(w);

    // Get the unique object
    ShadowCalculation sc = m.getUniqueModelObject<ShadowCalculation>();

    EXPECT_TRUE(sc.isShadingCalculationUpdateFrequencyMethodDefaulted());
    EXPECT_TRUE(sc.isShadingCalculationUpdateFrequencyDefaulted());
    EXPECT_TRUE(sc.isMaximumFiguresInShadowOverlapCalculationsDefaulted());

    // Doesn't have a isXXXDefaulted...
    EXPECT_EQ("ConvexWeilerAtherton", sc.polygonClippingAlgorithm());
    EXPECT_TRUE(sc.isEmpty(OS_ShadowCalculationFields::PolygonClippingAlgorithm));
    EXPECT_EQ("SimpleSkyDiffuseModeling", sc.skyDiffuseModelingAlgorithm());
    EXPECT_TRUE(sc.isEmpty(OS_ShadowCalculationFields::SkyDiffuseModelingAlgorithm));

    // New fields: Harcoded via model Ctor
    EXPECT_EQ("PolygonClipping,", sc.shadingCalculationMethod());
    EXPECT_EQ(512, sc.pixelCountingResolution());
    EXPECT_EQ(false, sc.outputExternalShadingCalculationResults());
    EXPECT_EQ(false, sc.disableSelfShadingWithinShadingZoneGroups());
    EXPECT_EQ(false, sc.disableSelfShadingFromShadingZoneGroupstoOtherZones());
    EXPECT_EQ(0u, sc.numberofShadingZoneGroups());

  }


  // Test the boolean fiels for correct mapping (easy to mess up)
  for (int i = 0; i < 3; ++i) {
    bool status[] = {false, false, false};
    status[i] = true;
    bool outputExternal = status[0];
    bool disableSelfShadingWithin = status[1];
    bool disableSelfShadingFrom = status[2];

    EXPECT_TRUE(_i_sc->setString(ShadowCalculationFields::OutputExternalShadingCalculationResults, boolToString(outputExternal)));
    EXPECT_TRUE(_i_sc->setString(ShadowCalculationFields::DisableSelfShadingWithinShadingZoneGroups, boolToString(disableSelfShadingWithin)));
    EXPECT_TRUE(_i_sc->setString(ShadowCalculationFields::DisableSelfShadingFromShadingZoneGroupstoOtherZones, boolToString(disableSelfShadingFrom)));

    Model m = rt.translateWorkspace(w);

    // Get the unique object
    ShadowCalculation sc = m.getUniqueModelObject<ShadowCalculation>();
    EXPECT_EQ(outputExternal, sc.outputExternalShadingCalculationResults());
    EXPECT_EQ(disableSelfShadingWithin, sc.disableSelfShadingWithinShadingZoneGroups());
    EXPECT_EQ(disableSelfShadingFrom, sc.disableSelfShadingFromShadingZoneGroupstoOtherZones());
  }

  EXPECT_TRUE(_i_sc->setString(ShadowCalculationFields::ShadingCalculationMethod, "PixelCounting"));
  EXPECT_TRUE(_i_sc->setString(ShadowCalculationFields::ShadingCalculationUpdateFrequencyMethod, "Timestep"));
  EXPECT_TRUE(_i_sc->setInt(ShadowCalculationFields::ShadingCalculationUpdateFrequency, 1));
  EXPECT_TRUE(_i_sc->setInt(ShadowCalculationFields::MaximumFiguresinShadowOverlapCalculations, 200));
  EXPECT_TRUE(_i_sc->setString(ShadowCalculationFields::PolygonClippingAlgorithm, "ConvexWeilerAtherton"));
  EXPECT_TRUE(_i_sc->setInt(ShadowCalculationFields::PixelCountingResolution, 499));
  EXPECT_TRUE(_i_sc->setString(ShadowCalculationFields::SkyDiffuseModelingAlgorithm, "DetailedSkyDiffuseModeling"));
  EXPECT_TRUE(_i_sc->setString(ShadowCalculationFields::OutputExternalShadingCalculationResults, boolToString(true)));
  EXPECT_TRUE(_i_sc->setString(ShadowCalculationFields::DisableSelfShadingWithinShadingZoneGroups, boolToString(false)));
  EXPECT_TRUE(_i_sc->setString(ShadowCalculationFields::DisableSelfShadingFromShadingZoneGroupstoOtherZones, boolToString(false)));

  {
    Model m = rt.translateWorkspace(w);

    // Get the unique object
    ShadowCalculation sc = m.getUniqueModelObject<ShadowCalculation>();

    EXPECT_EQ("PixelCounting", sc.shadingCalculationMethod());
    EXPECT_EQ("Timestep", sc.shadingCalculationUpdateFrequencyMethod());
    EXPECT_EQ(1, sc.shadingCalculationUpdateFrequency());
    EXPECT_EQ(200, sc.maximumFiguresInShadowOverlapCalculations());
    EXPECT_EQ("ConvexWeilerAtherton", sc.polygonClippingAlgorithm());
    EXPECT_EQ(499, sc.pixelCountingResolution());
    EXPECT_EQ("DetailedSkyDiffuseModeling", sc.skyDiffuseModelingAlgorithm());
    EXPECT_EQ(true, sc.outputExternalShadingCalculationResults());
    EXPECT_EQ(false, sc.disableSelfShadingWithinShadingZoneGroups());
    EXPECT_EQ(true, sc.disableSelfShadingFromShadingZoneGroupstoOtherZones());
    EXPECT_EQ(0u, sc.numberofShadingZoneGroups());
  }

}
