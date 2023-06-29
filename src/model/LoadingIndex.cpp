/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
