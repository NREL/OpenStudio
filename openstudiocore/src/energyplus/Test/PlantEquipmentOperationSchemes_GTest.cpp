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

#include "../../model/Model.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/Node.hpp"


#include "../../model/HeatExchangerFluidToFluid.hpp"
#include "../../model/WaterHeaterMixed.hpp"
#include "../../model/WaterHeaterStratified.hpp"


#include "../../model/BoilerHotWater.hpp"
#include "../../model/ChillerElectricEIR.hpp"

#include <utilities/idd/PlantLoop_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperationSchemes_FieldEnums.hxx>

#include <utilities/idd/PlantEquipmentOperation_HeatingLoad_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperation_CoolingLoad_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperation_Uncontrolled_FieldEnums.hxx>

#include <utilities/idd/PlantEquipmentList_FieldEnums.hxx>


#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"


#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

/*
 * Tests the "smart" logic for guessing the operation type of a Heat Exchanger
 */
TEST_F(EnergyPlusFixture, ForwardTranslator_PlantEquipmentOperationSchemes_HeatExchanger_UncontrolledOn_Both) {

    boost::optional<WorkspaceObject> _wo;
    ForwardTranslator forwardTranslator;

    Model m;

    PlantLoop use_loop(m);
    use_loop.setName("Use Loop");

    BoilerHotWater b1(m);
    HeatExchangerFluidToFluid hx(m);

    use_loop.addSupplyBranchForComponent(b1);
    use_loop.addSupplyBranchForComponent(hx);

    // Create  a Source Loop with both cooling and heating component on the Supply Side (should be categorized as ComponentType::BOTH
    // and the HX on the demand side
    PlantLoop source_loop(m);
    source_loop.setName("Source Loop");
    BoilerHotWater b2(m);
    ChillerElectricEIR ch(m);
    source_loop.addSupplyBranchForComponent(b2);
    source_loop.addSupplyBranchForComponent(ch);
    source_loop.addDemandBranchForComponent(hx);

    ASSERT_TRUE(hx.setControlType("UncontrolledOn"));

    Workspace w = forwardTranslator.translateModel(m);

    // Get the Use Loop, and find its plant operation scheme
    _wo = w.getObjectByTypeAndName(IddObjectType::PlantLoop, use_loop.name().get());
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_use_loop = _wo.get();
    WorkspaceObject idf_plant_op = idf_use_loop.getTarget(PlantLoopFields::PlantEquipmentOperationSchemeName).get();

    // Should have created a Heating Load one and an Uncontrolled one
    ASSERT_EQ(2u, idf_plant_op.extensibleGroups().size());
    WorkspaceExtensibleGroup w_eg = idf_plant_op.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
    ASSERT_EQ("PlantEquipmentOperation:HeatingLoad", w_eg.getString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType).get());

    w_eg = idf_plant_op.extensibleGroups()[1].cast<WorkspaceExtensibleGroup>();
    ASSERT_EQ("PlantEquipmentOperation:Uncontrolled", w_eg.getString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType).get());

    // Get the Operation Scheme
    _wo = w_eg.getTarget(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName);
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_op_scheme = _wo.get();

    // Get the Plant Equipment List of this Uncontrolled scheme
    _wo = idf_op_scheme.getTarget(PlantEquipmentOperation_UncontrolledFields::EquipmentListName);
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_peq_list = _wo.get();

    // Should have one equipment on it
    ASSERT_EQ(1u, idf_peq_list.extensibleGroups().size());
    IdfExtensibleGroup idf_eg(idf_peq_list.extensibleGroups()[0]);
    // Which should be the HX
    ASSERT_EQ(hx.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());

}

TEST_F(EnergyPlusFixture, ForwardTranslator_PlantEquipmentOperationSchemes_HeatExchanger_UncontrolledOn_Heating) {

    boost::optional<WorkspaceObject> _wo;
    ForwardTranslator forwardTranslator;

    Model m;

    PlantLoop use_loop(m);
    use_loop.setName("Use Loop");

    BoilerHotWater b1(m);
    HeatExchangerFluidToFluid hx(m);

    use_loop.addSupplyBranchForComponent(b1);
    use_loop.addSupplyBranchForComponent(hx);

    // Create  a Source Loop with only heating component on the Supply Side (should be categorized as ComponentType::HEATING
    // and the HX UncontrolledOn on the demand side (should inherit the source loop componentType)
    PlantLoop source_loop(m);
    source_loop.setName("Source Loop");
    BoilerHotWater b2(m);
    source_loop.addSupplyBranchForComponent(b2);
    source_loop.addDemandBranchForComponent(hx);

    ASSERT_TRUE(hx.setControlType("UncontrolledOn"));

    Workspace w = forwardTranslator.translateModel(m);


    // Get the Use Loop, and find its plant operation scheme
    _wo = w.getObjectByTypeAndName(IddObjectType::PlantLoop, use_loop.name().get());
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_use_loop = _wo.get();
    WorkspaceObject idf_plant_op = idf_use_loop.getTarget(PlantLoopFields::PlantEquipmentOperationSchemeName).get();

    // Should have created a Heating Load one only
    ASSERT_EQ(1u, idf_plant_op.extensibleGroups().size());
    WorkspaceExtensibleGroup w_eg = idf_plant_op.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
    ASSERT_EQ("PlantEquipmentOperation:HeatingLoad", w_eg.getString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType).get());

    // Get the Operation Scheme
    _wo = w_eg.getTarget(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName);
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_op_scheme = _wo.get();

    // Get the Plant Equipment List of this HeatingLoad scheme
    // There should only be one Load Range
    ASSERT_EQ(1u, idf_op_scheme.extensibleGroups().size());
    // Load range 1
    w_eg = idf_op_scheme.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
    _wo = w_eg.getTarget(PlantEquipmentOperation_HeatingLoadExtensibleFields::RangeEquipmentListName);
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_peq_list = _wo.get();

    // Should have two equipment on it: boiler, then HX
    ASSERT_EQ(2u, idf_peq_list.extensibleGroups().size());
    IdfExtensibleGroup idf_eg(idf_peq_list.extensibleGroups()[0]);
    ASSERT_EQ(b1.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());

    idf_eg = idf_peq_list.extensibleGroups()[1];
    ASSERT_EQ(hx.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());


}


