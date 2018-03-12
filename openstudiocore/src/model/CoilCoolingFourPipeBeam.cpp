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

// TODO: Check the following class names against object getters and setters.
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "UniVariateCurves.hpp"
#include "UniVariateCurves_Impl.hpp"
#include "UniVariateCurves.hpp"
#include "UniVariateCurves_Impl.hpp"
#include "UniVariateCurves.hpp"
#include "UniVariateCurves_Impl.hpp"

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
    }
    return result;
  }

  IddObjectType CoilCoolingFourPipeBeam_Impl::iddObjectType() const {
    return CoilCoolingFourPipeBeam::iddObjectType();
  }

  boost::optional<Connection> CoilCoolingFourPipeBeam_Impl::chilledWaterInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Coil_Cooling_FourPipeBeamFields::ChilledWaterInletNodeName);
  }

  boost::optional<Connection> CoilCoolingFourPipeBeam_Impl::chilledWaterOutletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Coil_Cooling_FourPipeBeamFields::ChilledWaterOutletNodeName);
  }

  double CoilCoolingFourPipeBeam_Impl::beamRatedCoolingCapacityperBeamLength() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedCoolingCapacityperBeamLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingFourPipeBeam_Impl::isBeamRatedCoolingCapacityperBeamLengthDefaulted() const {
    return isEmpty(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedCoolingCapacityperBeamLength);
  }

  double CoilCoolingFourPipeBeam_Impl::beamRatedCoolingRoomAirChilledWaterTemperatureDifference() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedCoolingRoomAirChilledWaterTemperatureDifference,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingFourPipeBeam_Impl::isBeamRatedCoolingRoomAirChilledWaterTemperatureDifferenceDefaulted() const {
    return isEmpty(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedCoolingRoomAirChilledWaterTemperatureDifference);
  }

  double CoilCoolingFourPipeBeam_Impl::beamRatedChilledWaterVolumeFlowRateperBeamLength() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedChilledWaterVolumeFlowRateperBeamLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingFourPipeBeam_Impl::isBeamRatedChilledWaterVolumeFlowRateperBeamLengthDefaulted() const {
    return isEmpty(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedChilledWaterVolumeFlowRateperBeamLength);
  }

  boost::optional<UniVariateCurves> CoilCoolingFourPipeBeam_Impl::beamCoolingCapacityTemperatureDifferenceModificationFactorCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<UniVariateCurves>(OS_Coil_Cooling_FourPipeBeamFields::BeamCoolingCapacityTemperatureDifferenceModificationFactorCurveName);
  }

  boost::optional<UniVariateCurves> CoilCoolingFourPipeBeam_Impl::beamCoolingCapacityAirFlowModificationFactorCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<UniVariateCurves>(OS_Coil_Cooling_FourPipeBeamFields::BeamCoolingCapacityAirFlowModificationFactorCurveName);
  }

  boost::optional<UniVariateCurves> CoilCoolingFourPipeBeam_Impl::beamCoolingCapacityChilledWaterFlowModificationFactorCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<UniVariateCurves>(OS_Coil_Cooling_FourPipeBeamFields::BeamCoolingCapacityChilledWaterFlowModificationFactorCurveName);
  }

  bool CoilCoolingFourPipeBeam_Impl::setChilledWaterInletNode(const Connection& connection) {
    bool result = setPointer(OS_Coil_Cooling_FourPipeBeamFields::ChilledWaterInletNodeName, connection.handle());
    return result;
  }

  void CoilCoolingFourPipeBeam_Impl::resetChilledWaterInletNode() {
    bool result = setString(OS_Coil_Cooling_FourPipeBeamFields::ChilledWaterInletNodeName, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingFourPipeBeam_Impl::setChilledWaterOutletNode(const Connection& connection) {
    bool result = setPointer(OS_Coil_Cooling_FourPipeBeamFields::ChilledWaterOutletNodeName, connection.handle());
    return result;
  }

  void CoilCoolingFourPipeBeam_Impl::resetChilledWaterOutletNode() {
    bool result = setString(OS_Coil_Cooling_FourPipeBeamFields::ChilledWaterOutletNodeName, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingFourPipeBeam_Impl::setBeamRatedCoolingCapacityperBeamLength(double beamRatedCoolingCapacityperBeamLength) {
    bool result = setDouble(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedCoolingCapacityperBeamLength, beamRatedCoolingCapacityperBeamLength);
    return result;
  }

  void CoilCoolingFourPipeBeam_Impl::resetBeamRatedCoolingCapacityperBeamLength() {
    bool result = setString(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedCoolingCapacityperBeamLength, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingFourPipeBeam_Impl::setBeamRatedCoolingRoomAirChilledWaterTemperatureDifference(double beamRatedCoolingRoomAirChilledWaterTemperatureDifference) {
    bool result = setDouble(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedCoolingRoomAirChilledWaterTemperatureDifference, beamRatedCoolingRoomAirChilledWaterTemperatureDifference);
    return result;
  }

  void CoilCoolingFourPipeBeam_Impl::resetBeamRatedCoolingRoomAirChilledWaterTemperatureDifference() {
    bool result = setString(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedCoolingRoomAirChilledWaterTemperatureDifference, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingFourPipeBeam_Impl::setBeamRatedChilledWaterVolumeFlowRateperBeamLength(double beamRatedChilledWaterVolumeFlowRateperBeamLength) {
    bool result = setDouble(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedChilledWaterVolumeFlowRateperBeamLength, beamRatedChilledWaterVolumeFlowRateperBeamLength);
    return result;
  }

  void CoilCoolingFourPipeBeam_Impl::resetBeamRatedChilledWaterVolumeFlowRateperBeamLength() {
    bool result = setString(OS_Coil_Cooling_FourPipeBeamFields::BeamRatedChilledWaterVolumeFlowRateperBeamLength, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingFourPipeBeam_Impl::setBeamCoolingCapacityTemperatureDifferenceModificationFactorCurve(const UniVariateCurves& uniVariateCurves) {
    bool result = setPointer(OS_Coil_Cooling_FourPipeBeamFields::BeamCoolingCapacityTemperatureDifferenceModificationFactorCurveName, uniVariateCurves.handle());
    return result;
  }

  void CoilCoolingFourPipeBeam_Impl::resetBeamCoolingCapacityTemperatureDifferenceModificationFactorCurve() {
    bool result = setString(OS_Coil_Cooling_FourPipeBeamFields::BeamCoolingCapacityTemperatureDifferenceModificationFactorCurveName, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingFourPipeBeam_Impl::setBeamCoolingCapacityAirFlowModificationFactorCurve(const UniVariateCurves& uniVariateCurves) {
    bool result = setPointer(OS_Coil_Cooling_FourPipeBeamFields::BeamCoolingCapacityAirFlowModificationFactorCurveName, uniVariateCurves.handle());
    return result;
  }

  void CoilCoolingFourPipeBeam_Impl::resetBeamCoolingCapacityAirFlowModificationFactorCurve() {
    bool result = setString(OS_Coil_Cooling_FourPipeBeamFields::BeamCoolingCapacityAirFlowModificationFactorCurveName, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingFourPipeBeam_Impl::setBeamCoolingCapacityChilledWaterFlowModificationFactorCurve(const UniVariateCurves& uniVariateCurves) {
    bool result = setPointer(OS_Coil_Cooling_FourPipeBeamFields::BeamCoolingCapacityChilledWaterFlowModificationFactorCurveName, uniVariateCurves.handle());
    return result;
  }

  void CoilCoolingFourPipeBeam_Impl::resetBeamCoolingCapacityChilledWaterFlowModificationFactorCurve() {
    bool result = setString(OS_Coil_Cooling_FourPipeBeamFields::BeamCoolingCapacityChilledWaterFlowModificationFactorCurveName, "");
    OS_ASSERT(result);
  }

} // detail

CoilCoolingFourPipeBeam::CoilCoolingFourPipeBeam(const Model& model)
  : StraightComponent(CoilCoolingFourPipeBeam::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingFourPipeBeam_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
}

IddObjectType CoilCoolingFourPipeBeam::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Cooling_FourPipeBeam);
}

boost::optional<Connection> CoilCoolingFourPipeBeam::chilledWaterInletNode() const {
  return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->chilledWaterInletNode();
}

boost::optional<Connection> CoilCoolingFourPipeBeam::chilledWaterOutletNode() const {
  return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->chilledWaterOutletNode();
}

double CoilCoolingFourPipeBeam::beamRatedCoolingCapacityperBeamLength() const {
  return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->beamRatedCoolingCapacityperBeamLength();
}

bool CoilCoolingFourPipeBeam::isBeamRatedCoolingCapacityperBeamLengthDefaulted() const {
  return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->isBeamRatedCoolingCapacityperBeamLengthDefaulted();
}

double CoilCoolingFourPipeBeam::beamRatedCoolingRoomAirChilledWaterTemperatureDifference() const {
  return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->beamRatedCoolingRoomAirChilledWaterTemperatureDifference();
}

bool CoilCoolingFourPipeBeam::isBeamRatedCoolingRoomAirChilledWaterTemperatureDifferenceDefaulted() const {
  return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->isBeamRatedCoolingRoomAirChilledWaterTemperatureDifferenceDefaulted();
}

double CoilCoolingFourPipeBeam::beamRatedChilledWaterVolumeFlowRateperBeamLength() const {
  return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->beamRatedChilledWaterVolumeFlowRateperBeamLength();
}

bool CoilCoolingFourPipeBeam::isBeamRatedChilledWaterVolumeFlowRateperBeamLengthDefaulted() const {
  return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->isBeamRatedChilledWaterVolumeFlowRateperBeamLengthDefaulted();
}

boost::optional<UniVariateCurves> CoilCoolingFourPipeBeam::beamCoolingCapacityTemperatureDifferenceModificationFactorCurve() const {
  return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->beamCoolingCapacityTemperatureDifferenceModificationFactorCurve();
}

boost::optional<UniVariateCurves> CoilCoolingFourPipeBeam::beamCoolingCapacityAirFlowModificationFactorCurve() const {
  return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->beamCoolingCapacityAirFlowModificationFactorCurve();
}

boost::optional<UniVariateCurves> CoilCoolingFourPipeBeam::beamCoolingCapacityChilledWaterFlowModificationFactorCurve() const {
  return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->beamCoolingCapacityChilledWaterFlowModificationFactorCurve();
}

bool CoilCoolingFourPipeBeam::setChilledWaterInletNode(const Connection& connection) {
  return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->setChilledWaterInletNode(connection);
}

void CoilCoolingFourPipeBeam::resetChilledWaterInletNode() {
  getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->resetChilledWaterInletNode();
}

bool CoilCoolingFourPipeBeam::setChilledWaterOutletNode(const Connection& connection) {
  return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->setChilledWaterOutletNode(connection);
}

void CoilCoolingFourPipeBeam::resetChilledWaterOutletNode() {
  getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->resetChilledWaterOutletNode();
}

bool CoilCoolingFourPipeBeam::setBeamRatedCoolingCapacityperBeamLength(double beamRatedCoolingCapacityperBeamLength) {
  return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->setBeamRatedCoolingCapacityperBeamLength(beamRatedCoolingCapacityperBeamLength);
}

void CoilCoolingFourPipeBeam::resetBeamRatedCoolingCapacityperBeamLength() {
  getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->resetBeamRatedCoolingCapacityperBeamLength();
}

bool CoilCoolingFourPipeBeam::setBeamRatedCoolingRoomAirChilledWaterTemperatureDifference(double beamRatedCoolingRoomAirChilledWaterTemperatureDifference) {
  return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->setBeamRatedCoolingRoomAirChilledWaterTemperatureDifference(beamRatedCoolingRoomAirChilledWaterTemperatureDifference);
}

void CoilCoolingFourPipeBeam::resetBeamRatedCoolingRoomAirChilledWaterTemperatureDifference() {
  getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->resetBeamRatedCoolingRoomAirChilledWaterTemperatureDifference();
}

bool CoilCoolingFourPipeBeam::setBeamRatedChilledWaterVolumeFlowRateperBeamLength(double beamRatedChilledWaterVolumeFlowRateperBeamLength) {
  return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->setBeamRatedChilledWaterVolumeFlowRateperBeamLength(beamRatedChilledWaterVolumeFlowRateperBeamLength);
}

void CoilCoolingFourPipeBeam::resetBeamRatedChilledWaterVolumeFlowRateperBeamLength() {
  getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->resetBeamRatedChilledWaterVolumeFlowRateperBeamLength();
}

bool CoilCoolingFourPipeBeam::setBeamCoolingCapacityTemperatureDifferenceModificationFactorCurve(const UniVariateCurves& uniVariateCurves) {
  return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->setBeamCoolingCapacityTemperatureDifferenceModificationFactorCurve(uniVariateCurves);
}

void CoilCoolingFourPipeBeam::resetBeamCoolingCapacityTemperatureDifferenceModificationFactorCurve() {
  getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->resetBeamCoolingCapacityTemperatureDifferenceModificationFactorCurve();
}

bool CoilCoolingFourPipeBeam::setBeamCoolingCapacityAirFlowModificationFactorCurve(const UniVariateCurves& uniVariateCurves) {
  return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->setBeamCoolingCapacityAirFlowModificationFactorCurve(uniVariateCurves);
}

void CoilCoolingFourPipeBeam::resetBeamCoolingCapacityAirFlowModificationFactorCurve() {
  getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->resetBeamCoolingCapacityAirFlowModificationFactorCurve();
}

bool CoilCoolingFourPipeBeam::setBeamCoolingCapacityChilledWaterFlowModificationFactorCurve(const UniVariateCurves& uniVariateCurves) {
  return getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->setBeamCoolingCapacityChilledWaterFlowModificationFactorCurve(uniVariateCurves);
}

void CoilCoolingFourPipeBeam::resetBeamCoolingCapacityChilledWaterFlowModificationFactorCurve() {
  getImpl<detail::CoilCoolingFourPipeBeam_Impl>()->resetBeamCoolingCapacityChilledWaterFlowModificationFactorCurve();
}

/// @cond
CoilCoolingFourPipeBeam::CoilCoolingFourPipeBeam(std::shared_ptr<detail::CoilCoolingFourPipeBeam_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

