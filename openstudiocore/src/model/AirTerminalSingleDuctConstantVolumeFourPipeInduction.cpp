/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "AirTerminalSingleDuctConstantVolumeFourPipeInduction.hpp"
#include "AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PortList.hpp"
#include "PortList_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "AirLoopHVACZoneSplitter.hpp"
#include "AirLoopHVACZoneSplitter_Impl.hpp"
#include "AirLoopHVACZoneMixer.hpp"
#include "AirLoopHVACZoneMixer_Impl.hpp"

#include <utilities/idd/OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInduction_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl(const IdfObject& idfObject,
                                                                                                                       Model_Impl* model,
                                                                                                                       bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirTerminalSingleDuctConstantVolumeFourPipeInduction::iddObjectType());
  }

  AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                                       Model_Impl* model,
                                                                                                                       bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirTerminalSingleDuctConstantVolumeFourPipeInduction::iddObjectType());
  }

  AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl(const AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl& other,
                                                                                                                       Model_Impl* model,
                                                                                                                       bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::iddObjectType() const {
    return AirTerminalSingleDuctConstantVolumeFourPipeInduction::iddObjectType();
  }

  std::vector<ScheduleTypeKey> AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("AirTerminalSingleDuctConstantVolumeFourPipeInduction","Availability"));
    }
    return result;
  }

  unsigned AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::inletPort()
  {
    return OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::SupplyAirInletNodeName;
  }

  unsigned AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::outletPort()
  {
    return OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::AirOutletNodeName;
  }

   bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::addToNode(Node & node)
  {
    Model _model = node.model();

    if( boost::optional<ModelObject> outlet = node.outletModelObject() )
    {
      boost::optional<ThermalZone> thermalZone;

      if( boost::optional<PortList> portList = outlet->optionalCast<PortList>()  )
      {
        thermalZone = portList->thermalZone();
      }

      if( thermalZone || (outlet->optionalCast<Mixer>() && node.airLoopHVAC()) )
      {
        if( boost::optional<ModelObject> inlet = node.inletModelObject() )
        {
          if( boost::optional<Splitter> splitter = inlet->optionalCast<Splitter>() )
          {
            boost::optional<ModelObject> sourceModelObject = inlet;
            boost::optional<unsigned> sourcePort = node.connectedObjectPort(node.inletPort());

            if( sourcePort && sourceModelObject )
            {
              Node inletNode(_model);

              ModelObject thisObject = getObject<ModelObject>();

              _model.connect( sourceModelObject.get(),
                              sourcePort.get(),
                              inletNode,
                              inletNode.inletPort() );

              _model.connect( inletNode,
                              inletNode.outletPort(),
                              thisObject,
                              inletPort() );

              _model.connect( thisObject,
                              outletPort(),
                              node,
                              node.inletPort() );

              if( thermalZone )
              {
                Node inducedAirInletNode(_model);

                PortList exhaustPortList = thermalZone->exhaustPortList();

                _model.connect( exhaustPortList,
                                exhaustPortList.nextPort(),
                                inducedAirInletNode,
                                inducedAirInletNode.inletPort() );

                _model.connect( inducedAirInletNode,
                                inducedAirInletNode.outletPort(),
                                thisObject,
                                inducedAirInletPort() );

                ModelObject mo = this->getObject<ModelObject>();

                thermalZone->addEquipment(mo);
              }

              return true;
            }
          }
        }
      }
    }

    return false;
  }


  std::vector<IdfObject> AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::remove()
  {
    Model _model = this->model();
    ModelObject thisObject = this->getObject<ModelObject>();
    boost::optional<PlantLoop> loop;

    HVACComponent _heatingCoil = heatingCoil();
    boost::optional<HVACComponent> _coolingCoil = coolingCoil();

    boost::optional<ModelObject> sourceModelObject = this->inletModelObject();
    boost::optional<unsigned> sourcePort = this->connectedObjectPort(this->inletPort());

    boost::optional<ModelObject> targetModelObject = this->outletModelObject();
    boost::optional<unsigned> targetPort = this->connectedObjectPort(this->outletPort());

    std::vector<ThermalZone> thermalZones = _model.getConcreteModelObjects<ThermalZone>();
    for( auto & thermalZone : thermalZones )
    {
      std::vector<ModelObject> equipment = thermalZone.equipment();

      if( std::find(equipment.begin(),equipment.end(),thisObject) != equipment.end() )
      {
        thermalZone.removeEquipment(thisObject);
        break;
      }
    }

    if( boost::optional<Node> t_inducedAirInletNode = inducedAirInletNode() )
    {
      t_inducedAirInletNode->disconnect();
      t_inducedAirInletNode->remove();
    }

    if( sourcePort && sourceModelObject
        && targetPort && targetModelObject )
    {
      if( boost::optional<Node> inletNode = sourceModelObject->optionalCast<Node>() )
      {
        if( boost::optional<ModelObject> source2ModelObject = inletNode->inletModelObject() )
        {
          if( boost::optional<unsigned> source2Port = inletNode->connectedObjectPort(inletNode->inletPort()) )
          {
            _model.connect( source2ModelObject.get(),
                            source2Port.get(),
                            targetModelObject.get(),
                            targetPort.get() );

            inletNode->disconnect();
            inletNode->remove();

            if( _coolingCoil && ( loop = _coolingCoil->plantLoop() ) )
            {
              loop->removeDemandBranchWithComponent(*_coolingCoil);
            }

            if( (loop = _heatingCoil.plantLoop()) )
            {
              loop->removeDemandBranchWithComponent(_heatingCoil);
            }

            return StraightComponent_Impl::remove();
          }
        }
      }
    }

    model().disconnect(getObject<ModelObject>(),inletPort());
    model().disconnect(getObject<ModelObject>(),outletPort());

    if( _coolingCoil && ( loop = _coolingCoil->plantLoop() ) )
    {
      loop->removeDemandBranchWithComponent(*_coolingCoil);
    }

    if( (loop = _heatingCoil.plantLoop()) )
    {
      loop->removeDemandBranchWithComponent(_heatingCoil);
    }

    return StraightComponent_Impl::remove();
  }

    bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::isRemovable() const
  {
    return true;
  }


  ModelObject AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::clone(Model model) const
  {
    AirTerminalSingleDuctConstantVolumeFourPipeInduction airTerminalCVFourPipeInductionClone = StraightComponent_Impl::clone(model).cast<AirTerminalSingleDuctConstantVolumeFourPipeInduction>();

    if( boost::optional<HVACComponent> coolingCoil = this->coolingCoil() ) {
      HVACComponent coilCoolingClone = coolingCoil->clone(model).cast<HVACComponent>();
      airTerminalCVFourPipeInductionClone.setCoolingCoil(coilCoolingClone);
    }

    HVACComponent coilHeatingClone = this->heatingCoil().clone(model).cast<HVACComponent>();
    airTerminalCVFourPipeInductionClone.setHeatingCoil(coilHeatingClone);

    return airTerminalCVFourPipeInductionClone;
  }

  std::vector<ModelObject> AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::children() const
  {
    std::vector<ModelObject> result;
    if (OptionalHVACComponent intermediate = coolingCoil())
    {
      result.push_back(*intermediate);
    }
    if (OptionalHVACComponent intermediate = optionalHeatingCoil())
    {
      result.push_back(*intermediate);
    }
    return result;
  }

  boost::optional<Schedule> AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::AvailabilityScheduleName);
  }

  boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::maximumTotalAirFlowRate() const {
    return getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumTotalAirFlowRate,true);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::isMaximumTotalAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumTotalAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::inductionRatio() const {
    boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::InductionRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::isInductionRatioDefaulted() const {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::InductionRatio);
  }

  HVACComponent AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::heatingCoil() const {
    boost::optional<HVACComponent> value = optionalHeatingCoil();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Coil attached.");
    }
    return value.get();
  }

  boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::maximumHotWaterFlowRate() const {
    return getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumHotWaterFlowRate,true);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::isMaximumHotWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumHotWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::minimumHotWaterFlowRate() const {
    boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MinimumHotWaterFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::isMinimumHotWaterFlowRateDefaulted() const {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MinimumHotWaterFlowRate);
  }

  double AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::heatingConvergenceTolerance() const {
    boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::HeatingConvergenceTolerance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::isHeatingConvergenceToleranceDefaulted() const {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::HeatingConvergenceTolerance);
  }

  boost::optional<HVACComponent> AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::coolingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::CoolingCoilName);
  }

  boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::maximumColdWaterFlowRate() const {
    return getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumColdWaterFlowRate,true);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::isMaximumColdWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumColdWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::minimumColdWaterFlowRate() const {
    boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MinimumColdWaterFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::isMinimumColdWaterFlowRateDefaulted() const {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MinimumColdWaterFlowRate);
  }

  double AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::coolingConvergenceTolerance() const {
    boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::CoolingConvergenceTolerance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::isCoolingConvergenceToleranceDefaulted() const {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::CoolingConvergenceTolerance);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::AvailabilityScheduleName,
                              "AirTerminalSingleDuctConstantVolumeFourPipeInduction",
                              "Availability",
                              schedule);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::AvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::setMaximumTotalAirFlowRate(boost::optional<double> maximumTotalAirFlowRate) {
    bool result(false);
    if (maximumTotalAirFlowRate) {
      result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumTotalAirFlowRate, maximumTotalAirFlowRate.get());
    }
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::autosizeMaximumTotalAirFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumTotalAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::setInductionRatio(double inductionRatio) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::InductionRatio, inductionRatio);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::resetInductionRatio() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::InductionRatio, "");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::setHeatingCoil(const HVACComponent& heatingCoil) {
    bool result = setPointer(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::HeatingCoilName, heatingCoil.handle());
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::setMaximumHotWaterFlowRate(boost::optional<double> maximumHotWaterFlowRate) {
    bool result(false);
    if (maximumHotWaterFlowRate) {
      result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumHotWaterFlowRate, maximumHotWaterFlowRate.get());
    }
    else {
      resetMaximumHotWaterFlowRate();
      result = true;
    }
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::resetMaximumHotWaterFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumHotWaterFlowRate, "");
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::autosizeMaximumHotWaterFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumHotWaterFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::setMinimumHotWaterFlowRate(double minimumHotWaterFlowRate) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MinimumHotWaterFlowRate, minimumHotWaterFlowRate);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::resetMinimumHotWaterFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MinimumHotWaterFlowRate, "");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::setHeatingConvergenceTolerance(double heatingConvergenceTolerance) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::HeatingConvergenceTolerance, heatingConvergenceTolerance);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::resetHeatingConvergenceTolerance() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::HeatingConvergenceTolerance, "");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::setCoolingCoil(const boost::optional<HVACComponent>& coolingCoil) {
    bool result(false);
    if (coolingCoil) {
      result = setPointer(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::CoolingCoilName, coolingCoil.get().handle());
    }
    else {
      resetCoolingCoil();
      result = true;
    }
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::resetCoolingCoil() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::CoolingCoilName, "");
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::setMaximumColdWaterFlowRate(boost::optional<double> maximumColdWaterFlowRate) {
    bool result(false);
    if (maximumColdWaterFlowRate) {
      result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumColdWaterFlowRate, maximumColdWaterFlowRate.get());
    }
    else {
      resetMaximumColdWaterFlowRate();
      result = true;
    }
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::resetMaximumColdWaterFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumColdWaterFlowRate, "");
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::autosizeMaximumColdWaterFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumColdWaterFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::setMinimumColdWaterFlowRate(double minimumColdWaterFlowRate) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MinimumColdWaterFlowRate, minimumColdWaterFlowRate);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::resetMinimumColdWaterFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MinimumColdWaterFlowRate, "");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::setCoolingConvergenceTolerance(double coolingConvergenceTolerance) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::CoolingConvergenceTolerance, coolingConvergenceTolerance);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::resetCoolingConvergenceTolerance() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::CoolingConvergenceTolerance, "");
    OS_ASSERT(result);
  }

  boost::optional<HVACComponent> AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::optionalHeatingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::HeatingCoilName);
  }

  boost::optional<Node> AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::inducedAirInletNode() const {
    boost::optional<Node> result;
    if( auto modelObject = connectedObject(inducedAirInletPort()) ) {
      result = modelObject->optionalCast<Node>();
    }
    return result;
  }

  unsigned AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::inducedAirInletPort() const {
    return OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::InducedAirInletNodeName;
  }

} // detail

