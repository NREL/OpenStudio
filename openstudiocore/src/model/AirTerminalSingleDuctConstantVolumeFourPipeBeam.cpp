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

#include "AirTerminalSingleDuctConstantVolumeFourPipeBeam.hpp"
#include "AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "CoolingCoilFourPipeBeam.hpp"
#include "CoolingCoilFourPipeBeam_Impl.hpp"
#include "HeatingCoilFourPipeBeam.hpp"
#include "HeatingCoilFourPipeBeam_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeam_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl(const IdfObject& idfObject,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirTerminalSingleDuctConstantVolumeFourPipeBeam::iddObjectType());
  }

  AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirTerminalSingleDuctConstantVolumeFourPipeBeam::iddObjectType());
  }

  AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl(const AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl& other,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::iddObjectType() const {
    return AirTerminalSingleDuctConstantVolumeFourPipeBeam::iddObjectType();
  }

  std::vector<ScheduleTypeKey> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirAvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("AirTerminalSingleDuctConstantVolumeFourPipeBeam","Primary Air Availability"));
    }
    if (std::find(b,e,OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::CoolingAvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("AirTerminalSingleDuctConstantVolumeFourPipeBeam","Cooling Availability"));
    }
    if (std::find(b,e,OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::HeatingAvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("AirTerminalSingleDuctConstantVolumeFourPipeBeam","Heating Availability"));
    }
    return result;
  }

  boost::optional<Schedule> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::primaryAirAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirAvailabilityScheduleName);
  }

  boost::optional<Schedule> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::coolingAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::CoolingAvailabilityScheduleName);
  }

  boost::optional<Schedule> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::heatingAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::HeatingAvailabilityScheduleName);
  }

  Connection AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::primaryAirInletNode() const {
    boost::optional<Connection> value = optionalPrimaryAirInletNode();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Primary Air Inlet Node attached.");
    }
    return value.get();
  }

  Connection AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::primaryAirOutletNode() const {
    boost::optional<Connection> value = optionalPrimaryAirOutletNode();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Primary Air Outlet Node attached.");
    }
    return value.get();
  }

  CoolingCoilFourPipeBeam AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::coolingCoil() const {
    boost::optional<CoolingCoilFourPipeBeam> value = optionalCoolingCoil();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Cooling Coil attached.");
    }
    return value.get();
  }

  HeatingCoilFourPipeBeam AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::heatingCoil() const {
    boost::optional<HeatingCoilFourPipeBeam> value = optionalHeatingCoil();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Coil attached.");
    }
    return value.get();
  }

  boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::designPrimaryAirVolumeFlowRate() const {
    return getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignPrimaryAirVolumeFlowRate,true);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::isDesignPrimaryAirVolumeFlowRateDefaulted() const {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignPrimaryAirVolumeFlowRate);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::isDesignPrimaryAirVolumeFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignPrimaryAirVolumeFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional <double> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::autosizedDesignPrimaryAirVolumeFlowRate() {
    return getAutosizedValue("TODO_CHECK_SQL Design Primary Air Volume Flow Rate", "m3/s");
  }

  boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::designChilledWaterVolumeFlowRate() const {
    return getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignChilledWaterVolumeFlowRate,true);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::isDesignChilledWaterVolumeFlowRateDefaulted() const {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignChilledWaterVolumeFlowRate);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::isDesignChilledWaterVolumeFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignChilledWaterVolumeFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional <double> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::autosizedDesignChilledWaterVolumeFlowRate() {
    return getAutosizedValue("TODO_CHECK_SQL Design Chilled Water Volume Flow Rate", "m3/s");
  }

  boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::designHotWaterVolumeFlowRate() const {
    return getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignHotWaterVolumeFlowRate,true);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::isDesignHotWaterVolumeFlowRateDefaulted() const {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignHotWaterVolumeFlowRate);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::isDesignHotWaterVolumeFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignHotWaterVolumeFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional <double> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::autosizedDesignHotWaterVolumeFlowRate() {
    return getAutosizedValue("TODO_CHECK_SQL Design Hot Water Volume Flow Rate", "m3/s");
  }

  boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::zoneTotalBeamLength() const {
    return getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::ZoneTotalBeamLength,true);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::isZoneTotalBeamLengthDefaulted() const {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::ZoneTotalBeamLength);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::isZoneTotalBeamLengthAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::ZoneTotalBeamLength, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional <double> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::autosizedZoneTotalBeamLength() {
    return getAutosizedValue("TODO_CHECK_SQL Zone Total Beam Length", "m");
  }

  double AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::ratedPrimaryAirFlowRateperBeamLength() const {
    boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::RatedPrimaryAirFlowRateperBeamLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::isRatedPrimaryAirFlowRateperBeamLengthDefaulted() const {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::RatedPrimaryAirFlowRateperBeamLength);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::setPrimaryAirAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirAvailabilityScheduleName,
                              "AirTerminalSingleDuctConstantVolumeFourPipeBeam",
                              "Primary Air Availability",
                              schedule);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::resetPrimaryAirAvailabilitySchedule() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirAvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::setCoolingAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::CoolingAvailabilityScheduleName,
                              "AirTerminalSingleDuctConstantVolumeFourPipeBeam",
                              "Cooling Availability",
                              schedule);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::resetCoolingAvailabilitySchedule() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::CoolingAvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::setHeatingAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::HeatingAvailabilityScheduleName,
                              "AirTerminalSingleDuctConstantVolumeFourPipeBeam",
                              "Heating Availability",
                              schedule);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::resetHeatingAvailabilitySchedule() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::HeatingAvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::setPrimaryAirInletNode(const Connection& connection) {
    bool result = setPointer(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirInletNodeName, connection.handle());
    return result;
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::setPrimaryAirOutletNode(const Connection& connection) {
    bool result = setPointer(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirOutletNodeName, connection.handle());
    return result;
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::setCoolingCoil(const CoolingCoilFourPipeBeam& coolingCoilFourPipeBeam) {
    bool result = setPointer(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::CoolingCoilName, coolingCoilFourPipeBeam.handle());
    return result;
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::setHeatingCoil(const HeatingCoilFourPipeBeam& heatingCoilFourPipeBeam) {
    bool result = setPointer(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::HeatingCoilName, heatingCoilFourPipeBeam.handle());
    return result;
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::setDesignPrimaryAirVolumeFlowRate(double designPrimaryAirVolumeFlowRate) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignPrimaryAirVolumeFlowRate, designPrimaryAirVolumeFlowRate);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::resetDesignPrimaryAirVolumeFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignPrimaryAirVolumeFlowRate, "");
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::autosizeDesignPrimaryAirVolumeFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignPrimaryAirVolumeFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::setDesignChilledWaterVolumeFlowRate(double designChilledWaterVolumeFlowRate) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignChilledWaterVolumeFlowRate, designChilledWaterVolumeFlowRate);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::resetDesignChilledWaterVolumeFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignChilledWaterVolumeFlowRate, "");
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::autosizeDesignChilledWaterVolumeFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignChilledWaterVolumeFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::setDesignHotWaterVolumeFlowRate(double designHotWaterVolumeFlowRate) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignHotWaterVolumeFlowRate, designHotWaterVolumeFlowRate);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::resetDesignHotWaterVolumeFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignHotWaterVolumeFlowRate, "");
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::autosizeDesignHotWaterVolumeFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignHotWaterVolumeFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::setZoneTotalBeamLength(double zoneTotalBeamLength) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::ZoneTotalBeamLength, zoneTotalBeamLength);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::resetZoneTotalBeamLength() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::ZoneTotalBeamLength, "");
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::autosizeZoneTotalBeamLength() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::ZoneTotalBeamLength, "autosize");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::setRatedPrimaryAirFlowRateperBeamLength(double ratedPrimaryAirFlowRateperBeamLength) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::RatedPrimaryAirFlowRateperBeamLength, ratedPrimaryAirFlowRateperBeamLength);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::resetRatedPrimaryAirFlowRateperBeamLength() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::RatedPrimaryAirFlowRateperBeamLength, "");
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::autosize() {
    autosizedDesignPrimaryAirVolumeFlowRate();
    autosizedDesignChilledWaterVolumeFlowRate();
    autosizedDesignHotWaterVolumeFlowRate();
    autosizedZoneTotalBeamLength();
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::applySizingValues() {
    boost::optional<double> val;
    val = autosizedDesignPrimaryAirVolumeFlowRate();
    if (val) {
      setDesignPrimaryAirVolumeFlowRate(val.get());
    }

    val = autosizedDesignChilledWaterVolumeFlowRate();
    if (val) {
      setDesignChilledWaterVolumeFlowRate(val.get());
    }

    val = autosizedDesignHotWaterVolumeFlowRate();
    if (val) {
      setDesignHotWaterVolumeFlowRate(val.get());
    }

    val = autosizedZoneTotalBeamLength();
    if (val) {
      setZoneTotalBeamLength(val.get());
    }

  }

  boost::optional<Connection> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::optionalPrimaryAirInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirInletNodeName);
  }

  boost::optional<Connection> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::optionalPrimaryAirOutletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirOutletNodeName);
  }

  boost::optional<CoolingCoilFourPipeBeam> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::optionalCoolingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<CoolingCoilFourPipeBeam>(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::CoolingCoilName);
  }

  boost::optional<HeatingCoilFourPipeBeam> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::optionalHeatingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HeatingCoilFourPipeBeam>(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::HeatingCoilName);
  }

} // detail