TEST_F(EnergyPlusFixture, ForwardTranslator_PlantEquipmentOperationSchemes_HeatExchanger_HeatingSetpointModulated) {

    boost::optional<WorkspaceObject> _wo;
    ForwardTranslator forwardTranslator;

    Model m;

    PlantLoop use_loop(m);
    use_loop.setName("Use Loop");

    BoilerHotWater b1(m);
    HeatExchangerFluidToFluid hx(m);

    use_loop.addSupplyBranchForComponent(b1);
    use_loop.addSupplyBranchForComponent(hx);

    // Create  a Source Loop with both cooling and heating component on the Supply Side (should be categorized as ComponentType::BOTH
    // and the HX on the demand side, but since the HX has a HeatingSetpointModulated Mode, it should be categorized as ComponentType:HEATING
    PlantLoop source_loop(m);
    source_loop.setName("Source Loop");
    BoilerHotWater b2(m);
    ChillerElectricEIR ch(m);
    source_loop.addSupplyBranchForComponent(b2);
    source_loop.addSupplyBranchForComponent(ch);
    source_loop.addDemandBranchForComponent(hx);

    ASSERT_TRUE(hx.setControlType("HeatingSetpointModulated"));

    Workspace w = forwardTranslator.translateModel(m);


    // Get the Use Loop, and find its plant operation scheme
    _wo = w.getObjectByTypeAndName(IddObjectType::PlantLoop, use_loop.name().get());
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_use_loop = _wo.get();
    WorkspaceObject idf_plant_op = idf_use_loop.getTarget(PlantLoopFields::PlantEquipmentOperationSchemeName).get();

    // Should have created a Heating Load one only
    ASSERT_EQ(1u, idf_plant_op.extensibleGroups().size());
    WorkspaceExtensibleGroup w_eg = idf_plant_op.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
    ASSERT_EQ("PlantEquipmentOperation:HeatingLoad", w_eg.getString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType).get());

    // Get the Operation Scheme
    _wo = w_eg.getTarget(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName);
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_op_scheme = _wo.get();

    // Get the Plant Equipment List of this HeatingLoad scheme
    // There should only be one Load Range
    ASSERT_EQ(1u, idf_op_scheme.extensibleGroups().size());
    // Load range 1
    w_eg = idf_op_scheme.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
    _wo = w_eg.getTarget(PlantEquipmentOperation_HeatingLoadExtensibleFields::RangeEquipmentListName);
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_peq_list = _wo.get();

    // Should have two equipment on it: boiler, then HX
    ASSERT_EQ(2u, idf_peq_list.extensibleGroups().size());
    IdfExtensibleGroup idf_eg(idf_peq_list.extensibleGroups()[0]);
    ASSERT_EQ(b1.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());

    idf_eg = idf_peq_list.extensibleGroups()[1];
    ASSERT_EQ(hx.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());


}

/*
 * WaterHeater:Mixed and WaterHeater:Strafied are Heating type if they have a capacity (autosized or not)
 */
