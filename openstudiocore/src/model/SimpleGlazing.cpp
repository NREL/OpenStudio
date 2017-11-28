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

#include "SimpleGlazing.hpp"
#include "SimpleGlazing_Impl.hpp"

#include <utilities/idd/OS_WindowMaterial_SimpleGlazingSystem_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SimpleGlazing_Impl::SimpleGlazing_Impl(const IdfObject& idfObject,
                                         Model_Impl* model,
                                         bool keepHandle)
    : Glazing_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SimpleGlazing::iddObjectType());
  }

  SimpleGlazing_Impl::SimpleGlazing_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle)
    : Glazing_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SimpleGlazing::iddObjectType());
  }

  SimpleGlazing_Impl::SimpleGlazing_Impl(const SimpleGlazing_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle)
    : Glazing_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SimpleGlazing_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SimpleGlazing_Impl::iddObjectType() const {
    return SimpleGlazing::iddObjectType();
  }

  boost::optional<double> SimpleGlazing_Impl::getVisibleTransmittance() const {
    return visibleTransmittance();
  }

  double SimpleGlazing_Impl::uFactor() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_SimpleGlazingSystemFields::UFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SimpleGlazing_Impl::getUFactor(bool returnIP) const {
    OptionalDouble value = uFactor();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_SimpleGlazingSystemFields::UFactor, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double SimpleGlazing_Impl::solarHeatGainCoefficient() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_SimpleGlazingSystemFields::SolarHeatGainCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SimpleGlazing_Impl::getSolarHeatGainCoefficient(bool returnIP) const {
    OptionalDouble value = solarHeatGainCoefficient();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_SimpleGlazingSystemFields::SolarHeatGainCoefficient, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  boost::optional<double> SimpleGlazing_Impl::visibleTransmittance() const {
    return getDouble(OS_WindowMaterial_SimpleGlazingSystemFields::VisibleTransmittance,true);
  }

  OSOptionalQuantity SimpleGlazing_Impl::getVisibleTransmittance(bool returnIP) const {
    OptionalDouble value = visibleTransmittance();
    return getQuantityFromDouble(OS_WindowMaterial_SimpleGlazingSystemFields::VisibleTransmittance, value, returnIP);
  }

  bool SimpleGlazing_Impl::setUFactor(double uFactor) {
    bool result = setDouble(OS_WindowMaterial_SimpleGlazingSystemFields::UFactor, uFactor);
    return result;
  }

  bool SimpleGlazing_Impl::setUFactor(const Quantity& uFactor) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_SimpleGlazingSystemFields::UFactor,uFactor);
    if (!value) {
      return false;
    }
    return setUFactor(value.get());
  }

  bool SimpleGlazing_Impl::setSolarHeatGainCoefficient(double solarHeatGainCoefficient) {
    bool result = setDouble(OS_WindowMaterial_SimpleGlazingSystemFields::SolarHeatGainCoefficient, solarHeatGainCoefficient);
    return result;
  }

  bool SimpleGlazing_Impl::setSolarHeatGainCoefficient(const Quantity& solarHeatGainCoefficient) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_SimpleGlazingSystemFields::SolarHeatGainCoefficient,solarHeatGainCoefficient);
    if (!value) {
      return false;
    }
    return setSolarHeatGainCoefficient(value.get());
  }

  bool SimpleGlazing_Impl::setVisibleTransmittance(boost::optional<double> visibleTransmittance) {
    bool result(false);
    if (visibleTransmittance) {
      result = setDouble(OS_WindowMaterial_SimpleGlazingSystemFields::VisibleTransmittance, visibleTransmittance.get());
    }
    else {
      resetVisibleTransmittance();
      result = true;
    }
    return result;
  }

  bool SimpleGlazing_Impl::setVisibleTransmittance(const OSOptionalQuantity& visibleTransmittance) {
    bool result(false);
    OptionalDouble value;
    if (visibleTransmittance.isSet()) {
      value = getDoubleFromQuantity(OS_WindowMaterial_SimpleGlazingSystemFields::VisibleTransmittance,visibleTransmittance.get());
      if (value) {
        result = setVisibleTransmittance(value);
      }
    }
    else {
      result = setVisibleTransmittance(value);
    }
    return result;
  }

  void SimpleGlazing_Impl::resetVisibleTransmittance() {
    bool result = setString(OS_WindowMaterial_SimpleGlazingSystemFields::VisibleTransmittance, "");
    OS_ASSERT(result);
  }

  openstudio::Quantity SimpleGlazing_Impl::uFactor_SI() const {
    return getUFactor(false);
  }

  openstudio::Quantity SimpleGlazing_Impl::uFactor_IP() const {
    return getUFactor(true);
  }

  openstudio::Quantity SimpleGlazing_Impl::solarHeatGainCoefficient_SI() const {
    return getSolarHeatGainCoefficient(false);
  }

  openstudio::Quantity SimpleGlazing_Impl::solarHeatGainCoefficient_IP() const {
    return getSolarHeatGainCoefficient(true);
  }

  openstudio::OSOptionalQuantity SimpleGlazing_Impl::visibleTransmittance_SI() const {
    return getVisibleTransmittance(false);
  }

  openstudio::OSOptionalQuantity SimpleGlazing_Impl::visibleTransmittance_IP() const {
    return getVisibleTransmittance(true);
  }

} // detail

