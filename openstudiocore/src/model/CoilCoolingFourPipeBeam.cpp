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


#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Coil_Cooling_FourPipeBeam_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilCoolingFourPipeBeam_Impl::CoilCoolingFourPipeBeam_Impl(const IdfObject& idfObject,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilCoolingFourPipeBeam::iddObjectType());
  }

  CoilCoolingFourPipeBeam_Impl::CoilCoolingFourPipeBeam_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilCoolingFourPipeBeam::iddObjectType());
  }

  CoilCoolingFourPipeBeam_Impl::CoilCoolingFourPipeBeam_Impl(const CoilCoolingFourPipeBeam_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilCoolingFourPipeBeam_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
      // None: all in parent AirTerminalSingleDuctConstantVolumeFourPipeBeam
    }
    return result;
  }

  IddObjectType CoilCoolingFourPipeBeam_Impl::iddObjectType() const {
    return CoilCoolingFourPipeBeam::iddObjectType();
  }

  unsigned CoilCoolingFourPipeBeam_Impl::inletPort()
  {
    return OS_Coil_Cooling_FourPipeBeamFields::ChilledWaterInletNodeName;
  }

  unsigned CoilCoolingFourPipeBeam_Impl::outletPort()
  {
    return OS_Coil_Cooling_FourPipeBeamFields::ChilledWaterOutletNodeName;
  }


  // contaningHVACComponent is important: used for airLoopHVAC::addBranchForZone to connect the coil to the right loop
  boost::optional<HVACComponent> CoilCoolingFourPipeBeam_Impl::containingHVACComponent() const
  {
    // return this->airTerminalSingleDuctConstantVolumeFourPipeBeam();
    std::vector<AirTerminalSingleDuctConstantVolumeFourPipeBeam> atuFourPipeBeams;

    atuFourPipeBeams = this->model().getConcreteModelObjects<AirTerminalSingleDuctConstantVolumeFourPipeBeam>();

    for( const auto & elem : atuFourPipeBeams )
    {
      if( boost::optional<CoilCoolingFourPipeBeam> coil = elem.coolingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return elem;
        }
      }
    }
    return boost::none;
  }

  boost::optional<StraightComponent> CoilCoolingFourPipeBeam_Impl::containingStraightComponent() const
  {
    // return this->airTerminalSingleDuctConstantVolumeFourPipeBeam();
    std::vector<AirTerminalSingleDuctConstantVolumeFourPipeBeam> atuFourPipeBeams;

    atuFourPipeBeams = this->model().getConcreteModelObjects<AirTerminalSingleDuctConstantVolumeFourPipeBeam>();

    for( const auto & elem : atuFourPipeBeams )
    {
      if( boost::optional<CoilCoolingFourPipeBeam> coil = elem.coolingCoil() )
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
  bool CoilCoolingFourPipeBeam_Impl::addToNode(Node & node)
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

  boost::optional<Node> CoilCoolingFourPipeBeam_Impl::chilledWaterInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_Coil_Cooling_FourPipeBeamFields::ChilledWaterInletNodeName);
  }

  boost::optional<Node> CoilCoolingFourPipeBeam_Impl::chilledWaterOutletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_Coil_Cooling_FourPipeBeamFields::ChilledWaterOutletNodeName);
  }


  /* double with default fields */

  // Beam Rated Cooling Capacity per Beam Length

  double CoilCoolingFourPipeBeam_Impl::beamRatedCoolingCapacityperBeamLength() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedCoolingCapacityperBeamLength,true);
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
    boost::optional<double> value = getDouble(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedCoolingRoomAirChilledWaterTemperatureDifference,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingFourPipeBeam_Impl::isBeamRatedCoolingRoomAirChilledWaterTemperatureDifferenceDefaulted() const {
    return isEmpty(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedCoolingRoomAirChilledWaterTemperatureDifference);
  }

  bool CoilCoolingFourPipeBeam_Impl::setBeamRatedCoolingRoomAirChilledWaterTemperatureDifference(double beamRatedCoolingRoomAirChilledWaterTemperatureDifference) {
    bool result = setDouble(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedCoolingRoomAirChilledWaterTemperatureDifference, beamRatedCoolingRoomAirChilledWaterTemperatureDifference);
    return result;
  }

  void CoilCoolingFourPipeBeam_Impl::resetBeamRatedCoolingRoomAirChilledWaterTemperatureDifference() {
    bool result = setString(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedCoolingRoomAirChilledWaterTemperatureDifference, "");
    OS_ASSERT(result);
  }


  // Beam Rated Chilled Water Volume Flow Rate per Beam Length

  double CoilCoolingFourPipeBeam_Impl::beamRatedChilledWaterVolumeFlowRateperBeamLength() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedChilledWaterVolumeFlowRateperBeamLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingFourPipeBeam_Impl::isBeamRatedChilledWaterVolumeFlowRateperBeamLengthDefaulted() const {
    return isEmpty(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedChilledWaterVolumeFlowRateperBeamLength);
  }

  bool CoilCoolingFourPipeBeam_Impl::setBeamRatedChilledWaterVolumeFlowRateperBeamLength(double beamRatedChilledWaterVolumeFlowRateperBeamLength) {
    bool result = setDouble(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedChilledWaterVolumeFlowRateperBeamLength, beamRatedChilledWaterVolumeFlowRateperBeamLength);
    return result;
  }

  void CoilCoolingFourPipeBeam_Impl::resetBeamRatedChilledWaterVolumeFlowRateperBeamLength() {
    bool result = setString(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedChilledWaterVolumeFlowRateperBeamLength, "");
    OS_ASSERT(result);
  }



  /* Curves */

  // Beam Cooling Capacity Temperature Difference Modification Factor Curve

  Curve CoilCoolingFourPipeBeam_Impl::beamCoolingCapacityTemperatureDifferenceModificationFactorCurve() const {
    boost::optional<Curve> c = getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_FourPipeBeamFields::BeamCoolingCapacityTemperatureDifferenceModificationFactorCurveName);
    if (!c) {
      LOG_AND_THROW(briefDescription() << " doesn't have the required Beam Cooling Capacity Temperature Difference Modification Factor Curve");
    }
    OS_ASSERT(c);
    return c;
  }

  bool CoilCoolingFourPipeBeam_Impl::setBeamCoolingCapacityTemperatureDifferenceModificationFactorCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_FourPipeBeamFields::BeamCoolingCapacityTemperatureDifferenceModificationFactorCurveName, curve.handle());
    return result;
  }


  // Beam Cooling Capacity Air Flow Modification Factor Curve

  Curve CoilCoolingFourPipeBeam_Impl::beamCoolingCapacityAirFlowModificationFactorCurve() const {
    boost::optional<Curve> c = getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_FourPipeBeamFields::BeamCoolingCapacityAirFlowModificationFactorCurveName);
    if (!c) {
      LOG_AND_THROW(briefDescription() << " doesn't have the required Beam Cooling Capacity Air Flow Modification Factor Curve");
    }
    OS_ASSERT(c);
    return c;
  }

  bool CoilCoolingFourPipeBeam_Impl::setBeamCoolingCapacityAirFlowModificationFactorCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_FourPipeBeamFields::BeamCoolingCapacityAirFlowModificationFactorCurveName, curve.handle());
    return result;
  }


  // Beam Cooling Capacity Chilled Water Flow Modification Factor Curve

  Curve CoilCoolingFourPipeBeam_Impl::beamCoolingCapacityChilledWaterFlowModificationFactorCurve() const {
    boost::optional<Curve> c = getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_FourPipeBeamFields::BeamCoolingCapacityChilledWaterFlowModificationFactorCurveName);
    if (!c) {
      LOG_AND_THROW(briefDescription() << " doesn't have the required Beam Cooling Capacity Chilled Water Flow Modification Factor Curve");
    }
    OS_ASSERT(c);
    return c;
  }

  bool CoilCoolingFourPipeBeam_Impl::setBeamCoolingCapacityChilledWaterFlowModificationFactorCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_FourPipeBeamFields::BeamCoolingCapacityChilledWaterFlowModificationFactorCurveName, curve.handle());
    return result;
  }



  /*  Convenience functions */

  boost::optional<AirTerminalSingleDuctConstantVolumeFourPipeBeam> CoilCoolingFourPipeBeam_Impl::airTerminalSingleDuctConstantVolumeFourPipeBeam() const {
    // this coil can only be found in a AirTerminalSingleDuctConstantVolumeFourPipeBeam
    // check all AirTerminalSingleDuctConstantVolumeFourPipeBeam in the models, seeing if this coil
    // is inside of one of them.  Return the one it is inside of.

    std::vector<AirTerminalSingleDuctConstantVolumeFourPipeBeam> atuFourPipeBeams;

    atuFourPipeBeams = this->model().getConcreteModelObjects<AirTerminalSingleDuctConstantVolumeFourPipeBeam>();

    for( const auto & elem : atuFourPipeBeams )
    {
      if( boost::optional<CoilCoolingFourPipeBeam> coil = elem.coolingCoil() )
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

/* Ctor: nothing particular to do */
CoilCoolingFourPipeBeam::CoilCoolingFourPipeBeam(const Model& model)
  : StraightComponent(CoilCoolingFourPipeBeam::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingFourPipeBeam_Impl>());
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

bool CoilCoolingFourPipeBeam::setBeamRatedCoolingRoomAirChilledWaterTemperatureDifference(double beamRatedCoolingRoomAirChilledWaterTemperatureDifference) {
  return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->setBeamRatedCoolingRoomAirChilledWaterTemperatureDifference(beamRatedCoolingRoomAirChilledWaterTemperatureDifference);
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
  return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->setBeamRatedChilledWaterVolumeFlowRateperBeamLength(beamRatedChilledWaterVolumeFlowRateperBeamLength);
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
CoilCoolingFourPipeBeam::CoilCoolingFourPipeBeam(std::shared_ptr<detail::CoilCoolingFourPipeBeam_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

