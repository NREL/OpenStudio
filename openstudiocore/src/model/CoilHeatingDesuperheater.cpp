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

#include "CoilHeatingDesuperheater.hpp"
#include "CoilHeatingDesuperheater_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ModelObject.hpp"
#include "ModelObject_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/OS_Coil_Heating_Desuperheater_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilHeatingDesuperheater_Impl::CoilHeatingDesuperheater_Impl(const IdfObject& idfObject,
                                                               Model_Impl* model,
                                                               bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilHeatingDesuperheater::iddObjectType());
  }

  CoilHeatingDesuperheater_Impl::CoilHeatingDesuperheater_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                               Model_Impl* model,
                                                               bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilHeatingDesuperheater::iddObjectType());
  }

  CoilHeatingDesuperheater_Impl::CoilHeatingDesuperheater_Impl(const CoilHeatingDesuperheater_Impl& other,
                                                               Model_Impl* model,
                                                               bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilHeatingDesuperheater_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilHeatingDesuperheater_Impl::iddObjectType() const {
    return CoilHeatingDesuperheater::iddObjectType();
  }

  std::vector<ScheduleTypeKey> CoilHeatingDesuperheater_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Coil_Heating_DesuperheaterFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoilHeatingDesuperheater","Availability"));
    }
    return result;
  }

  ModelObject CoilHeatingDesuperheater_Impl::clone(Model model) const {
      CoilHeatingDesuperheater modelObjectClone = StraightComponent_Impl::clone(model).cast<CoilHeatingDesuperheater>();

      modelObjectClone.resetHeatingSource();

      return modelObjectClone;
  }

  bool CoilHeatingDesuperheater_Impl::addToNode(Node & node) {
    if( boost::optional<AirLoopHVAC> airLoop = node.airLoopHVAC() )
    {
      if( airLoop->supplyComponent(node.handle()) )
      {
        return StraightComponent_Impl::addToNode(node);
      }
    }

    return false;
  }

  boost::optional<Schedule> CoilHeatingDesuperheater_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Heating_DesuperheaterFields::AvailabilityScheduleName);
  }

  double CoilHeatingDesuperheater_Impl::heatReclaimRecoveryEfficiency() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_DesuperheaterFields::HeatReclaimRecoveryEfficiency,true);
      OS_ASSERT(value);
      return value.get();
  }

  bool CoilHeatingDesuperheater_Impl::isHeatReclaimRecoveryEfficiencyDefaulted() const {
      return isEmpty(OS_Coil_Heating_DesuperheaterFields::HeatReclaimRecoveryEfficiency);
  }

  unsigned CoilHeatingDesuperheater_Impl::inletPort()
  {
      return OS_Coil_Heating_DesuperheaterFields::AirInletNodeName;
  }

  unsigned CoilHeatingDesuperheater_Impl::outletPort()
  {
      return OS_Coil_Heating_DesuperheaterFields::AirOutletNodeName;
  }

  boost::optional<ModelObject> CoilHeatingDesuperheater_Impl::heatingSource() const {
      return getObject<ModelObject>().getModelObjectTarget<ModelObject>(OS_Coil_Heating_DesuperheaterFields::HeatingSourceName);
  }

  double CoilHeatingDesuperheater_Impl::parasiticElectricLoad() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_DesuperheaterFields::ParasiticElectricLoad,true);
      OS_ASSERT(value);
      return value.get();
  }

  bool CoilHeatingDesuperheater_Impl::isParasiticElectricLoadDefaulted() const {
      return isEmpty(OS_Coil_Heating_DesuperheaterFields::ParasiticElectricLoad);
  }

  bool CoilHeatingDesuperheater_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_Heating_DesuperheaterFields::AvailabilityScheduleName,
                              "CoilHeatingDesuperheater",
                              "Availability",
                              schedule);
    return result;
  }

  void CoilHeatingDesuperheater_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_Coil_Heating_DesuperheaterFields::AvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingDesuperheater_Impl::setHeatReclaimRecoveryEfficiency(double heatReclaimRecoveryEfficiency) {
      bool result = setDouble(OS_Coil_Heating_DesuperheaterFields::HeatReclaimRecoveryEfficiency, heatReclaimRecoveryEfficiency);
      return result;
  }

  void CoilHeatingDesuperheater_Impl::resetHeatReclaimRecoveryEfficiency() {
    bool result = setString(OS_Coil_Heating_DesuperheaterFields::HeatReclaimRecoveryEfficiency, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingDesuperheater_Impl::setHeatingSource(const boost::optional<ModelObject>& modelObject) {
      bool result(false);
      if (modelObject) {
          result = setPointer(OS_Coil_Heating_DesuperheaterFields::HeatingSourceName, modelObject.get().handle());
      }
      else {
          resetHeatingSource();
          result = true;
      }
      return result;
  }

  void CoilHeatingDesuperheater_Impl::resetHeatingSource() {
      bool result = setString(OS_Coil_Heating_DesuperheaterFields::HeatingSourceName, "");
      OS_ASSERT(result);
  }

  bool CoilHeatingDesuperheater_Impl::setParasiticElectricLoad(double parasiticElectricLoad) {
      bool result = setDouble(OS_Coil_Heating_DesuperheaterFields::ParasiticElectricLoad, parasiticElectricLoad);
      return result;
  }

  void CoilHeatingDesuperheater_Impl::resetParasiticElectricLoad() {
    bool result = setString(OS_Coil_Heating_DesuperheaterFields::ParasiticElectricLoad, "");
    OS_ASSERT(result);
  }

  boost::optional<HVACComponent> CoilHeatingDesuperheater_Impl::containingHVACComponent() const
  {
    // AirLoopHVACUnitarySystem
    std::vector<AirLoopHVACUnitarySystem> airLoopHVACUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

    for( const auto & airLoopHVACUnitarySystem : airLoopHVACUnitarySystems )
    {
      if( boost::optional<HVACComponent> heatingCoil = airLoopHVACUnitarySystem.heatingCoil() )
      {
        if( heatingCoil->handle() == this->handle() )
        {
          return airLoopHVACUnitarySystem;
        }
      }
      if( boost::optional<HVACComponent> suppHeatingCoil = airLoopHVACUnitarySystem.supplementalHeatingCoil() )
      {
        if( suppHeatingCoil->handle() == this->handle() )
        {
          return airLoopHVACUnitarySystem;
        }
      }
    }
    return boost::none;
  }

} // detail

CoilHeatingDesuperheater::CoilHeatingDesuperheater(const Model& model)
  : StraightComponent(CoilHeatingDesuperheater::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingDesuperheater_Impl>());

  bool ok = setHeatReclaimRecoveryEfficiency(0.8);
  OS_ASSERT(ok);
  ok = setParasiticElectricLoad(0.0);
  OS_ASSERT(ok);
}

