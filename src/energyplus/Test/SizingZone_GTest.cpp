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
#include "../../model/SizingZone.hpp"
#include "../../model/SizingZone_Impl.hpp"

#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/DesignDay.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/Sizing_Zone_FieldEnums.hxx>
#include <utilities/idd/DesignSpecification_ZoneAirDistribution_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_SizingZone) {

  ForwardTranslator ft;

  // Create a model
  Model m;

  // We need a ThermalZone, with at least one equipment (or useIdealAirLoads) AND a DesignDay, for the Sizing:Zone to be translated
  // The Zone itself needs at least one space to be translated
  ThermalZone z(m);
  Space s(m);
  s.setThermalZone(z);
  SizingZone sz = z.sizingZone();

  DesignDay d(m);

  // The Zone isn't "conditioned" yet, so not translated
  {
    Workspace w = ft.translateModel(m);

    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Zone).size());
    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Sizing_Zone);
    EXPECT_EQ(0u, idfObjs.size());
  }

  // Zone is conditioned, it's translated
  // Since all the fields that belong to DesignSpecification:ZoneAirDistribution (DSZAD) are not filled, the DSZAD isn't translated
  EXPECT_TRUE(z.setUseIdealAirLoads(true));
  EXPECT_TRUE(sz.isDesignZoneAirDistributionEffectivenessinCoolingModeDefaulted());
  EXPECT_TRUE(sz.isDesignZoneAirDistributionEffectivenessinHeatingModeDefaulted());
  EXPECT_TRUE(sz.isDesignZoneSecondaryRecirculationFractionDefaulted());
  EXPECT_TRUE(sz.isDesignMinimumZoneVentilationEfficiencyDefaulted());
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Sizing_Zone);
    ASSERT_EQ(1u, idfObjs.size());
    EXPECT_EQ("", idfObjs[0].getString(Sizing_ZoneFields::DesignSpecificationZoneAirDistributionObjectName).get());
    EXPECT_EQ(0u, w.getObjectsByType(IddObjectType::DesignSpecification_ZoneAirDistribution).size());
  }

  EXPECT_TRUE(sz.setDesignZoneAirDistributionEffectivenessinCoolingMode(0.8));
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Sizing_Zone);
    ASSERT_EQ(1u, idfObjs.size());
    WorkspaceObject idf_sz(idfObjs[0]);

    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::DesignSpecification_ZoneAirDistribution).size());
    boost::optional<WorkspaceObject> _i_dszad = idf_sz.getTarget(Sizing_ZoneFields::DesignSpecificationZoneAirDistributionObjectName);
    ASSERT_TRUE(_i_dszad);
    EXPECT_EQ(0.8, _i_dszad->getDouble(DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinCoolingMode).get());
    EXPECT_FALSE(_i_dszad->getDouble(DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinHeatingMode));
    EXPECT_FALSE(_i_dszad->getDouble(DesignSpecification_ZoneAirDistributionFields::ZoneSecondaryRecirculationFraction));
    EXPECT_FALSE(_i_dszad->getDouble(DesignSpecification_ZoneAirDistributionFields::MinimumZoneVentilationEfficiency));
  }

  EXPECT_TRUE(sz.setDesignZoneAirDistributionEffectivenessinHeatingMode(0.7));
  EXPECT_TRUE(sz.setDesignZoneSecondaryRecirculationFraction(0.6));
  EXPECT_TRUE(sz.setDesignMinimumZoneVentilationEfficiency(0.5));
  EXPECT_TRUE(sz.setDesignZoneAirDistributionEffectivenessinCoolingMode(0.8));
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Sizing_Zone);
    ASSERT_EQ(1u, idfObjs.size());
    WorkspaceObject idf_sz(idfObjs[0]);

    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::DesignSpecification_ZoneAirDistribution).size());
    boost::optional<WorkspaceObject> _i_dszad = idf_sz.getTarget(Sizing_ZoneFields::DesignSpecificationZoneAirDistributionObjectName);
    ASSERT_TRUE(_i_dszad);
    EXPECT_EQ(0.8, _i_dszad->getDouble(DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinCoolingMode).get());
    EXPECT_EQ(0.7, _i_dszad->getDouble(DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinHeatingMode));
    EXPECT_EQ(0.6, _i_dszad->getDouble(DesignSpecification_ZoneAirDistributionFields::ZoneSecondaryRecirculationFraction));
    EXPECT_EQ(0.5, _i_dszad->getDouble(DesignSpecification_ZoneAirDistributionFields::MinimumZoneVentilationEfficiency));
  }
}
