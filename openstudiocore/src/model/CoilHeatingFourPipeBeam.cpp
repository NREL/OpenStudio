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

#include "CoilHeatingFourPipeBeam.hpp"
#include "CoilHeatingFourPipeBeam_Impl.hpp"

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
#include <utilities/idd/OS_Coil_Heating_FourPipeBeam_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilHeatingFourPipeBeam_Impl::CoilHeatingFourPipeBeam_Impl(const IdfObject& idfObject,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilHeatingFourPipeBeam::iddObjectType());
  }

  CoilHeatingFourPipeBeam_Impl::CoilHeatingFourPipeBeam_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilHeatingFourPipeBeam::iddObjectType());
  }

  CoilHeatingFourPipeBeam_Impl::CoilHeatingFourPipeBeam_Impl(const CoilHeatingFourPipeBeam_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

const std::vector<std::string>& CoilHeatingFourPipeBeam_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    // None: all in parent AirTerminalSingleDuctConstantVolumeFourPipeBeam
    return result;
  }

  IddObjectType CoilHeatingFourPipeBeam_Impl::iddObjectType() const {
    return CoilHeatingFourPipeBeam::iddObjectType();
  }

  unsigned CoilHeatingFourPipeBeam_Impl::inletPort() const
  {
    return OS_Coil_Heating_FourPipeBeamFields::HotWaterInletNodeName;
  }

  unsigned CoilHeatingFourPipeBeam_Impl::outletPort() const
  {
    return OS_Coil_Heating_FourPipeBeamFields::HotWaterOutletNodeName;
  }


  // contaningHVACComponent is important: used for airLoopHVAC::addBranchForZone to connect the coil to the right loop
  boost::optional<HVACComponent> CoilHeatingFourPipeBeam_Impl::containingHVACComponent() const
  {
    //return this->airTerminalSingleDuctConstantVolumeFourPipeBeam();
    std::vector<AirTerminalSingleDuctConstantVolumeFourPipeBeam> atuFourPipeBeams;

    atuFourPipeBeams = this->model().getConcreteModelObjects<AirTerminalSingleDuctConstantVolumeFourPipeBeam>();

    for( const auto & elem : atuFourPipeBeams )
    {
      if( boost::optional<HVACComponent> coil = elem.heatingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return elem;
        }
      }
    }
    return boost::none;

  }

  boost::optional<StraightComponent> CoilHeatingFourPipeBeam_Impl::containingStraightComponent() const
  {
    //return this->airTerminalSingleDuctConstantVolumeFourPipeBeam();
    std::vector<AirTerminalSingleDuctConstantVolumeFourPipeBeam> atuFourPipeBeams;

    atuFourPipeBeams = this->model().getConcreteModelObjects<AirTerminalSingleDuctConstantVolumeFourPipeBeam>();

    for( const auto & elem : atuFourPipeBeams )
    {
      if( boost::optional<HVACComponent> coil = elem.heatingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return elem;
        }
      }
    }
    return boost::none;

  }

  /* Only allow connection on the demand side of a plantLoop */
  bool CoilHeatingFourPipeBeam_Impl::addToNode(Node & node)
  {
    if( boost::optional<PlantLoop> plant = node.plantLoop() )
    {
      if( plant->demandComponent(node.handle()) )
      {
        return StraightComponent_Impl::addToNode(node);
      }
    }

    return false;
  }



  /* Nodes */

  boost::optional<Node> CoilHeatingFourPipeBeam_Impl::hotWaterInletNode() const {
    unsigned port = inletPort();
    boost::optional<ModelObject> mo = connectedObject(port);
    boost::optional<Node> result;

    if( mo )
    {
      if( boost::optional<Node> node = mo->optionalCast<Node>() )
      {
        result = node;
      }
    }

    return result;
  }

  boost::optional<Node> CoilHeatingFourPipeBeam_Impl::hotWaterOutletNode() const {
    unsigned port = outletPort();

    boost::optional<ModelObject> mo = connectedObject(port);
    boost::optional<Node> result;

    if( mo )
    {
      if( boost::optional<Node> node = mo->optionalCast<Node>() )
      {
        result = node;
      }
    }

    return result;
  }


  /* double with default fields */

  // Beam Rated Heating Capacity per Beam Length

  double CoilHeatingFourPipeBeam_Impl::beamRatedHeatingCapacityperBeamLength() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_FourPipeBeamFields::BeamRatedHeatingCapacityperBeamLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilHeatingFourPipeBeam_Impl::isBeamRatedHeatingCapacityperBeamLengthDefaulted() const {
    return isEmpty(OS_Coil_Heating_FourPipeBeamFields::BeamRatedHeatingCapacityperBeamLength);
  }

  bool CoilHeatingFourPipeBeam_Impl::setBeamRatedHeatingCapacityperBeamLength(double beamRatedHeatingCapacityperBeamLength) {
    bool result = setDouble(OS_Coil_Heating_FourPipeBeamFields::BeamRatedHeatingCapacityperBeamLength, beamRatedHeatingCapacityperBeamLength);
    return result;
  }

  void CoilHeatingFourPipeBeam_Impl::resetBeamRatedHeatingCapacityperBeamLength() {
    bool result = setString(OS_Coil_Heating_FourPipeBeamFields::BeamRatedHeatingCapacityperBeamLength, "");
    OS_ASSERT(result);
  }


  // Beam Rated Heating Room Air Hot Water Temperature Difference

  double CoilHeatingFourPipeBeam_Impl::beamRatedHeatingRoomAirHotWaterTemperatureDifference() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_FourPipeBeamFields::BeamRatedHeatingRoomAirHotWaterTemperatureDifference,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilHeatingFourPipeBeam_Impl::isBeamRatedHeatingRoomAirHotWaterTemperatureDifferenceDefaulted() const {
    return isEmpty(OS_Coil_Heating_FourPipeBeamFields::BeamRatedHeatingRoomAirHotWaterTemperatureDifference);
  }

  bool CoilHeatingFourPipeBeam_Impl::setBeamRatedHeatingRoomAirHotWaterTemperatureDifference(double beamRatedHeatingRoomAirHotWaterTemperatureDifference) {
    bool result = setDouble(OS_Coil_Heating_FourPipeBeamFields::BeamRatedHeatingRoomAirHotWaterTemperatureDifference, beamRatedHeatingRoomAirHotWaterTemperatureDifference);
    return result;
  }

  void CoilHeatingFourPipeBeam_Impl::resetBeamRatedHeatingRoomAirHotWaterTemperatureDifference() {
    bool result = setString(OS_Coil_Heating_FourPipeBeamFields::BeamRatedHeatingRoomAirHotWaterTemperatureDifference, "");
    OS_ASSERT(result);
  }


  // Beam Rated Hot Water Volume Flow Rate per Beam Length

  double CoilHeatingFourPipeBeam_Impl::beamRatedHotWaterVolumeFlowRateperBeamLength() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_FourPipeBeamFields::BeamRatedHotWaterVolumeFlowRateperBeamLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilHeatingFourPipeBeam_Impl::isBeamRatedHotWaterVolumeFlowRateperBeamLengthDefaulted() const {
    return isEmpty(OS_Coil_Heating_FourPipeBeamFields::BeamRatedHotWaterVolumeFlowRateperBeamLength);
  }

  bool CoilHeatingFourPipeBeam_Impl::setBeamRatedHotWaterVolumeFlowRateperBeamLength(double beamRatedHotWaterVolumeFlowRateperBeamLength) {
    bool result = setDouble(OS_Coil_Heating_FourPipeBeamFields::BeamRatedHotWaterVolumeFlowRateperBeamLength, beamRatedHotWaterVolumeFlowRateperBeamLength);
    return result;
  }

  void CoilHeatingFourPipeBeam_Impl::resetBeamRatedHotWaterVolumeFlowRateperBeamLength() {
    bool result = setString(OS_Coil_Heating_FourPipeBeamFields::BeamRatedHotWaterVolumeFlowRateperBeamLength, "");
    OS_ASSERT(result);
  }



  /* Curves */

  // Beam Heating Capacity Temperature Difference Modification Factor Curve

  Curve CoilHeatingFourPipeBeam_Impl::beamHeatingCapacityTemperatureDifferenceModificationFactorCurve() const {
    boost::optional<Curve> c = getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_FourPipeBeamFields::BeamHeatingCapacityTemperatureDifferenceModificationFactorCurveName);
    if (!c) {
      LOG_AND_THROW(briefDescription() << " doesn't have the required Beam Heating Capacity Temperature Difference Modification Factor Curve");
    }
    OS_ASSERT(c);
    return c.get();
  }

  bool CoilHeatingFourPipeBeam_Impl::setBeamHeatingCapacityTemperatureDifferenceModificationFactorCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_FourPipeBeamFields::BeamHeatingCapacityTemperatureDifferenceModificationFactorCurveName, curve.handle());
    return result;
  }


  // Beam Heating Capacity Air Flow Modification Factor Curve

  Curve CoilHeatingFourPipeBeam_Impl::beamHeatingCapacityAirFlowModificationFactorCurve() const {
    boost::optional<Curve> c = getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_FourPipeBeamFields::BeamHeatingCapacityAirFlowModificationFactorCurveName);
    if (!c) {
      LOG_AND_THROW(briefDescription() << " doesn't have the required Beam Heating Capacity Air Flow Modification Factor Curve");
    }
    OS_ASSERT(c);
    return c.get();
  }

  bool CoilHeatingFourPipeBeam_Impl::setBeamHeatingCapacityAirFlowModificationFactorCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_FourPipeBeamFields::BeamHeatingCapacityAirFlowModificationFactorCurveName, curve.handle());
    return result;
  }


  // Beam Heating Capacity Hot Water Flow Modification Factor Curve

  Curve CoilHeatingFourPipeBeam_Impl::beamHeatingCapacityHotWaterFlowModificationFactorCurve() const {
    boost::optional<Curve> c = getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_FourPipeBeamFields::BeamHeatingCapacityHotWaterFlowModificationFactorCurveName);
    if (!c) {
      LOG_AND_THROW(briefDescription() << " doesn't have the required Beam Heating Capacity Hot Water Flow Modification Factor Curve");
    }
    OS_ASSERT(c);
    return c.get();
  }

  bool CoilHeatingFourPipeBeam_Impl::setBeamHeatingCapacityHotWaterFlowModificationFactorCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_FourPipeBeamFields::BeamHeatingCapacityHotWaterFlowModificationFactorCurveName, curve.handle());
    return result;
  }



  /*  Convenience functions */

  boost::optional<AirTerminalSingleDuctConstantVolumeFourPipeBeam> CoilHeatingFourPipeBeam_Impl::airTerminalSingleDuctConstantVolumeFourPipeBeam() const {
    // this coil can only be found in a AirTerminalSingleDuctConstantVolumeFourPipeBeam
    // check all AirTerminalSingleDuctConstantVolumeFourPipeBeam in the models, seeing if this coil
    // is inside of one of them.  Return the one it is inside of.

    std::vector<AirTerminalSingleDuctConstantVolumeFourPipeBeam> atuFourPipeBeams;

    atuFourPipeBeams = this->model().getConcreteModelObjects<AirTerminalSingleDuctConstantVolumeFourPipeBeam>();

    for( const auto & elem : atuFourPipeBeams )
    {
      if( boost::optional<HVACComponent> coil = elem.heatingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return elem;
        }
      }
    }
    return boost::none;
  }


} // detail

