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

#include "ModelFixture.hpp"

#include "../FanComponentModel.hpp"
#include "../FanComponentModel_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "../Connection.hpp"
#include "../Connection_Impl.hpp"

#include "../Connection.hpp"
#include "../Connection_Impl.hpp"

#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

#include "../BivariateFunctions.hpp"
#include "../BivariateFunctions_Impl.hpp"

#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"

#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"

#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"

#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"

#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"

#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"

#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"

#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"

#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"

#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"

#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"

#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, FanComponentModel_GettersSetters) {
  Model m;
  // TODO: Check regular Ctor arguments
  FanComponentModel fanComponentModel(m);
  // TODO: Or if a UniqueModelObject (and make sure _Impl is included)
  // FanComponentModel fanComponentModel = m.getUniqueModelObject<FanComponentModel>();

  fanComponentModel.setName("My FanComponentModel");

  // Air Inlet Node Name: Required Object
  Connection obj(m);
  EXPECT_TRUE(fanComponentModel.setAirInletNode(obj));
EXPECT_EQ(obj, fanComponentModel.airInletNode());

  // Air Outlet Node Name: Required Object
  Connection obj(m);
  EXPECT_TRUE(fanComponentModel.setAirOutletNode(obj));
EXPECT_EQ(obj, fanComponentModel.airOutletNode());

  // Availability Schedule Name: Required Object
  Schedule obj(m);
  EXPECT_TRUE(fanComponentModel.setAvailabilitySchedule(obj));
