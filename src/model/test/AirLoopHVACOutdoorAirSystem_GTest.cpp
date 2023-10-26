/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ModelFixture.hpp"

#include "../AirLoopHVACOutdoorAirSystem.hpp"
#include "../AirLoopHVACOutdoorAirSystem_Impl.hpp"

#include "../AirLoopHVAC.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../CoilHeatingElectric_Impl.hpp"
#include "../CoilCoolingDXVariableSpeed.hpp"
#include "../ControllerOutdoorAir.hpp"
#include "../ControllerOutdoorAir_Impl.hpp"
#include "../HeatExchangerAirToAirSensibleAndLatent.hpp"
#include "../HeatExchangerAirToAirSensibleAndLatent_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>

#include <algorithm>
#include <iterator>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirLoopHVACOutdoorAirSystem_GettersSetters) {
  Model m;

  ControllerOutdoorAir controller(m);
  AirLoopHVACOutdoorAirSystem oaSystem(m, controller);
  oaSystem.setName("My AirLoopHVACOutdoorAirSystem");
  EXPECT_EQ(controller, oaSystem.getControllerOutdoorAir());

  ControllerOutdoorAir controller2(m);
  EXPECT_TRUE(oaSystem.setControllerOutdoorAir(controller2));
  EXPECT_EQ(controller2, oaSystem.getControllerOutdoorAir());

  EXPECT_EQ(2, oaSystem.components().size());
  EXPECT_EQ(1, oaSystem.reliefComponents().size());
  ASSERT_TRUE(oaSystem.outboardReliefNode());
  auto outboardReliefNode = oaSystem.outboardReliefNode().get();
  outboardReliefNode.setName("Outboard Relief Node");

  ASSERT_TRUE(oaSystem.reliefAirModelObject());
  EXPECT_EQ(oaSystem.outboardReliefNode().get(), oaSystem.reliefAirModelObject().get());

  EXPECT_EQ(1, oaSystem.oaComponents().size());
  ASSERT_TRUE(oaSystem.outboardOANode());
  auto outboardOANode = oaSystem.outboardOANode().get();
  outboardOANode.setName("Outboard OA Node");

  ASSERT_TRUE(oaSystem.outdoorAirModelObject());
  EXPECT_EQ(oaSystem.outboardOANode().get(), oaSystem.outdoorAirModelObject().get());

  EXPECT_FALSE(oaSystem.returnAirModelObject());
  EXPECT_FALSE(oaSystem.mixedAirModelObject());

  EXPECT_FALSE(oaSystem.airLoopHVAC());

  AirLoopHVAC a(m);
  EXPECT_FALSE(a.mixedAirNode());
  EXPECT_FALSE(a.returnAirNode());
  EXPECT_FALSE(a.outdoorAirNode());
  EXPECT_FALSE(a.reliefAirNode());
  EXPECT_EQ(2, a.supplyComponents().size());
  EXPECT_EQ(0, a.supplyComponents(openstudio::IddObjectType::OS_AirLoopHVAC_OutdoorAirSystem).size());

  auto demandInletNode = a.demandInletNode();
  EXPECT_FALSE(oaSystem.addToNode(demandInletNode));
  EXPECT_FALSE(a.addBranchForHVACComponent(oaSystem));

  auto supplyInletNode = a.supplyInletNode();
  EXPECT_TRUE(oaSystem.addToNode(supplyInletNode));
  ASSERT_TRUE(oaSystem.airLoopHVAC());
  EXPECT_EQ(a, oaSystem.airLoopHVAC().get());

  EXPECT_EQ(3, a.supplyComponents().size());
  EXPECT_EQ(1, a.supplyComponents(openstudio::IddObjectType::OS_AirLoopHVAC_OutdoorAirSystem).size());
  ASSERT_TRUE(a.supplyComponent(oaSystem.handle()));

  ASSERT_TRUE(a.reliefAirNode());
  EXPECT_EQ(2, oaSystem.components().size());
  EXPECT_EQ(1, oaSystem.reliefComponents().size());
  ASSERT_TRUE(oaSystem.outboardReliefNode());
  ASSERT_TRUE(oaSystem.reliefAirModelObject());
  EXPECT_EQ(oaSystem.outboardReliefNode().get(), oaSystem.reliefAirModelObject().get());
  EXPECT_EQ(a.reliefAirNode().get(), oaSystem.reliefAirModelObject().get());

  ASSERT_TRUE(a.outdoorAirNode());
  EXPECT_EQ(1, oaSystem.oaComponents().size());
  ASSERT_TRUE(oaSystem.outboardOANode());
  ASSERT_TRUE(oaSystem.outdoorAirModelObject());
  EXPECT_EQ(oaSystem.outboardOANode().get(), oaSystem.outdoorAirModelObject().get());
  EXPECT_EQ(a.outdoorAirNode().get(), oaSystem.outdoorAirModelObject().get());

  ASSERT_TRUE(a.mixedAirNode());
  a.mixedAirNode().get().setName("Mixed Air Node");
  ASSERT_TRUE(oaSystem.mixedAirModelObject());
  EXPECT_EQ(a.mixedAirNode().get(), oaSystem.mixedAirModelObject().get());

  ASSERT_TRUE(a.returnAirNode());
  a.returnAirNode().get().setName("Return Air Node");
  ASSERT_TRUE(oaSystem.returnAirModelObject());
  EXPECT_EQ(a.returnAirNode().get(), oaSystem.returnAirModelObject().get());

  CoilHeatingElectric oaCoil(m);
  EXPECT_TRUE(oaCoil.addToNode(outboardOANode));

  EXPECT_EQ(4, oaSystem.components().size());
  EXPECT_EQ(1, oaSystem.reliefComponents().size());                    // (Relief)
  EXPECT_EQ(outboardReliefNode, oaSystem.outboardReliefNode().get());  //
  EXPECT_EQ(3, oaSystem.oaComponents().size());                        // (OA)-----Coil------o
  EXPECT_EQ(outboardOANode, oaSystem.outboardOANode().get());

  HeatExchangerAirToAirSensibleAndLatent erv(m);
  EXPECT_TRUE(erv.addToNode(outboardOANode));

  EXPECT_EQ(8, oaSystem.components().size());
  EXPECT_EQ(outboardReliefNode, oaSystem.outboardReliefNode().get());
  EXPECT_EQ(outboardOANode, oaSystem.outboardOANode().get());
  EXPECT_EQ(3, oaSystem.reliefComponents().size());  //     o-----ERV-----(Relief)
  EXPECT_EQ(5, oaSystem.oaComponents().size());      //  (OA)-----ERV-----o-----Coil1-----o
                                                     //  <=>
                                                     //  (RE)---- ERV---------------------o
                                                     //            X
                                                     //  (OA)-----ERV-----o-----Coil1-----o

  CoilHeatingElectric reliefCoil(m);
  EXPECT_TRUE(reliefCoil.addToNode(outboardReliefNode));
  EXPECT_EQ(10, oaSystem.components().size());
  EXPECT_EQ(outboardReliefNode, oaSystem.outboardReliefNode().get());
  EXPECT_EQ(outboardOANode, oaSystem.outboardOANode().get());
  EXPECT_EQ(5, oaSystem.reliefComponents().size());  //     o-----ERV-----o-----Coil2----(Relief)
  EXPECT_EQ(5, oaSystem.oaComponents().size());      //  (OA)-----ERV-----o-----Coil1-----o
                                                     //  <=>
                                                     //  (RE)---- Coil2-----o----ERV---------------------o
                                                     //                           X
                                                     //  (OA)--------------------ERV-----o-----Coil1-----o
}

