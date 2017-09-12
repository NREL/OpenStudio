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

#include "AirTerminalSingleDuctConstantVolumeCooledBeam.hpp"
#include "AirTerminalSingleDuctConstantVolumeCooledBeam_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "StraightComponent.hpp"
#include "StraightComponent_Impl.hpp"
#include "CoilCoolingCooledBeam.hpp"
#include "CoilCoolingCooledBeam_Impl.hpp"
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
#include "AirLoopHVACZoneSplitter.hpp"
#include "AirLoopHVACZoneSplitter_Impl.hpp"
#include "AirLoopHVACZoneMixer.hpp"
#include "AirLoopHVACZoneMixer_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeam_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl(const IdfObject& idfObject,Model_Impl* model,bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirTerminalSingleDuctConstantVolumeCooledBeam::iddObjectType());
  }

  AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl(
  const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirTerminalSingleDuctConstantVolumeCooledBeam::iddObjectType());
  }

  AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl(
  const AirTerminalSingleDuctConstantVolumeCooledBeam_Impl& other,Model_Impl* model,bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}
  const std::vector<std::string>&

AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty())
    {
    result.push_back("Zone Air Terminal Beam Sensible Cooling Energy [J]");
    result.push_back("Zone Air Terminal Beam Sensible Cooling Rate [W]");
    result.push_back("Zone Air Terminal Supply Air Sensible Cooling Energy [J]");
    result.push_back("Zone Air Terminal Supply Air Sensible Cooling Rate [W]");
    result.push_back("Zone Air Terminal Supply Air Sensible Heating Energy [J]");
    result.push_back("Zone Air Terminal Supply Air Sensible Heating Rate [W]");
    result.push_back("Zone Air Terminal Beam Chilled Water Energy [J]");
    }
    return result;
  }

  IddObjectType AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::iddObjectType() const
  {
    return AirTerminalSingleDuctConstantVolumeCooledBeam::iddObjectType();
  }

  std::vector<ScheduleTypeKey> AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("AirTerminalSingleDuctConstantVolumeCooledBeam","Availability"));
    }
    return result;
  }

  Schedule AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::availabilitySchedule() const {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if(!value){
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<AirTerminalSingleDuctConstantVolumeCooledBeam_Impl*>(this)->setAvailabilitySchedule(*value);
      value = optionalAvailabilitySchedule();
    }
    OS_ASSERT(value);
    return value.get();
  }

   bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::setAvailabilitySchedule(Schedule& schedule)
  {
    bool result = setSchedule(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::AvailabilityScheduleName,
                              "AirTerminalSingleDuctConstantVolumeCooledBeam",
                              "Availability",
                              schedule);
    return result;
  }
  unsigned AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::inletPort()
  {
    return OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::SupplyAirInletNodeName;
  }

  unsigned AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::outletPort()
  {
    return OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::SupplyAirOutletNodeName;
  }

   bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::addToNode(Node & node)
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

              _model.connect( sourceModelObject.get(),
                              sourcePort.get(),
                              inletNode,
                              inletNode.inletPort() );

              _model.connect( inletNode,
                              inletNode.outletPort(),
                              this->getObject<ModelObject>(),
                              this->inletPort() );

              _model.connect( this->getObject<ModelObject>(),
                              outletPort(),
                              node,
                              node.inletPort() );

              if( thermalZone )
              {
                AirTerminalSingleDuctConstantVolumeCooledBeam mo = this->getObject<AirTerminalSingleDuctConstantVolumeCooledBeam>();

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


  std::vector<IdfObject> AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::remove()
  {
    Model _model = this->model();
    ModelObject thisObject = this->getObject<ModelObject>();

    HVACComponent _coolingCoil = coilCoolingCooledBeam();

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

            if( boost::optional<PlantLoop> loop = _coolingCoil.plantLoop() )
            {
              loop->removeDemandBranchWithComponent(_coolingCoil);
            }

            return StraightComponent_Impl::remove();
          }
        }
      }
    }

    model().disconnect(getObject<ModelObject>(),inletPort());
    model().disconnect(getObject<ModelObject>(),outletPort());

    if( boost::optional<PlantLoop> loop = _coolingCoil.plantLoop() )
    {
      loop->removeDemandBranchWithComponent(_coolingCoil);
    }

    return StraightComponent_Impl::remove();
  }

    bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::isRemovable() const
  {
    return true;
  }


  ModelObject AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::clone(Model model) const
  {
    AirTerminalSingleDuctConstantVolumeCooledBeam airTerminalCVCooledBeamClone = StraightComponent_Impl::clone(model).cast<AirTerminalSingleDuctConstantVolumeCooledBeam>();

    HVACComponent coilCoolingClone = this->coilCoolingCooledBeam().clone(model).cast<HVACComponent>();

    airTerminalCVCooledBeamClone.setCoolingCoil(coilCoolingClone);

    return airTerminalCVCooledBeamClone;
  }

  std::vector<ModelObject> AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::children() const
  {
    std::vector<ModelObject> result;
    if (OptionalHVACComponent intermediate = optionalCoolingCoil())
    {
      result.push_back(*intermediate);
    }
    return result;
  }

  HVACComponent AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::coilCoolingCooledBeam() const
  {
    boost::optional<HVACComponent> value = optionalCoolingCoil();
    if (!value)
    {
      LOG_AND_THROW(briefDescription() << " does not have an Cooling Coil attached.");
    }
    return value.get();
  }

  std::string AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::cooledBeamType() const
  {
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::CooledBeamType,true);
    OS_ASSERT(value);
    return value.get();
  }


  boost::optional<double> AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::supplyAirVolumetricFlowRate() const
  {
    return getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::SupplyAirVolumetricFlowRate,true);
  }

  bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::isSupplyAirVolumetricFlowRateDefaulted() const
  {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::SupplyAirVolumetricFlowRate);
  }

  bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::isSupplyAirVolumetricFlowRateAutosized() const
  {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::SupplyAirVolumetricFlowRate, true);
    if (value)
    {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::maximumTotalChilledWaterVolumetricFlowRate() const
  {
    return getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::MaximumTotalChilledWaterVolumetricFlowRate,true);
  }

  bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::isMaximumTotalChilledWaterVolumetricFlowRateDefaulted() const
  {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::MaximumTotalChilledWaterVolumetricFlowRate);
  }

  bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::isMaximumTotalChilledWaterVolumetricFlowRateAutosized() const
  {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::MaximumTotalChilledWaterVolumetricFlowRate, true);
    if (value)
    {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<int> AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::numberofBeams() const
  {
    return getInt(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::NumberofBeams,true);
  }

  bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::isNumberofBeamsDefaulted() const
  {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::NumberofBeams);
  }

  bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::isNumberofBeamsAutosized() const
  {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::NumberofBeams, true);
    if (value)
    {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::beamLength() const
  {
    return getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::BeamLength,true);
  }

  bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::isBeamLengthDefaulted() const
  {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::BeamLength);
  }

  bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::isBeamLengthAutosized() const
  {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::BeamLength, true);
    if (value)
    {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::designInletWaterTemperature() const
  {
    boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::DesignInletWaterTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::isDesignInletWaterTemperatureDefaulted() const
  {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::DesignInletWaterTemperature);
  }

  double AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::designOutletWaterTemperature() const
  {
    boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::DesignOutletWaterTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::isDesignOutletWaterTemperatureDefaulted() const
  {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::DesignOutletWaterTemperature);
  }

  boost::optional<double> AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::coefficientofInductionKin() const
  {
    return getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::CoefficientofInductionKin,true);
  }

  bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::isCoefficientofInductionKinDefaulted() const
  {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::CoefficientofInductionKin);
  }

  bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::isCoefficientofInductionKinAutocalculated() const
  {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::CoefficientofInductionKin, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autocalculate");
    }
    return result;
  }



  bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::setCooledBeamType(std::string cooledBeamType)
  {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::CooledBeamType, cooledBeamType);
    return result;
  }

  bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::setCoolingCoil(HVACComponent& coolingCoilCooledBeam)
  {
    bool result = setPointer(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::CoolingCoilName, coolingCoilCooledBeam.handle());
    return result;
  }

  bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::setSupplyAirVolumetricFlowRate(boost::optional<double> supplyAirVolumetricFlowRate)
  {
    bool result(false);
    if (supplyAirVolumetricFlowRate)
    {
      result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::SupplyAirVolumetricFlowRate, supplyAirVolumetricFlowRate.get());
    }
    else
    {
      resetSupplyAirVolumetricFlowRate();
      result = true;
    }
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::resetSupplyAirVolumetricFlowRate()
  {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::SupplyAirVolumetricFlowRate, "");
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::autosizeSupplyAirVolumetricFlowRate()
  {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::SupplyAirVolumetricFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::setMaximumTotalChilledWaterVolumetricFlowRate(boost::optional<double> maximumTotalChilledWaterVolumetricFlowRate)
  {
    bool result(false);
    if (maximumTotalChilledWaterVolumetricFlowRate)
    {
      result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::MaximumTotalChilledWaterVolumetricFlowRate, maximumTotalChilledWaterVolumetricFlowRate.get());
    }
    else
    {
      resetMaximumTotalChilledWaterVolumetricFlowRate();
      result = true;
    }
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::resetMaximumTotalChilledWaterVolumetricFlowRate()
  {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::MaximumTotalChilledWaterVolumetricFlowRate, "");
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::autosizeMaximumTotalChilledWaterVolumetricFlowRate()
  {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::MaximumTotalChilledWaterVolumetricFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::setNumberofBeams(boost::optional<int> numberofBeams)
  {
    bool result(false);
    if (numberofBeams)
    {
      result = setInt(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::NumberofBeams, numberofBeams.get());
    }
    else
    {
      resetNumberofBeams();
      result = true;
    }
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::resetNumberofBeams()
  {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::NumberofBeams, "");
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::autosizeNumberofBeams()
  {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::NumberofBeams, "autosize");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::setBeamLength(boost::optional<double> beamLength)
  {
    bool result(false);
    if (beamLength)
    {
      result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::BeamLength, beamLength.get());
    }
    else
    {
      resetBeamLength();
      result = true;
    }
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::resetBeamLength()
  {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::BeamLength, "");
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::autosizeBeamLength()
  {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::BeamLength, "autosize");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::setDesignInletWaterTemperature(double designInletWaterTemperature)
  {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::DesignInletWaterTemperature, designInletWaterTemperature);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::resetDesignInletWaterTemperature()
  {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::DesignInletWaterTemperature, "");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::setDesignOutletWaterTemperature(double designOutletWaterTemperature)
  {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::DesignOutletWaterTemperature, designOutletWaterTemperature);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::resetDesignOutletWaterTemperature()
  {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::DesignOutletWaterTemperature, "");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::setCoefficientofInductionKin(boost::optional<double> coefficientofInductionKin)
  {
    bool result(false);
    if (coefficientofInductionKin)
    {
      result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::CoefficientofInductionKin, coefficientofInductionKin.get());
    }
    else
    {
      resetCoefficientofInductionKin();
      result = true;
    }
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::resetCoefficientofInductionKin()
  {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::CoefficientofInductionKin, "");
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::autocalculateCoefficientofInductionKin()
  {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::CoefficientofInductionKin, "autocalculate");
    OS_ASSERT(result);
  }

  boost::optional<Schedule> AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::optionalAvailabilitySchedule() const
  {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::AvailabilityScheduleName);
  }

  boost::optional<HVACComponent> AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::optionalCoolingCoil() const
  {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::CoolingCoilName);
  }

  boost::optional<ModelObject> AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::availabilityScheduleAsModelObject() const
  {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  boost::optional<ModelObject> AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::coolingCoilAsModelObject() const
  {
    OptionalModelObject result = coilCoolingCooledBeam();
    return result;
  }

  bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject)
    {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setAvailabilitySchedule(schedule);
      }
    }
    return false;
  }

  bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::setCoolingCoilAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject)
    {
      OptionalHVACComponent intermediate = modelObject->optionalCast<HVACComponent>();
      if (intermediate)
      {
        HVACComponent coilCoolingCooledBeam(*intermediate);
        return setCoolingCoil(coilCoolingCooledBeam);
      }
    }
    return false;
  }


} // detail