AirTerminalSingleDuctConstantVolumeFourPipeInduction::AirTerminalSingleDuctConstantVolumeFourPipeInduction(const Model& model, HVACComponent& heatingCoil)
  : StraightComponent(AirTerminalSingleDuctConstantVolumeFourPipeInduction::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>());

  autosizeMaximumTotalAirFlowRate();
  autosizeMaximumColdWaterFlowRate();
  autosizeMaximumHotWaterFlowRate();
  setHeatingCoil(heatingCoil);
  setMinimumColdWaterFlowRate(0.0);
  setCoolingConvergenceTolerance(0.001);
  setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::ZoneMixerName,"");
}

IddObjectType AirTerminalSingleDuctConstantVolumeFourPipeInduction::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInduction);
}

boost::optional<Schedule> AirTerminalSingleDuctConstantVolumeFourPipeInduction::availabilitySchedule() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->availabilitySchedule();
}

boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeInduction::maximumTotalAirFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->maximumTotalAirFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::isMaximumTotalAirFlowRateAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->isMaximumTotalAirFlowRateAutosized();
}

double AirTerminalSingleDuctConstantVolumeFourPipeInduction::inductionRatio() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->inductionRatio();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::isInductionRatioDefaulted() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->isInductionRatioDefaulted();
}

