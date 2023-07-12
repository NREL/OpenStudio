/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
#include "TableLookup.hpp"
#include "TableIndependentVariable.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Coil_Cooling_FourPipeBeam_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

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

    ComponentType CoilCoolingFourPipeBeam_Impl::componentType() const {
      return ComponentType::Cooling;
    }

    std::vector<FuelType> CoilCoolingFourPipeBeam_Impl::coolingFuelTypes() const {
      if (auto p_ = plantLoop()) {
        return p_->coolingFuelTypes();
      }
      return {};
    }

    std::vector<FuelType> CoilCoolingFourPipeBeam_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> CoilCoolingFourPipeBeam_Impl::appGHeatingFuelTypes() const {
      return {};
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
    TableLookup coolCapModFuncOfSAFlow(model);
    {
      coolCapModFuncOfSAFlow.setName("CoolCapModFuncOfSAFlow");
      coolCapModFuncOfSAFlow.setMinimumOutput(0.8234);
      coolCapModFuncOfSAFlow.setMaximumOutput(1.1256);
      coolCapModFuncOfSAFlow.setOutputUnitType("Dimensionless");
      coolCapModFuncOfSAFlow.setOutputValues({0.823403, 1.0, 1.1256});

      TableIndependentVariable coolCapModFuncOfSAFlowVar1(model);
      coolCapModFuncOfSAFlow.addIndependentVariable(coolCapModFuncOfSAFlowVar1);
      coolCapModFuncOfSAFlowVar1.setName("CoolCapModFuncOfWaterFlow_IndependentVariable1");
      coolCapModFuncOfSAFlowVar1.setInterpolationMethod("Cubic");
      coolCapModFuncOfSAFlowVar1.setExtrapolationMethod("Constant");
      coolCapModFuncOfSAFlowVar1.setMinimumValue(0.714);
      coolCapModFuncOfSAFlowVar1.setMaximumValue(1.2857);
      coolCapModFuncOfSAFlowVar1.setUnitType("Dimensionless");
      coolCapModFuncOfSAFlowVar1.setValues({0.714286, 1.0, 1.2857});
    }

    ok = setBeamCoolingCapacityAirFlowModificationFactorCurve(coolCapModFuncOfSAFlow);
    OS_ASSERT(ok);

    // Beam Cooling Capacity Chilled Water Flow Modification Factor Curve
    // CapModFuncOfWaterFlow: Table:OneIndependentVariable

    TableLookup capModFuncOfWaterFlow(model);
    {
      capModFuncOfWaterFlow.setName("CoolCapModFuncOfWaterFlow");
      capModFuncOfWaterFlow.setMinimumOutput(0.0);
      capModFuncOfWaterFlow.setMaximumOutput(1.04);
      capModFuncOfWaterFlow.setOutputUnitType("Dimensionless");
      capModFuncOfWaterFlow.setOutputValues({0.0, 0.001, 0.71, 0.85, 0.92, 0.97, 1.0, 1.04});

      TableIndependentVariable capModFuncOfWaterFlowVar1(model);
      capModFuncOfWaterFlow.addIndependentVariable(capModFuncOfWaterFlowVar1);
      capModFuncOfWaterFlowVar1.setName("CoolCapModFuncOfWaterFlow_IndependentVariable1");
      capModFuncOfWaterFlowVar1.setInterpolationMethod("Cubic");
      capModFuncOfWaterFlowVar1.setExtrapolationMethod("Constant");
      capModFuncOfWaterFlowVar1.setMinimumValue(0.0);
      capModFuncOfWaterFlowVar1.setMaximumValue(1.33);
      capModFuncOfWaterFlowVar1.setUnitType("Dimensionless");
      capModFuncOfWaterFlowVar1.setValues({0.0, 0.05, 0.33333, 0.5, 0.666667, 0.833333, 1.0, 1.333333});
    }

    ok = setBeamCoolingCapacityChilledWaterFlowModificationFactorCurve(capModFuncOfWaterFlow);
    OS_ASSERT(ok);
  }

  IddObjectType CoilCoolingFourPipeBeam::iddObjectType() {
    return {IddObjectType::OS_Coil_Cooling_FourPipeBeam};
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
