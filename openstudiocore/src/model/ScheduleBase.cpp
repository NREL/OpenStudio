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

#include "ScheduleBase.hpp"
#include "ScheduleBase_Impl.hpp"

#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeLimits_Impl.hpp"

#include "../utilities/idf/ValidityReport.hpp"

#include "../utilities/units/Unit.hpp"
#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSQuantityVector.hpp"
#include "../utilities/units/QuantityConverter.hpp"
#include "../utilities/units/ScaleFactory.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  // constructor
  ScheduleBase_Impl::ScheduleBase_Impl(const IdfObject& idfObject,
                                       Model_Impl* model,
                                       bool keepHandle)
    : ResourceObject_Impl(idfObject, model, keepHandle)
  {}

  ScheduleBase_Impl::ScheduleBase_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
    : ResourceObject_Impl(other, model,keepHandle)
  {}

  ScheduleBase_Impl::ScheduleBase_Impl(const ScheduleBase_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandles)
    : ResourceObject_Impl(other, model,keepHandles)
  {}

  OSQuantityVector ScheduleBase_Impl::getValues(bool returnIP) const {
    OSQuantityVector result;
    if (OptionalScheduleTypeLimits scheduleTypeLimits = this->scheduleTypeLimits()) {
      if (OptionalUnit siUnits = ScheduleTypeLimits::units(scheduleTypeLimits->unitType(),false)) {
        result = OSQuantityVector(*siUnits,values());
        if (returnIP) {
          OptionalUnit ipUnits = ScheduleTypeLimits::units(scheduleTypeLimits->unitType(),returnIP);
          OS_ASSERT(ipUnits);
          if (ipUnits.get() != siUnits.get()) {
            result = convert(result,*ipUnits);
          }
        }
      }
    }
    return result;
  }

  boost::optional<Quantity> ScheduleBase_Impl::toQuantity(double value, bool returnIP) const {
    OptionalQuantity result;
    if (OptionalScheduleTypeLimits scheduleTypeLimits = this->scheduleTypeLimits()) {
      if (OptionalUnit siUnits = ScheduleTypeLimits::units(scheduleTypeLimits->unitType(),false)) {
        result = Quantity(value,*siUnits);
        if (returnIP) {
          OptionalUnit ipUnits = ScheduleTypeLimits::units(scheduleTypeLimits->unitType(),returnIP);
          OS_ASSERT(ipUnits);
          if (ipUnits.get() != siUnits.get()) {
            result = convert(*result,*ipUnits);
          }
        }
      }
    }
    return result;
  }

  boost::optional<double> ScheduleBase_Impl::toDouble(const Quantity& quantity) const {
    OptionalDouble result;
    OptionalQuantity working(quantity);
    if (OptionalScheduleTypeLimits scheduleTypeLimits = this->scheduleTypeLimits()) {
      if (OptionalUnit siUnits = ScheduleTypeLimits::units(scheduleTypeLimits->unitType(),false)) {
        if (siUnits) {
          if (working->units() == *siUnits) {
            working->setScale(siUnits->scale().exponent);
          }
          else {
            working = convert(*working,*siUnits);
          }
          if (working) {
            result = working->value();
          }
        }
      }
    }
    return result;
  }

  void ScheduleBase_Impl::populateValidityReport(ValidityReport& report,bool checkNames) const {
    // Inherit lower-level errors
    ModelObject_Impl::populateValidityReport(report,checkNames);
    // StrictnessLevel::Draft
    if (report.level() > StrictnessLevel::None) {
      OptionalScheduleTypeLimits scheduleTypeLimits = this->scheduleTypeLimits();
      if (scheduleTypeLimits && !candidateIsCompatibleWithCurrentUse(*scheduleTypeLimits)) {
        report.insertError(DataError(getObject<ScheduleBase>(),
                                     DataErrorType(DataErrorType::DataType)));
      }
    }

    // StrictnessLevel::Final
    if (report.level() > StrictnessLevel::Draft) {
      if (!valuesAreWithinBounds()) {
        report.insertError(DataError(getObject<ScheduleBase>(),
                                     DataErrorType(DataErrorType::NumericBound)));
      }
    }

  }

  bool ScheduleBase_Impl::okToResetScheduleTypeLimits() const {
    // can only reset if not used by object with keys
    for (const ModelObject& user : getObject<ScheduleBase>().getModelObjectSources<ModelObject>()) {
      if (!user.optionalCast<ScheduleBase>()) {
        return false;
      }
    }
    return true;
  }

  bool ScheduleBase_Impl::valuesAreWithinBounds() const {
    if (OptionalScheduleTypeLimits scheduleTypeLimits = this->scheduleTypeLimits()) {
      OptionalDouble minBound = scheduleTypeLimits->lowerLimitValue();
      OptionalDouble maxBound = scheduleTypeLimits->upperLimitValue();
      if (minBound || maxBound) {
        for (double value : values()) {
          if (minBound && (value < *minBound)) {
            return false;
          }
          if (maxBound && (value > *maxBound)) {
            return false;
          }
        }
      }
    }
    return true;
  }

  boost::optional<ModelObject> ScheduleBase_Impl::scheduleTypeLimitsAsModelObject() const {
    OptionalModelObject result;
    OptionalScheduleTypeLimits intermediate = scheduleTypeLimits();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool ScheduleBase_Impl::setScheduleTypeLimitsAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalScheduleTypeLimits intermediate = modelObject->optionalCast<ScheduleTypeLimits>();
      if (intermediate) {
        return setScheduleTypeLimits(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetScheduleTypeLimits();
    }
    return true;
  }

} // detail

boost::optional<ScheduleTypeLimits> ScheduleBase::scheduleTypeLimits() const {
  return getImpl<detail::ScheduleBase_Impl>()->scheduleTypeLimits();
}

bool ScheduleBase::setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) {
  return getImpl<detail::ScheduleBase_Impl>()->setScheduleTypeLimits(scheduleTypeLimits);
}

bool ScheduleBase::resetScheduleTypeLimits() {
  return getImpl<detail::ScheduleBase_Impl>()->resetScheduleTypeLimits();
}

void ScheduleBase::ensureNoLeapDays()
{
  getImpl<detail::ScheduleBase_Impl>()->ensureNoLeapDays();
}

/// @cond
ScheduleBase::ScheduleBase(std::shared_ptr<detail::ScheduleBase_Impl> impl)
  : ResourceObject(impl)
{}

ScheduleBase::ScheduleBase(IddObjectType type, const Model &model)
  : ResourceObject(type,model)
{}
/// @endcond

} // model
} // openstudio

