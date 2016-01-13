/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "CoilHeatingGasMultiStage.hpp"
#include "CoilHeatingGasMultiStage_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CoilHeatingGasMultiStageStageData.hpp"
#include "CoilHeatingGasMultiStageStageData_Impl.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/OS_Coil_Heating_Gas_MultiStage_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilHeatingGasMultiStage_Impl::CoilHeatingGasMultiStage_Impl(const IdfObject& idfObject,
                                                               Model_Impl* model,
                                                               bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilHeatingGasMultiStage::iddObjectType());
  }

  CoilHeatingGasMultiStage_Impl::CoilHeatingGasMultiStage_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                               Model_Impl* model,
                                                               bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilHeatingGasMultiStage::iddObjectType());
  }

  CoilHeatingGasMultiStage_Impl::CoilHeatingGasMultiStage_Impl(const CoilHeatingGasMultiStage_Impl& other,
                                                               Model_Impl* model,
                                                               bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilHeatingGasMultiStage_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilHeatingGasMultiStage_Impl::iddObjectType() const {
    return CoilHeatingGasMultiStage::iddObjectType();
  }

  std::vector<ScheduleTypeKey> CoilHeatingGasMultiStage_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Coil_Heating_Gas_MultiStageFields::AvailabilitySchedule) != e)
    {
      result.push_back(ScheduleTypeKey("CoilHeatingGasMultiStage","Availability Schedule"));
    }
    return result;
  }

  boost::optional<Schedule> CoilHeatingGasMultiStage_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Heating_Gas_MultiStageFields::AvailabilitySchedule);
  }

  boost::optional<Curve> CoilHeatingGasMultiStage_Impl::partLoadFractionCorrelationCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_Gas_MultiStageFields::PartLoadFractionCorrelationCurve);
  }

  boost::optional<double> CoilHeatingGasMultiStage_Impl::parasiticGasLoad() const {
    return getDouble(OS_Coil_Heating_Gas_MultiStageFields::ParasiticGasLoad,true);
  }

  bool CoilHeatingGasMultiStage_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_Heating_Gas_MultiStageFields::AvailabilitySchedule,
                              "CoilHeatingGasMultiStage",
                              "Availability Schedule",
                              schedule);
    return result;
  }

  void CoilHeatingGasMultiStage_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_Coil_Heating_Gas_MultiStageFields::AvailabilitySchedule, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingGasMultiStage_Impl::setPartLoadFractionCorrelationCurve(const boost::optional<Curve>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_Coil_Heating_Gas_MultiStageFields::PartLoadFractionCorrelationCurve, curve.get().handle());
    }
    else {
      resetPartLoadFractionCorrelationCurve();
      result = true;
    }
    return result;
  }

  void CoilHeatingGasMultiStage_Impl::resetPartLoadFractionCorrelationCurve() {
    bool result = setString(OS_Coil_Heating_Gas_MultiStageFields::PartLoadFractionCorrelationCurve, "");
    OS_ASSERT(result);
  }

  void CoilHeatingGasMultiStage_Impl::setParasiticGasLoad(boost::optional<double> parasiticGasLoad) {
    bool result(false);
    if (parasiticGasLoad) {
      result = setDouble(OS_Coil_Heating_Gas_MultiStageFields::ParasiticGasLoad, parasiticGasLoad.get());
    }
    else {
      resetParasiticGasLoad();
      result = true;
    }
    OS_ASSERT(result);
  }

  void CoilHeatingGasMultiStage_Impl::resetParasiticGasLoad() {
    bool result = setString(OS_Coil_Heating_Gas_MultiStageFields::ParasiticGasLoad, "");
    OS_ASSERT(result);
  }

  unsigned CoilHeatingGasMultiStage_Impl::inletPort() {
    return OS_Coil_Heating_Gas_MultiStageFields::AirInletNode;
  }

  unsigned CoilHeatingGasMultiStage_Impl::outletPort() {
    return OS_Coil_Heating_Gas_MultiStageFields::AirOutletNode;
  }

  ModelObject CoilHeatingGasMultiStage_Impl::clone(Model model) const {
    auto t_clone = StraightComponent_Impl::clone(model).cast<CoilHeatingGasMultiStage>();

    auto t_stages = stages();
    for( auto stage: t_stages ) {
      auto stageClone = stage.clone(model).cast<CoilHeatingGasMultiStageStageData>();
      t_clone.addStage(stageClone);
    }
    return t_clone;
  }

  std::vector<ModelObject> CoilHeatingGasMultiStage_Impl::children() const {
    return subsetCastVector<ModelObject>(stages());
  }

  std::vector<CoilHeatingGasMultiStageStageData> CoilHeatingGasMultiStage_Impl::stages() const {
    std::vector<CoilHeatingGasMultiStageStageData> result;
    auto groups = extensibleGroups();
    for( auto group: groups ) {
      auto target = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_Coil_Heating_Gas_MultiStageExtensibleFields::Stage);
      if( target ) {
        if( auto stage = target->optionalCast<CoilHeatingGasMultiStageStageData>() ) {
          result.push_back(stage.get());
        }
      }
    }
    return result;
  }

  void CoilHeatingGasMultiStage_Impl::addStage(CoilHeatingGasMultiStageStageData& stage) {
    auto group = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
    group.setPointer(OS_Coil_Heating_Gas_MultiStageExtensibleFields::Stage,stage.handle());
  }

  boost::optional<HVACComponent> CoilHeatingGasMultiStage_Impl::containingHVACComponent() const
  {
    // AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed
    {
      auto systems = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed>();

      for( const auto & system : systems ) {
        auto coolingCoil = system.coolingCoil();
        if( coolingCoil.handle() == this->handle() ) {
          return system;
        }
      }
    }

    return boost::none;
  }

  bool CoilHeatingGasMultiStage_Impl::addToNode(Node & node)
  {
    return false;
  }

} // detail