EXPECT_EQ(obj, fanComponentModel.availabilitySchedule());

  // Maximum Flow Rate: Required Double
  EXPECT_TRUE(fanComponentModel.setMaximumFlowRate(0.1));
  EXPECT_EQ(0.1, fanComponentModel.maximumFlowRate());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setMaximumFlowRate(-10.0));
  EXPECT_EQ(0.1, fanComponentModel.maximumFlowRate());

  // Minimum Flow Rate: Required Double
  EXPECT_TRUE(fanComponentModel.setMinimumFlowRate(0.1));
  EXPECT_EQ(0.1, fanComponentModel.minimumFlowRate());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setMinimumFlowRate(-10.0));
  EXPECT_EQ(0.1, fanComponentModel.minimumFlowRate());

  // Fan Sizing Factor: Required Double
  EXPECT_TRUE(fanComponentModel.setFanSizingFactor(1.1));
  EXPECT_EQ(1.1, fanComponentModel.fanSizingFactor());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setFanSizingFactor(-9.0));
  EXPECT_EQ(1.1, fanComponentModel.fanSizingFactor());

  // Fan Wheel Diameter: Required Double
  EXPECT_TRUE(fanComponentModel.setFanWheelDiameter(0.1));
  EXPECT_EQ(0.1, fanComponentModel.fanWheelDiameter());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setFanWheelDiameter(-10.0));
  EXPECT_EQ(0.1, fanComponentModel.fanWheelDiameter());

  // Fan Outlet Area: Required Double
  EXPECT_TRUE(fanComponentModel.setFanOutletArea(0.1));
  EXPECT_EQ(0.1, fanComponentModel.fanOutletArea());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setFanOutletArea(-10.0));
  EXPECT_EQ(0.1, fanComponentModel.fanOutletArea());

  // Maximum Fan Static Efficiency: Required Double
  EXPECT_TRUE(fanComponentModel.setMaximumFanStaticEfficiency(0.5));
  EXPECT_EQ(0.5, fanComponentModel.maximumFanStaticEfficiency());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setMaximumFanStaticEfficiency(-10.0));
  EXPECT_EQ(0.5, fanComponentModel.maximumFanStaticEfficiency());

  // Euler Number at Maximum Fan Static Efficiency: Required Double
  EXPECT_TRUE(fanComponentModel.setEulerNumberatMaximumFanStaticEfficiency(0.1));
  EXPECT_EQ(0.1, fanComponentModel.eulerNumberatMaximumFanStaticEfficiency());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setEulerNumberatMaximumFanStaticEfficiency(-10.0));
  EXPECT_EQ(0.1, fanComponentModel.eulerNumberatMaximumFanStaticEfficiency());

  // Maximum Dimensionless Fan Airflow: Required Double
  EXPECT_TRUE(fanComponentModel.setMaximumDimensionlessFanAirflow(0.1));
  EXPECT_EQ(0.1, fanComponentModel.maximumDimensionlessFanAirflow());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setMaximumDimensionlessFanAirflow(-10.0));
  EXPECT_EQ(0.1, fanComponentModel.maximumDimensionlessFanAirflow());

  // Motor Fan Pulley Ratio: Required Double
  fanComponentModel.autosizeMotorFanPulleyRatio();
  EXPECT_TRUE(fanComponentModel.isMotorFanPulleyRatioAutosized());
  EXPECT_TRUE(fanComponentModel.setMotorFanPulleyRatio(0.1));
  ASSERT_TRUE(fanComponentModel.motorFanPulleyRatio());
  EXPECT_EQ(0.1, fanComponentModel.motorFanPulleyRatio().get());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setMotorFanPulleyRatio(-10.0));
  ASSERT_TRUE(fanComponentModel.motorFanPulleyRatio());
  EXPECT_EQ(0.1, fanComponentModel.motorFanPulleyRatio().get());
  EXPECT_FALSE(fanComponentModel.isMotorFanPulleyRatioAutosized());

  // Belt Maximum Torque: Required Double
  fanComponentModel.autosizeBeltMaximumTorque();
  EXPECT_TRUE(fanComponentModel.isBeltMaximumTorqueAutosized());
  EXPECT_TRUE(fanComponentModel.setBeltMaximumTorque(0.1));
  ASSERT_TRUE(fanComponentModel.beltMaximumTorque());
  EXPECT_EQ(0.1, fanComponentModel.beltMaximumTorque().get());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setBeltMaximumTorque(-10.0));
  ASSERT_TRUE(fanComponentModel.beltMaximumTorque());
  EXPECT_EQ(0.1, fanComponentModel.beltMaximumTorque().get());
  EXPECT_FALSE(fanComponentModel.isBeltMaximumTorqueAutosized());

  // Belt Sizing Factor: Required Double
  EXPECT_TRUE(fanComponentModel.setBeltSizingFactor(1.1));
  EXPECT_EQ(1.1, fanComponentModel.beltSizingFactor());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setBeltSizingFactor(-9.0));
  EXPECT_EQ(1.1, fanComponentModel.beltSizingFactor());

  // Belt Fractional Torque Transition: Required Double
  EXPECT_TRUE(fanComponentModel.setBeltFractionalTorqueTransition(0.5));
  EXPECT_EQ(0.5, fanComponentModel.beltFractionalTorqueTransition());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setBeltFractionalTorqueTransition(-10.0));
  EXPECT_EQ(0.5, fanComponentModel.beltFractionalTorqueTransition());

  // Motor Maximum Speed: Required Double
  EXPECT_TRUE(fanComponentModel.setMotorMaximumSpeed(0.1));
  EXPECT_EQ(0.1, fanComponentModel.motorMaximumSpeed());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setMotorMaximumSpeed(-10.0));
  EXPECT_EQ(0.1, fanComponentModel.motorMaximumSpeed());

  // Maximum Motor Output Power: Required Double
  fanComponentModel.autosizeMaximumMotorOutputPower();
  EXPECT_TRUE(fanComponentModel.isMaximumMotorOutputPowerAutosized());
  EXPECT_TRUE(fanComponentModel.setMaximumMotorOutputPower(0.1));
  ASSERT_TRUE(fanComponentModel.maximumMotorOutputPower());
  EXPECT_EQ(0.1, fanComponentModel.maximumMotorOutputPower().get());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setMaximumMotorOutputPower(-10.0));
  ASSERT_TRUE(fanComponentModel.maximumMotorOutputPower());
  EXPECT_EQ(0.1, fanComponentModel.maximumMotorOutputPower().get());
  EXPECT_FALSE(fanComponentModel.isMaximumMotorOutputPowerAutosized());

  // Motor Sizing Factor: Required Double
  EXPECT_TRUE(fanComponentModel.setMotorSizingFactor(1.1));
  EXPECT_EQ(1.1, fanComponentModel.motorSizingFactor());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setMotorSizingFactor(-9.0));
  EXPECT_EQ(1.1, fanComponentModel.motorSizingFactor());

  // Motor In Airstream Fraction: Required Double
  EXPECT_TRUE(fanComponentModel.setMotorInAirstreamFraction(0.5));
  EXPECT_EQ(0.5, fanComponentModel.motorInAirstreamFraction());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setMotorInAirstreamFraction(-10.0));
  EXPECT_EQ(0.5, fanComponentModel.motorInAirstreamFraction());

  // VFD Efficiency Type: Optional String
  EXPECT_TRUE(fanComponentModel.setVFDEfficiencyType("Speed"));
  ASSERT_TRUE(fanComponentModel.vFDEfficiencyType());
  EXPECT_EQ("Speed", fanComponentModel.vFDEfficiencyType().get());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setVFDEfficiencyType("BADENUM"));
  ASSERT_TRUE(fanComponentModel.vFDEfficiencyType());
  EXPECT_EQ("Speed", fanComponentModel.vFDEfficiencyType().get());

  // Maximum VFD Output Power: Required Double
  fanComponentModel.autosizeMaximumVFDOutputPower();
  EXPECT_TRUE(fanComponentModel.isMaximumVFDOutputPowerAutosized());
  EXPECT_TRUE(fanComponentModel.setMaximumVFDOutputPower(0.1));
  ASSERT_TRUE(fanComponentModel.maximumVFDOutputPower());
  EXPECT_EQ(0.1, fanComponentModel.maximumVFDOutputPower().get());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setMaximumVFDOutputPower(-10.0));
  ASSERT_TRUE(fanComponentModel.maximumVFDOutputPower());
  EXPECT_EQ(0.1, fanComponentModel.maximumVFDOutputPower().get());
  EXPECT_FALSE(fanComponentModel.isMaximumVFDOutputPowerAutosized());

  // VFD Sizing Factor: Required Double
  EXPECT_TRUE(fanComponentModel.setVFDSizingFactor(1.1));
  EXPECT_EQ(1.1, fanComponentModel.vFDSizingFactor());
  // Bad Value
  EXPECT_FALSE(fanComponentModel.setVFDSizingFactor(-9.0));
  EXPECT_EQ(1.1, fanComponentModel.vFDSizingFactor());

  // Fan Pressure Rise Curve Name: Required Object
  BivariateFunctions obj(m);
  EXPECT_TRUE(fanComponentModel.setFanPressureRiseCurve(obj));