HVACComponent AirTerminalSingleDuctConstantVolumeFourPipeInduction::heatingCoil() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->heatingCoil();
}

boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeInduction::maximumHotWaterFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->maximumHotWaterFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::isMaximumHotWaterFlowRateAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->isMaximumHotWaterFlowRateAutosized();
}

double AirTerminalSingleDuctConstantVolumeFourPipeInduction::minimumHotWaterFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->minimumHotWaterFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::isMinimumHotWaterFlowRateDefaulted() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->isMinimumHotWaterFlowRateDefaulted();
}

double AirTerminalSingleDuctConstantVolumeFourPipeInduction::heatingConvergenceTolerance() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->heatingConvergenceTolerance();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::isHeatingConvergenceToleranceDefaulted() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->isHeatingConvergenceToleranceDefaulted();
}

boost::optional<HVACComponent> AirTerminalSingleDuctConstantVolumeFourPipeInduction::coolingCoil() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->coolingCoil();
}

boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeInduction::maximumColdWaterFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->maximumColdWaterFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::isMaximumColdWaterFlowRateAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->isMaximumColdWaterFlowRateAutosized();
}

double AirTerminalSingleDuctConstantVolumeFourPipeInduction::minimumColdWaterFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->minimumColdWaterFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::isMinimumColdWaterFlowRateDefaulted() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->isMinimumColdWaterFlowRateDefaulted();
}

