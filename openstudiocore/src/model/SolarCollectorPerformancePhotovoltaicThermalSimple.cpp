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

#include "SolarCollectorPerformancePhotovoltaicThermalSimple.hpp"
#include "SolarCollectorPerformancePhotovoltaicThermalSimple_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SolarCollectorPerformance_PhotovoltaicThermal_Simple_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SolarCollectorPerformancePhotovoltaicThermalSimple_Impl::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl(const IdfObject& idfObject,
                                                                                                                     Model_Impl* model,
                                                                                                                     bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle)
    {
      OS_ASSERT(idfObject.iddObject().type() == SolarCollectorPerformancePhotovoltaicThermalSimple::iddObjectType());
    }

    SolarCollectorPerformancePhotovoltaicThermalSimple_Impl::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                                     Model_Impl* model,
                                                                                                                     bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle)
    {
      OS_ASSERT(other.iddObject().type() == SolarCollectorPerformancePhotovoltaicThermalSimple::iddObjectType());
    }

    SolarCollectorPerformancePhotovoltaicThermalSimple_Impl::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl(const SolarCollectorPerformancePhotovoltaicThermalSimple_Impl& other,
                                                                                                                     Model_Impl* model,
                                                                                                                     bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle)
    {}

    const std::vector<std::string>& SolarCollectorPerformancePhotovoltaicThermalSimple_Impl::outputVariableNames() const
    {
      static std::vector<std::string> result;
      if (result.empty()){
      }
      return result;
    }

    IddObjectType SolarCollectorPerformancePhotovoltaicThermalSimple_Impl::iddObjectType() const {
      return SolarCollectorPerformancePhotovoltaicThermalSimple::iddObjectType();
    }

    std::vector<ScheduleTypeKey> SolarCollectorPerformancePhotovoltaicThermalSimple_Impl::getScheduleTypeKeys(const Schedule& schedule) const
    {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::ThermalConversionEfficiencyScheduleName) != e)
      {
        result.push_back(ScheduleTypeKey("SolarCollectorPerformancePhotovoltaicThermalSimple", "Thermal Conversion Efficiency"));
      }
      return result;
    }

    double SolarCollectorPerformancePhotovoltaicThermalSimple_Impl::fractionOfSurfaceAreaWithActiveThermalCollector() const {
      boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::FractionofSurfaceAreawithActiveThermalCollector, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string SolarCollectorPerformancePhotovoltaicThermalSimple_Impl::thermalConversionEfficiencyInputModeType() const {
      boost::optional<std::string> result = getString(OS_SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::ThermalConversionEfficiencyInputModeType, true);
      OS_ASSERT(result);
      return *result;
    }

    boost::optional<double> SolarCollectorPerformancePhotovoltaicThermalSimple_Impl::thermalConversionEfficiency() const {
      boost::optional<double> result;
      if (istringEqual("Fixed", thermalConversionEfficiencyInputModeType())){
        result = getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::ValueforThermalConversionEfficiencyifFixed, true);
        if (!result){
          result = 0.3;
        }
      }
      return result;
    }


  boost::optional<Schedule> SolarCollectorPerformancePhotovoltaicThermalSimple_Impl::thermalConversionEfficiencySchedule() const {
    boost::optional<Schedule> result;
    if (istringEqual("Scheduled", thermalConversionEfficiencyInputModeType())){
      result = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::ThermalConversionEfficiencyScheduleName);
    }
    return result;
  }

  double SolarCollectorPerformancePhotovoltaicThermalSimple_Impl::frontSurfaceEmittance() const {
    boost::optional<double> value = getDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::FrontSurfaceEmittance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SolarCollectorPerformancePhotovoltaicThermalSimple_Impl::isFrontSurfaceEmittanceDefaulted() const {
    return isEmpty(OS_SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::FrontSurfaceEmittance);
  }

  bool SolarCollectorPerformancePhotovoltaicThermalSimple_Impl::setFractionOfSurfaceAreaWithActiveThermalCollector(double fractionofSurfaceAreawithActiveThermalCollector) {
    bool result = setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::FractionofSurfaceAreawithActiveThermalCollector, fractionofSurfaceAreawithActiveThermalCollector);
    return result;
  }


  bool SolarCollectorPerformancePhotovoltaicThermalSimple_Impl::setThermalConversionEfficiency(double fixedThermalConversionEfficiency) {

    bool result = setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::ValueforThermalConversionEfficiencyifFixed, fixedThermalConversionEfficiency);
    if (result){
      result = setString(OS_SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::ThermalConversionEfficiencyInputModeType, "Fixed");
      OS_ASSERT(result);
      resetThermalConversionEfficiencySchedule();
    }
    return result;
  }

  void SolarCollectorPerformancePhotovoltaicThermalSimple_Impl::resetThermalConversionEfficiency() {
    bool result = setString(OS_SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::ValueforThermalConversionEfficiencyifFixed, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorPerformancePhotovoltaicThermalSimple_Impl::setThermalConversionEfficiencySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::ThermalConversionEfficiencyScheduleName,
                              "SolarCollectorPerformancePhotovoltaicThermalSimple",
                              "Thermal Conversion Efficiency",
                              schedule);
    if (result){
      result = setString(OS_SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::ThermalConversionEfficiencyInputModeType, "Scheduled");
      OS_ASSERT(result);
      resetThermalConversionEfficiency();
    }
    return result;
  }

  void SolarCollectorPerformancePhotovoltaicThermalSimple_Impl::resetThermalConversionEfficiencySchedule() {
    bool result = setString(OS_SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::ThermalConversionEfficiencyScheduleName, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorPerformancePhotovoltaicThermalSimple_Impl::setFrontSurfaceEmittance(double frontSurfaceEmittance) {
    bool result = setDouble(OS_SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::FrontSurfaceEmittance, frontSurfaceEmittance);
    return result;
  }

  void SolarCollectorPerformancePhotovoltaicThermalSimple_Impl::resetFrontSurfaceEmittance() {
    bool result = setString(OS_SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::FrontSurfaceEmittance, "");
    OS_ASSERT(result);
  }

} // detail

SolarCollectorPerformancePhotovoltaicThermalSimple::SolarCollectorPerformancePhotovoltaicThermalSimple(const Model& model)
  : ModelObject(SolarCollectorPerformancePhotovoltaicThermalSimple::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl>());

  // defaults from \EnergyPlusV8-3-0\ExampleFiles\ShopWithSimplePVT.idf

  bool ok = true;
  ok = setThermalConversionEfficiency(0.3);
  OS_ASSERT(ok);
}