AirTerminalSingleDuctConstantVolumeCooledBeam::AirTerminalSingleDuctConstantVolumeCooledBeam(const Model& model,
                                                                                                   Schedule& availabilitySchedule,
                                                                                                   HVACComponent& coilCoolingCooledBeam)
  : StraightComponent(AirTerminalSingleDuctConstantVolumeCooledBeam::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>());

  bool ok = setAvailabilitySchedule(availabilitySchedule);
  if (!ok)
  {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s availability schedule to "
                  << availabilitySchedule.briefDescription() << ".");
  }

  ok = setCoolingCoil(coilCoolingCooledBeam);
  this->setCooledBeamType("Passive");
  OS_ASSERT(ok);
}

IddObjectType AirTerminalSingleDuctConstantVolumeCooledBeam::iddObjectType()
{
  return IddObjectType(IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeam);
}

std::vector<std::string> AirTerminalSingleDuctConstantVolumeCooledBeam::cooledBeamTypeValues()
{
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::CooledBeamType);
}

Schedule AirTerminalSingleDuctConstantVolumeCooledBeam::availabilitySchedule() const
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->availabilitySchedule();
}

HVACComponent AirTerminalSingleDuctConstantVolumeCooledBeam::coilCoolingCooledBeam() const
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->coilCoolingCooledBeam();
}

