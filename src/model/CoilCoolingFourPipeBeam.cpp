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

#include "CoilCoolingFourPipeBeam.hpp"
#include "CoilCoolingFourPipeBeam_Impl.hpp"

#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "Model.hpp"
#include "PlantLoop.hpp"
#include "AirTerminalSingleDuctConstantVolumeFourPipeBeam.hpp"
#include "AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl.hpp"

// For Ctor default curves
#include "CurveLinear.hpp"
#include "TableMultiVariableLookup.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Coil_Cooling_FourPipeBeam_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CoilCoolingFourPipeBeam_Impl::CoilCoolingFourPipeBeam_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilCoolingFourPipeBeam::iddObjectType());
    }

    CoilCoolingFourPipeBeam_Impl::CoilCoolingFourPipeBeam_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                               bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilCoolingFourPipeBeam::iddObjectType());
    }

    CoilCoolingFourPipeBeam_Impl::CoilCoolingFourPipeBeam_Impl(const CoilCoolingFourPipeBeam_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilCoolingFourPipeBeam_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      // None: all in parent AirTerminalSingleDuctConstantVolumeFourPipeBeam
      return result;
    }

    IddObjectType CoilCoolingFourPipeBeam_Impl::iddObjectType() const {
      return CoilCoolingFourPipeBeam::iddObjectType();
    }

    unsigned CoilCoolingFourPipeBeam_Impl::inletPort() const {
      return OS_Coil_Cooling_FourPipeBeamFields::ChilledWaterInletNodeName;
    }

    unsigned CoilCoolingFourPipeBeam_Impl::outletPort() const {
      return OS_Coil_Cooling_FourPipeBeamFields::ChilledWaterOutletNodeName;
    }

    // contaningHVACComponent is important: used for airLoopHVAC::addBranchForZone to connect the coil to the right loop
    boost::optional<HVACComponent> CoilCoolingFourPipeBeam_Impl::containingHVACComponent() const {
      // return this->airTerminalSingleDuctConstantVolumeFourPipeBeam();
      std::vector<AirTerminalSingleDuctConstantVolumeFourPipeBeam> atuFourPipeBeams;

      atuFourPipeBeams = this->model().getConcreteModelObjects<AirTerminalSingleDuctConstantVolumeFourPipeBeam>();

      for (const auto& elem : atuFourPipeBeams) {
        if (boost::optional<HVACComponent> coil = elem.coolingCoil()) {
          if (coil->handle() == this->handle()) {
            return elem;
          }
        }
      }
      return boost::none;
    }

    boost::optional<StraightComponent> CoilCoolingFourPipeBeam_Impl::containingStraightComponent() const {
      // return this->airTerminalSingleDuctConstantVolumeFourPipeBeam();
      std::vector<AirTerminalSingleDuctConstantVolumeFourPipeBeam> atuFourPipeBeams;

      atuFourPipeBeams = this->model().getConcreteModelObjects<AirTerminalSingleDuctConstantVolumeFourPipeBeam>();

      for (const auto& elem : atuFourPipeBeams) {
        if (boost::optional<HVACComponent> coil = elem.coolingCoil()) {
          if (coil->handle() == this->handle()) {
            return elem;
          }
        }
      }
      return boost::none;
    }

    /* Only allow connection on the demand side of a plantLoop */
    bool CoilCoolingFourPipeBeam_Impl::addToNode(Node& node) {
      if (boost::optional<PlantLoop> plant = node.plantLoop()) {
        if (plant->demandComponent(node.handle())) {
          return StraightComponent_Impl::addToNode(node);
        }
      }

      return false;
    }

    /* Nodes */

    boost::optional<Node> CoilCoolingFourPipeBeam_Impl::chilledWaterInletNode() const {
      // TODO: use this once port methods are set to const
      // unsigned port = inletPort();
      unsigned port = OS_Coil_Cooling_FourPipeBeamFields::ChilledWaterInletNodeName;
      boost::optional<ModelObject> mo = connectedObject(port);
      boost::optional<Node> result;

      if (mo) {
        if (boost::optional<Node> node = mo->optionalCast<Node>()) {
          result = node;
        }
      }

      return result;
    }

    boost::optional<Node> CoilCoolingFourPipeBeam_Impl::chilledWaterOutletNode() const {
      // TODO: use this once port methods are set to const
      // unsigned port = outletPort();
      unsigned port = OS_Coil_Cooling_FourPipeBeamFields::ChilledWaterOutletNodeName;
      boost::optional<ModelObject> mo = connectedObject(port);
      boost::optional<Node> result;

      if (mo) {
        if (boost::optional<Node> node = mo->optionalCast<Node>()) {
          result = node;
        }
      }

      return result;
    }

    /* double with default fields */

    // Beam Rated Cooling Capacity per Beam Length

    double CoilCoolingFourPipeBeam_Impl::beamRatedCoolingCapacityperBeamLength() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedCoolingCapacityperBeamLength, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingFourPipeBeam_Impl::isBeamRatedCoolingCapacityperBeamLengthDefaulted() const {
      return isEmpty(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedCoolingCapacityperBeamLength);
    }

    bool CoilCoolingFourPipeBeam_Impl::setBeamRatedCoolingCapacityperBeamLength(double beamRatedCoolingCapacityperBeamLength) {
      bool result = setDouble(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedCoolingCapacityperBeamLength, beamRatedCoolingCapacityperBeamLength);
      return result;
    }

    void CoilCoolingFourPipeBeam_Impl::resetBeamRatedCoolingCapacityperBeamLength() {
      bool result = setString(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedCoolingCapacityperBeamLength, "");
      OS_ASSERT(result);
    }

    // Beam Rated Cooling Room Air Chilled Water Temperature Difference

    double CoilCoolingFourPipeBeam_Impl::beamRatedCoolingRoomAirChilledWaterTemperatureDifference() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedCoolingRoomAirChilledWaterTemperatureDifference, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingFourPipeBeam_Impl::isBeamRatedCoolingRoomAirChilledWaterTemperatureDifferenceDefaulted() const {
      return isEmpty(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedCoolingRoomAirChilledWaterTemperatureDifference);
    }

    bool CoilCoolingFourPipeBeam_Impl::setBeamRatedCoolingRoomAirChilledWaterTemperatureDifference(
      double beamRatedCoolingRoomAirChilledWaterTemperatureDifference) {
      bool result = setDouble(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedCoolingRoomAirChilledWaterTemperatureDifference,
                              beamRatedCoolingRoomAirChilledWaterTemperatureDifference);
      return result;
    }

    void CoilCoolingFourPipeBeam_Impl::resetBeamRatedCoolingRoomAirChilledWaterTemperatureDifference() {
      bool result = setString(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedCoolingRoomAirChilledWaterTemperatureDifference, "");
      OS_ASSERT(result);
    }

    // Beam Rated Chilled Water Volume Flow Rate per Beam Length

    double CoilCoolingFourPipeBeam_Impl::beamRatedChilledWaterVolumeFlowRateperBeamLength() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedChilledWaterVolumeFlowRateperBeamLength, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingFourPipeBeam_Impl::isBeamRatedChilledWaterVolumeFlowRateperBeamLengthDefaulted() const {
      return isEmpty(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedChilledWaterVolumeFlowRateperBeamLength);
    }

    bool CoilCoolingFourPipeBeam_Impl::setBeamRatedChilledWaterVolumeFlowRateperBeamLength(double beamRatedChilledWaterVolumeFlowRateperBeamLength) {
      bool result = setDouble(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedChilledWaterVolumeFlowRateperBeamLength,
                              beamRatedChilledWaterVolumeFlowRateperBeamLength);
      return result;
    }

    void CoilCoolingFourPipeBeam_Impl::resetBeamRatedChilledWaterVolumeFlowRateperBeamLength() {
      bool result = setString(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedChilledWaterVolumeFlowRateperBeamLength, "");
      OS_ASSERT(result);
    }

    /* Curves */

    // Beam Cooling Capacity Temperature Difference Modification Factor Curve

    Curve CoilCoolingFourPipeBeam_Impl::beamCoolingCapacityTemperatureDifferenceModificationFactorCurve() const {
      boost::optional<Curve> c = getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_FourPipeBeamFields::BeamCoolingCapacityTemperatureDifferenceModificationFactorCurveName);
      if (!c) {
        LOG_AND_THROW(briefDescription() << " doesn't have the required Beam Cooling Capacity Temperature Difference Modification Factor Curve");
      }
      OS_ASSERT(c);
      return c.get();
    }

    bool CoilCoolingFourPipeBeam_Impl::setBeamCoolingCapacityTemperatureDifferenceModificationFactorCurve(const Curve& curve) {
      bool result =
        setPointer(OS_Coil_Cooling_FourPipeBeamFields::BeamCoolingCapacityTemperatureDifferenceModificationFactorCurveName, curve.handle());
      return result;
    }

    // Beam Cooling Capacity Air Flow Modification Factor Curve

    Curve CoilCoolingFourPipeBeam_Impl::beamCoolingCapacityAirFlowModificationFactorCurve() const {
      boost::optional<Curve> c = getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_FourPipeBeamFields::BeamCoolingCapacityAirFlowModificationFactorCurveName);
      if (!c) {
        LOG_AND_THROW(briefDescription() << " doesn't have the required Beam Cooling Capacity Air Flow Modification Factor Curve");
      }
      OS_ASSERT(c);
      return c.get();
    }

    bool CoilCoolingFourPipeBeam_Impl::setBeamCoolingCapacityAirFlowModificationFactorCurve(const Curve& curve) {
      bool result = setPointer(OS_Coil_Cooling_FourPipeBeamFields::BeamCoolingCapacityAirFlowModificationFactorCurveName, curve.handle());
      return result;
    }

    // Beam Cooling Capacity Chilled Water Flow Modification Factor Curve

    Curve CoilCoolingFourPipeBeam_Impl::beamCoolingCapacityChilledWaterFlowModificationFactorCurve() const {
      boost::optional<Curve> c = getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_FourPipeBeamFields::BeamCoolingCapacityChilledWaterFlowModificationFactorCurveName);
      if (!c) {
        LOG_AND_THROW(briefDescription() << " doesn't have the required Beam Cooling Capacity Chilled Water Flow Modification Factor Curve");
      }
      OS_ASSERT(c);
      return c.get();
    }

    bool CoilCoolingFourPipeBeam_Impl::setBeamCoolingCapacityChilledWaterFlowModificationFactorCurve(const Curve& curve) {
      bool result = setPointer(OS_Coil_Cooling_FourPipeBeamFields::BeamCoolingCapacityChilledWaterFlowModificationFactorCurveName, curve.handle());
      return result;
    }

    /*  Convenience functions */

    boost::optional<AirTerminalSingleDuctConstantVolumeFourPipeBeam>
      CoilCoolingFourPipeBeam_Impl::airTerminalSingleDuctConstantVolumeFourPipeBeam() const {
      // this coil can only be found in a AirTerminalSingleDuctConstantVolumeFourPipeBeam
      // check all AirTerminalSingleDuctConstantVolumeFourPipeBeam in the models, seeing if this coil
      // is inside of one of them.  Return the one it is inside of.

      std::vector<AirTerminalSingleDuctConstantVolumeFourPipeBeam> atuFourPipeBeams;

      atuFourPipeBeams = this->model().getConcreteModelObjects<AirTerminalSingleDuctConstantVolumeFourPipeBeam>();

      for (const auto& elem : atuFourPipeBeams) {
        if (boost::optional<HVACComponent> coil = elem.coolingCoil()) {
          if (coil->handle() == this->handle()) {
            return elem;
          }
        }
      }
      return boost::none;
    }

  }  // namespace detail

  /* Ctor: defaulting Curves like in the FourPipeBeamLargeOffice.idf Example File from E+ */
  CoilCoolingFourPipeBeam::CoilCoolingFourPipeBeam(const Model& model) : StraightComponent(CoilCoolingFourPipeBeam::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilCoolingFourPipeBeam_Impl>());

    bool ok = true;

    //Beam Cooling Capacity Temperature Difference Modification Factor Curve Nam
    CurveLinear capModFuncOfTempDiff(model);
    capModFuncOfTempDiff.setName("CapModFuncOfTempDiff");
    capModFuncOfTempDiff.setCoefficient1Constant(0);
    capModFuncOfTempDiff.setCoefficient2x(1);
    capModFuncOfTempDiff.setMinimumValueofx(0.0);
    capModFuncOfTempDiff.setMaximumValueofx(1.5);
    capModFuncOfTempDiff.setMinimumCurveOutput(0.0);
    capModFuncOfTempDiff.setMaximumCurveOutput(1.5);

    ok = setBeamCoolingCapacityTemperatureDifferenceModificationFactorCurve(capModFuncOfTempDiff);
    OS_ASSERT(ok);

    // Beam Cooling Capacity Air Flow Modification Factor Curve Name
    // CoolCapModFuncOfSAFlow: Table:OneIndependentVariable,

    TableMultiVariableLookup coolCapModFuncOfSAFlow(model, 1);
    coolCapModFuncOfSAFlow.setName("CoolCapModFuncOfSAFlow");
    coolCapModFuncOfSAFlow.setCurveType("Quadratic");
    coolCapModFuncOfSAFlow.setInterpolationMethod("EvaluateCurveToLimits");
    coolCapModFuncOfSAFlow.setMinimumValueofX1(0.714);
    coolCapModFuncOfSAFlow.setMaximumValueofX1(1.2857);
    coolCapModFuncOfSAFlow.setMinimumTableOutput(0.8234);
    coolCapModFuncOfSAFlow.setMaximumTableOutput(1.1256);
    coolCapModFuncOfSAFlow.setInputUnitTypeforX1("Dimensionless");
    coolCapModFuncOfSAFlow.setOutputUnitType("Dimensionless");

    coolCapModFuncOfSAFlow.addPoint(0.714286, 0.823403);
    coolCapModFuncOfSAFlow.addPoint(1.0, 1.0);
    coolCapModFuncOfSAFlow.addPoint(1.2857, 1.1256);

    ok = setBeamCoolingCapacityAirFlowModificationFactorCurve(coolCapModFuncOfSAFlow);
    OS_ASSERT(ok);

    // Beam Cooling Capacity Chilled Water Flow Modification Factor Curve
    // CapModFuncOfWaterFlow: Table:OneIndependentVariable

    TableMultiVariableLookup capModFuncOfWaterFlow(model, 1);
    capModFuncOfWaterFlow.setName("CapModFuncOfWaterFlow");
    capModFuncOfWaterFlow.setCurveType("Quadratic");
    capModFuncOfWaterFlow.setInterpolationMethod("EvaluateCurveToLimits");
    capModFuncOfWaterFlow.setMinimumValueofX1(0);
    capModFuncOfWaterFlow.setMaximumValueofX1(1.33);
    capModFuncOfWaterFlow.setMinimumTableOutput(0.0);
    capModFuncOfWaterFlow.setMaximumTableOutput(1.04);
    capModFuncOfWaterFlow.setInputUnitTypeforX1("Dimensionless");
    capModFuncOfWaterFlow.setOutputUnitType("Dimensionless");

    capModFuncOfWaterFlow.addPoint(0.0, 0.0);
    capModFuncOfWaterFlow.addPoint(0.05, 0.001);
    capModFuncOfWaterFlow.addPoint(0.33333, 0.71);
    capModFuncOfWaterFlow.addPoint(0.5, 0.85);
    capModFuncOfWaterFlow.addPoint(0.666667, 0.92);
    capModFuncOfWaterFlow.addPoint(0.833333, 0.97);
    capModFuncOfWaterFlow.addPoint(1.0, 1.0);
    capModFuncOfWaterFlow.addPoint(1.333333, 1.04);

    ok = setBeamCoolingCapacityChilledWaterFlowModificationFactorCurve(capModFuncOfWaterFlow);
    OS_ASSERT(ok);
  }

  IddObjectType CoilCoolingFourPipeBeam::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Coil_Cooling_FourPipeBeam);
  }

  /* Nodes */
  boost::optional<Node> CoilCoolingFourPipeBeam::chilledWaterInletNode() const {
    return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->chilledWaterInletNode();
  }

  boost::optional<Node> CoilCoolingFourPipeBeam::chilledWaterOutletNode() const {
    return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->chilledWaterOutletNode();
  }

  /* double with default fields */

  // Beam Rated Cooling Capacity per Beam Length

  double CoilCoolingFourPipeBeam::beamRatedCoolingCapacityperBeamLength() const {
    return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->beamRatedCoolingCapacityperBeamLength();
  }

  bool CoilCoolingFourPipeBeam::isBeamRatedCoolingCapacityperBeamLengthDefaulted() const {
    return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->isBeamRatedCoolingCapacityperBeamLengthDefaulted();
  }

  bool CoilCoolingFourPipeBeam::setBeamRatedCoolingCapacityperBeamLength(double beamRatedCoolingCapacityperBeamLength) {
    return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->setBeamRatedCoolingCapacityperBeamLength(beamRatedCoolingCapacityperBeamLength);
  }

  void CoilCoolingFourPipeBeam::resetBeamRatedCoolingCapacityperBeamLength() {
    getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->resetBeamRatedCoolingCapacityperBeamLength();
  }

  // Beam Rated Cooling Room Air Chilled Water Temperature Difference

  double CoilCoolingFourPipeBeam::beamRatedCoolingRoomAirChilledWaterTemperatureDifference() const {
    return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->beamRatedCoolingRoomAirChilledWaterTemperatureDifference();
  }

  bool CoilCoolingFourPipeBeam::isBeamRatedCoolingRoomAirChilledWaterTemperatureDifferenceDefaulted() const {
    return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->isBeamRatedCoolingRoomAirChilledWaterTemperatureDifferenceDefaulted();
  }

  bool CoilCoolingFourPipeBeam::setBeamRatedCoolingRoomAirChilledWaterTemperatureDifference(
    double beamRatedCoolingRoomAirChilledWaterTemperatureDifference) {
    return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->setBeamRatedCoolingRoomAirChilledWaterTemperatureDifference(
      beamRatedCoolingRoomAirChilledWaterTemperatureDifference);
  }

  void CoilCoolingFourPipeBeam::resetBeamRatedCoolingRoomAirChilledWaterTemperatureDifference() {
    getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->resetBeamRatedCoolingRoomAirChilledWaterTemperatureDifference();
  }

  // Beam Rated Chilled Water Volume Flow Rate per Beam Length

  double CoilCoolingFourPipeBeam::beamRatedChilledWaterVolumeFlowRateperBeamLength() const {
    return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->beamRatedChilledWaterVolumeFlowRateperBeamLength();
  }

  bool CoilCoolingFourPipeBeam::isBeamRatedChilledWaterVolumeFlowRateperBeamLengthDefaulted() const {
    return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->isBeamRatedChilledWaterVolumeFlowRateperBeamLengthDefaulted();
  }

  bool CoilCoolingFourPipeBeam::setBeamRatedChilledWaterVolumeFlowRateperBeamLength(double beamRatedChilledWaterVolumeFlowRateperBeamLength) {
    return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->setBeamRatedChilledWaterVolumeFlowRateperBeamLength(
      beamRatedChilledWaterVolumeFlowRateperBeamLength);
  }

  void CoilCoolingFourPipeBeam::resetBeamRatedChilledWaterVolumeFlowRateperBeamLength() {
    getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->resetBeamRatedChilledWaterVolumeFlowRateperBeamLength();
  }

  /* Curves */

  // Beam Cooling Capacity Temperature Difference Modification Factor Curve
  Curve CoilCoolingFourPipeBeam::beamCoolingCapacityTemperatureDifferenceModificationFactorCurve() const {
    return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->beamCoolingCapacityTemperatureDifferenceModificationFactorCurve();
  }

  bool CoilCoolingFourPipeBeam::setBeamCoolingCapacityTemperatureDifferenceModificationFactorCurve(const Curve& curve) {
    return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->setBeamCoolingCapacityTemperatureDifferenceModificationFactorCurve(curve);
  }

  // Beam Cooling Capacity Air Flow Modification Factor Curve

  Curve CoilCoolingFourPipeBeam::beamCoolingCapacityAirFlowModificationFactorCurve() const {
    return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->beamCoolingCapacityAirFlowModificationFactorCurve();
  }

  bool CoilCoolingFourPipeBeam::setBeamCoolingCapacityAirFlowModificationFactorCurve(const Curve& curve) {
    return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->setBeamCoolingCapacityAirFlowModificationFactorCurve(curve);
  }

  // Beam Cooling Capacity Chilled Water Flow Modification Factor Curve

  Curve CoilCoolingFourPipeBeam::beamCoolingCapacityChilledWaterFlowModificationFactorCurve() const {
    return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->beamCoolingCapacityChilledWaterFlowModificationFactorCurve();
  }

  bool CoilCoolingFourPipeBeam::setBeamCoolingCapacityChilledWaterFlowModificationFactorCurve(const Curve& curve) {
    return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->setBeamCoolingCapacityChilledWaterFlowModificationFactorCurve(curve);
  }

  /* Convenience functions */

  boost::optional<AirTerminalSingleDuctConstantVolumeFourPipeBeam> CoilCoolingFourPipeBeam::airTerminalSingleDuctConstantVolumeFourPipeBeam() const {
    return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->airTerminalSingleDuctConstantVolumeFourPipeBeam();
  }

  /// @cond
  CoilCoolingFourPipeBeam::CoilCoolingFourPipeBeam(std::shared_ptr<detail::CoilCoolingFourPipeBeam_Impl> impl) : StraightComponent(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
