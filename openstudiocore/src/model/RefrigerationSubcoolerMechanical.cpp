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

#include "RefrigerationSubcoolerMechanical.hpp"
#include "RefrigerationSubcoolerMechanical_Impl.hpp"

#include "RefrigerationSystem.hpp"
#include "RefrigerationSystem_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/OS_Refrigeration_Subcooler_Mechanical_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  RefrigerationSubcoolerMechanical_Impl::RefrigerationSubcoolerMechanical_Impl(const IdfObject& idfObject,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == RefrigerationSubcoolerMechanical::iddObjectType());
  }

  RefrigerationSubcoolerMechanical_Impl::RefrigerationSubcoolerMechanical_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == RefrigerationSubcoolerMechanical::iddObjectType());
  }

  RefrigerationSubcoolerMechanical_Impl::RefrigerationSubcoolerMechanical_Impl(const RefrigerationSubcoolerMechanical_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& RefrigerationSubcoolerMechanical_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType RefrigerationSubcoolerMechanical_Impl::iddObjectType() const {
    return RefrigerationSubcoolerMechanical::iddObjectType();
  }

  ModelObject RefrigerationSubcoolerMechanical_Impl::clone(Model model) const
  {
    RefrigerationSubcoolerMechanical modelObjectClone = ModelObject_Impl::clone(model).cast<RefrigerationSubcoolerMechanical>();

    modelObjectClone.resetCapacityProvidingSystem();

    return modelObjectClone;
  }

  boost::optional<RefrigerationSystem> RefrigerationSubcoolerMechanical_Impl::capacityProvidingSystem() const {
    return getObject<ModelObject>().getModelObjectTarget<RefrigerationSystem>(OS_Refrigeration_Subcooler_MechanicalFields::CapacityProvidingSystem);
  }

  boost::optional<double> RefrigerationSubcoolerMechanical_Impl::outletControlTemperature() const {
    return getDouble(OS_Refrigeration_Subcooler_MechanicalFields::OutletControlTemperature,true);
  }

  bool RefrigerationSubcoolerMechanical_Impl::setCapacityProvidingSystem(const boost::optional<RefrigerationSystem>& refrigerationSystem) {
    bool result(false);
    if (refrigerationSystem) {
      result = setPointer(OS_Refrigeration_Subcooler_MechanicalFields::CapacityProvidingSystem, refrigerationSystem.get().handle());
    }
    else {
      resetCapacityProvidingSystem();
      result = true;
    }
    return result;
  }

  void RefrigerationSubcoolerMechanical_Impl::resetCapacityProvidingSystem() {
    bool result = setString(OS_Refrigeration_Subcooler_MechanicalFields::CapacityProvidingSystem, "");
    OS_ASSERT(result);
  }

  void RefrigerationSubcoolerMechanical_Impl::setOutletControlTemperature(boost::optional<double> outletControlTemperature) {
    bool result(false);
    if (outletControlTemperature) {
      result = setDouble(OS_Refrigeration_Subcooler_MechanicalFields::OutletControlTemperature, outletControlTemperature.get());
    }
    else {
      resetOutletControlTemperature();
      result = true;
    }
    OS_ASSERT(result);
  }

  void RefrigerationSubcoolerMechanical_Impl::resetOutletControlTemperature() {
    bool result = setString(OS_Refrigeration_Subcooler_MechanicalFields::OutletControlTemperature, "");
    OS_ASSERT(result);
  }

} // detail

RefrigerationSubcoolerMechanical::RefrigerationSubcoolerMechanical(const Model& model)
  : ModelObject(RefrigerationSubcoolerMechanical::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::RefrigerationSubcoolerMechanical_Impl>());

  setOutletControlTemperature(10.0);
}

IddObjectType RefrigerationSubcoolerMechanical::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Refrigeration_Subcooler_Mechanical);
}

boost::optional<RefrigerationSystem> RefrigerationSubcoolerMechanical::capacityProvidingSystem() const {
  return getImpl<detail::RefrigerationSubcoolerMechanical_Impl>()->capacityProvidingSystem();
}

boost::optional<double> RefrigerationSubcoolerMechanical::outletControlTemperature() const {
  return getImpl<detail::RefrigerationSubcoolerMechanical_Impl>()->outletControlTemperature();
}

bool RefrigerationSubcoolerMechanical::setCapacityProvidingSystem(const RefrigerationSystem& refrigerationSystem) {
  return getImpl<detail::RefrigerationSubcoolerMechanical_Impl>()->setCapacityProvidingSystem(refrigerationSystem);
}

void RefrigerationSubcoolerMechanical::resetCapacityProvidingSystem() {
  getImpl<detail::RefrigerationSubcoolerMechanical_Impl>()->resetCapacityProvidingSystem();
}

void RefrigerationSubcoolerMechanical::setOutletControlTemperature(double outletControlTemperature) {
  getImpl<detail::RefrigerationSubcoolerMechanical_Impl>()->setOutletControlTemperature(outletControlTemperature);
}

void RefrigerationSubcoolerMechanical::resetOutletControlTemperature() {
  getImpl<detail::RefrigerationSubcoolerMechanical_Impl>()->resetOutletControlTemperature();
}

/// @cond
RefrigerationSubcoolerMechanical::RefrigerationSubcoolerMechanical(std::shared_ptr<detail::RefrigerationSubcoolerMechanical_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

