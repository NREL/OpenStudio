/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../../model/Model.hpp"
#include "../../model/CoilCoolingDX.hpp"
#include "../../model/CoilCoolingDX_Impl.hpp"
#include "../../model/CoilCoolingDXCurveFitPerformance.hpp"
#include "../../model/CoilCoolingDXCurveFitPerformance_Impl.hpp"
#include "../../model/CoilCoolingDXCurveFitOperatingMode.hpp"
#include "../../model/CoilCoolingDXCurveFitOperatingMode_Impl.hpp"

#include <utilities/idd/Coil_Cooling_DX_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_CurveFit_Performance_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilCoolingDX) {
  Model m;
  CoilCoolingDXCurveFitOperatingMode operatingMode(m);
  CoilCoolingDXCurveFitPerformance performance(m, operatingMode);
  CoilCoolingDX dx(m, performance);

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idfDXs(w.getObjectsByType(IddObjectType::Coil_Cooling_DX));
  ASSERT_EQ(1u, idfDXs.size());
  WorkspaceObject idfDX(idfDXs[0]);

  boost::optional<WorkspaceObject> woCoilCoolingDXCurveFitPerformance(idfDX.getTarget(Coil_Cooling_DXFields::PerformanceObjectName));
  EXPECT_TRUE(woCoilCoolingDXCurveFitPerformance);
  if (woCoilCoolingDXCurveFitPerformance) {
    EXPECT_EQ(woCoilCoolingDXCurveFitPerformance->iddObject().type(), IddObjectType::Coil_Cooling_DX_CurveFit_Performance);
  }

  WorkspaceObjectVector idfPerformances(w.getObjectsByType(IddObjectType::Coil_Cooling_DX_CurveFit_Performance));
  ASSERT_EQ(1u, idfPerformances.size());
  WorkspaceObject idfPerformance(idfPerformances[0]);

  EXPECT_EQ(woCoilCoolingDXCurveFitPerformance, idfPerformance);

  boost::optional<WorkspaceObject> woBaseOperatingMode(idfPerformance.getTarget(Coil_Cooling_DX_CurveFit_PerformanceFields::BaseOperatingMode));
  EXPECT_TRUE(woBaseOperatingMode);
  if (woBaseOperatingMode) {
    EXPECT_EQ(woBaseOperatingMode->iddObject().type(), IddObjectType::Coil_Cooling_DX_CurveFit_OperatingMode);
  }  
  boost::optional<WorkspaceObject> woAlternativeOperatingMode1(idfPerformance.getTarget(Coil_Cooling_DX_CurveFit_PerformanceFields::AlternativeOperatingMode1));
  EXPECT_FALSE(woAlternativeOperatingMode1);
  boost::optional<WorkspaceObject> woAlternativeOperatingMode2(idfPerformance.getTarget(Coil_Cooling_DX_CurveFit_PerformanceFields::AlternativeOperatingMode2));
  EXPECT_FALSE(woAlternativeOperatingMode2);

  WorkspaceObjectVector idfOperatingModes(w.getObjectsByType(IddObjectType::Coil_Cooling_DX_CurveFit_OperatingMode));
  ASSERT_EQ(1u, idfOperatingModes.size());
  WorkspaceObject idfOperatingMode(idfOperatingModes[0]);

  EXPECT_EQ(woBaseOperatingMode, idfOperatingMode);
}
