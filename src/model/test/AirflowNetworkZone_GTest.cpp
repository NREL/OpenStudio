/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../AirflowNetworkZone.hpp"
#include "../AirflowNetworkZone_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../CurveLinear.hpp"
#include "../CurveLinear_Impl.hpp"
#include "../AirflowNetworkDuct.hpp"
#include "../AirflowNetworkDuct_Impl.hpp"
#include "../AirflowNetworkDistributionLinkage.hpp"
#include "../AirflowNetworkDistributionLinkage_Impl.hpp"
#include "../AirflowNetworkDistributionNode.hpp"
#include "../AirflowNetworkDistributionNode_Impl.hpp"
#include "../AirflowNetworkOccupantVentilationControl.hpp"
#include "../AirflowNetworkOccupantVentilationControl_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirflowNetwork_Zone_Basic) {
  Model model;
  ThermalZone thermalZone(model);
  Space space1(model);
  Space space2(model);

  EXPECT_FALSE(thermalZone.airflowNetworkZone());
  EXPECT_FALSE(space1.thermalZone());
  EXPECT_FALSE(space2.thermalZone());
  EXPECT_EQ(0u, thermalZone.spaces().size());

  EXPECT_TRUE(space1.setThermalZone(thermalZone));
  ASSERT_TRUE(space1.thermalZone());
  EXPECT_EQ(thermalZone.handle(), space1.thermalZone()->handle());
  EXPECT_FALSE(space2.thermalZone());
  EXPECT_EQ(1u, thermalZone.spaces().size());

  ASSERT_TRUE(space1.thermalZone());
  EXPECT_EQ(thermalZone.handle(), space1.thermalZone()->handle());
  EXPECT_TRUE(space2.setThermalZone(thermalZone));
  ASSERT_TRUE(space2.thermalZone());
  EXPECT_EQ(thermalZone.handle(), space2.thermalZone()->handle());
  EXPECT_EQ(2u, thermalZone.spaces().size());

  auto zone = thermalZone.getAirflowNetworkZone();
  auto optzone = thermalZone.airflowNetworkZone();
  ASSERT_TRUE(optzone);
  EXPECT_EQ(zone, optzone.get());

  zone.setFacadeWidth(50.0);
  CurveLinear curve(model);
  curve.setCoefficient1Constant(55.0);
  curve.setCoefficient2x(0.0);

  AirflowNetworkOccupantVentilationControl ovc(model, curve);
  EXPECT_FALSE(zone.occupantVentilationControl());
  EXPECT_TRUE(zone.setOccupantVentilationControl(ovc));
  ASSERT_TRUE(zone.occupantVentilationControl());
  EXPECT_EQ(ovc, zone.occupantVentilationControl().get());

  auto thermalZoneClone = thermalZone.clone(model).cast<ThermalZone>();
  optzone = thermalZoneClone.airflowNetworkZone();
  ASSERT_TRUE(optzone);
  EXPECT_NE(zone, optzone.get());
  EXPECT_EQ(50.0, optzone.get().facadeWidth());
  ASSERT_TRUE(optzone.get().occupantVentilationControl());
  EXPECT_EQ(ovc, optzone.get().occupantVentilationControl().get());
}

TEST_F(ModelFixture, AirflowNetwork_Zone_Linking) {
  Model model;
  ThermalZone thermalZone(model);

  EXPECT_FALSE(thermalZone.airflowNetworkZone());

  auto zone = thermalZone.getAirflowNetworkZone();
  auto optzone = thermalZone.airflowNetworkZone();
  ASSERT_TRUE(optzone);
  EXPECT_EQ(zone, optzone.get());

  AirflowNetworkDistributionNode node_afn(model);
  AirflowNetworkDuct duct(model);

  AirflowNetworkDistributionLinkage link(model, node_afn, zone, duct);

  EXPECT_EQ(node_afn, link.node1());
  EXPECT_EQ(zone, link.node2());
}