EXPECT_EQ(obj, fanComponentModel.fanPressureRiseCurve());

  // Duct Static Pressure Reset Curve Name: Required Object
  UnivariateFunctions obj(m);
  EXPECT_TRUE(fanComponentModel.setDuctStaticPressureResetCurve(obj));
EXPECT_EQ(obj, fanComponentModel.ductStaticPressureResetCurve());

  // Normalized Fan Static Efficiency Curve Name-Non-Stall Region: Required Object
  UnivariateFunctions obj(m);
  EXPECT_TRUE(fanComponentModel.setNormalizedFanStaticEfficiencyCurveNonStallRegion(obj));
EXPECT_EQ(obj, fanComponentModel.normalizedFanStaticEfficiencyCurveNonStallRegion());

  // Normalized Fan Static Efficiency Curve Name-Stall Region: Required Object
  UnivariateFunctions obj(m);
  EXPECT_TRUE(fanComponentModel.setNormalizedFanStaticEfficiencyCurveStallRegion(obj));
EXPECT_EQ(obj, fanComponentModel.normalizedFanStaticEfficiencyCurveStallRegion());

  // Normalized Dimensionless Airflow Curve Name-Non-Stall Region: Required Object
  UnivariateFunctions obj(m);
  EXPECT_TRUE(fanComponentModel.setNormalizedDimensionlessAirflowCurveNonStallRegion(obj));