/* Ctor: defaulting Curves like in the FourPipeBeamLargeOffice.idf Example File from E+ */
CoilHeatingFourPipeBeam::CoilHeatingFourPipeBeam(const Model& model)
  : StraightComponent(CoilHeatingFourPipeBeam::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingFourPipeBeam_Impl>());

  bool ok = true;

  //Beam Heating Capacity Temperature Difference Modification Factor Curve Nam
  CurveLinear capModFuncOfTempDiff(model);
  capModFuncOfTempDiff.setName("CapModFuncOfTempDiff");
  capModFuncOfTempDiff.setCoefficient1Constant(0);
  capModFuncOfTempDiff.setCoefficient2x(1);
  capModFuncOfTempDiff.setMinimumValueofx(0.0);
  capModFuncOfTempDiff.setMaximumValueofx(1.5);
  capModFuncOfTempDiff.setMinimumCurveOutput(0.0);
  capModFuncOfTempDiff.setMaximumCurveOutput(1.5);

  ok = setBeamHeatingCapacityTemperatureDifferenceModificationFactorCurve(capModFuncOfTempDiff);
  OS_ASSERT(ok);

  // Beam Heating Capacity Air Flow Modification Factor Curve Name
  // HeatCapModFuncOfSAFlow: Table:OneIndependentVariable,

  TableMultiVariableLookup heatCapModFuncOfSAFlow(model, 1);
  heatCapModFuncOfSAFlow.setName("HeatCapModFuncOfSAFlow");
  heatCapModFuncOfSAFlow.setCurveType("Quadratic");
  heatCapModFuncOfSAFlow.setInterpolationMethod("EvaluateCurveToLimits");
  heatCapModFuncOfSAFlow.setMinimumValueofX1(0.714);
  heatCapModFuncOfSAFlow.setMaximumValueofX1(1.2857);
  heatCapModFuncOfSAFlow.setMinimumTableOutput(0.8234);
  heatCapModFuncOfSAFlow.setMaximumTableOutput(1.0778);
  heatCapModFuncOfSAFlow.setInputUnitTypeforX1("Dimensionless");
  heatCapModFuncOfSAFlow.setOutputUnitType("Dimensionless");

  heatCapModFuncOfSAFlow.addPoint(0.714286, 0.8554);
  heatCapModFuncOfSAFlow.addPoint(1.0, 1.0);
  heatCapModFuncOfSAFlow.addPoint(1.2857, 1.0778);

  ok = setBeamHeatingCapacityAirFlowModificationFactorCurve(heatCapModFuncOfSAFlow);
  OS_ASSERT(ok);

  // Beam Heating Capacity Hot Water Flow Modification Factor Curve
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

  ok = setBeamHeatingCapacityHotWaterFlowModificationFactorCurve(capModFuncOfWaterFlow);
  OS_ASSERT(ok);

}