CoilHeatingGasMultiStage::CoilHeatingGasMultiStage(const Model& model)
  : StraightComponent(CoilHeatingGasMultiStage::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingGasMultiStage_Impl>());
}

IddObjectType CoilHeatingGasMultiStage::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Heating_Gas_MultiStage);
}

boost::optional<Schedule> CoilHeatingGasMultiStage::availabilitySchedule() const {
  return getImpl<detail::CoilHeatingGasMultiStage_Impl>()->availabilitySchedule();
}

boost::optional<Curve> CoilHeatingGasMultiStage::partLoadFractionCorrelationCurve() const {
  return getImpl<detail::CoilHeatingGasMultiStage_Impl>()->partLoadFractionCorrelationCurve();
}

boost::optional<double> CoilHeatingGasMultiStage::parasiticGasLoad() const {
  return getImpl<detail::CoilHeatingGasMultiStage_Impl>()->parasiticGasLoad();
}

bool CoilHeatingGasMultiStage::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::CoilHeatingGasMultiStage_Impl>()->setAvailabilitySchedule(schedule);
}

void CoilHeatingGasMultiStage::resetAvailabilitySchedule() {
  getImpl<detail::CoilHeatingGasMultiStage_Impl>()->resetAvailabilitySchedule();
}

bool CoilHeatingGasMultiStage::setPartLoadFractionCorrelationCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingGasMultiStage_Impl>()->setPartLoadFractionCorrelationCurve(curve);
}

void CoilHeatingGasMultiStage::resetPartLoadFractionCorrelationCurve() {
  getImpl<detail::CoilHeatingGasMultiStage_Impl>()->resetPartLoadFractionCorrelationCurve();
}

void CoilHeatingGasMultiStage::setParasiticGasLoad(double parasiticGasLoad) {
  getImpl<detail::CoilHeatingGasMultiStage_Impl>()->setParasiticGasLoad(parasiticGasLoad);
}

void CoilHeatingGasMultiStage::resetParasiticGasLoad() {
  getImpl<detail::CoilHeatingGasMultiStage_Impl>()->resetParasiticGasLoad();
}

std::vector<CoilHeatingGasMultiStageStageData> CoilHeatingGasMultiStage::stages() const {
  return getImpl<detail::CoilHeatingGasMultiStage_Impl>()->stages();
}

void CoilHeatingGasMultiStage::addStage(CoilHeatingGasMultiStageStageData& stage) {
  getImpl<detail::CoilHeatingGasMultiStage_Impl>()->addStage(stage);
}

/// @cond
CoilHeatingGasMultiStage::CoilHeatingGasMultiStage(std::shared_ptr<detail::CoilHeatingGasMultiStage_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

