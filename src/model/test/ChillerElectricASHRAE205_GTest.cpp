/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ModelFixture.hpp"

#include "../ChillerElectricASHRAE205.hpp"
#include "../ChillerElectricASHRAE205_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "../ExternalFile.hpp"
#include "../ExternalFile_Impl.hpp"

#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"

#include "../Connection.hpp"
#include "../Connection_Impl.hpp"

#include "../Connection.hpp"
#include "../Connection_Impl.hpp"

#include "../Connection.hpp"
#include "../Connection_Impl.hpp"

#include "../Connection.hpp"
#include "../Connection_Impl.hpp"

#include "../Connection.hpp"
#include "../Connection_Impl.hpp"

#include "../Connection.hpp"
#include "../Connection_Impl.hpp"

#include "../Connection.hpp"
#include "../Connection_Impl.hpp"

#include "../Connection.hpp"
#include "../Connection_Impl.hpp"

#include "../Connection.hpp"
#include "../Connection_Impl.hpp"

#include "../Connection.hpp"
#include "../Connection_Impl.hpp"

#include "../Connection.hpp"
#include "../Connection_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ChillerElectricASHRAE205_GettersSetters) {
  Model m;
  // TODO: Check regular Ctor arguments
  ChillerElectricASHRAE205 chillerElectricASHRAE205(m);
  // TODO: Or if a UniqueModelObject (and make sure _Impl is included)
  // ChillerElectricASHRAE205 chillerElectricASHRAE205 = m.getUniqueModelObject<ChillerElectricASHRAE205>();

  chillerElectricASHRAE205.setName("My ChillerElectricASHRAE205");

  // Representation File Name: Required Object
  ExternalFile obj(m);
  EXPECT_TRUE(chillerElectricASHRAE205.setRepresentationFile(obj));
  EXPECT_EQ(obj, chillerElectricASHRAE205.representationFile());

  // Performance Interpolation Method: Required String
  EXPECT_TRUE(chillerElectricASHRAE205.setPerformanceInterpolationMethod("Linear"));
  EXPECT_EQ("Linear", chillerElectricASHRAE205.performanceInterpolationMethod());
  // Bad Value
  EXPECT_FALSE(chillerElectricASHRAE205.setPerformanceInterpolationMethod("BADENUM"));
  EXPECT_EQ("Linear", chillerElectricASHRAE205.performanceInterpolationMethod());

  // Rated Capacity: Required Double
  // Autosize
  chillerElectricASHRAE205.autosizeRatedCapacity();
  EXPECT_TRUE(chillerElectricASHRAE205.isRatedCapacityAutosized());
  // Set
  EXPECT_TRUE(chillerElectricASHRAE205.setRatedCapacity(0.5));
  ASSERT_TRUE(chillerElectricASHRAE205.ratedCapacity());
  EXPECT_EQ(0.5, chillerElectricASHRAE205.ratedCapacity().get());
  // Bad Value
  EXPECT_FALSE(chillerElectricASHRAE205.setRatedCapacity(-10.0));
  ASSERT_TRUE(chillerElectricASHRAE205.ratedCapacity());
  EXPECT_EQ(0.5, chillerElectricASHRAE205.ratedCapacity().get());
  EXPECT_FALSE(chillerElectricASHRAE205.isRatedCapacityAutosized());

  // Sizing Factor: Required Double
  EXPECT_TRUE(chillerElectricASHRAE205.setSizingFactor(0.6));
  EXPECT_EQ(0.6, chillerElectricASHRAE205.sizingFactor());
  // Bad Value
  EXPECT_FALSE(chillerElectricASHRAE205.setSizingFactor(-10.0));
  EXPECT_EQ(0.6, chillerElectricASHRAE205.sizingFactor());

  // Ambient Temperature Indicator: Required String
  EXPECT_TRUE(chillerElectricASHRAE205.setAmbientTemperatureIndicator("Schedule"));
  EXPECT_EQ("Schedule", chillerElectricASHRAE205.ambientTemperatureIndicator());
  // Bad Value
  EXPECT_FALSE(chillerElectricASHRAE205.setAmbientTemperatureIndicator("BADENUM"));
  EXPECT_EQ("Schedule", chillerElectricASHRAE205.ambientTemperatureIndicator());

  // Ambient Temperature Schedule Name: Optional Object
  boost::optional<Schedule> obj(m);
  EXPECT_TRUE(chillerElectricASHRAE205.setAmbientTemperatureSchedule(obj));
  ASSERT_TRUE(chillerElectricASHRAE205.ambientTemperatureSchedule());
  EXPECT_EQ(obj, chillerElectricASHRAE205.ambientTemperatureSchedule().get());

  // Ambient Temperature Zone Name: Optional Object
  boost::optional<ThermalZone> obj(m);
  EXPECT_TRUE(chillerElectricASHRAE205.setAmbientTemperatureZone(obj));
  ASSERT_TRUE(chillerElectricASHRAE205.ambientTemperatureZone());
  EXPECT_EQ(obj, chillerElectricASHRAE205.ambientTemperatureZone().get());

  // Ambient Temperature Outdoor Air Node Name: Optional Object
  boost::optional<Connection> obj(m);
  EXPECT_TRUE(chillerElectricASHRAE205.setAmbientTemperatureOutdoorAirNode(obj));
  ASSERT_TRUE(chillerElectricASHRAE205.ambientTemperatureOutdoorAirNode());
  EXPECT_EQ(obj, chillerElectricASHRAE205.ambientTemperatureOutdoorAirNode().get());

  // Chilled Water Inlet Node Name: Required Object
  Connection obj(m);
  EXPECT_TRUE(chillerElectricASHRAE205.setChilledWaterInletNode(obj));
  EXPECT_EQ(obj, chillerElectricASHRAE205.chilledWaterInletNode());

  // Chilled Water Outlet Node Name: Required Object
  Connection obj(m);
  EXPECT_TRUE(chillerElectricASHRAE205.setChilledWaterOutletNode(obj));
  EXPECT_EQ(obj, chillerElectricASHRAE205.chilledWaterOutletNode());

  // Chilled Water Maximum Requested Flow Rate: Required Double
  // Autosize
  chillerElectricASHRAE205.autosizeChilledWaterMaximumRequestedFlowRate();
  EXPECT_TRUE(chillerElectricASHRAE205.isChilledWaterMaximumRequestedFlowRateAutosized());
  // Set
  EXPECT_TRUE(chillerElectricASHRAE205.setChilledWaterMaximumRequestedFlowRate(1.3));
  ASSERT_TRUE(chillerElectricASHRAE205.chilledWaterMaximumRequestedFlowRate());
  EXPECT_EQ(1.3, chillerElectricASHRAE205.chilledWaterMaximumRequestedFlowRate().get());
  // Bad Value
  EXPECT_FALSE(chillerElectricASHRAE205.setChilledWaterMaximumRequestedFlowRate(-10.0));
  ASSERT_TRUE(chillerElectricASHRAE205.chilledWaterMaximumRequestedFlowRate());
  EXPECT_EQ(1.3, chillerElectricASHRAE205.chilledWaterMaximumRequestedFlowRate().get());
  EXPECT_FALSE(chillerElectricASHRAE205.isChilledWaterMaximumRequestedFlowRateAutosized());

  // Condenser Inlet Node Name: Optional Object
  boost::optional<Connection> obj(m);
  EXPECT_TRUE(chillerElectricASHRAE205.setCondenserInletNode(obj));
  ASSERT_TRUE(chillerElectricASHRAE205.condenserInletNode());
  EXPECT_EQ(obj, chillerElectricASHRAE205.condenserInletNode().get());

  // Condenser Outlet Node Name: Optional Object
  boost::optional<Connection> obj(m);
  EXPECT_TRUE(chillerElectricASHRAE205.setCondenserOutletNode(obj));
  ASSERT_TRUE(chillerElectricASHRAE205.condenserOutletNode());
  EXPECT_EQ(obj, chillerElectricASHRAE205.condenserOutletNode().get());

  // Condenser Maximum Requested Flow Rate: Required Double
  // Autosize
  chillerElectricASHRAE205.autosizeCondenserMaximumRequestedFlowRate();
  EXPECT_TRUE(chillerElectricASHRAE205.isCondenserMaximumRequestedFlowRateAutosized());
  // Set
  EXPECT_TRUE(chillerElectricASHRAE205.setCondenserMaximumRequestedFlowRate(1.6));
  ASSERT_TRUE(chillerElectricASHRAE205.condenserMaximumRequestedFlowRate());
  EXPECT_EQ(1.6, chillerElectricASHRAE205.condenserMaximumRequestedFlowRate().get());
  // Bad Value
  EXPECT_FALSE(chillerElectricASHRAE205.setCondenserMaximumRequestedFlowRate(-10.0));
  ASSERT_TRUE(chillerElectricASHRAE205.condenserMaximumRequestedFlowRate());
  EXPECT_EQ(1.6, chillerElectricASHRAE205.condenserMaximumRequestedFlowRate().get());
  EXPECT_FALSE(chillerElectricASHRAE205.isCondenserMaximumRequestedFlowRateAutosized());

  // Chiller Flow Mode: Required String
  EXPECT_TRUE(chillerElectricASHRAE205.setChillerFlowMode("ConstantFlow"));
  EXPECT_EQ("ConstantFlow", chillerElectricASHRAE205.chillerFlowMode());
  // Bad Value
  EXPECT_FALSE(chillerElectricASHRAE205.setChillerFlowMode("BADENUM"));
  EXPECT_EQ("ConstantFlow", chillerElectricASHRAE205.chillerFlowMode());

  // Oil Cooler Inlet Node Name: Optional Object
  boost::optional<Connection> obj(m);
  EXPECT_TRUE(chillerElectricASHRAE205.setOilCoolerInletNode(obj));
  ASSERT_TRUE(chillerElectricASHRAE205.oilCoolerInletNode());
  EXPECT_EQ(obj, chillerElectricASHRAE205.oilCoolerInletNode().get());

  // Oil Cooler Outlet Node Name: Optional Object
  boost::optional<Connection> obj(m);
  EXPECT_TRUE(chillerElectricASHRAE205.setOilCoolerOutletNode(obj));
  ASSERT_TRUE(chillerElectricASHRAE205.oilCoolerOutletNode());
  EXPECT_EQ(obj, chillerElectricASHRAE205.oilCoolerOutletNode().get());

  // Oil Cooler Design Flow Rate: Optional Double
  EXPECT_TRUE(chillerElectricASHRAE205.setOilCoolerDesignFlowRate(2.0));
  ASSERT_TRUE(chillerElectricASHRAE205.oilCoolerDesignFlowRate());
  EXPECT_EQ(2.0, chillerElectricASHRAE205.oilCoolerDesignFlowRate().get());
  // Bad Value
  EXPECT_FALSE(chillerElectricASHRAE205.setOilCoolerDesignFlowRate(-10.0));
  ASSERT_TRUE(chillerElectricASHRAE205.oilCoolerDesignFlowRate());
  EXPECT_EQ(2.0, chillerElectricASHRAE205.oilCoolerDesignFlowRate().get());

  // Auxiliary Inlet Node Name: Optional Object
  boost::optional<Connection> obj(m);
  EXPECT_TRUE(chillerElectricASHRAE205.setAuxiliaryInletNode(obj));
  ASSERT_TRUE(chillerElectricASHRAE205.auxiliaryInletNode());
  EXPECT_EQ(obj, chillerElectricASHRAE205.auxiliaryInletNode().get());

  // Auxiliary Outlet Node Name: Optional Object
  boost::optional<Connection> obj(m);
  EXPECT_TRUE(chillerElectricASHRAE205.setAuxiliaryOutletNode(obj));
  ASSERT_TRUE(chillerElectricASHRAE205.auxiliaryOutletNode());
  EXPECT_EQ(obj, chillerElectricASHRAE205.auxiliaryOutletNode().get());

  // Auxiliary Cooling Design Flow Rate: Optional Double
  EXPECT_TRUE(chillerElectricASHRAE205.setAuxiliaryCoolingDesignFlowRate(2.3));
  ASSERT_TRUE(chillerElectricASHRAE205.auxiliaryCoolingDesignFlowRate());
  EXPECT_EQ(2.3, chillerElectricASHRAE205.auxiliaryCoolingDesignFlowRate().get());
  // Bad Value
  EXPECT_FALSE(chillerElectricASHRAE205.setAuxiliaryCoolingDesignFlowRate(-10.0));
  ASSERT_TRUE(chillerElectricASHRAE205.auxiliaryCoolingDesignFlowRate());
  EXPECT_EQ(2.3, chillerElectricASHRAE205.auxiliaryCoolingDesignFlowRate().get());

  // Heat Recovery Inlet Node Name: Optional Object
  boost::optional<Connection> obj(m);
  EXPECT_TRUE(chillerElectricASHRAE205.setHeatRecoveryInletNode(obj));
  ASSERT_TRUE(chillerElectricASHRAE205.heatRecoveryInletNode());
  EXPECT_EQ(obj, chillerElectricASHRAE205.heatRecoveryInletNode().get());

  // Heat Recovery Outlet Node Name: Optional Object
  boost::optional<Connection> obj(m);
  EXPECT_TRUE(chillerElectricASHRAE205.setHeatRecoveryOutletNode(obj));
  ASSERT_TRUE(chillerElectricASHRAE205.heatRecoveryOutletNode());
  EXPECT_EQ(obj, chillerElectricASHRAE205.heatRecoveryOutletNode().get());

  // End-Use Subcategory: Optional String
  // Default value from IDD
  EXPECT_TRUE(chillerElectricASHRAE205.isEndUseSubcategoryDefaulted());
  EXPECT_EQ("General", chillerElectricASHRAE205.endUseSubcategory());
  // Set
  EXPECT_TRUE(chillerElectricASHRAE205.setEndUseSubcategory());
  EXPECT_EQ(, chillerElectricASHRAE205.endUseSubcategory());
  EXPECT_FALSE(chillerElectricASHRAE205.isEndUseSubcategoryDefaulted());
  // Reset
  chillerElectricASHRAE205.resetEndUseSubcategory();
  EXPECT_TRUE(chillerElectricASHRAE205.isEndUseSubcategoryDefaulted());
}
