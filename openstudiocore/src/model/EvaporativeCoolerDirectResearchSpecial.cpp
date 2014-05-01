/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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

#include <model/EvaporativeCoolerDirectResearchSpecial.hpp>
#include <model/EvaporativeCoolerDirectResearchSpecial_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/Model.hpp>
#include <utilities/idd/OS_EvaporativeCooler_Direct_ResearchSpecial_FieldEnums.hxx>
#include <utilities/core/Compare.hpp>
#include <utilities/core/Assert.hpp>
#include <boost/foreach.hpp>

namespace openstudio {

namespace model {

namespace detail{

  EvaporativeCoolerDirectResearchSpecial_Impl::EvaporativeCoolerDirectResearchSpecial_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : StraightComponent_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == EvaporativeCoolerDirectResearchSpecial::iddObjectType());
  }

  EvaporativeCoolerDirectResearchSpecial_Impl::EvaporativeCoolerDirectResearchSpecial_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == EvaporativeCoolerDirectResearchSpecial::iddObjectType());
  }

  EvaporativeCoolerDirectResearchSpecial_Impl::EvaporativeCoolerDirectResearchSpecial_Impl(
        const EvaporativeCoolerDirectResearchSpecial_Impl& other,
        Model_Impl* model,
        bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& EvaporativeCoolerDirectResearchSpecial_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType EvaporativeCoolerDirectResearchSpecial_Impl::iddObjectType() const {
    return EvaporativeCoolerDirectResearchSpecial::iddObjectType();
  }

  std::vector<ScheduleTypeKey> EvaporativeCoolerDirectResearchSpecial_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_EvaporativeCooler_Direct_ResearchSpecialFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("EvaporativeCoolerDirectResearchSpecial","Availability"));
    }
    return result;
  }

  unsigned EvaporativeCoolerDirectResearchSpecial_Impl::inletPort() 
  {
    return OS_EvaporativeCooler_Direct_ResearchSpecialFields::AirInletNodeName;
  }

  unsigned EvaporativeCoolerDirectResearchSpecial_Impl::outletPort() 
  {
    return OS_EvaporativeCooler_Direct_ResearchSpecialFields::AirOutletNodeName;
  }

  bool EvaporativeCoolerDirectResearchSpecial_Impl::addToNode(Node & node)
  {
    if( boost::optional<AirLoopHVAC> airLoop = node.airLoopHVAC() )
    {
      if( ! airLoop->demandComponent(node.handle()) )
      {
        if( StraightComponent_Impl::addToNode( node ) )
        {
          if( boost::optional<Node> node = this->outletModelObject()->optionalCast<Node>() )
          {
            this->setSensorNode(node.get());
          }
          return true;
        }
      }
    }

    return false;
  }

  std::vector<openstudio::IdfObject> EvaporativeCoolerDirectResearchSpecial_Impl::remove()
  {
    return StraightComponent_Impl::remove();
  }

  Schedule EvaporativeCoolerDirectResearchSpecial_Impl::availabilitySchedule() const {
    boost::optional<Schedule> value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_EvaporativeCooler_Direct_ResearchSpecialFields::AvailabilityScheduleName);
    if (!value){
      // it is an error if we get here, however we don't want to crash
      // so we hook up to global always on schedule
      LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<EvaporativeCoolerDirectResearchSpecial_Impl*>(this)->setAvailabilitySchedule(*value);
      value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_EvaporativeCooler_Direct_ResearchSpecialFields::AvailabilityScheduleName);
    }
    OS_ASSERT(value);
    return value.get();
  }
  
  bool EvaporativeCoolerDirectResearchSpecial_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_EvaporativeCooler_Direct_ResearchSpecialFields::AvailabilityScheduleName,
                              "EvaporativeCoolerDirectResearchSpecial",
                              "Availability",
                              schedule);
    return result;
  }
  
  double EvaporativeCoolerDirectResearchSpecial_Impl::coolerEffectiveness() const
  {
    return this->getDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::CoolerEffectiveness).get();
  }
  
  void EvaporativeCoolerDirectResearchSpecial_Impl::setCoolerEffectiveness( double value )
  {
    this->setDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::CoolerEffectiveness,value);
  }
  
  double EvaporativeCoolerDirectResearchSpecial_Impl::recirculatingWaterPumpPowerConsumption() const
  {
    return this->getDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::RecirculatingWaterPumpPowerConsumption).get();
  }
  
  void EvaporativeCoolerDirectResearchSpecial_Impl::setRecirculatingWaterPumpPowerConsumption( double value )
  {
    this->setDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::RecirculatingWaterPumpPowerConsumption,value);
  }
  
  boost::optional<Node> EvaporativeCoolerDirectResearchSpecial_Impl::sensorNode() const
  {
    boost::optional<WorkspaceObject> wo = this->getTarget(OS_EvaporativeCooler_Direct_ResearchSpecialFields::SensorNodeName);

    if( wo )
    {
      return wo->optionalCast<Node>();      
    }
    else
    {
      return boost::optional<Node>();
    }
  }
  
  void EvaporativeCoolerDirectResearchSpecial_Impl::setSensorNode( const Node & node )
  {
    this->setPointer(OS_EvaporativeCooler_Direct_ResearchSpecialFields::SensorNodeName,node.handle());
  }
  
  double EvaporativeCoolerDirectResearchSpecial_Impl::driftLossFraction() const
  {
    return this->getDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::DriftLossFraction).get();
  }
  
  void EvaporativeCoolerDirectResearchSpecial_Impl::setDriftLossFraction( double value )
  {
    this->setDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::DriftLossFraction,value);
  }
  
  double EvaporativeCoolerDirectResearchSpecial_Impl::blowdownConcentrationRatio() const
  {
    return this->getDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::BlowdownConcentrationRatio).get();
  }
  
  void EvaporativeCoolerDirectResearchSpecial_Impl::setBlowdownConcentrationRatio( double value )
  {
    this->setDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::BlowdownConcentrationRatio,value);
  }

  boost::optional<ModelObject> EvaporativeCoolerDirectResearchSpecial_Impl::availabilityScheduleAsModelObject() const {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  bool EvaporativeCoolerDirectResearchSpecial_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setAvailabilitySchedule(schedule);
      }
    }
    return false;
  }

} // detail

