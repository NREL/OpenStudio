/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SizingPlant.hpp"
#include "SizingPlant_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Sizing_Plant_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SizingPlant_Impl::SizingPlant_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SizingPlant::iddObjectType());
    }

    SizingPlant_Impl::SizingPlant_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SizingPlant::iddObjectType());
    }

    SizingPlant_Impl::SizingPlant_Impl(const SizingPlant_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SizingPlant_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SizingPlant_Impl::iddObjectType() const {
      return SizingPlant::iddObjectType();
    }

    bool SizingPlant_Impl::setParent(ParentObject& newParent) {
      bool result = false;
      if (boost::optional<PlantLoop> plantLoop = newParent.optionalCast<PlantLoop>()) {
        result = this->setPlantLoop(plantLoop.get());
      }
      return result;
    }

    PlantLoop SizingPlant_Impl::plantLoop() const {
      boost::optional<PlantLoop> value = optionalPlantLoop();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have a Plant Loop attached.");
      }
      return value.get();
    }

    std::string SizingPlant_Impl::loopType() const {
      boost::optional<std::string> value = getString(OS_Sizing_PlantFields::LoopType, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SizingPlant_Impl::designLoopExitTemperature() const {
      boost::optional<double> value = getDouble(OS_Sizing_PlantFields::DesignLoopExitTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SizingPlant_Impl::loopDesignTemperatureDifference() const {
      boost::optional<double> value = getDouble(OS_Sizing_PlantFields::LoopDesignTemperatureDifference, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SizingPlant_Impl::setPlantLoop(const PlantLoop& plantLoop) {
      bool result = setPointer(OS_Sizing_PlantFields::PlantorCondenserLoopName, plantLoop.handle());
      return result;
    }

    bool SizingPlant_Impl::setLoopType(const std::string& loopType) {
      bool result = setString(OS_Sizing_PlantFields::LoopType, loopType);
      return result;
    }

    bool SizingPlant_Impl::setDesignLoopExitTemperature(double designLoopExitTemperature) {
      bool result = setDouble(OS_Sizing_PlantFields::DesignLoopExitTemperature, designLoopExitTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool SizingPlant_Impl::setLoopDesignTemperatureDifference(double loopDesignTemperatureDifference) {
      bool result = setDouble(OS_Sizing_PlantFields::LoopDesignTemperatureDifference, loopDesignTemperatureDifference);
      return result;
    }

    boost::optional<PlantLoop> SizingPlant_Impl::optionalPlantLoop() const {
      return getObject<ModelObject>().getModelObjectTarget<PlantLoop>(OS_Sizing_PlantFields::PlantorCondenserLoopName);
    }

    std::vector<std::string> SizingPlant_Impl::loopTypeValues() const {
      return SizingPlant::loopTypeValues();
    }

    boost::optional<ModelObject> SizingPlant_Impl::plantLoopAsModelObject() const {
      OptionalModelObject result = plantLoop();
      return result;
    }

    bool SizingPlant_Impl::setPlantLoopAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalPlantLoop intermediate = modelObject->optionalCast<PlantLoop>();
        if (intermediate) {
          PlantLoop plantLoop(*intermediate);
          return setPlantLoop(plantLoop);
        }
      }
      return false;
    }

    std::string SizingPlant_Impl::sizingOption() const {
      boost::optional<std::string> value = getString(OS_Sizing_PlantFields::SizingOption, true);
      OS_ASSERT(value);
      return value.get();
    }

    int SizingPlant_Impl::zoneTimestepsinAveragingWindow() const {
      boost::optional<int> value = getInt(OS_Sizing_PlantFields::ZoneTimestepsinAveragingWindow, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string SizingPlant_Impl::coincidentSizingFactorMode() const {
      boost::optional<std::string> value = getString(OS_Sizing_PlantFields::CoincidentSizingFactorMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SizingPlant_Impl::setSizingOption(const std::string& sizingOption) {
      bool result = setString(OS_Sizing_PlantFields::SizingOption, sizingOption);
      return result;
    }

    bool SizingPlant_Impl::setZoneTimestepsinAveragingWindow(int zoneTimestepsinAveragingWindow) {
      bool result = setInt(OS_Sizing_PlantFields::ZoneTimestepsinAveragingWindow, zoneTimestepsinAveragingWindow);
      return result;
    }

    bool SizingPlant_Impl::setCoincidentSizingFactorMode(const std::string& coincidentSizingFactorMode) {
      bool result = setString(OS_Sizing_PlantFields::CoincidentSizingFactorMode, coincidentSizingFactorMode);
      return result;
    }

  }  // namespace detail

  SizingPlant::SizingPlant(const Model& model, const PlantLoop& plantLoop) : ModelObject(SizingPlant::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SizingPlant_Impl>());

    setPlantLoop(plantLoop);

    setLoopType("Heating");
    setDesignLoopExitTemperature(82.0);
    setLoopDesignTemperatureDifference(11.0);
    setSizingOption("NonCoincident");
    setZoneTimestepsinAveragingWindow(1);
    setCoincidentSizingFactorMode("None");
  }

  IddObjectType SizingPlant::iddObjectType() {
    return {IddObjectType::OS_Sizing_Plant};
  }

  std::vector<std::string> SizingPlant::loopTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Sizing_PlantFields::LoopType);
  }

  PlantLoop SizingPlant::plantLoop() const {
    return getImpl<detail::SizingPlant_Impl>()->plantLoop();
  }

  std::string SizingPlant::loopType() const {
    return getImpl<detail::SizingPlant_Impl>()->loopType();
  }

  double SizingPlant::designLoopExitTemperature() const {
    return getImpl<detail::SizingPlant_Impl>()->designLoopExitTemperature();
  }

  double SizingPlant::loopDesignTemperatureDifference() const {
    return getImpl<detail::SizingPlant_Impl>()->loopDesignTemperatureDifference();
  }

  bool SizingPlant::setPlantLoop(const PlantLoop& plantLoop) {
    return getImpl<detail::SizingPlant_Impl>()->setPlantLoop(plantLoop);
  }

  bool SizingPlant::setLoopType(const std::string& loopType) {
    return getImpl<detail::SizingPlant_Impl>()->setLoopType(loopType);
  }

  bool SizingPlant::setDesignLoopExitTemperature(double designLoopExitTemperature) {
    return getImpl<detail::SizingPlant_Impl>()->setDesignLoopExitTemperature(designLoopExitTemperature);
  }

  bool SizingPlant::setLoopDesignTemperatureDifference(double loopDesignTemperatureDifference) {
    return getImpl<detail::SizingPlant_Impl>()->setLoopDesignTemperatureDifference(loopDesignTemperatureDifference);
  }

  std::vector<std::string> SizingPlant::sizingOptionValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Sizing_PlantFields::SizingOption);
  }

  std::vector<std::string> SizingPlant::coincidentSizingFactorModeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Sizing_PlantFields::CoincidentSizingFactorMode);
  }

  std::string SizingPlant::sizingOption() const {
    return getImpl<detail::SizingPlant_Impl>()->sizingOption();
  }

  int SizingPlant::zoneTimestepsinAveragingWindow() const {
    return getImpl<detail::SizingPlant_Impl>()->zoneTimestepsinAveragingWindow();
  }

  bool SizingPlant::setSizingOption(const std::string& sizingOption) {
    return getImpl<detail::SizingPlant_Impl>()->setSizingOption(sizingOption);
  }

  bool SizingPlant::setZoneTimestepsinAveragingWindow(int zoneTimestepsinAveragingWindow) {
    return getImpl<detail::SizingPlant_Impl>()->setZoneTimestepsinAveragingWindow(zoneTimestepsinAveragingWindow);
  }

  bool SizingPlant::setCoincidentSizingFactorMode(const std::string& coincidentSizingFactorMode) {
    return getImpl<detail::SizingPlant_Impl>()->setCoincidentSizingFactorMode(coincidentSizingFactorMode);
  }

  std::string SizingPlant::coincidentSizingFactorMode() const {
    return getImpl<detail::SizingPlant_Impl>()->coincidentSizingFactorMode();
  }

  /// @cond
  SizingPlant::SizingPlant(std::shared_ptr<detail::SizingPlant_Impl> impl) : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