TEST_F(ModelFixture, AirLoopHVACOutdoorAirSystem_Clone_Simple) {
  Model m;

  ControllerOutdoorAir controller(m);
  AirLoopHVACOutdoorAirSystem oaSystem(m, controller);

  ASSERT_TRUE(oaSystem.outboardReliefNode());
  auto outboardReliefNode = oaSystem.outboardReliefNode().get();
  auto outboardOANode = oaSystem.outboardOANode().get();
  outboardOANode.setName("Outboard OA Node");

  CoilHeatingElectric oaCoil(m);
  EXPECT_TRUE(oaCoil.addToNode(outboardOANode));

  CoilHeatingElectric reliefCoil(m);
  EXPECT_TRUE(reliefCoil.addToNode(outboardReliefNode));

  EXPECT_EQ(6, oaSystem.components().size());
  EXPECT_EQ(3, oaSystem.reliefComponents().size());
  EXPECT_EQ(3, oaSystem.oaComponents().size());  // (OA)-----Coil------o
  EXPECT_EQ(1, m.getConcreteModelObjects<AirLoopHVACOutdoorAirSystem>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<ControllerOutdoorAir>().size());
  EXPECT_EQ(4, m.getConcreteModelObjects<Node>().size());
  EXPECT_EQ(2, m.getConcreteModelObjects<CoilHeatingElectric>().size());

  {
    // Other model first
    Model m2;
    auto oaSystem2 = oaSystem.clone(m2).cast<AirLoopHVACOutdoorAirSystem>();

    EXPECT_NE(controller, oaSystem2.getControllerOutdoorAir());

    EXPECT_EQ(6, oaSystem2.components().size());
    EXPECT_EQ(3, oaSystem2.reliefComponents().size());
    EXPECT_EQ(3, oaSystem2.oaComponents().size());  // (OA)-----Coil------o
    EXPECT_EQ(1, m2.getConcreteModelObjects<AirLoopHVACOutdoorAirSystem>().size());
    EXPECT_EQ(1, m2.getConcreteModelObjects<ControllerOutdoorAir>().size());
    EXPECT_EQ(4, m2.getConcreteModelObjects<Node>().size());
    EXPECT_EQ(2, m2.getConcreteModelObjects<CoilHeatingElectric>().size());
  }

  {
    // Same model
    auto oaSystem2 = oaSystem.clone(m).cast<AirLoopHVACOutdoorAirSystem>();

    EXPECT_NE(controller, oaSystem2.getControllerOutdoorAir());

    EXPECT_EQ(6, oaSystem2.components().size());
    EXPECT_EQ(3, oaSystem2.reliefComponents().size());
    EXPECT_EQ(3, oaSystem2.oaComponents().size());  // (OA)-----Coil------o
    EXPECT_EQ(2, m.getConcreteModelObjects<AirLoopHVACOutdoorAirSystem>().size());
    EXPECT_EQ(2, m.getConcreteModelObjects<ControllerOutdoorAir>().size());
    EXPECT_EQ(8, m.getConcreteModelObjects<Node>().size());
    EXPECT_EQ(4, m.getConcreteModelObjects<CoilHeatingElectric>().size());
  }
}

