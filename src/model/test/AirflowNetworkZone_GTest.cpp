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

  ThermalZone thermalZoneClone = thermalZone.clone(model).cast<ThermalZone>();
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