std::string AirTerminalSingleDuctConstantVolumeCooledBeam::cooledBeamType() const
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->cooledBeamType();
}

boost::optional<double> AirTerminalSingleDuctConstantVolumeCooledBeam::supplyAirVolumetricFlowRate() const
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->supplyAirVolumetricFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeCooledBeam::isSupplyAirVolumetricFlowRateDefaulted() const
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->isSupplyAirVolumetricFlowRateDefaulted();
}

bool AirTerminalSingleDuctConstantVolumeCooledBeam::isSupplyAirVolumetricFlowRateAutosized() const
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->isSupplyAirVolumetricFlowRateAutosized();
}

boost::optional<double> AirTerminalSingleDuctConstantVolumeCooledBeam::maximumTotalChilledWaterVolumetricFlowRate() const
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->maximumTotalChilledWaterVolumetricFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeCooledBeam::isMaximumTotalChilledWaterVolumetricFlowRateDefaulted() const
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->isMaximumTotalChilledWaterVolumetricFlowRateDefaulted();
}

bool AirTerminalSingleDuctConstantVolumeCooledBeam::isMaximumTotalChilledWaterVolumetricFlowRateAutosized() const
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->isMaximumTotalChilledWaterVolumetricFlowRateAutosized();
}

boost::optional<int> AirTerminalSingleDuctConstantVolumeCooledBeam::numberofBeams() const
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->numberofBeams();
}