TEST_F(ModelFixture, AirLoopHVACOutdoorAirSystem_Clone_Shared) {
  Model m;

  ControllerOutdoorAir controller(m);
  AirLoopHVACOutdoorAirSystem oaSystem(m, controller);

  ASSERT_TRUE(oaSystem.outboardReliefNode());
  auto outboardReliefNode = oaSystem.outboardReliefNode().get();
  auto outboardOANode = oaSystem.outboardOANode().get();
  outboardOANode.setName("Outboard OA Node");

  CoilHeatingElectric oaCoil(m);
  oaCoil.setName("OACoil");
  EXPECT_TRUE(oaCoil.addToNode(outboardOANode));

  HeatExchangerAirToAirSensibleAndLatent erv(m);
  erv.setName("ERV");
  EXPECT_TRUE(erv.addToNode(outboardOANode));

  CoilHeatingElectric reliefCoil(m);
  reliefCoil.setName("RECoil");
  EXPECT_TRUE(reliefCoil.addToNode(outboardReliefNode));

  EXPECT_EQ(10, oaSystem.components().size());       // ERV counted twice
  EXPECT_EQ(5, oaSystem.reliefComponents().size());  //     o-----ERV-----o-----RECoil----(RE)
  EXPECT_EQ(5, oaSystem.oaComponents().size());      //  (OA)-----ERV-----o-----OACoil-----o
                                                     //  <=>
                                                     //  (RE)---- RECoil-----o-----ERV----------------------o
                                                     //                             X
                                                     //  (OA)----------------------ERV-----o-----OACoil-----o
  EXPECT_EQ(1, m.getConcreteModelObjects<AirLoopHVACOutdoorAirSystem>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<ControllerOutdoorAir>().size());
  EXPECT_EQ(6, m.getConcreteModelObjects<Node>().size());
  EXPECT_EQ(2, m.getConcreteModelObjects<CoilHeatingElectric>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<HeatExchangerAirToAirSensibleAndLatent>().size());

  {
    // Other model first
    Model m2;
    auto oaSystem2 = oaSystem.clone(m2).cast<AirLoopHVACOutdoorAirSystem>();

    EXPECT_NE(controller, oaSystem2.getControllerOutdoorAir());

    EXPECT_EQ(10, oaSystem2.components().size());
    EXPECT_EQ(5, oaSystem2.reliefComponents().size());
    EXPECT_EQ(5, oaSystem2.oaComponents().size());
    EXPECT_EQ(1, m2.getConcreteModelObjects<AirLoopHVACOutdoorAirSystem>().size());
    EXPECT_EQ(1, m2.getConcreteModelObjects<ControllerOutdoorAir>().size());
    EXPECT_EQ(6, m2.getConcreteModelObjects<Node>().size());
    EXPECT_EQ(2, m2.getConcreteModelObjects<CoilHeatingElectric>().size());
    EXPECT_EQ(1, m2.getConcreteModelObjects<HeatExchangerAirToAirSensibleAndLatent>().size());
  }

  {
    // Same model
    auto oaSystem2 = oaSystem.clone(m).cast<AirLoopHVACOutdoorAirSystem>();

    EXPECT_NE(controller, oaSystem2.getControllerOutdoorAir());

    EXPECT_EQ(10, oaSystem2.components().size());
    EXPECT_EQ(5, oaSystem2.reliefComponents().size());
    EXPECT_EQ(5, oaSystem2.oaComponents().size());
    EXPECT_EQ(2, m.getConcreteModelObjects<AirLoopHVACOutdoorAirSystem>().size());
    EXPECT_EQ(2, m.getConcreteModelObjects<ControllerOutdoorAir>().size());
    EXPECT_EQ(12, m.getConcreteModelObjects<Node>().size());
    EXPECT_EQ(4, m.getConcreteModelObjects<CoilHeatingElectric>().size());
    EXPECT_EQ(2, m.getConcreteModelObjects<HeatExchangerAirToAirSensibleAndLatent>().size());
  }
}

