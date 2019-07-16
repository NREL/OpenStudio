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

#include "../../model/FanVariableVolume.hpp"

#include "../../model/Model.hpp"
#include "../../model/Node.hpp"
#include "../../model/AirLoopHVAC.hpp"

#include <utilities/idd/Fan_VariableVolume_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_FanVariableVolume_EndUseSubCategory) {

  Model m;

  FanVariableVolume fan(m);

  std::string endUse("Special Fan");
  fan.setEndUseSubcategory(endUse);

  // Assign it to a loop
  AirLoopHVAC a(m);
  Node supplyOutletNode = a.supplyOutletNode();
  fan.addToNode(supplyOutletNode);

  // ForwardTranslate
  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idfObjs(w.getObjectsByType(IddObjectType::Fan_VariableVolume));
  ASSERT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_fan(idfObjs[0]);

  // End Use Subcategory
  boost::optional<std::string> _idfEndUse = idf_fan.getString(Fan_VariableVolumeFields::EndUseSubcategory);
  ASSERT_TRUE(_idfEndUse);
  EXPECT_EQ(endUse, _idfEndUse.get());

}

// The Forward Translator should not translate fan if not used (by an AirLoopHVAC, or a ZoneHVACComponent )
TEST_F(EnergyPlusFixture, ForwardTranslator_FanVariableVolume_NotUsed) {

  Model m;

  FanVariableVolume fan(m);

  // ForwardTranslate
  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idfObjs(w.getObjectsByType(IddObjectType::Fan_VariableVolume));
  EXPECT_EQ(0u, idfObjs.size());
}