SimpleGlazing::SimpleGlazing(const Model& model,
                             double uFactor,
                             double solarHeatGainCoefficient)
  : Glazing(SimpleGlazing::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SimpleGlazing_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  ok = setUFactor(uFactor);
  OS_ASSERT(ok);
  ok = setSolarHeatGainCoefficient(solarHeatGainCoefficient);
  OS_ASSERT(ok);
}

IddObjectType SimpleGlazing::iddObjectType() {
  return IddObjectType(IddObjectType::OS_WindowMaterial_SimpleGlazingSystem);
}

double SimpleGlazing::uFactor() const {
  return getImpl<detail::SimpleGlazing_Impl>()->uFactor();
}

Quantity SimpleGlazing::getUFactor(bool returnIP) const {
  return getImpl<detail::SimpleGlazing_Impl>()->getUFactor(returnIP);
}

double SimpleGlazing::solarHeatGainCoefficient() const {
  return getImpl<detail::SimpleGlazing_Impl>()->solarHeatGainCoefficient();
}

Quantity SimpleGlazing::getSolarHeatGainCoefficient(bool returnIP) const {
  return getImpl<detail::SimpleGlazing_Impl>()->getSolarHeatGainCoefficient(returnIP);
}

boost::optional<double> SimpleGlazing::visibleTransmittance() const {
  return getImpl<detail::SimpleGlazing_Impl>()->visibleTransmittance();
}

OSOptionalQuantity SimpleGlazing::getVisibleTransmittance(bool returnIP) const {
  return getImpl<detail::SimpleGlazing_Impl>()->getVisibleTransmittance(returnIP);
}

bool SimpleGlazing::setUFactor(double uFactor) {
  return getImpl<detail::SimpleGlazing_Impl>()->setUFactor(uFactor);
}

bool SimpleGlazing::setUFactor(const Quantity& uFactor) {
  return getImpl<detail::SimpleGlazing_Impl>()->setUFactor(uFactor);
}

bool SimpleGlazing::setSolarHeatGainCoefficient(double solarHeatGainCoefficient) {
  return getImpl<detail::SimpleGlazing_Impl>()->setSolarHeatGainCoefficient(solarHeatGainCoefficient);
}

bool SimpleGlazing::setSolarHeatGainCoefficient(const Quantity& solarHeatGainCoefficient) {
  return getImpl<detail::SimpleGlazing_Impl>()->setSolarHeatGainCoefficient(solarHeatGainCoefficient);
}

bool SimpleGlazing::setVisibleTransmittance(double visibleTransmittance) {
  return getImpl<detail::SimpleGlazing_Impl>()->setVisibleTransmittance(visibleTransmittance);
}

bool SimpleGlazing::setVisibleTransmittance(const Quantity& visibleTransmittance) {
  return getImpl<detail::SimpleGlazing_Impl>()->setVisibleTransmittance(visibleTransmittance);
}

void SimpleGlazing::resetVisibleTransmittance() {
  getImpl<detail::SimpleGlazing_Impl>()->resetVisibleTransmittance();
}

/// @cond
SimpleGlazing::SimpleGlazing(std::shared_ptr<detail::SimpleGlazing_Impl> impl)
  : Glazing(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