TEST_F(ModelFixture, AirLoopHVACOutdoorAirSystem_Clone_SeveralERVs) {

  // Add the OASystem to AirLoopHVAC and save resulting OSM to check the clones in OSApp
  constexpr bool debug = true;

  Model m;

  ControllerOutdoorAir controller(m);
  AirLoopHVACOutdoorAirSystem oaSystem(m, controller);

  ASSERT_TRUE(oaSystem.outboardReliefNode());
  auto outboardReliefNode = oaSystem.outboardReliefNode().get();
  auto outboardOANode = oaSystem.outboardOANode().get();
  outboardOANode.setName("(OA)");
  outboardReliefNode.setName("(RE)");

  CoilHeatingElectric oaCoilA(m);
  oaCoilA.setName("OACoilA");
  oaCoilA.setComment("OACoilA");
  EXPECT_TRUE(oaCoilA.addToNode(outboardOANode));

  CoilHeatingElectric reliefCoilA(m);
  reliefCoilA.setName("RECoilA");
  reliefCoilA.setComment("RECoilA");
  EXPECT_TRUE(reliefCoilA.addToNode(outboardReliefNode));

  HeatExchangerAirToAirSensibleAndLatent ervA(m);
  ervA.setName("ERVA");
  ervA.setComment("ERVA");
  EXPECT_TRUE(ervA.addToNode(outboardOANode));

  CoilHeatingElectric oaCoilB(m);
  oaCoilB.setName("OACoilB");
  oaCoilB.setComment("OACoilB");
  EXPECT_TRUE(oaCoilB.addToNode(outboardOANode));

  HeatExchangerAirToAirSensibleAndLatent ervB(m);
  ervB.setName("ERVB");
  ervB.setComment("ERVB");
  EXPECT_TRUE(ervB.addToNode(outboardOANode));

  CoilHeatingElectric reliefCoilB(m);
  reliefCoilB.setName("RECoilB");
  reliefCoilB.setComment("RECoilB");
  EXPECT_TRUE(reliefCoilB.addToNode(outboardReliefNode));

  EXPECT_EQ(18, oaSystem.components().size());       // ERV counted twice
  EXPECT_EQ(9, oaSystem.reliefComponents().size());  //     o-----RECoilA-----o-----ERVA-----o-----ERVB-----o-----RECoilB-----(RE)
  EXPECT_EQ(9, oaSystem.oaComponents().size());      //  (OA)-----ERVB-----o-----OACoilB-----o-----ERVA-----o-----OACoilA-----o
                                                     //  <=>
                                                     //  (RE)---- RECoilB-----o----ERVB-----o-----------------------ERVA-----o-----RECoilA-----o
                                                     //                             X                                 X
                                                     //  (OA)----------------------ERVB-----o-----OACoilB-----o-----ERVA-----o-----OACoilA-----o

  EXPECT_EQ(1, m.getConcreteModelObjects<AirLoopHVACOutdoorAirSystem>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<ControllerOutdoorAir>().size());
  EXPECT_EQ(10, m.getConcreteModelObjects<Node>().size());
  EXPECT_EQ(4, m.getConcreteModelObjects<CoilHeatingElectric>().size());
  EXPECT_EQ(2, m.getConcreteModelObjects<HeatExchangerAirToAirSensibleAndLatent>().size());

  auto getTypes = [](const auto& comps) {
    std::vector<IddObjectType> compTypes;
    compTypes.reserve(comps.size());
    std::transform(comps.cbegin(), comps.cend(), std::back_inserter(compTypes), [](const auto& s) { return s.iddObjectType(); });
    return compTypes;
  };
  auto getComments = [](const auto& comps) {
    std::vector<std::string> compComments;
    compComments.reserve(comps.size());
    std::transform(comps.cbegin(), comps.cend(), std::back_inserter(compComments), [](const auto& s) { return s.comment(); });
    return compComments;
  };

  auto testStreamLayout = [&getTypes, &getComments](const std::vector<ModelObject>& lhsComps, const std::vector<ModelObject>& rhsComps) {
    ASSERT_EQ(lhsComps.size(), rhsComps.size());
    auto lhsTypes = getTypes(lhsComps);
    auto rhsTypes = getTypes(rhsComps);
    EXPECT_EQ(lhsTypes, rhsTypes);
    auto lhsComments = getComments(lhsComps);
    auto rhsComments = getComments(rhsComps);
    EXPECT_EQ(lhsComments, rhsComments);
  };

  auto testLayout = [&testStreamLayout](const AirLoopHVACOutdoorAirSystem& lhs, const AirLoopHVACOutdoorAirSystem& rhs) {
    testStreamLayout(lhs.reliefComponents(), rhs.reliefComponents());
    testStreamLayout(lhs.oaComponents(), rhs.oaComponents());
  };

  {
    // Other model first
    Model m2;
    auto oaSystem2 = oaSystem.clone(m2).cast<AirLoopHVACOutdoorAirSystem>();

    EXPECT_NE(controller, oaSystem2.getControllerOutdoorAir());

    EXPECT_EQ(18, oaSystem2.components().size());  // ERV counted twice
    EXPECT_EQ(9, oaSystem2.reliefComponents().size());
    EXPECT_EQ(9, oaSystem2.oaComponents().size());  // (OA)-----Coil------o
    EXPECT_EQ(1, m2.getConcreteModelObjects<AirLoopHVACOutdoorAirSystem>().size());
    EXPECT_EQ(1, m2.getConcreteModelObjects<ControllerOutdoorAir>().size());
    EXPECT_EQ(10, m2.getConcreteModelObjects<Node>().size());
    EXPECT_EQ(4, m2.getConcreteModelObjects<CoilHeatingElectric>().size());
    EXPECT_EQ(2, m2.getConcreteModelObjects<HeatExchangerAirToAirSensibleAndLatent>().size());

    testLayout(oaSystem, oaSystem2);

    if constexpr (debug) {
      AirLoopHVAC a2(m2);
      a2.setName("Cloned");
      auto node = a2.supplyOutletNode();
      oaSystem2.addToNode(node);
      m2.save("AirLoopHVACOutdoorAirSystem_Clone_SeveralERVs_OtherModel.osm", true);
    }
  }

  if constexpr (debug) {
    AirLoopHVAC a(m);
    a.setName("Ori");
    auto node = a.supplyOutletNode();
    oaSystem.addToNode(node);
  }

  {
    // Same model
    auto oaSystem2 = oaSystem.clone(m).cast<AirLoopHVACOutdoorAirSystem>();

    EXPECT_NE(controller, oaSystem2.getControllerOutdoorAir());

    EXPECT_EQ(18, oaSystem2.components().size());  // ERV counted twice
    EXPECT_EQ(9, oaSystem2.reliefComponents().size());
    EXPECT_EQ(9, oaSystem2.oaComponents().size());  // (OA)-----Coil------o
    EXPECT_EQ(2, m.getConcreteModelObjects<AirLoopHVACOutdoorAirSystem>().size());
    EXPECT_EQ(2, m.getConcreteModelObjects<ControllerOutdoorAir>().size());
    if constexpr (debug) {
      EXPECT_EQ(25, m.getConcreteModelObjects<Node>().size());
    } else {
      EXPECT_EQ(20, m.getConcreteModelObjects<Node>().size());
    }
    EXPECT_EQ(8, m.getConcreteModelObjects<CoilHeatingElectric>().size());
    EXPECT_EQ(4, m.getConcreteModelObjects<HeatExchangerAirToAirSensibleAndLatent>().size());

    testLayout(oaSystem, oaSystem2);

    if constexpr (debug) {
      AirLoopHVAC a2(m);
      a2.setName("Cloned");
      auto node = a2.supplyOutletNode();
      oaSystem2.addToNode(node);
      m.save("AirLoopHVACOutdoorAirSystem_Clone_SeveralERVs.osm", true);
    }
  }
}

