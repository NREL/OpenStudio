/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "EvaporativeCoolerIndirectResearchSpecial.hpp"
#include "EvaporativeCoolerIndirectResearchSpecial_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/OS_EvaporativeCooler_Indirect_ResearchSpecial_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  EvaporativeCoolerIndirectResearchSpecial_Impl::EvaporativeCoolerIndirectResearchSpecial_Impl(const IdfObject& idfObject,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == EvaporativeCoolerIndirectResearchSpecial::iddObjectType());
  }

  EvaporativeCoolerIndirectResearchSpecial_Impl::EvaporativeCoolerIndirectResearchSpecial_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == EvaporativeCoolerIndirectResearchSpecial::iddObjectType());
  }

  EvaporativeCoolerIndirectResearchSpecial_Impl::EvaporativeCoolerIndirectResearchSpecial_Impl(const EvaporativeCoolerIndirectResearchSpecial_Impl& other,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& EvaporativeCoolerIndirectResearchSpecial_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType EvaporativeCoolerIndirectResearchSpecial_Impl::iddObjectType() const {
    return EvaporativeCoolerIndirectResearchSpecial::iddObjectType();
  }

  std::vector<ScheduleTypeKey> EvaporativeCoolerIndirectResearchSpecial_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_EvaporativeCooler_Indirect_ResearchSpecialFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("EvaporativeCoolerIndirectResearchSpecial","Availability"));
    }
    return result;
  }

  boost::optional<Schedule> EvaporativeCoolerIndirectResearchSpecial_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_EvaporativeCooler_Indirect_ResearchSpecialFields::AvailabilityScheduleName);
  }

  double EvaporativeCoolerIndirectResearchSpecial_Impl::coolerMaximumEffectiveness() const {
    boost::optional<double> value = getDouble(OS_EvaporativeCooler_Indirect_ResearchSpecialFields::CoolerMaximumEffectiveness,true);
    OS_ASSERT(value);
    return value.get();
  }

  double EvaporativeCoolerIndirectResearchSpecial_Impl::recirculatingWaterPumpPowerConsumption() const {
    boost::optional<double> value = getDouble(OS_EvaporativeCooler_Indirect_ResearchSpecialFields::RecirculatingWaterPumpPowerConsumption,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> EvaporativeCoolerIndirectResearchSpecial_Impl::secondaryFanFlowRate() const {
    return getDouble(OS_EvaporativeCooler_Indirect_ResearchSpecialFields::SecondaryFanFlowRate,true);
  }

  bool EvaporativeCoolerIndirectResearchSpecial_Impl::isSecondaryFanFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_EvaporativeCooler_Indirect_ResearchSpecialFields::SecondaryFanFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double EvaporativeCoolerIndirectResearchSpecial_Impl::secondaryFanTotalEfficiency() const {
    boost::optional<double> value = getDouble(OS_EvaporativeCooler_Indirect_ResearchSpecialFields::SecondaryFanTotalEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  double EvaporativeCoolerIndirectResearchSpecial_Impl::secondaryFanDeltaPressure() const {
    boost::optional<double> value = getDouble(OS_EvaporativeCooler_Indirect_ResearchSpecialFields::SecondaryFanDeltaPressure,true);
    OS_ASSERT(value);
    return value.get();
  }

  double EvaporativeCoolerIndirectResearchSpecial_Impl::dewpointEffectivenessFactor() const {
    boost::optional<double> value = getDouble(OS_EvaporativeCooler_Indirect_ResearchSpecialFields::DewpointEffectivenessFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> EvaporativeCoolerIndirectResearchSpecial_Impl::driftLossFraction() const {
    return getDouble(OS_EvaporativeCooler_Indirect_ResearchSpecialFields::DriftLossFraction,true);
  }

  boost::optional<double> EvaporativeCoolerIndirectResearchSpecial_Impl::blowdownConcentrationRatio() const {
    return getDouble(OS_EvaporativeCooler_Indirect_ResearchSpecialFields::BlowdownConcentrationRatio,true);
  }

  bool EvaporativeCoolerIndirectResearchSpecial_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_EvaporativeCooler_Indirect_ResearchSpecialFields::AvailabilityScheduleName,
                              "EvaporativeCoolerIndirectResearchSpecial",
                              "Availability",
                              schedule);
    return result;
  }

  void EvaporativeCoolerIndirectResearchSpecial_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_EvaporativeCooler_Indirect_ResearchSpecialFields::AvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  bool EvaporativeCoolerIndirectResearchSpecial_Impl::setCoolerMaximumEffectiveness(double coolerMaximumEffectiveness) {
    bool result = setDouble(OS_EvaporativeCooler_Indirect_ResearchSpecialFields::CoolerMaximumEffectiveness, coolerMaximumEffectiveness);
    return result;
  }

  void EvaporativeCoolerIndirectResearchSpecial_Impl::setRecirculatingWaterPumpPowerConsumption(double recirculatingWaterPumpPowerConsumption) {
    bool result = setDouble(OS_EvaporativeCooler_Indirect_ResearchSpecialFields::RecirculatingWaterPumpPowerConsumption, recirculatingWaterPumpPowerConsumption);
    OS_ASSERT(result);
  }

  bool EvaporativeCoolerIndirectResearchSpecial_Impl::setSecondaryFanFlowRate(boost::optional<double> secondaryFanFlowRate) {
    bool result(false);
    if (secondaryFanFlowRate) {
      result = setDouble(OS_EvaporativeCooler_Indirect_ResearchSpecialFields::SecondaryFanFlowRate, secondaryFanFlowRate.get());
    }
    else {
      resetSecondaryFanFlowRate();
      result = true;
    }
    return result;
  }

  void EvaporativeCoolerIndirectResearchSpecial_Impl::resetSecondaryFanFlowRate() {
    bool result = setString(OS_EvaporativeCooler_Indirect_ResearchSpecialFields::SecondaryFanFlowRate, "");
    OS_ASSERT(result);
  }

  void EvaporativeCoolerIndirectResearchSpecial_Impl::autosizeSecondaryFanFlowRate() {
    bool result = setString(OS_EvaporativeCooler_Indirect_ResearchSpecialFields::SecondaryFanFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool EvaporativeCoolerIndirectResearchSpecial_Impl::setSecondaryFanTotalEfficiency(double secondaryFanTotalEfficiency) {
    bool result = setDouble(OS_EvaporativeCooler_Indirect_ResearchSpecialFields::SecondaryFanTotalEfficiency, secondaryFanTotalEfficiency);
    return result;
  }

  bool EvaporativeCoolerIndirectResearchSpecial_Impl::setSecondaryFanDeltaPressure(double secondaryFanDeltaPressure) {
    bool result = setDouble(OS_EvaporativeCooler_Indirect_ResearchSpecialFields::SecondaryFanDeltaPressure, secondaryFanDeltaPressure);
    return result;
  }

  void EvaporativeCoolerIndirectResearchSpecial_Impl::setDewpointEffectivenessFactor(double dewpointEffectivenessFactor) {
    bool result = setDouble(OS_EvaporativeCooler_Indirect_ResearchSpecialFields::DewpointEffectivenessFactor, dewpointEffectivenessFactor);
    OS_ASSERT(result);
  }

  bool EvaporativeCoolerIndirectResearchSpecial_Impl::setDriftLossFraction(boost::optional<double> driftLossFraction) {
    bool result(false);
    if (driftLossFraction) {
      result = setDouble(OS_EvaporativeCooler_Indirect_ResearchSpecialFields::DriftLossFraction, driftLossFraction.get());
    }
    else {
      resetDriftLossFraction();
      result = true;
    }
    return result;
  }

  void EvaporativeCoolerIndirectResearchSpecial_Impl::resetDriftLossFraction() {
    bool result = setString(OS_EvaporativeCooler_Indirect_ResearchSpecialFields::DriftLossFraction, "");
    OS_ASSERT(result);
  }

  bool EvaporativeCoolerIndirectResearchSpecial_Impl::setBlowdownConcentrationRatio(boost::optional<double> blowdownConcentrationRatio) {
    bool result(false);
    if (blowdownConcentrationRatio) {
      result = setDouble(OS_EvaporativeCooler_Indirect_ResearchSpecialFields::BlowdownConcentrationRatio, blowdownConcentrationRatio.get());
    }
    else {
      resetBlowdownConcentrationRatio();
      result = true;
    }
    return result;
  }

  void EvaporativeCoolerIndirectResearchSpecial_Impl::resetBlowdownConcentrationRatio() {
    bool result = setString(OS_EvaporativeCooler_Indirect_ResearchSpecialFields::BlowdownConcentrationRatio, "");
    OS_ASSERT(result);
  }

  unsigned EvaporativeCoolerIndirectResearchSpecial_Impl::inletPort()
  {
    return OS_EvaporativeCooler_Indirect_ResearchSpecialFields::PrimaryAirInletNode;
  }

  unsigned EvaporativeCoolerIndirectResearchSpecial_Impl::outletPort()
  {
    return OS_EvaporativeCooler_Indirect_ResearchSpecialFields::PrimaryAirOutletNode;
  }


  bool EvaporativeCoolerIndirectResearchSpecial_Impl::addToNode(Node & node)
  {
    if( boost::optional<AirLoopHVAC> airLoop = node.airLoopHVAC() )
    {
      if( ! airLoop->demandComponent(node.handle()) )
      {
        if( StraightComponent_Impl::addToNode( node ) )
        {
          return true;
        }
      }
    }

    return false;
  }

  bool EvaporativeCoolerIndirectResearchSpecial_Impl::setReliefAirInletNode(const Node & node)
  {
    return setPointer(OS_EvaporativeCooler_Indirect_ResearchSpecialFields::ReliefAirInletNode, node.handle());
  }

  boost::optional<Node> EvaporativeCoolerIndirectResearchSpecial_Impl::reliefAirInletNode() const
  {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_EvaporativeCooler_Indirect_ResearchSpecialFields::ReliefAirInletNode);
  }

} // detail

EvaporativeCoolerIndirectResearchSpecial::EvaporativeCoolerIndirectResearchSpecial(const Model& model)
  : StraightComponent(EvaporativeCoolerIndirectResearchSpecial::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::EvaporativeCoolerIndirectResearchSpecial_Impl>());

  setCoolerMaximumEffectiveness(0.75);
  setRecirculatingWaterPumpPowerConsumption(30.0);
  setSecondaryFanTotalEfficiency(0.6);
  setSecondaryFanDeltaPressure(124.6);
  setDewpointEffectivenessFactor(0.9);
  setDriftLossFraction(0.0);
  resetBlowdownConcentrationRatio();
}

IddObjectType EvaporativeCoolerIndirectResearchSpecial::iddObjectType() {
  return IddObjectType(IddObjectType::OS_EvaporativeCooler_Indirect_ResearchSpecial);
}

boost::optional<Schedule> EvaporativeCoolerIndirectResearchSpecial::availabilitySchedule() const {
  return getImpl<detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->availabilitySchedule();
}

double EvaporativeCoolerIndirectResearchSpecial::coolerMaximumEffectiveness() const {
  return getImpl<detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->coolerMaximumEffectiveness();
}

double EvaporativeCoolerIndirectResearchSpecial::recirculatingWaterPumpPowerConsumption() const {
  return getImpl<detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->recirculatingWaterPumpPowerConsumption();
}

boost::optional<double> EvaporativeCoolerIndirectResearchSpecial::secondaryFanFlowRate() const {
  return getImpl<detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->secondaryFanFlowRate();
}

bool EvaporativeCoolerIndirectResearchSpecial::isSecondaryFanFlowRateAutosized() const {
  return getImpl<detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->isSecondaryFanFlowRateAutosized();
}

double EvaporativeCoolerIndirectResearchSpecial::secondaryFanTotalEfficiency() const {
  return getImpl<detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->secondaryFanTotalEfficiency();
}

double EvaporativeCoolerIndirectResearchSpecial::secondaryFanDeltaPressure() const {
  return getImpl<detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->secondaryFanDeltaPressure();
}

double EvaporativeCoolerIndirectResearchSpecial::dewpointEffectivenessFactor() const {
  return getImpl<detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->dewpointEffectivenessFactor();
}

boost::optional<double> EvaporativeCoolerIndirectResearchSpecial::driftLossFraction() const {
  return getImpl<detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->driftLossFraction();
}

boost::optional<double> EvaporativeCoolerIndirectResearchSpecial::blowdownConcentrationRatio() const {
  return getImpl<detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->blowdownConcentrationRatio();
}

bool EvaporativeCoolerIndirectResearchSpecial::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->setAvailabilitySchedule(schedule);
}

void EvaporativeCoolerIndirectResearchSpecial::resetAvailabilitySchedule() {
  getImpl<detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->resetAvailabilitySchedule();
}

bool EvaporativeCoolerIndirectResearchSpecial::setCoolerMaximumEffectiveness(double coolerMaximumEffectiveness) {
  return getImpl<detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->setCoolerMaximumEffectiveness(coolerMaximumEffectiveness);
}

void EvaporativeCoolerIndirectResearchSpecial::setRecirculatingWaterPumpPowerConsumption(double recirculatingWaterPumpPowerConsumption) {
  getImpl<detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->setRecirculatingWaterPumpPowerConsumption(recirculatingWaterPumpPowerConsumption);
}

bool EvaporativeCoolerIndirectResearchSpecial::setSecondaryFanFlowRate(double secondaryFanFlowRate) {
  return getImpl<detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->setSecondaryFanFlowRate(secondaryFanFlowRate);
}

void EvaporativeCoolerIndirectResearchSpecial::resetSecondaryFanFlowRate() {
  getImpl<detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->resetSecondaryFanFlowRate();
}

void EvaporativeCoolerIndirectResearchSpecial::autosizeSecondaryFanFlowRate() {
  getImpl<detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->autosizeSecondaryFanFlowRate();
}

bool EvaporativeCoolerIndirectResearchSpecial::setSecondaryFanTotalEfficiency(double secondaryFanTotalEfficiency) {
  return getImpl<detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->setSecondaryFanTotalEfficiency(secondaryFanTotalEfficiency);
}

bool EvaporativeCoolerIndirectResearchSpecial::setSecondaryFanDeltaPressure(double secondaryFanDeltaPressure) {
  return getImpl<detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->setSecondaryFanDeltaPressure(secondaryFanDeltaPressure);
}

void EvaporativeCoolerIndirectResearchSpecial::setDewpointEffectivenessFactor(double dewpointEffectivenessFactor) {
  getImpl<detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->setDewpointEffectivenessFactor(dewpointEffectivenessFactor);
}

bool EvaporativeCoolerIndirectResearchSpecial::setDriftLossFraction(double driftLossFraction) {
  return getImpl<detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->setDriftLossFraction(driftLossFraction);
}

void EvaporativeCoolerIndirectResearchSpecial::resetDriftLossFraction() {
  getImpl<detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->resetDriftLossFraction();
}

bool EvaporativeCoolerIndirectResearchSpecial::setBlowdownConcentrationRatio(double blowdownConcentrationRatio) {
  return getImpl<detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->setBlowdownConcentrationRatio(blowdownConcentrationRatio);
}

void EvaporativeCoolerIndirectResearchSpecial::resetBlowdownConcentrationRatio() {
  getImpl<detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->resetBlowdownConcentrationRatio();
}

/// @cond
EvaporativeCoolerIndirectResearchSpecial::EvaporativeCoolerIndirectResearchSpecial(std::shared_ptr<detail::EvaporativeCoolerIndirectResearchSpecial_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