IddObjectType SolarCollectorPerformancePhotovoltaicThermalSimple::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SolarCollectorPerformance_PhotovoltaicThermal_Simple);
}

std::vector<std::string> SolarCollectorPerformancePhotovoltaicThermalSimple::thermalConversionEfficiencyInputModeTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::ThermalConversionEfficiencyInputModeType);
}

double SolarCollectorPerformancePhotovoltaicThermalSimple::fractionOfSurfaceAreaWithActiveThermalCollector() const {
  return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl>()->fractionOfSurfaceAreaWithActiveThermalCollector();
}

std::string SolarCollectorPerformancePhotovoltaicThermalSimple::thermalConversionEfficiencyInputModeType() const {
  return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl>()->thermalConversionEfficiencyInputModeType();
}

boost::optional<double> SolarCollectorPerformancePhotovoltaicThermalSimple::thermalConversionEfficiency() const {
  return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl>()->thermalConversionEfficiency();
}

boost::optional<Schedule> SolarCollectorPerformancePhotovoltaicThermalSimple::thermalConversionEfficiencySchedule() const {
  return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl>()->thermalConversionEfficiencySchedule();
}

double SolarCollectorPerformancePhotovoltaicThermalSimple::frontSurfaceEmittance() const {
  return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl>()->frontSurfaceEmittance();
}

bool SolarCollectorPerformancePhotovoltaicThermalSimple::isFrontSurfaceEmittanceDefaulted() const {
  return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl>()->isFrontSurfaceEmittanceDefaulted();
}

bool SolarCollectorPerformancePhotovoltaicThermalSimple::setFractionOfSurfaceAreaWithActiveThermalCollector(double fractionofSurfaceAreawithActiveThermalCollector) {
  return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl>()->setFractionOfSurfaceAreaWithActiveThermalCollector(fractionofSurfaceAreawithActiveThermalCollector);
}

bool SolarCollectorPerformancePhotovoltaicThermalSimple::setThermalConversionEfficiency(double valueforThermalConversionEfficiencyifFixed) {
  return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl>()->setThermalConversionEfficiency(valueforThermalConversionEfficiencyifFixed);
}

void SolarCollectorPerformancePhotovoltaicThermalSimple::resetThermalConversionEfficiency() {
  getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl>()->resetThermalConversionEfficiency();
}

bool SolarCollectorPerformancePhotovoltaicThermalSimple::setThermalConversionEfficiencySchedule(Schedule& schedule) {
  return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl>()->setThermalConversionEfficiencySchedule(schedule);
}

void SolarCollectorPerformancePhotovoltaicThermalSimple::resetThermalConversionEfficiencySchedule() {
  getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl>()->resetThermalConversionEfficiencySchedule();
}

bool SolarCollectorPerformancePhotovoltaicThermalSimple::setFrontSurfaceEmittance(double frontSurfaceEmittance) {
  return getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl>()->setFrontSurfaceEmittance(frontSurfaceEmittance);
}

void SolarCollectorPerformancePhotovoltaicThermalSimple::resetFrontSurfaceEmittance() {
  getImpl<detail::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl>()->resetFrontSurfaceEmittance();
}

/// @cond
SolarCollectorPerformancePhotovoltaicThermalSimple::SolarCollectorPerformancePhotovoltaicThermalSimple(std::shared_ptr<detail::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