AirTerminalSingleDuctConstantVolumeFourPipeBeam::AirTerminalSingleDuctConstantVolumeFourPipeBeam(const Model& model)
  : StraightComponent(AirTerminalSingleDuctConstantVolumeFourPipeBeam::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirInletNodeName
  //     OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirOutletNodeName
  //     OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::CoolingCoilName
  //     OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::HeatingCoilName
  bool ok = true;
  // ok = setPrimaryAirInletNode();
  OS_ASSERT(ok);
  // ok = setPrimaryAirOutletNode();
  OS_ASSERT(ok);
  // ok = setCoolingCoil();
  OS_ASSERT(ok);
  // ok = setHeatingCoil();
  OS_ASSERT(ok);
}

IddObjectType AirTerminalSingleDuctConstantVolumeFourPipeBeam::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeam);
}

boost::optional<Schedule> AirTerminalSingleDuctConstantVolumeFourPipeBeam::primaryAirAvailabilitySchedule() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->primaryAirAvailabilitySchedule();
}

boost::optional<Schedule> AirTerminalSingleDuctConstantVolumeFourPipeBeam::coolingAvailabilitySchedule() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->coolingAvailabilitySchedule();
}

boost::optional<Schedule> AirTerminalSingleDuctConstantVolumeFourPipeBeam::heatingAvailabilitySchedule() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->heatingAvailabilitySchedule();
}

