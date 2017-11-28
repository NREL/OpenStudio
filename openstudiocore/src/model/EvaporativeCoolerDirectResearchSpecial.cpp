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

#include "EvaporativeCoolerDirectResearchSpecial.hpp"
#include "EvaporativeCoolerDirectResearchSpecial_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "Model.hpp"
#include <utilities/idd/OS_EvaporativeCooler_Direct_ResearchSpecial_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"

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

  boost::optional<double> EvaporativeCoolerDirectResearchSpecial_Impl::recirculatingWaterPumpPowerConsumption() const
  {
    return this->getDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::RecirculatingWaterPumpPowerConsumption);
  }

  void EvaporativeCoolerDirectResearchSpecial_Impl::setRecirculatingWaterPumpPowerConsumption( double value )
  {
    this->setDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::RecirculatingWaterPumpPowerConsumption,value);
  }

  void EvaporativeCoolerDirectResearchSpecial_Impl::autosizeRecirculatingWaterPumpPowerConsumption()
  {
    this->setString(OS_EvaporativeCooler_Direct_ResearchSpecialFields::RecirculatingWaterPumpPowerConsumption,"Autosize");
  }

  bool EvaporativeCoolerDirectResearchSpecial_Impl::isRecirculatingWaterPumpPowerConsumptionAutosized() const
  {
    bool result = false;
    boost::optional<std::string> value = getString(OS_EvaporativeCooler_Direct_ResearchSpecialFields::RecirculatingWaterPumpPowerConsumption, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> EvaporativeCoolerDirectResearchSpecial_Impl::primaryAirDesignFlowRate() const
  {
    return this->getDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::PrimaryAirDesignFlowRate);
  }

  void EvaporativeCoolerDirectResearchSpecial_Impl::setPrimaryAirDesignFlowRate( double value )
  {
    this->setDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::PrimaryAirDesignFlowRate,value);
  }

  void EvaporativeCoolerDirectResearchSpecial_Impl::autosizePrimaryAirDesignFlowRate()
  {
    this->setString(OS_EvaporativeCooler_Direct_ResearchSpecialFields::PrimaryAirDesignFlowRate,"Autosize");
  }

  bool EvaporativeCoolerDirectResearchSpecial_Impl::isPrimaryAirDesignFlowRateAutosized() const
  {
    bool result = false;
    boost::optional<std::string> value = getString(OS_EvaporativeCooler_Direct_ResearchSpecialFields::PrimaryAirDesignFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
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

  boost::optional<Curve> EvaporativeCoolerDirectResearchSpecial_Impl::effectivenessFlowRatioModifierCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_EvaporativeCooler_Direct_ResearchSpecialFields::EffectivenessFlowRatioModifierCurveName);
  }

  double EvaporativeCoolerDirectResearchSpecial_Impl::waterPumpPowerSizingFactor() const {
    boost::optional<double> value = getDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::WaterPumpPowerSizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Curve> EvaporativeCoolerDirectResearchSpecial_Impl::waterPumpPowerModifierCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_EvaporativeCooler_Direct_ResearchSpecialFields::WaterPumpPowerModifierCurveName);
  }

  bool EvaporativeCoolerDirectResearchSpecial_Impl::setEffectivenessFlowRatioModifierCurve(const Curve& curve) {
    return setPointer(OS_EvaporativeCooler_Direct_ResearchSpecialFields::EffectivenessFlowRatioModifierCurveName, curve.handle());
  }

  void EvaporativeCoolerDirectResearchSpecial_Impl::resetEffectivenessFlowRatioModifierCurve() {
    bool result = setString(OS_EvaporativeCooler_Direct_ResearchSpecialFields::EffectivenessFlowRatioModifierCurveName, "");
    OS_ASSERT(result);
  }

  void EvaporativeCoolerDirectResearchSpecial_Impl::setWaterPumpPowerSizingFactor(double waterPumpPowerSizingFactor) {
    bool result = setDouble(OS_EvaporativeCooler_Direct_ResearchSpecialFields::WaterPumpPowerSizingFactor, waterPumpPowerSizingFactor);
    OS_ASSERT(result);
  }

  bool EvaporativeCoolerDirectResearchSpecial_Impl::setWaterPumpPowerModifierCurve(const Curve& curve) {
    return setPointer(OS_EvaporativeCooler_Direct_ResearchSpecialFields::WaterPumpPowerModifierCurveName, curve.handle());
  }

  void EvaporativeCoolerDirectResearchSpecial_Impl::resetWaterPumpPowerModifierCurve() {
    bool result = setString(OS_EvaporativeCooler_Direct_ResearchSpecialFields::WaterPumpPowerModifierCurveName, "");
    OS_ASSERT(result);
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

  setWaterPumpPowerSizingFactor(0.1);
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

boost::optional<double> EvaporativeCoolerDirectResearchSpecial::recirculatingWaterPumpPowerConsumption() const
{
  return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->recirculatingWaterPumpPowerConsumption();
}

void EvaporativeCoolerDirectResearchSpecial::setRecirculatingWaterPumpPowerConsumption( double value )
{
  getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->setRecirculatingWaterPumpPowerConsumption(value);
}


void EvaporativeCoolerDirectResearchSpecial::autosizeRecirculatingWaterPumpPowerConsumption()
{
  getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->autosizeRecirculatingWaterPumpPowerConsumption();
}

bool EvaporativeCoolerDirectResearchSpecial::isRecirculatingWaterPumpPowerConsumptionAutosized() const
{
  return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->isRecirculatingWaterPumpPowerConsumptionAutosized();
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

boost::optional<Curve> EvaporativeCoolerDirectResearchSpecial::effectivenessFlowRatioModifierCurve() const {
  return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->effectivenessFlowRatioModifierCurve();
}

double EvaporativeCoolerDirectResearchSpecial::waterPumpPowerSizingFactor() const {
  return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->waterPumpPowerSizingFactor();
}

boost::optional<Curve> EvaporativeCoolerDirectResearchSpecial::waterPumpPowerModifierCurve() const {
  return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->waterPumpPowerModifierCurve();
}
bool EvaporativeCoolerDirectResearchSpecial::setEffectivenessFlowRatioModifierCurve(const Curve& curve) {
  return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->setEffectivenessFlowRatioModifierCurve(curve);
}

void EvaporativeCoolerDirectResearchSpecial::resetEffectivenessFlowRatioModifierCurve() {
  getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->resetEffectivenessFlowRatioModifierCurve();
}

void EvaporativeCoolerDirectResearchSpecial::setWaterPumpPowerSizingFactor(double waterPumpPowerSizingFactor) {
  getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->setWaterPumpPowerSizingFactor(waterPumpPowerSizingFactor);
}

bool EvaporativeCoolerDirectResearchSpecial::setWaterPumpPowerModifierCurve(const Curve& curve) {
  return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->setWaterPumpPowerModifierCurve(curve);
}

void EvaporativeCoolerDirectResearchSpecial::resetWaterPumpPowerModifierCurve() {
  getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->resetWaterPumpPowerModifierCurve();
}

boost::optional<double> EvaporativeCoolerDirectResearchSpecial::primaryAirDesignFlowRate() const
{
  return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->primaryAirDesignFlowRate();
}

void EvaporativeCoolerDirectResearchSpecial::setPrimaryAirDesignFlowRate( double value )
{
  getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->setPrimaryAirDesignFlowRate(value);
}

void EvaporativeCoolerDirectResearchSpecial::autosizePrimaryAirDesignFlowRate()
{
  getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->autosizePrimaryAirDesignFlowRate();
}

bool EvaporativeCoolerDirectResearchSpecial::isPrimaryAirDesignFlowRateAutosized() const
{
  return getImpl<detail::EvaporativeCoolerDirectResearchSpecial_Impl>()->isPrimaryAirDesignFlowRateAutosized();
}

EvaporativeCoolerDirectResearchSpecial::EvaporativeCoolerDirectResearchSpecial(
  std::shared_ptr<detail::EvaporativeCoolerDirectResearchSpecial_Impl> p)
  : StraightComponent(std::move(p))
{}

IddObjectType EvaporativeCoolerDirectResearchSpecial::iddObjectType() {
  IddObjectType result(IddObjectType::OS_EvaporativeCooler_Direct_ResearchSpecial);
  return result;
}

} // model
} // openstudio
