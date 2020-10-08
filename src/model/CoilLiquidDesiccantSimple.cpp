/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "CoilLiquidDesiccantSimple.hpp"
#include "CoilLiquidDesiccantSimple_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Model.hpp"
#include "ScheduleCompact.hpp"
#include "ScheduleCompact_Impl.hpp"

#include <utilities/idd/OS_Coil_LiquidDesiccant_Simple_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilLiquidDesiccantSimple_Impl::CoilLiquidDesiccantSimple_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : WaterToAirComponent_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilLiquidDesiccantSimple::iddObjectType());
  }

  CoilLiquidDesiccantSimple_Impl::CoilLiquidDesiccantSimple_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
        : WaterToAirComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilLiquidDesiccantSimple::iddObjectType());
  }

  CoilLiquidDesiccantSimple_Impl::CoilLiquidDesiccantSimple_Impl(const CoilLiquidDesiccantSimple_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
                                                                 : WaterToAirComponent_Impl(other,model,keepHandle)
  {}

  CoilLiquidDesiccantSimple_Impl::~CoilLiquidDesiccantSimple_Impl(){}

  std::vector<openstudio::IdfObject> CoilLiquidDesiccantSimple_Impl::remove() {
    if( isRemovable() ) {
      return WaterToAirComponent_Impl::remove();
    }

    return std::vector<IdfObject>();
  }

  ModelObject CoilLiquidDesiccantSimple_Impl::clone(Model model) const {
    CoilLiquidDesiccantSimple newCoil = WaterToAirComponent_Impl::clone( model ).optionalCast<CoilLiquidDesiccantSimple>().get();

    return newCoil;
  }

  IddObjectType CoilLiquidDesiccantSimple_Impl::iddObjectType() const {
    return CoilLiquidDesiccantSimple::iddObjectType();
  }

  const std::vector<std::string>& CoilLiquidDesiccantSimple_Impl::outputVariableNames() const {
    static const std::vector<std::string> result{

    };
    return result;
  }

  std::vector<ScheduleTypeKey> CoilLiquidDesiccantSimple_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Coil_LiquidDesiccant_SimpleFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoilLiquidDesiccantSimple","Availability"));
    }
    return result;
  }

  unsigned CoilLiquidDesiccantSimple_Impl::airInletPort() const {
    return OS_Coil_LiquidDesiccant_SimpleFields::AirInletNodeName;
  }

  unsigned CoilLiquidDesiccantSimple_Impl::airOutletPort() const {
    return OS_Coil_LiquidDesiccant_SimpleFields::AirOutletNodeName;
  }

  unsigned CoilLiquidDesiccantSimple_Impl::waterInletPort() const {
    return OS_Coil_LiquidDesiccant_SimpleFields::WaterInletNodeName;
  }

  unsigned CoilLiquidDesiccantSimple_Impl::waterOutletPort() const {
    return OS_Coil_LiquidDesiccant_SimpleFields::WaterOutletNodeName;
  }

  std::vector<ModelObject> CoilLiquidDesiccantSimple_Impl::children() const {
    std::vector<ModelObject> result;

    return result;
  }

  Schedule CoilLiquidDesiccantSimple_Impl::availabilitySchedule() const {
    OptionalSchedule value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_LiquidDesiccant_SimpleFields::AvailabilityScheduleName);
    if (!value){
      // it is an error if we get here, however we don't want to crash
      // so we hook up to global always on schedule
      LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<CoilLiquidDesiccantSimple_Impl*>(this)->setAvailabilitySchedule(*value);
      value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_LiquidDesiccant_SimpleFields::AvailabilityScheduleName);
    }
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilLiquidDesiccantSimple_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_LiquidDesiccant_SimpleFields::AvailabilityScheduleName,
                              "CoilLiquidDesiccantSimple",
                              "Availability",
                              schedule);
    return result;
  }










} // detail

CoilLiquidDesiccantSimple::CoilLiquidDesiccantSimple(const Model& model, Schedule& availabilitySchedule)
  : WaterToAirComponent(CoilLiquidDesiccantSimple::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilLiquidDesiccantSimple_Impl>());

  setAvailabilitySchedule(availabilitySchedule);
}

CoilLiquidDesiccantSimple::CoilLiquidDesiccantSimple(const Model& model)
  : WaterToAirComponent(CoilLiquidDesiccantSimple::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilLiquidDesiccantSimple_Impl>());

  auto availabilitySchedule = model.alwaysOnDiscreteSchedule();
  setAvailabilitySchedule(availabilitySchedule);
}

CoilLiquidDesiccantSimple::CoilLiquidDesiccantSimple(std::shared_ptr<detail::CoilLiquidDesiccantSimple_Impl> p)
  : WaterToAirComponent(std::move(p))
{}

Schedule CoilLiquidDesiccantSimple::availabilitySchedule() const {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->availabilitySchedule();
}

bool CoilLiquidDesiccantSimple::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->setAvailabilitySchedule(schedule);
}



} // model
} // openstudio
