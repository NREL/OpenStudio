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

#include "CoilHeatingFourPipeBeam.hpp"
#include "CoilHeatingFourPipeBeam_Impl.hpp"

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
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilHeatingFourPipeBeam_Impl::iddObjectType() const {
    return CoilHeatingFourPipeBeam::iddObjectType();
  }

  boost::optional<Connection> CoilHeatingFourPipeBeam_Impl::hotWaterInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Coil_Heating_FourPipeBeamFields::HotWaterInletNodeName);
  }

  boost::optional<Connection> CoilHeatingFourPipeBeam_Impl::hotWaterOutletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Coil_Heating_FourPipeBeamFields::HotWaterOutletNodeName);
  }

  double CoilHeatingFourPipeBeam_Impl::beamRatedHeatingCapacityperBeamLength() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_FourPipeBeamFields::BeamRatedHeatingCapacityperBeamLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilHeatingFourPipeBeam_Impl::isBeamRatedHeatingCapacityperBeamLengthDefaulted() const {
    return isEmpty(OS_Coil_Heating_FourPipeBeamFields::BeamRatedHeatingCapacityperBeamLength);
  }

  double CoilHeatingFourPipeBeam_Impl::beamRatedHeatingRoomAirHotWaterTemperatureDifference() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_FourPipeBeamFields::BeamRatedHeatingRoomAirHotWaterTemperatureDifference,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilHeatingFourPipeBeam_Impl::isBeamRatedHeatingRoomAirHotWaterTemperatureDifferenceDefaulted() const {
    return isEmpty(OS_Coil_Heating_FourPipeBeamFields::BeamRatedHeatingRoomAirHotWaterTemperatureDifference);
  }

  double CoilHeatingFourPipeBeam_Impl::beamRatedHotWaterVolumeFlowRateperBeamLength() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_FourPipeBeamFields::BeamRatedHotWaterVolumeFlowRateperBeamLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilHeatingFourPipeBeam_Impl::isBeamRatedHotWaterVolumeFlowRateperBeamLengthDefaulted() const {
    return isEmpty(OS_Coil_Heating_FourPipeBeamFields::BeamRatedHotWaterVolumeFlowRateperBeamLength);
  }

  boost::optional<UniVariateCurves> CoilHeatingFourPipeBeam_Impl::beamHeatingCapacityTemperatureDifferenceModificationFactorCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<UniVariateCurves>(OS_Coil_Heating_FourPipeBeamFields::BeamHeatingCapacityTemperatureDifferenceModificationFactorCurveName);
  }

  boost::optional<UniVariateCurves> CoilHeatingFourPipeBeam_Impl::beamHeatingCapacityAirFlowModificationFactorCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<UniVariateCurves>(OS_Coil_Heating_FourPipeBeamFields::BeamHeatingCapacityAirFlowModificationFactorCurveName);
  }

  boost::optional<UniVariateCurves> CoilHeatingFourPipeBeam_Impl::beamHeatingCapacityHotWaterFlowModificationFactorCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<UniVariateCurves>(OS_Coil_Heating_FourPipeBeamFields::BeamHeatingCapacityHotWaterFlowModificationFactorCurveName);
  }

  bool CoilHeatingFourPipeBeam_Impl::setHotWaterInletNode(const Connection& connection) {
    bool result = setPointer(OS_Coil_Heating_FourPipeBeamFields::HotWaterInletNodeName, connection.handle());
    return result;
  }

  void CoilHeatingFourPipeBeam_Impl::resetHotWaterInletNode() {
    bool result = setString(OS_Coil_Heating_FourPipeBeamFields::HotWaterInletNodeName, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingFourPipeBeam_Impl::setHotWaterOutletNode(const Connection& connection) {
    bool result = setPointer(OS_Coil_Heating_FourPipeBeamFields::HotWaterOutletNodeName, connection.handle());
    return result;
  }

  void CoilHeatingFourPipeBeam_Impl::resetHotWaterOutletNode() {
    bool result = setString(OS_Coil_Heating_FourPipeBeamFields::HotWaterOutletNodeName, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingFourPipeBeam_Impl::setBeamRatedHeatingCapacityperBeamLength(double beamRatedHeatingCapacityperBeamLength) {
    bool result = setDouble(OS_Coil_Heating_FourPipeBeamFields::BeamRatedHeatingCapacityperBeamLength, beamRatedHeatingCapacityperBeamLength);
    return result;
  }

  void CoilHeatingFourPipeBeam_Impl::resetBeamRatedHeatingCapacityperBeamLength() {
    bool result = setString(OS_Coil_Heating_FourPipeBeamFields::BeamRatedHeatingCapacityperBeamLength, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingFourPipeBeam_Impl::setBeamRatedHeatingRoomAirHotWaterTemperatureDifference(double beamRatedHeatingRoomAirHotWaterTemperatureDifference) {
    bool result = setDouble(OS_Coil_Heating_FourPipeBeamFields::BeamRatedHeatingRoomAirHotWaterTemperatureDifference, beamRatedHeatingRoomAirHotWaterTemperatureDifference);
    return result;
  }

  void CoilHeatingFourPipeBeam_Impl::resetBeamRatedHeatingRoomAirHotWaterTemperatureDifference() {
    bool result = setString(OS_Coil_Heating_FourPipeBeamFields::BeamRatedHeatingRoomAirHotWaterTemperatureDifference, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingFourPipeBeam_Impl::setBeamRatedHotWaterVolumeFlowRateperBeamLength(double beamRatedHotWaterVolumeFlowRateperBeamLength) {
    bool result = setDouble(OS_Coil_Heating_FourPipeBeamFields::BeamRatedHotWaterVolumeFlowRateperBeamLength, beamRatedHotWaterVolumeFlowRateperBeamLength);
    return result;
  }

  void CoilHeatingFourPipeBeam_Impl::resetBeamRatedHotWaterVolumeFlowRateperBeamLength() {
    bool result = setString(OS_Coil_Heating_FourPipeBeamFields::BeamRatedHotWaterVolumeFlowRateperBeamLength, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingFourPipeBeam_Impl::setBeamHeatingCapacityTemperatureDifferenceModificationFactorCurve(const UniVariateCurves& uniVariateCurves) {
    bool result = setPointer(OS_Coil_Heating_FourPipeBeamFields::BeamHeatingCapacityTemperatureDifferenceModificationFactorCurveName, uniVariateCurves.handle());
    return result;
  }

  void CoilHeatingFourPipeBeam_Impl::resetBeamHeatingCapacityTemperatureDifferenceModificationFactorCurve() {
    bool result = setString(OS_Coil_Heating_FourPipeBeamFields::BeamHeatingCapacityTemperatureDifferenceModificationFactorCurveName, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingFourPipeBeam_Impl::setBeamHeatingCapacityAirFlowModificationFactorCurve(const UniVariateCurves& uniVariateCurves) {
    bool result = setPointer(OS_Coil_Heating_FourPipeBeamFields::BeamHeatingCapacityAirFlowModificationFactorCurveName, uniVariateCurves.handle());
    return result;
  }

  void CoilHeatingFourPipeBeam_Impl::resetBeamHeatingCapacityAirFlowModificationFactorCurve() {
    bool result = setString(OS_Coil_Heating_FourPipeBeamFields::BeamHeatingCapacityAirFlowModificationFactorCurveName, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingFourPipeBeam_Impl::setBeamHeatingCapacityHotWaterFlowModificationFactorCurve(const UniVariateCurves& uniVariateCurves) {
    bool result = setPointer(OS_Coil_Heating_FourPipeBeamFields::BeamHeatingCapacityHotWaterFlowModificationFactorCurveName, uniVariateCurves.handle());
    return result;
  }

  void CoilHeatingFourPipeBeam_Impl::resetBeamHeatingCapacityHotWaterFlowModificationFactorCurve() {
    bool result = setString(OS_Coil_Heating_FourPipeBeamFields::BeamHeatingCapacityHotWaterFlowModificationFactorCurveName, "");
    OS_ASSERT(result);
  }

} // detail

CoilHeatingFourPipeBeam::CoilHeatingFourPipeBeam(const Model& model)
  : StraightComponent(CoilHeatingFourPipeBeam::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingFourPipeBeam_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
}

IddObjectType CoilHeatingFourPipeBeam::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Heating_FourPipeBeam);
}

boost::optional<Connection> CoilHeatingFourPipeBeam::hotWaterInletNode() const {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->hotWaterInletNode();
}

boost::optional<Connection> CoilHeatingFourPipeBeam::hotWaterOutletNode() const {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->hotWaterOutletNode();
}

double CoilHeatingFourPipeBeam::beamRatedHeatingCapacityperBeamLength() const {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->beamRatedHeatingCapacityperBeamLength();
}

bool CoilHeatingFourPipeBeam::isBeamRatedHeatingCapacityperBeamLengthDefaulted() const {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->isBeamRatedHeatingCapacityperBeamLengthDefaulted();
}

double CoilHeatingFourPipeBeam::beamRatedHeatingRoomAirHotWaterTemperatureDifference() const {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->beamRatedHeatingRoomAirHotWaterTemperatureDifference();
}

bool CoilHeatingFourPipeBeam::isBeamRatedHeatingRoomAirHotWaterTemperatureDifferenceDefaulted() const {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->isBeamRatedHeatingRoomAirHotWaterTemperatureDifferenceDefaulted();
}

double CoilHeatingFourPipeBeam::beamRatedHotWaterVolumeFlowRateperBeamLength() const {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->beamRatedHotWaterVolumeFlowRateperBeamLength();
}

bool CoilHeatingFourPipeBeam::isBeamRatedHotWaterVolumeFlowRateperBeamLengthDefaulted() const {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->isBeamRatedHotWaterVolumeFlowRateperBeamLengthDefaulted();
}

boost::optional<UniVariateCurves> CoilHeatingFourPipeBeam::beamHeatingCapacityTemperatureDifferenceModificationFactorCurve() const {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->beamHeatingCapacityTemperatureDifferenceModificationFactorCurve();
}

boost::optional<UniVariateCurves> CoilHeatingFourPipeBeam::beamHeatingCapacityAirFlowModificationFactorCurve() const {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->beamHeatingCapacityAirFlowModificationFactorCurve();
}

boost::optional<UniVariateCurves> CoilHeatingFourPipeBeam::beamHeatingCapacityHotWaterFlowModificationFactorCurve() const {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->beamHeatingCapacityHotWaterFlowModificationFactorCurve();
}

bool CoilHeatingFourPipeBeam::setHotWaterInletNode(const Connection& connection) {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->setHotWaterInletNode(connection);
}

void CoilHeatingFourPipeBeam::resetHotWaterInletNode() {
  getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->resetHotWaterInletNode();
}

bool CoilHeatingFourPipeBeam::setHotWaterOutletNode(const Connection& connection) {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->setHotWaterOutletNode(connection);
}

void CoilHeatingFourPipeBeam::resetHotWaterOutletNode() {
  getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->resetHotWaterOutletNode();
}

bool CoilHeatingFourPipeBeam::setBeamRatedHeatingCapacityperBeamLength(double beamRatedHeatingCapacityperBeamLength) {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->setBeamRatedHeatingCapacityperBeamLength(beamRatedHeatingCapacityperBeamLength);
}

void CoilHeatingFourPipeBeam::resetBeamRatedHeatingCapacityperBeamLength() {
  getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->resetBeamRatedHeatingCapacityperBeamLength();
}

bool CoilHeatingFourPipeBeam::setBeamRatedHeatingRoomAirHotWaterTemperatureDifference(double beamRatedHeatingRoomAirHotWaterTemperatureDifference) {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->setBeamRatedHeatingRoomAirHotWaterTemperatureDifference(beamRatedHeatingRoomAirHotWaterTemperatureDifference);
}

void CoilHeatingFourPipeBeam::resetBeamRatedHeatingRoomAirHotWaterTemperatureDifference() {
  getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->resetBeamRatedHeatingRoomAirHotWaterTemperatureDifference();
}

bool CoilHeatingFourPipeBeam::setBeamRatedHotWaterVolumeFlowRateperBeamLength(double beamRatedHotWaterVolumeFlowRateperBeamLength) {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->setBeamRatedHotWaterVolumeFlowRateperBeamLength(beamRatedHotWaterVolumeFlowRateperBeamLength);
}

void CoilHeatingFourPipeBeam::resetBeamRatedHotWaterVolumeFlowRateperBeamLength() {
  getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->resetBeamRatedHotWaterVolumeFlowRateperBeamLength();
}

bool CoilHeatingFourPipeBeam::setBeamHeatingCapacityTemperatureDifferenceModificationFactorCurve(const UniVariateCurves& uniVariateCurves) {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->setBeamHeatingCapacityTemperatureDifferenceModificationFactorCurve(uniVariateCurves);
}

void CoilHeatingFourPipeBeam::resetBeamHeatingCapacityTemperatureDifferenceModificationFactorCurve() {
  getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->resetBeamHeatingCapacityTemperatureDifferenceModificationFactorCurve();
}

bool CoilHeatingFourPipeBeam::setBeamHeatingCapacityAirFlowModificationFactorCurve(const UniVariateCurves& uniVariateCurves) {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->setBeamHeatingCapacityAirFlowModificationFactorCurve(uniVariateCurves);
}

void CoilHeatingFourPipeBeam::resetBeamHeatingCapacityAirFlowModificationFactorCurve() {
  getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->resetBeamHeatingCapacityAirFlowModificationFactorCurve();
}

bool CoilHeatingFourPipeBeam::setBeamHeatingCapacityHotWaterFlowModificationFactorCurve(const UniVariateCurves& uniVariateCurves) {
  return getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->setBeamHeatingCapacityHotWaterFlowModificationFactorCurve(uniVariateCurves);
}

void CoilHeatingFourPipeBeam::resetBeamHeatingCapacityHotWaterFlowModificationFactorCurve() {
  getImpl<detail::CoilHeatingFourPipeBeam_Impl>()->resetBeamHeatingCapacityHotWaterFlowModificationFactorCurve();
}

/// @cond
CoilHeatingFourPipeBeam::CoilHeatingFourPipeBeam(std::shared_ptr<detail::CoilHeatingFourPipeBeam_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