// create a new EvaporativeCoolerDirectResearchSpecial object in the model's workspace
EvaporativeCoolerDirectResearchSpecial::EvaporativeCoolerDirectResearchSpecial(const Model& model,Schedule & schedule)
  : StraightComponent(EvaporativeCoolerDirectResearchSpecial::iddObjectType(),model) 
{
  OS_ASSERT(getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>());

  this->setAvailabilitySchedule(schedule);

  setCoolerEffectiveness(1.0);

  setRecirculatingWaterPumpPowerConsumption(0.0);

  setDriftLossFraction(0.0);

  setBlowdownConcentrationRatio(0.0);
}

Schedule EvaporativeCoolerDirectResearchSpecial::availabilitySchedule() const
{
  return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->availabilitySchedule();
}

Schedule EvaporativeCoolerDirectResearchSpecial::availableSchedule() const
{
  return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->availabilitySchedule();
}

bool EvaporativeCoolerDirectResearchSpecial::setAvailabilitySchedule(Schedule & schedule )
{
  return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->setAvailabilitySchedule(schedule);
}

bool EvaporativeCoolerDirectResearchSpecial::setAvailableSchedule(Schedule & schedule )
{
  return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->setAvailabilitySchedule(schedule);
}

double EvaporativeCoolerDirectResearchSpecial::coolerEffectiveness() const
{
  return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->coolerEffectiveness();
}

void EvaporativeCoolerDirectResearchSpecial::setCoolerEffectiveness( double value )
{
  getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->setCoolerEffectiveness(value);
}

double EvaporativeCoolerDirectResearchSpecial::recirculatingWaterPumpPowerConsumption() const
{
  return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->recirculatingWaterPumpPowerConsumption();
}

void EvaporativeCoolerDirectResearchSpecial::setRecirculatingWaterPumpPowerConsumption( double value )
{
  getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->setRecirculatingWaterPumpPowerConsumption(value);
}

boost::optional<Node> EvaporativeCoolerDirectResearchSpecial::sensorNode() const
{
  return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->sensorNode();
}

void EvaporativeCoolerDirectResearchSpecial::setSensorNode( const Node & node )
{
  getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->setSensorNode(node);
}

double EvaporativeCoolerDirectResearchSpecial::driftLossFraction() const
{
  return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->driftLossFraction();
}

void EvaporativeCoolerDirectResearchSpecial::setDriftLossFraction( double value )
{
  getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->setDriftLossFraction(value);
}

double EvaporativeCoolerDirectResearchSpecial::blowdownConcentrationRatio() const
{
  return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->blowdownConcentrationRatio();
}

void EvaporativeCoolerDirectResearchSpecial::setBlowdownConcentrationRatio( double value )
{
  getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->setBlowdownConcentrationRatio(value);
}

EvaporativeCoolerDirectResearchSpecial::EvaporativeCoolerDirectResearchSpecial(
  boost::shared_ptr<detail::EvaporativeCoolerDirectResearchSpecial_Impl> p)
  : StraightComponent(p)
{}

IddObjectType EvaporativeCoolerDirectResearchSpecial::iddObjectType() {
  IddObjectType result(IddObjectType::OS_EvaporativeCooler_Direct_ResearchSpecial);
  return result;
}

} // model
} // openstudio
