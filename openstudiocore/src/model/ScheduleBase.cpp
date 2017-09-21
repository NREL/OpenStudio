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
  : ResourceObject(std::move(impl))
{}

ScheduleBase::ScheduleBase(IddObjectType type, const Model &model)
  : ResourceObject(type,model)
{}
/// @endcond

} // model
} // openstudio