Connection AirTerminalSingleDuctConstantVolumeFourPipeBeam::primaryAirInletNode() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->primaryAirInletNode();
}

Connection AirTerminalSingleDuctConstantVolumeFourPipeBeam::primaryAirOutletNode() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->primaryAirOutletNode();
}

CoolingCoilFourPipeBeam AirTerminalSingleDuctConstantVolumeFourPipeBeam::coolingCoil() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->coolingCoil();
}

HeatingCoilFourPipeBeam AirTerminalSingleDuctConstantVolumeFourPipeBeam::heatingCoil() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->heatingCoil();
}

boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeBeam::designPrimaryAirVolumeFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->designPrimaryAirVolumeFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::isDesignPrimaryAirVolumeFlowRateDefaulted() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->isDesignPrimaryAirVolumeFlowRateDefaulted();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::isDesignPrimaryAirVolumeFlowRateAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->isDesignPrimaryAirVolumeFlowRateAutosized();
}

  boost::optional <double> AirTerminalSingleDuctConstantVolumeFourPipeBeam::autosizedDesignPrimaryAirVolumeFlowRate() {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->autosizedDesignPrimaryAirVolumeFlowRate();
  }

boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeBeam::designChilledWaterVolumeFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->designChilledWaterVolumeFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::isDesignChilledWaterVolumeFlowRateDefaulted() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->isDesignChilledWaterVolumeFlowRateDefaulted();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::isDesignChilledWaterVolumeFlowRateAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->isDesignChilledWaterVolumeFlowRateAutosized();
}

  boost::optional <double> AirTerminalSingleDuctConstantVolumeFourPipeBeam::autosizedDesignChilledWaterVolumeFlowRate() {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->autosizedDesignChilledWaterVolumeFlowRate();
  }

boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeBeam::designHotWaterVolumeFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->designHotWaterVolumeFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::isDesignHotWaterVolumeFlowRateDefaulted() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->isDesignHotWaterVolumeFlowRateDefaulted();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::isDesignHotWaterVolumeFlowRateAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->isDesignHotWaterVolumeFlowRateAutosized();
}

  boost::optional <double> AirTerminalSingleDuctConstantVolumeFourPipeBeam::autosizedDesignHotWaterVolumeFlowRate() {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->autosizedDesignHotWaterVolumeFlowRate();
  }

boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeBeam::zoneTotalBeamLength() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->zoneTotalBeamLength();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::isZoneTotalBeamLengthDefaulted() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->isZoneTotalBeamLengthDefaulted();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::isZoneTotalBeamLengthAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->isZoneTotalBeamLengthAutosized();
}

  boost::optional <double> AirTerminalSingleDuctConstantVolumeFourPipeBeam::autosizedZoneTotalBeamLength() {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->autosizedZoneTotalBeamLength();
  }