IddObjectType CoilHeatingFourPipeBeam::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Heating_FourPipeBeam);
}


/* Nodes */

boost::optional<Node> CoilHeatingFourPipeBeam::hotWaterInletNode() const {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->hotWaterInletNode();
}

boost::optional<Node> CoilHeatingFourPipeBeam::hotWaterOutletNode() const {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->hotWaterOutletNode();
}


/* double with default fields */

// Beam Rated Heating Capacity per Beam Length

double CoilHeatingFourPipeBeam::beamRatedHeatingCapacityperBeamLength() const {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->beamRatedHeatingCapacityperBeamLength();
}

bool CoilHeatingFourPipeBeam::isBeamRatedHeatingCapacityperBeamLengthDefaulted() const {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->isBeamRatedHeatingCapacityperBeamLengthDefaulted();
}

bool CoilHeatingFourPipeBeam::setBeamRatedHeatingCapacityperBeamLength(double beamRatedHeatingCapacityperBeamLength) {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->setBeamRatedHeatingCapacityperBeamLength(beamRatedHeatingCapacityperBeamLength);
}

void CoilHeatingFourPipeBeam::resetBeamRatedHeatingCapacityperBeamLength() {
  getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->resetBeamRatedHeatingCapacityperBeamLength();
}


// Beam Rated Heating Room Air Hot Water Temperature Difference