double AirTerminalSingleDuctConstantVolumeFourPipeInduction::coolingConvergenceTolerance() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->coolingConvergenceTolerance();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::isCoolingConvergenceToleranceDefaulted() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->isCoolingConvergenceToleranceDefaulted();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->setAvailabilitySchedule(schedule);
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::resetAvailabilitySchedule() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->resetAvailabilitySchedule();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::setMaximumTotalAirFlowRate(double maximumTotalAirFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->setMaximumTotalAirFlowRate(maximumTotalAirFlowRate);
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::autosizeMaximumTotalAirFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->autosizeMaximumTotalAirFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::setInductionRatio(double inductionRatio) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->setInductionRatio(inductionRatio);
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::resetInductionRatio() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->resetInductionRatio();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::setHeatingCoil(const HVACComponent& heatingCoil) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->setHeatingCoil(heatingCoil);
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::setMaximumHotWaterFlowRate(double maximumHotWaterFlowRate) {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->setMaximumHotWaterFlowRate(maximumHotWaterFlowRate);
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::resetMaximumHotWaterFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->resetMaximumHotWaterFlowRate();
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::autosizeMaximumHotWaterFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->autosizeMaximumHotWaterFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::setMinimumHotWaterFlowRate(double minimumHotWaterFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->setMinimumHotWaterFlowRate(minimumHotWaterFlowRate);
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::resetMinimumHotWaterFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->resetMinimumHotWaterFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::setHeatingConvergenceTolerance(double heatingConvergenceTolerance) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->setHeatingConvergenceTolerance(heatingConvergenceTolerance);
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::resetHeatingConvergenceTolerance() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->resetHeatingConvergenceTolerance();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::setCoolingCoil(const HVACComponent& coolingCoil) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->setCoolingCoil(coolingCoil);
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::resetCoolingCoil() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->resetCoolingCoil();
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::setMaximumColdWaterFlowRate(double maximumColdWaterFlowRate) {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->setMaximumColdWaterFlowRate(maximumColdWaterFlowRate);
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::resetMaximumColdWaterFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->resetMaximumColdWaterFlowRate();
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::autosizeMaximumColdWaterFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->autosizeMaximumColdWaterFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::setMinimumColdWaterFlowRate(double minimumColdWaterFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->setMinimumColdWaterFlowRate(minimumColdWaterFlowRate);
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::resetMinimumColdWaterFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->resetMinimumColdWaterFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::setCoolingConvergenceTolerance(double coolingConvergenceTolerance) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->setCoolingConvergenceTolerance(coolingConvergenceTolerance);
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::resetCoolingConvergenceTolerance() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->resetCoolingConvergenceTolerance();
}

boost::optional<Node> AirTerminalSingleDuctConstantVolumeFourPipeInduction::inducedAirInletNode() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->inducedAirInletNode();
}

unsigned AirTerminalSingleDuctConstantVolumeFourPipeInduction::inducedAirInletPort() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->inducedAirInletPort();
}

/// @cond
AirTerminalSingleDuctConstantVolumeFourPipeInduction::AirTerminalSingleDuctConstantVolumeFourPipeInduction(std::shared_ptr<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