bool AirTerminalSingleDuctConstantVolumeCooledBeam::isNumberofBeamsDefaulted() const
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->isNumberofBeamsDefaulted();
}

bool AirTerminalSingleDuctConstantVolumeCooledBeam::isNumberofBeamsAutosized() const
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->isNumberofBeamsAutosized();
}

boost::optional<double> AirTerminalSingleDuctConstantVolumeCooledBeam::beamLength() const
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->beamLength();
}

bool AirTerminalSingleDuctConstantVolumeCooledBeam::isBeamLengthDefaulted() const
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->isBeamLengthDefaulted();
}

bool AirTerminalSingleDuctConstantVolumeCooledBeam::isBeamLengthAutosized() const
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->isBeamLengthAutosized();
}

double AirTerminalSingleDuctConstantVolumeCooledBeam::designInletWaterTemperature() const
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->designInletWaterTemperature();
}

bool AirTerminalSingleDuctConstantVolumeCooledBeam::isDesignInletWaterTemperatureDefaulted() const
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->isDesignInletWaterTemperatureDefaulted();
}

double AirTerminalSingleDuctConstantVolumeCooledBeam::designOutletWaterTemperature() const
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->designOutletWaterTemperature();
}

bool AirTerminalSingleDuctConstantVolumeCooledBeam::isDesignOutletWaterTemperatureDefaulted() const
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->isDesignOutletWaterTemperatureDefaulted();
}

boost::optional<double> AirTerminalSingleDuctConstantVolumeCooledBeam::coefficientofInductionKin() const
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->coefficientofInductionKin();
}

bool AirTerminalSingleDuctConstantVolumeCooledBeam::isCoefficientofInductionKinDefaulted() const
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->isCoefficientofInductionKinDefaulted();
}

bool AirTerminalSingleDuctConstantVolumeCooledBeam::isCoefficientofInductionKinAutocalculated() const
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->isCoefficientofInductionKinAutocalculated();
}