double CoilHeatingFourPipeBeam::beamRatedHeatingRoomAirHotWaterTemperatureDifference() const {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->beamRatedHeatingRoomAirHotWaterTemperatureDifference();
}

bool CoilHeatingFourPipeBeam::isBeamRatedHeatingRoomAirHotWaterTemperatureDifferenceDefaulted() const {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->isBeamRatedHeatingRoomAirHotWaterTemperatureDifferenceDefaulted();
}

bool CoilHeatingFourPipeBeam::setBeamRatedHeatingRoomAirHotWaterTemperatureDifference(double beamRatedHeatingRoomAirHotWaterTemperatureDifference) {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->setBeamRatedHeatingRoomAirHotWaterTemperatureDifference(beamRatedHeatingRoomAirHotWaterTemperatureDifference);
}

void CoilHeatingFourPipeBeam::resetBeamRatedHeatingRoomAirHotWaterTemperatureDifference() {
  getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->resetBeamRatedHeatingRoomAirHotWaterTemperatureDifference();
}


// Beam Rated Hot Water Volume Flow Rate per Beam Length

double CoilHeatingFourPipeBeam::beamRatedHotWaterVolumeFlowRateperBeamLength() const {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->beamRatedHotWaterVolumeFlowRateperBeamLength();
}

