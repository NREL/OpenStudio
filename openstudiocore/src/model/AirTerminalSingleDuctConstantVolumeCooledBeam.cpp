/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include <model/AirTerminalSingleDuctConstantVolumeCooledBeam.hpp>
#include <model/AirTerminalSingleDuctConstantVolumeCooledBeam_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeRegistry.hpp>
#include <model/StraightComponent.hpp>
#include <model/StraightComponent_Impl.hpp>
#include <model/CoilCoolingCooledBeam.hpp>
#include <model/CoilCoolingCooledBeam_Impl.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/PortList.hpp>
#include <model/PortList_Impl.hpp>
#include <model/HVACComponent.hpp>
#include <model/HVACComponent_Impl.hpp>
#include <model/AirLoopHVACZoneSplitter.hpp>
#include <model/AirLoopHVACZoneSplitter_Impl.hpp>
#include <model/AirLoopHVACZoneMixer.hpp>
#include <model/AirLoopHVACZoneMixer_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeam_FieldEnums.hxx>
#include <utilities/core/Compare.hpp>
#include <utilities/units/Unit.hpp>
#include <utilities/core/Assert.hpp>
#include <boost/foreach.hpp>

namespace openstudio {
namespace model {

namespace detail {

  AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl(const IdfObject& idfObject,Model_Impl* model,bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    BOOST_ASSERT(idfObject.iddObject().type() == AirTerminalSingleDuctConstantVolumeCooledBeam::iddObjectType());
  }

  AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl(
  const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    BOOST_ASSERT(other.iddObject().type() == AirTerminalSingleDuctConstantVolumeCooledBeam::iddObjectType());
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
      BOOST_ASSERT(value);
      const_cast<AirTerminalSingleDuctConstantVolumeCooledBeam_Impl*>(this)->setAvailabilitySchedule(*value);
      value = optionalAvailabilitySchedule();
    }
    BOOST_ASSERT(value);
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

      if( thermalZone || outlet->optionalCast<AirLoopHVACZoneMixer>() )
      {
        if( boost::optional<ModelObject> inlet = node.inletModelObject() )
        {
          if( boost::optional<AirLoopHVACZoneSplitter> splitter = inlet->optionalCast<AirLoopHVACZoneSplitter>() )
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
 //               thermalZone->setHeatingPriority(mo,1);
                thermalZone->setCoolingPriority(mo,1);
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

    std::vector<IdfObject> result;

    HVACComponent _coolingCoil = coilCoolingCooledBeam();

    boost::optional<ModelObject> sourceModelObject = this->inletModelObject();
    boost::optional<unsigned> sourcePort = this->connectedObjectPort(this->inletPort());
    
    boost::optional<ModelObject> targetModelObject = this->outletModelObject();
    boost::optional<unsigned> targetPort = this->connectedObjectPort(this->outletPort());

    std::vector<ThermalZone> thermalZones = _model.getModelObjects<ThermalZone>();
    for( std::vector<ThermalZone>::iterator it = thermalZones.begin();
         it != thermalZones.end();
         it++ )
    {
      std::vector<ModelObject> equipment = it->equipment();

      if( std::find(equipment.begin(),equipment.end(),thisObject) != equipment.end() )
      {
        it->removeEquipment(thisObject);

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
    BOOST_ASSERT(value);
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
    BOOST_ASSERT(value);
    return value.get();
  }

  bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::isDesignInletWaterTemperatureDefaulted() const 
  {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::DesignInletWaterTemperature);
  }

  double AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::designOutletWaterTemperature() const 
  {
    boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::DesignOutletWaterTemperature,true);
    BOOST_ASSERT(value);
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
    BOOST_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::autosizeSupplyAirVolumetricFlowRate() 
  {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::SupplyAirVolumetricFlowRate, "autosize");
    BOOST_ASSERT(result);
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
    BOOST_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::autosizeMaximumTotalChilledWaterVolumetricFlowRate() 
  {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::MaximumTotalChilledWaterVolumetricFlowRate, "autosize");
    BOOST_ASSERT(result);
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
    BOOST_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::autosizeNumberofBeams() 
  {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::NumberofBeams, "autosize");
    BOOST_ASSERT(result);
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
    BOOST_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::autosizeBeamLength() 
  {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::BeamLength, "autosize");
    BOOST_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::setDesignInletWaterTemperature(double designInletWaterTemperature) 
  {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::DesignInletWaterTemperature, designInletWaterTemperature);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::resetDesignInletWaterTemperature() 
  {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::DesignInletWaterTemperature, "");
    BOOST_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::setDesignOutletWaterTemperature(double designOutletWaterTemperature) 
  {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::DesignOutletWaterTemperature, designOutletWaterTemperature);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::resetDesignOutletWaterTemperature() 
  {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::DesignOutletWaterTemperature, "");
    BOOST_ASSERT(result);
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
    BOOST_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeCooledBeam_Impl::autocalculateCoefficientofInductionKin() 
  {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::CoefficientofInductionKin, "autocalculate");
    BOOST_ASSERT(result);
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
  BOOST_ASSERT(getImpl<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl>());

  bool ok = setAvailabilitySchedule(availabilitySchedule);
  if (!ok) 
  {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s availability schedule to "
                  << availabilitySchedule.briefDescription() << ".");
  }
  
  ok = setCoolingCoil(coilCoolingCooledBeam);
  this->setCooledBeamType("Passive");
  BOOST_ASSERT(ok);
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

AirTerminalSingleDuctConstantVolumeCooledBeam::AirTerminalSingleDuctConstantVolumeCooledBeam(boost::shared_ptr<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

