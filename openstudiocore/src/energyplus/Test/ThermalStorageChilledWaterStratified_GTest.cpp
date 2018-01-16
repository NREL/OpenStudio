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
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/ThermalStorageChilledWaterStratified.hpp"
#include "../../model/Model.hpp"
#include "../../model/PlantLoop.hpp"

#include <utilities/idd/ThermalStorage_ChilledWater_Stratified_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

/* Gtest for https://github.com/NREL/OpenStudio/issues/2373 */
TEST_F(EnergyPlusFixture, ForwardTranslator_ThermalStorageChilledWaterStratified_NumNodesAndTankTime) {
    Model model;

    ThermalStorageChilledWaterStratified t(model);

    ASSERT_TRUE(t.setTankRecoveryTime(4.25));
    ASSERT_TRUE(t.setNumberofNodes(7));

    PlantLoop p(model);
    ASSERT_TRUE(p.addSupplyBranchForComponent(t));

    ForwardTranslator forwardTranslator;
    Workspace workspace = forwardTranslator.translateModel(model);

    WorkspaceObjectVector idfObjs(workspace.getObjectsByType(IddObjectType::ThermalStorage_ChilledWater_Stratified));
    EXPECT_EQ(1u, idfObjs.size());
    WorkspaceObject idf_t(idfObjs[0]);

    EXPECT_DOUBLE_EQ(4.25, *idf_t.getDouble(ThermalStorage_ChilledWater_StratifiedFields::TankRecoveryTime));
    EXPECT_EQ(7, *idf_t.getInt(ThermalStorage_ChilledWater_StratifiedFields::NumberofNodes));

}
