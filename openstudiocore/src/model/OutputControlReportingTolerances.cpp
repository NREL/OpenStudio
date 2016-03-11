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

#include "OutputControlReportingTolerances.hpp"
#include "OutputControlReportingTolerances_Impl.hpp"

#include <utilities/idd/OS_OutputControl_ReportingTolerances_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  OutputControlReportingTolerances_Impl::OutputControlReportingTolerances_Impl(const IdfObject& idfObject,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == OutputControlReportingTolerances::iddObjectType());
  }

  OutputControlReportingTolerances_Impl::OutputControlReportingTolerances_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == OutputControlReportingTolerances::iddObjectType());
  }

  OutputControlReportingTolerances_Impl::OutputControlReportingTolerances_Impl(const OutputControlReportingTolerances_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& OutputControlReportingTolerances_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType OutputControlReportingTolerances_Impl::iddObjectType() const {
    return OutputControlReportingTolerances::iddObjectType();
  }

  double OutputControlReportingTolerances_Impl::toleranceforTimeHeatingSetpointNotMet() const {
    boost::optional<double> value = getDouble(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeHeatingSetpointNotMet,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity OutputControlReportingTolerances_Impl::getToleranceforTimeHeatingSetpointNotMet(bool returnIP) const {
    OptionalDouble value = toleranceforTimeHeatingSetpointNotMet();
    OSOptionalQuantity result = getQuantityFromDouble(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeHeatingSetpointNotMet, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool OutputControlReportingTolerances_Impl::isToleranceforTimeHeatingSetpointNotMetDefaulted() const {
    return isEmpty(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeHeatingSetpointNotMet);
  }

  double OutputControlReportingTolerances_Impl::toleranceforTimeCoolingSetpointNotMet() const {
    boost::optional<double> value = getDouble(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeCoolingSetpointNotMet,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity OutputControlReportingTolerances_Impl::getToleranceforTimeCoolingSetpointNotMet(bool returnIP) const {
    OptionalDouble value = toleranceforTimeCoolingSetpointNotMet();
    OSOptionalQuantity result = getQuantityFromDouble(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeCoolingSetpointNotMet, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool OutputControlReportingTolerances_Impl::isToleranceforTimeCoolingSetpointNotMetDefaulted() const {
    return isEmpty(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeCoolingSetpointNotMet);
  }

  bool OutputControlReportingTolerances_Impl::setToleranceforTimeHeatingSetpointNotMet(double toleranceforTimeHeatingSetpointNotMet) {
    bool result = setDouble(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeHeatingSetpointNotMet, toleranceforTimeHeatingSetpointNotMet);
    return result;
  }

  bool OutputControlReportingTolerances_Impl::setToleranceforTimeHeatingSetpointNotMet(const Quantity& toleranceforTimeHeatingSetpointNotMet) {
    OptionalDouble value = getDoubleFromQuantity(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeHeatingSetpointNotMet,toleranceforTimeHeatingSetpointNotMet);
    if (!value) {
      return false;
    }
    return setToleranceforTimeHeatingSetpointNotMet(value.get());
  }

  void OutputControlReportingTolerances_Impl::resetToleranceforTimeHeatingSetpointNotMet() {
    bool result = setString(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeHeatingSetpointNotMet, "");
    OS_ASSERT(result);
  }

  bool OutputControlReportingTolerances_Impl::setToleranceforTimeCoolingSetpointNotMet(double toleranceforTimeCoolingSetpointNotMet) {
    bool result = setDouble(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeCoolingSetpointNotMet, toleranceforTimeCoolingSetpointNotMet);
    return result;
  }

  bool OutputControlReportingTolerances_Impl::setToleranceforTimeCoolingSetpointNotMet(const Quantity& toleranceforTimeCoolingSetpointNotMet) {
    OptionalDouble value = getDoubleFromQuantity(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeCoolingSetpointNotMet,toleranceforTimeCoolingSetpointNotMet);
    if (!value) {
      return false;
    }
    return setToleranceforTimeCoolingSetpointNotMet(value.get());
  }

  void OutputControlReportingTolerances_Impl::resetToleranceforTimeCoolingSetpointNotMet() {
    bool result = setString(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeCoolingSetpointNotMet, "");
    OS_ASSERT(result);
  }

  openstudio::Quantity OutputControlReportingTolerances_Impl::toleranceforTimeHeatingSetpointNotMet_SI() const {
    return getToleranceforTimeHeatingSetpointNotMet(false);
  }

  openstudio::Quantity OutputControlReportingTolerances_Impl::toleranceforTimeHeatingSetpointNotMet_IP() const {
    return getToleranceforTimeHeatingSetpointNotMet(true);
  }

  openstudio::Quantity OutputControlReportingTolerances_Impl::toleranceforTimeCoolingSetpointNotMet_SI() const {
    return getToleranceforTimeCoolingSetpointNotMet(false);
  }

  openstudio::Quantity OutputControlReportingTolerances_Impl::toleranceforTimeCoolingSetpointNotMet_IP() const {
    return getToleranceforTimeCoolingSetpointNotMet(true);
  }

} // detail

IddObjectType OutputControlReportingTolerances::iddObjectType() {
  return IddObjectType(IddObjectType::OS_OutputControl_ReportingTolerances);
}

double OutputControlReportingTolerances::toleranceforTimeHeatingSetpointNotMet() const {
  return getImpl<detail::OutputControlReportingTolerances_Impl>()->toleranceforTimeHeatingSetpointNotMet();
}

Quantity OutputControlReportingTolerances::getToleranceforTimeHeatingSetpointNotMet(bool returnIP) const {
  return getImpl<detail::OutputControlReportingTolerances_Impl>()->getToleranceforTimeHeatingSetpointNotMet(returnIP);
}

bool OutputControlReportingTolerances::isToleranceforTimeHeatingSetpointNotMetDefaulted() const {
  return getImpl<detail::OutputControlReportingTolerances_Impl>()->isToleranceforTimeHeatingSetpointNotMetDefaulted();
}

double OutputControlReportingTolerances::toleranceforTimeCoolingSetpointNotMet() const {
  return getImpl<detail::OutputControlReportingTolerances_Impl>()->toleranceforTimeCoolingSetpointNotMet();
}

Quantity OutputControlReportingTolerances::getToleranceforTimeCoolingSetpointNotMet(bool returnIP) const {
  return getImpl<detail::OutputControlReportingTolerances_Impl>()->getToleranceforTimeCoolingSetpointNotMet(returnIP);
}

bool OutputControlReportingTolerances::isToleranceforTimeCoolingSetpointNotMetDefaulted() const {
  return getImpl<detail::OutputControlReportingTolerances_Impl>()->isToleranceforTimeCoolingSetpointNotMetDefaulted();
}

bool OutputControlReportingTolerances::setToleranceforTimeHeatingSetpointNotMet(double toleranceforTimeHeatingSetpointNotMet) {
  return getImpl<detail::OutputControlReportingTolerances_Impl>()->setToleranceforTimeHeatingSetpointNotMet(toleranceforTimeHeatingSetpointNotMet);
}

bool OutputControlReportingTolerances::setToleranceforTimeHeatingSetpointNotMet(const Quantity& toleranceforTimeHeatingSetpointNotMet) {
  return getImpl<detail::OutputControlReportingTolerances_Impl>()->setToleranceforTimeHeatingSetpointNotMet(toleranceforTimeHeatingSetpointNotMet);
}

void OutputControlReportingTolerances::resetToleranceforTimeHeatingSetpointNotMet() {
  getImpl<detail::OutputControlReportingTolerances_Impl>()->resetToleranceforTimeHeatingSetpointNotMet();
}

bool OutputControlReportingTolerances::setToleranceforTimeCoolingSetpointNotMet(double toleranceforTimeCoolingSetpointNotMet) {
  return getImpl<detail::OutputControlReportingTolerances_Impl>()->setToleranceforTimeCoolingSetpointNotMet(toleranceforTimeCoolingSetpointNotMet);
}

bool OutputControlReportingTolerances::setToleranceforTimeCoolingSetpointNotMet(const Quantity& toleranceforTimeCoolingSetpointNotMet) {
  return getImpl<detail::OutputControlReportingTolerances_Impl>()->setToleranceforTimeCoolingSetpointNotMet(toleranceforTimeCoolingSetpointNotMet);
}

void OutputControlReportingTolerances::resetToleranceforTimeCoolingSetpointNotMet() {
  getImpl<detail::OutputControlReportingTolerances_Impl>()->resetToleranceforTimeCoolingSetpointNotMet();
}

/// @cond
OutputControlReportingTolerances::OutputControlReportingTolerances(std::shared_ptr<detail::OutputControlReportingTolerances_Impl> impl)
  : ModelObject(impl)
{}
OutputControlReportingTolerances::OutputControlReportingTolerances(Model& model)
  : ModelObject(OutputControlReportingTolerances::iddObjectType(),model)
{}

/// @endcond

} // model
} // openstudio