TEST_F(ModelFixture, AirLoopHVACOutdoorAirSystem_HeatCoolFuelTypes) {
  Model m;

  ControllerOutdoorAir controller(m);
  AirLoopHVACOutdoorAirSystem oaSystem(m, controller);
  auto outboardReliefNode = oaSystem.outboardReliefNode().get();
  auto outboardOANode = oaSystem.outboardOANode().get();

  EXPECT_EQ(ComponentType(ComponentType::None), oaSystem.componentType());
  testFuelTypeEquality({}, oaSystem.coolingFuelTypes());
  testFuelTypeEquality({}, oaSystem.heatingFuelTypes());
  testAppGFuelTypeEquality({}, oaSystem.appGHeatingFuelTypes());

  CoilHeatingElectric oaCoil(m);
  EXPECT_TRUE(oaCoil.addToNode(outboardOANode));

  EXPECT_EQ(ComponentType(ComponentType::Heating), oaSystem.componentType());
  testFuelTypeEquality({}, oaSystem.coolingFuelTypes());
  testFuelTypeEquality({FuelType::Electricity}, oaSystem.heatingFuelTypes());
  testAppGFuelTypeEquality({AppGFuelType::Electric}, oaSystem.appGHeatingFuelTypes());

  CoilCoolingDXVariableSpeed reliefCoil(m);
  EXPECT_TRUE(reliefCoil.addToNode(outboardReliefNode));

  EXPECT_EQ(ComponentType(ComponentType::Both), oaSystem.componentType());
  testFuelTypeEquality({FuelType::Electricity}, oaSystem.coolingFuelTypes());
  testFuelTypeEquality({FuelType::Electricity}, oaSystem.heatingFuelTypes());
  testAppGFuelTypeEquality({AppGFuelType::Electric}, oaSystem.appGHeatingFuelTypes());
}