bool AirTerminalSingleDuctConstantVolumeCooledBeam::setAvailabilitySchedule(Schedule& schedule)
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->setAvailabilitySchedule(schedule);
}

bool AirTerminalSingleDuctConstantVolumeCooledBeam::setCooledBeamType(std::string cooledBeamType)
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->setCooledBeamType(cooledBeamType);
}

bool AirTerminalSingleDuctConstantVolumeCooledBeam::setCoolingCoil(HVACComponent& coolingCoilCooledBeam)
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->setCoolingCoil(coolingCoilCooledBeam);
}

bool AirTerminalSingleDuctConstantVolumeCooledBeam::setSupplyAirVolumetricFlowRate(double supplyAirVolumetricFlowRate)
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->setSupplyAirVolumetricFlowRate(supplyAirVolumetricFlowRate);
}

void AirTerminalSingleDuctConstantVolumeCooledBeam::resetSupplyAirVolumetricFlowRate()
{
  getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->resetSupplyAirVolumetricFlowRate();
}

void AirTerminalSingleDuctConstantVolumeCooledBeam::autosizeSupplyAirVolumetricFlowRate()
{
  getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->autosizeSupplyAirVolumetricFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeCooledBeam::setMaximumTotalChilledWaterVolumetricFlowRate(double maximumTotalChilledWaterVolumetricFlowRate)
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->setMaximumTotalChilledWaterVolumetricFlowRate(maximumTotalChilledWaterVolumetricFlowRate);
}

void AirTerminalSingleDuctConstantVolumeCooledBeam::resetMaximumTotalChilledWaterVolumetricFlowRate()
{
  getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->resetMaximumTotalChilledWaterVolumetricFlowRate();
}

void AirTerminalSingleDuctConstantVolumeCooledBeam::autosizeMaximumTotalChilledWaterVolumetricFlowRate()
{
  getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->autosizeMaximumTotalChilledWaterVolumetricFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeCooledBeam::setNumberofBeams(int numberofBeams)
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->setNumberofBeams(numberofBeams);
}

void AirTerminalSingleDuctConstantVolumeCooledBeam::resetNumberofBeams()
{
  getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->resetNumberofBeams();
}

void AirTerminalSingleDuctConstantVolumeCooledBeam::autosizeNumberofBeams()
{
  getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->autosizeNumberofBeams();
}

bool AirTerminalSingleDuctConstantVolumeCooledBeam::setBeamLength(double beamLength)
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->setBeamLength(beamLength);
}

void AirTerminalSingleDuctConstantVolumeCooledBeam::resetBeamLength()
{
  getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->resetBeamLength();
}

void AirTerminalSingleDuctConstantVolumeCooledBeam::autosizeBeamLength()
{
  getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->autosizeBeamLength();
}

bool AirTerminalSingleDuctConstantVolumeCooledBeam::setDesignInletWaterTemperature(double designInletWaterTemperature)
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->setDesignInletWaterTemperature(designInletWaterTemperature);
}

void AirTerminalSingleDuctConstantVolumeCooledBeam::resetDesignInletWaterTemperature()
{
  getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->resetDesignInletWaterTemperature();
}

bool AirTerminalSingleDuctConstantVolumeCooledBeam::setDesignOutletWaterTemperature(double designOutletWaterTemperature)
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->setDesignOutletWaterTemperature(designOutletWaterTemperature);
}

void AirTerminalSingleDuctConstantVolumeCooledBeam::resetDesignOutletWaterTemperature()
{
  getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->resetDesignOutletWaterTemperature();
}

bool AirTerminalSingleDuctConstantVolumeCooledBeam::setCoefficientofInductionKin(double coefficientofInductionKin)
{
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->setCoefficientofInductionKin(coefficientofInductionKin);
}

void AirTerminalSingleDuctConstantVolumeCooledBeam::resetCoefficientofInductionKin()
{
  getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->resetCoefficientofInductionKin();
}

void AirTerminalSingleDuctConstantVolumeCooledBeam::autocalculateCoefficientofInductionKin()
{
  getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>()->autocalculateCoefficientofInductionKin();
}

AirTerminalSingleDuctConstantVolumeCooledBeam::AirTerminalSingleDuctConstantVolumeCooledBeam(std::shared_ptr<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