EXPECT_EQ(obj, fanComponentModel.normalizedDimensionlessAirflowCurveNonStallRegion());

  // Normalized Dimensionless Airflow Curve Name-Stall Region: Required Object
  UnivariateFunctions obj(m);
  EXPECT_TRUE(fanComponentModel.setNormalizedDimensionlessAirflowCurveStallRegion(obj));
EXPECT_EQ(obj, fanComponentModel.normalizedDimensionlessAirflowCurveStallRegion());

  // Maximum Belt Efficiency Curve Name: Optional Object
  boost::optional<UnivariateFunctions> obj(m);
  EXPECT_TRUE(fanComponentModel.setMaximumBeltEfficiencyCurve(obj));
  ASSERT_TRUE(fanComponentModel.maximumBeltEfficiencyCurve());
  EXPECT_EQ(obj, fanComponentModel.maximumBeltEfficiencyCurve().get());

  // Normalized Belt Efficiency Curve Name - Region 1: Optional Object
  boost::optional<UnivariateFunctions> obj(m);
  EXPECT_TRUE(fanComponentModel.setNormalizedBeltEfficiencyCurveRegion1(obj));
  ASSERT_TRUE(fanComponentModel.normalizedBeltEfficiencyCurveRegion1());
  EXPECT_EQ(obj, fanComponentModel.normalizedBeltEfficiencyCurveRegion1().get());

  // Normalized Belt Efficiency Curve Name - Region 2: Optional Object
  boost::optional<UnivariateFunctions> obj(m);
  EXPECT_TRUE(fanComponentModel.setNormalizedBeltEfficiencyCurveRegion2(obj));
  ASSERT_TRUE(fanComponentModel.normalizedBeltEfficiencyCurveRegion2());
  EXPECT_EQ(obj, fanComponentModel.normalizedBeltEfficiencyCurveRegion2().get());

  // Normalized Belt Efficiency Curve Name - Region 3: Optional Object
  boost::optional<UnivariateFunctions> obj(m);
  EXPECT_TRUE(fanComponentModel.setNormalizedBeltEfficiencyCurveRegion3(obj));
  ASSERT_TRUE(fanComponentModel.normalizedBeltEfficiencyCurveRegion3());
  EXPECT_EQ(obj, fanComponentModel.normalizedBeltEfficiencyCurveRegion3().get());

  // Maximum Motor Efficiency Curve Name: Optional Object
  boost::optional<UnivariateFunctions> obj(m);
  EXPECT_TRUE(fanComponentModel.setMaximumMotorEfficiencyCurve(obj));
  ASSERT_TRUE(fanComponentModel.maximumMotorEfficiencyCurve());
  EXPECT_EQ(obj, fanComponentModel.maximumMotorEfficiencyCurve().get());

  // Normalized Motor Efficiency Curve Name: Optional Object
  boost::optional<UnivariateFunctions> obj(m);
  EXPECT_TRUE(fanComponentModel.setNormalizedMotorEfficiencyCurve(obj));
  ASSERT_TRUE(fanComponentModel.normalizedMotorEfficiencyCurve());
  EXPECT_EQ(obj, fanComponentModel.normalizedMotorEfficiencyCurve().get());

  // VFD Efficiency Curve Name: Optional Object
  boost::optional<UnivariateFunctions> obj(m);
  EXPECT_TRUE(fanComponentModel.setVFDEfficiencyCurve(obj));
  ASSERT_TRUE(fanComponentModel.vFDEfficiencyCurve());
  EXPECT_EQ(obj, fanComponentModel.vFDEfficiencyCurve().get());

  // End-Use Subcategory: Required String
  EXPECT_TRUE(fanComponentModel.setEndUseSubcategory());
  EXPECT_EQ(, fanComponentModel.endUseSubcategory());