bool CoilHeatingFourPipeBeam::isBeamRatedHotWaterVolumeFlowRateperBeamLengthDefaulted() const {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->isBeamRatedHotWaterVolumeFlowRateperBeamLengthDefaulted();
}

bool CoilHeatingFourPipeBeam::setBeamRatedHotWaterVolumeFlowRateperBeamLength(double beamRatedHotWaterVolumeFlowRateperBeamLength) {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->setBeamRatedHotWaterVolumeFlowRateperBeamLength(beamRatedHotWaterVolumeFlowRateperBeamLength);
}

void CoilHeatingFourPipeBeam::resetBeamRatedHotWaterVolumeFlowRateperBeamLength() {
  getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->resetBeamRatedHotWaterVolumeFlowRateperBeamLength();
}



/* Curves */

// Beam Heating Capacity Temperature Difference Modification Factor Curve

Curve CoilHeatingFourPipeBeam::beamHeatingCapacityTemperatureDifferenceModificationFactorCurve() const {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->beamHeatingCapacityTemperatureDifferenceModificationFactorCurve();
}

bool CoilHeatingFourPipeBeam::setBeamHeatingCapacityTemperatureDifferenceModificationFactorCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->setBeamHeatingCapacityTemperatureDifferenceModificationFactorCurve(curve);
}


// Beam Heating Capacity Air Flow Modification Factor Curve

Curve CoilHeatingFourPipeBeam::beamHeatingCapacityAirFlowModificationFactorCurve() const {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->beamHeatingCapacityAirFlowModificationFactorCurve();
}

bool CoilHeatingFourPipeBeam::setBeamHeatingCapacityAirFlowModificationFactorCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->setBeamHeatingCapacityAirFlowModificationFactorCurve(curve);
}


// Beam Heating Capacity Hot Water Flow Modification Factor Curve

Curve CoilHeatingFourPipeBeam::beamHeatingCapacityHotWaterFlowModificationFactorCurve() const {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->beamHeatingCapacityHotWaterFlowModificationFactorCurve();
}

bool CoilHeatingFourPipeBeam::setBeamHeatingCapacityHotWaterFlowModificationFactorCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->setBeamHeatingCapacityHotWaterFlowModificationFactorCurve(curve);
}



/* Convenience functions */

boost::optional<AirTerminalSingleDuctConstantVolumeFourPipeBeam> CoilHeatingFourPipeBeam::airTerminalSingleDuctConstantVolumeFourPipeBeam() const {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->airTerminalSingleDuctConstantVolumeFourPipeBeam();
}



/// @cond
CoilHeatingFourPipeBeam::CoilHeatingFourPipeBeam(std::shared_ptr<detail::CoilHeatingFourPipeBeam_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