double AirTerminalSingleDuctConstantVolumeFourPipeBeam::ratedPrimaryAirFlowRateperBeamLength() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->ratedPrimaryAirFlowRateperBeamLength();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::isRatedPrimaryAirFlowRateperBeamLengthDefaulted() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->isRatedPrimaryAirFlowRateperBeamLengthDefaulted();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::setPrimaryAirAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->setPrimaryAirAvailabilitySchedule(schedule);
}

void AirTerminalSingleDuctConstantVolumeFourPipeBeam::resetPrimaryAirAvailabilitySchedule() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->resetPrimaryAirAvailabilitySchedule();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::setCoolingAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->setCoolingAvailabilitySchedule(schedule);
}

void AirTerminalSingleDuctConstantVolumeFourPipeBeam::resetCoolingAvailabilitySchedule() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->resetCoolingAvailabilitySchedule();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::setHeatingAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->setHeatingAvailabilitySchedule(schedule);
}

void AirTerminalSingleDuctConstantVolumeFourPipeBeam::resetHeatingAvailabilitySchedule() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->resetHeatingAvailabilitySchedule();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::setPrimaryAirInletNode(const Connection& connection) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->setPrimaryAirInletNode(connection);
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::setPrimaryAirOutletNode(const Connection& connection) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->setPrimaryAirOutletNode(connection);
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::setCoolingCoil(const CoolingCoilFourPipeBeam& coolingCoilFourPipeBeam) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->setCoolingCoil(coolingCoilFourPipeBeam);
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::setHeatingCoil(const HeatingCoilFourPipeBeam& heatingCoilFourPipeBeam) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->setHeatingCoil(heatingCoilFourPipeBeam);
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::setDesignPrimaryAirVolumeFlowRate(double designPrimaryAirVolumeFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->setDesignPrimaryAirVolumeFlowRate(designPrimaryAirVolumeFlowRate);
}

void AirTerminalSingleDuctConstantVolumeFourPipeBeam::resetDesignPrimaryAirVolumeFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->resetDesignPrimaryAirVolumeFlowRate();
}

void AirTerminalSingleDuctConstantVolumeFourPipeBeam::autosizeDesignPrimaryAirVolumeFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->autosizeDesignPrimaryAirVolumeFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::setDesignChilledWaterVolumeFlowRate(double designChilledWaterVolumeFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->setDesignChilledWaterVolumeFlowRate(designChilledWaterVolumeFlowRate);
}

void AirTerminalSingleDuctConstantVolumeFourPipeBeam::resetDesignChilledWaterVolumeFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->resetDesignChilledWaterVolumeFlowRate();
}

void AirTerminalSingleDuctConstantVolumeFourPipeBeam::autosizeDesignChilledWaterVolumeFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->autosizeDesignChilledWaterVolumeFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::setDesignHotWaterVolumeFlowRate(double designHotWaterVolumeFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->setDesignHotWaterVolumeFlowRate(designHotWaterVolumeFlowRate);
}

void AirTerminalSingleDuctConstantVolumeFourPipeBeam::resetDesignHotWaterVolumeFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->resetDesignHotWaterVolumeFlowRate();
}

void AirTerminalSingleDuctConstantVolumeFourPipeBeam::autosizeDesignHotWaterVolumeFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->autosizeDesignHotWaterVolumeFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::setZoneTotalBeamLength(double zoneTotalBeamLength) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->setZoneTotalBeamLength(zoneTotalBeamLength);
}

void AirTerminalSingleDuctConstantVolumeFourPipeBeam::resetZoneTotalBeamLength() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->resetZoneTotalBeamLength();
}

void AirTerminalSingleDuctConstantVolumeFourPipeBeam::autosizeZoneTotalBeamLength() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->autosizeZoneTotalBeamLength();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::setRatedPrimaryAirFlowRateperBeamLength(double ratedPrimaryAirFlowRateperBeamLength) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->setRatedPrimaryAirFlowRateperBeamLength(ratedPrimaryAirFlowRateperBeamLength);
}

void AirTerminalSingleDuctConstantVolumeFourPipeBeam::resetRatedPrimaryAirFlowRateperBeamLength() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->resetRatedPrimaryAirFlowRateperBeamLength();
}

/// @cond
AirTerminalSingleDuctConstantVolumeFourPipeBeam::AirTerminalSingleDuctConstantVolumeFourPipeBeam(std::shared_ptr<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