TEST_F(EnergyPlusFixture, ForwardTranslator_PlantEquipmentOperationSchemes_WaterHeaters_Capacity_NoSource) {

    boost::optional<WorkspaceObject> _wo;

    Model m;

    PlantLoop use_loop(m);
    use_loop.setName("Use Loop");

    BoilerHotWater b1(m);
    WaterHeaterMixed wh(m);
    WaterHeaterStratified wh_s(m);

    use_loop.addSupplyBranchForComponent(b1);
    use_loop.addSupplyBranchForComponent(wh);
    use_loop.addSupplyBranchForComponent(wh_s);


    ASSERT_TRUE(wh.setHeaterMaximumCapacity(50000));
    wh_s.autosizeHeater1Capacity();
    ASSERT_TRUE(wh_s.setHeater2Capacity(0));

    ForwardTranslator forwardTranslator;
    Workspace w = forwardTranslator.translateModel(m);


    // Get the Use Loop, and find its plant operation scheme
    _wo = w.getObjectByTypeAndName(IddObjectType::PlantLoop, use_loop.name().get());
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_use_loop = _wo.get();
    WorkspaceObject idf_plant_op = idf_use_loop.getTarget(PlantLoopFields::PlantEquipmentOperationSchemeName).get();

    // Should have created a Heating Load one only
    ASSERT_EQ(1u, idf_plant_op.extensibleGroups().size());
    WorkspaceExtensibleGroup w_eg = idf_plant_op.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
    ASSERT_EQ("PlantEquipmentOperation:HeatingLoad", w_eg.getString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType).get());

    // Get the Operation Scheme
    _wo = w_eg.getTarget(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName);
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_op_scheme = _wo.get();

    // Get the Plant Equipment List of this HeatingLoad scheme
    // There should only be one Load Range
    ASSERT_EQ(1u, idf_op_scheme.extensibleGroups().size());
    // Load range 1
    w_eg = idf_op_scheme.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
    _wo = w_eg.getTarget(PlantEquipmentOperation_HeatingLoadExtensibleFields::RangeEquipmentListName);
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_peq_list = _wo.get();

    // Should have two equipment on it: boiler, then WaterHeater:Mixed, then WaterHeater:Stratified
    ASSERT_EQ(3u, idf_peq_list.extensibleGroups().size());
    IdfExtensibleGroup idf_eg(idf_peq_list.extensibleGroups()[0]);
    ASSERT_EQ(b1.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());

    idf_eg = idf_peq_list.extensibleGroups()[1];
    ASSERT_EQ(wh.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());

    idf_eg = idf_peq_list.extensibleGroups()[2];
    ASSERT_EQ(wh_s.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());
}

/*
* WaterHeater:Mixed and WaterHeater:Strafied used as storage tanks (no capacity and connected to source loop) inherit source loop type
*/
TEST_F(EnergyPlusFixture, ForwardTranslator_PlantEquipmentOperationSchemes_WaterHeaters_Tank_SourceHeating) {

    boost::optional<WorkspaceObject> _wo;

    Model m;

    PlantLoop use_loop(m);
    use_loop.setName("Use Loop");

    BoilerHotWater b1(m);

    WaterHeaterMixed wh(m);
    ASSERT_TRUE(wh.setHeaterMaximumCapacity(0));

    WaterHeaterStratified wh_s(m);
    ASSERT_TRUE(wh_s.setHeater1Capacity(0));
    ASSERT_TRUE(wh_s.setHeater1Capacity(0));

    use_loop.addSupplyBranchForComponent(b1);
    use_loop.addSupplyBranchForComponent(wh);
    use_loop.addSupplyBranchForComponent(wh_s);

    PlantLoop source_loop(m);
    source_loop.setName("Source Loop");
    BoilerHotWater b2(m);

    source_loop.addSupplyBranchForComponent(b2);
    source_loop.addDemandBranchForComponent(wh);


    ForwardTranslator forwardTranslator;
    Workspace w = forwardTranslator.translateModel(m);


    // Get the Use Loop, and find its plant operation scheme
    _wo = w.getObjectByTypeAndName(IddObjectType::PlantLoop, use_loop.name().get());
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_use_loop = _wo.get();
    WorkspaceObject idf_plant_op = idf_use_loop.getTarget(PlantLoopFields::PlantEquipmentOperationSchemeName).get();

    // Should have created a Heating Load one only
    ASSERT_EQ(1u, idf_plant_op.extensibleGroups().size());
    WorkspaceExtensibleGroup w_eg = idf_plant_op.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
    ASSERT_EQ("PlantEquipmentOperation:HeatingLoad", w_eg.getString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType).get());

    // Get the Operation Scheme
    _wo = w_eg.getTarget(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName);
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_op_scheme = _wo.get();

    // Get the Plant Equipment List of this HeatingLoad scheme
    // There should only be one Load Range
    ASSERT_EQ(1u, idf_op_scheme.extensibleGroups().size());
    // Load range 1
    w_eg = idf_op_scheme.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
    _wo = w_eg.getTarget(PlantEquipmentOperation_HeatingLoadExtensibleFields::RangeEquipmentListName);
    ASSERT_TRUE(_wo.is_initialized());
    WorkspaceObject idf_peq_list = _wo.get();

    // Should have two equipment on it: boiler, then WaterHeater:Mixed, then WaterHeater:Stratified
    ASSERT_EQ(3u, idf_peq_list.extensibleGroups().size());
    IdfExtensibleGroup idf_eg(idf_peq_list.extensibleGroups()[0]);
    ASSERT_EQ(b1.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());

    idf_eg = idf_peq_list.extensibleGroups()[1];
    ASSERT_EQ(wh.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());

    idf_eg = idf_peq_list.extensibleGroups()[2];
    ASSERT_EQ(wh_s.name().get(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());
}
