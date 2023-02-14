/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "LoadingIndex.hpp"
#include "LoadingIndex_Impl.hpp"

#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_LoadingIndex_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    LoadingIndex_Impl::LoadingIndex_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == LoadingIndex::iddObjectType());
    }

    LoadingIndex_Impl::LoadingIndex_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == LoadingIndex::iddObjectType());
    }

    LoadingIndex_Impl::LoadingIndex_Impl(const LoadingIndex_Impl& other, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& LoadingIndex_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      return result;
    }

    IddObjectType LoadingIndex_Impl::iddObjectType() const {
      return LoadingIndex::iddObjectType();
    }

    ModelObject LoadingIndex_Impl::clone(Model model) const {
      // Don't clone the curves (listed as children)
      return ModelObject_Impl::clone(model).cast<LoadingIndex>();  // NOLINT(bugprone-parent-virtual-call)
    }

    std::vector<ModelObject> LoadingIndex_Impl::children() const {
      return {evaporativeCapacityMultiplierFunctionofTemperatureCurve(), compressorPowerMultiplierFunctionofTemperatureCurve()};
    }

    double LoadingIndex_Impl::compressorSpeed() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_LoadingIndexFields::CompressorSpeed, true);
      OS_ASSERT(value);
      return value.get();
    }

    Curve LoadingIndex_Impl::evaporativeCapacityMultiplierFunctionofTemperatureCurve() const {
      boost::optional<Curve> value = optionalEvaporativeCapacityMultiplierFunctionofTemperatureCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Evaporative Capacity Multiplier Function of Temperature Curve attached.");
      }
      return value.get();
    }

    Curve LoadingIndex_Impl::compressorPowerMultiplierFunctionofTemperatureCurve() const {
      boost::optional<Curve> value = optionalCompressorPowerMultiplierFunctionofTemperatureCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Compressor Power Multiplier Function of Temperature Curve attached.");
      }
      return value.get();
    }

    bool LoadingIndex_Impl::setCompressorSpeed(double compressorSpeed) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_LoadingIndexFields::CompressorSpeed, compressorSpeed);
      return result;
    }

    bool LoadingIndex_Impl::setEvaporativeCapacityMultiplierFunctionofTemperatureCurve(const Curve& curve) {
      bool result = setPointer(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_LoadingIndexFields::
                                 EvaporativeCapacityMultiplierFunctionofTemperatureCurveName,
                               curve.handle());
      return result;
    }

    bool LoadingIndex_Impl::setCompressorPowerMultiplierFunctionofTemperatureCurve(const Curve& curve) {
      bool result = setPointer(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_LoadingIndexFields::CompressorPowerMultiplierFunctionofTemperatureCurveName,
        curve.handle());
      return result;
    }

    boost::optional<Curve> LoadingIndex_Impl::optionalEvaporativeCapacityMultiplierFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_LoadingIndexFields::
          EvaporativeCapacityMultiplierFunctionofTemperatureCurveName);
    }

    boost::optional<Curve> LoadingIndex_Impl::optionalCompressorPowerMultiplierFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_LoadingIndexFields::
          CompressorPowerMultiplierFunctionofTemperatureCurveName);
    }

  }  // namespace detail

  LoadingIndex::LoadingIndex(const Model& model, double compressorSpeed, const Curve& evaporativeCapacityMultiplierFunctionofTemperatureCurve,
                             const Curve& compressorPowerMultiplierFunctionofTemperatureCurve)
    : ParentObject(LoadingIndex::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::LoadingIndex_Impl>());

    bool ok = true;
    ok = setCompressorSpeed(compressorSpeed);
    OS_ASSERT(ok);
    ok = setEvaporativeCapacityMultiplierFunctionofTemperatureCurve(evaporativeCapacityMultiplierFunctionofTemperatureCurve);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s Evaporative Capacity Multiplier Function of Temperature Curve to "
                                     << evaporativeCapacityMultiplierFunctionofTemperatureCurve.briefDescription() << ".");
    }
    ok = setCompressorPowerMultiplierFunctionofTemperatureCurve(compressorPowerMultiplierFunctionofTemperatureCurve);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s Compressor Power Multiplier Function of Temperature Curve to "
                                     << compressorPowerMultiplierFunctionofTemperatureCurve.briefDescription() << ".");
    }
  }

  IddObjectType LoadingIndex::iddObjectType() {
    return IddObjectType(IddObjectType::OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_LoadingIndex);
  }

  double LoadingIndex::compressorSpeed() const {
    return getImpl<detail::LoadingIndex_Impl>()->compressorSpeed();
  }

  Curve LoadingIndex::evaporativeCapacityMultiplierFunctionofTemperatureCurve() const {
    return getImpl<detail::LoadingIndex_Impl>()->evaporativeCapacityMultiplierFunctionofTemperatureCurve();
  }

  Curve LoadingIndex::compressorPowerMultiplierFunctionofTemperatureCurve() const {
    return getImpl<detail::LoadingIndex_Impl>()->compressorPowerMultiplierFunctionofTemperatureCurve();
  }

  bool LoadingIndex::setCompressorSpeed(double compressorSpeed) {
    return getImpl<detail::LoadingIndex_Impl>()->setCompressorSpeed(compressorSpeed);
  }

  bool LoadingIndex::setEvaporativeCapacityMultiplierFunctionofTemperatureCurve(const Curve& curve) {
    return getImpl<detail::LoadingIndex_Impl>()->setEvaporativeCapacityMultiplierFunctionofTemperatureCurve(curve);
  }

  bool LoadingIndex::setCompressorPowerMultiplierFunctionofTemperatureCurve(const Curve& curve) {
    return getImpl<detail::LoadingIndex_Impl>()->setCompressorPowerMultiplierFunctionofTemperatureCurve(curve);
  }

  /// @cond
  LoadingIndex::LoadingIndex(std::shared_ptr<detail::LoadingIndex_Impl> impl) : ParentObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