IddObjectType CoilHeatingDesuperheater::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Heating_Desuperheater);
}

boost::optional<Schedule> CoilHeatingDesuperheater::availabilitySchedule() const {
  return getImpl<detail::CoilHeatingDesuperheater_Impl>()->availabilitySchedule();
}

double CoilHeatingDesuperheater::heatReclaimRecoveryEfficiency() const {
    return getImpl<detail::CoilHeatingDesuperheater_Impl>()->heatReclaimRecoveryEfficiency();
}

bool CoilHeatingDesuperheater::isHeatReclaimRecoveryEfficiencyDefaulted() const {
    return getImpl<detail::CoilHeatingDesuperheater_Impl>()->isHeatReclaimRecoveryEfficiencyDefaulted();
}

boost::optional<ModelObject> CoilHeatingDesuperheater::heatingSource() const {
    return getImpl<detail::CoilHeatingDesuperheater_Impl>()->heatingSource();
}

double CoilHeatingDesuperheater::parasiticElectricLoad() const {
    return getImpl<detail::CoilHeatingDesuperheater_Impl>()->parasiticElectricLoad();
}

bool CoilHeatingDesuperheater::isParasiticElectricLoadDefaulted() const {
    return getImpl<detail::CoilHeatingDesuperheater_Impl>()->isParasiticElectricLoadDefaulted();
}

bool CoilHeatingDesuperheater::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::CoilHeatingDesuperheater_Impl>()->setAvailabilitySchedule(schedule);
}

void CoilHeatingDesuperheater::resetAvailabilitySchedule() {
  getImpl<detail::CoilHeatingDesuperheater_Impl>()->resetAvailabilitySchedule();
}

bool CoilHeatingDesuperheater::setHeatReclaimRecoveryEfficiency(double heatReclaimRecoveryEfficiency) {
  return getImpl<detail::CoilHeatingDesuperheater_Impl>()->setHeatReclaimRecoveryEfficiency(heatReclaimRecoveryEfficiency);
}

void CoilHeatingDesuperheater::resetHeatReclaimRecoveryEfficiency() {
  getImpl<detail::CoilHeatingDesuperheater_Impl>()->resetHeatReclaimRecoveryEfficiency();
}

bool CoilHeatingDesuperheater::setHeatingSource(const ModelObject& modelObject) {
    return getImpl<detail::CoilHeatingDesuperheater_Impl>()->setHeatingSource(modelObject);
}

void CoilHeatingDesuperheater::resetHeatingSource() {
    getImpl<detail::CoilHeatingDesuperheater_Impl>()->resetHeatingSource();
}

bool CoilHeatingDesuperheater::setParasiticElectricLoad(double parasiticElectricLoad) {
  return getImpl<detail::CoilHeatingDesuperheater_Impl>()->setParasiticElectricLoad(parasiticElectricLoad);
}

void CoilHeatingDesuperheater::resetParasiticElectricLoad() {
  getImpl<detail::CoilHeatingDesuperheater_Impl>()->resetParasiticElectricLoad();
}

/// @cond
CoilHeatingDesuperheater::CoilHeatingDesuperheater(std::shared_ptr<detail::CoilHeatingDesuperheater_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

