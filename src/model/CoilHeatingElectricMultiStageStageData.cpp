/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "CoilHeatingElectricMultiStageStageData.hpp"
#include "CoilHeatingElectricMultiStageStageData_Impl.hpp"
#include "../model/CoilHeatingElectricMultiStage.hpp"
#include "../model/CoilHeatingElectricMultiStage_Impl.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include <utilities/idd/OS_Coil_Heating_Electric_MultiStage_StageData_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CoilHeatingElectricMultiStageStageData_Impl::CoilHeatingElectricMultiStageStageData_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                             bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilHeatingElectricMultiStageStageData::iddObjectType());
    }

    CoilHeatingElectricMultiStageStageData_Impl::CoilHeatingElectricMultiStageStageData_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilHeatingElectricMultiStageStageData::iddObjectType());
    }

    CoilHeatingElectricMultiStageStageData_Impl::CoilHeatingElectricMultiStageStageData_Impl(const CoilHeatingElectricMultiStageStageData_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilHeatingElectricMultiStageStageData_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType CoilHeatingElectricMultiStageStageData_Impl::iddObjectType() const {
      return CoilHeatingElectricMultiStageStageData::iddObjectType();
    }

    double CoilHeatingElectricMultiStageStageData_Impl::efficiency() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_Electric_MultiStage_StageDataFields::Efficiency, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CoilHeatingElectricMultiStageStageData_Impl::nominalCapacity() const {
      return getDouble(OS_Coil_Heating_Electric_MultiStage_StageDataFields::NominalCapacity, true);
    }

    bool CoilHeatingElectricMultiStageStageData_Impl::isNominalCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Heating_Electric_MultiStage_StageDataFields::NominalCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    bool CoilHeatingElectricMultiStageStageData_Impl::setEfficiency(double Efficiency) {
      bool result = setDouble(OS_Coil_Heating_Electric_MultiStage_StageDataFields::Efficiency, Efficiency);
      return result;
    }

    bool CoilHeatingElectricMultiStageStageData_Impl::setNominalCapacity(boost::optional<double> NominalCapacity) {
      bool result(false);
      if (NominalCapacity) {
        result = setDouble(OS_Coil_Heating_Electric_MultiStage_StageDataFields::NominalCapacity, NominalCapacity.get());
      }
      return result;
    }

    void CoilHeatingElectricMultiStageStageData_Impl::autosizeNominalCapacity() {
      bool result = setString(OS_Coil_Heating_Electric_MultiStage_StageDataFields::NominalCapacity, "autosize");
      OS_ASSERT(result);
    }

    std::vector<IdfObject> CoilHeatingElectricMultiStageStageData_Impl::remove() {
      if (auto _coil = parentCoil()) {
        _coil->removeStage(getObject<CoilHeatingElectricMultiStageStageData>());
      }
      return ModelObject_Impl::remove();
    }

    boost::optional<CoilHeatingElectricMultiStage> CoilHeatingElectricMultiStageStageData_Impl::parentCoil() const {
      auto coils = getObject<ModelObject>().getModelObjectSources<CoilHeatingElectricMultiStage>(CoilHeatingElectricMultiStage::iddObjectType());
      auto count = coils.size();
      if (count == 1) {
        return coils[0];
      } else if (count > 1) {
        LOG(Error, briefDescription() << " is referenced by more than one CoilHeatingElectricMultiStage, returning the first");
        return coils[0];
      }
      return boost::none;
    }

    boost::optional<std::tuple<int, CoilHeatingElectricMultiStage>> CoilHeatingElectricMultiStageStageData_Impl::stageIndexAndParentCoil() const {

      boost::optional<std::tuple<int, CoilHeatingElectricMultiStage>> result;

      if (auto _coil = parentCoil()) {
        result = std::make_tuple(_coil->stageIndex(getObject<CoilHeatingElectricMultiStageStageData>()).get(), _coil.get());
      } else {
        LOG(Warn, name().get() + " was not found inside a CoilHeatingElectricMultiStage in the model, cannot retrieve the autosized value.");
      }

      return result;
    }

    boost::optional<double> CoilHeatingElectricMultiStageStageData_Impl::autosizedNominalCapacity() const {
      auto indexAndNameOpt = stageIndexAndParentCoil();
      boost::optional<double> result;
      if (!indexAndNameOpt) {
        return result;
      }
      auto [index, parentCoil] = indexAndNameOpt.get();
      const std::string sqlField = "Design Size Stage " + std::to_string(index) + " Nominal Capacity";

      // EPLUS-SQL-INCONSISTENCY
      return parentCoil.getImpl<CoilHeatingElectricMultiStage_Impl>()->getAutosizedValue(sqlField, "W", "Coil:Heating:ElectricMultiStage");
    }

    void CoilHeatingElectricMultiStageStageData_Impl::autosize() {
      autosizeNominalCapacity();
    }

    void CoilHeatingElectricMultiStageStageData_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedNominalCapacity();
      if (val) {
        setNominalCapacity(val.get());
      }
    }

  }  // namespace detail

  CoilHeatingElectricMultiStageStageData::CoilHeatingElectricMultiStageStageData(const Model& model)
    : ModelObject(CoilHeatingElectricMultiStageStageData::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilHeatingElectricMultiStageStageData_Impl>());

    setEfficiency(1.0);
    autosizeNominalCapacity();
  }

  IddObjectType CoilHeatingElectricMultiStageStageData::iddObjectType() {
    return {IddObjectType::OS_Coil_Heating_Electric_MultiStage_StageData};
  }

  double CoilHeatingElectricMultiStageStageData::efficiency() const {
    return getImpl<detail::CoilHeatingElectricMultiStageStageData_Impl>()->efficiency();
  }

  boost::optional<double> CoilHeatingElectricMultiStageStageData::nominalCapacity() const {
    return getImpl<detail::CoilHeatingElectricMultiStageStageData_Impl>()->nominalCapacity();
  }

  bool CoilHeatingElectricMultiStageStageData::isNominalCapacityAutosized() const {
    return getImpl<detail::CoilHeatingElectricMultiStageStageData_Impl>()->isNominalCapacityAutosized();
  }

  bool CoilHeatingElectricMultiStageStageData::setEfficiency(double Efficiency) {
    return getImpl<detail::CoilHeatingElectricMultiStageStageData_Impl>()->setEfficiency(Efficiency);
  }

  bool CoilHeatingElectricMultiStageStageData::setNominalCapacity(double NominalCapacity) {
    return getImpl<detail::CoilHeatingElectricMultiStageStageData_Impl>()->setNominalCapacity(NominalCapacity);
  }

  void CoilHeatingElectricMultiStageStageData::autosizeNominalCapacity() {
    getImpl<detail::CoilHeatingElectricMultiStageStageData_Impl>()->autosizeNominalCapacity();
  }

  /// @cond
  CoilHeatingElectricMultiStageStageData::CoilHeatingElectricMultiStageStageData(
    std::shared_ptr<detail::CoilHeatingElectricMultiStageStageData_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

  boost::optional<double> CoilHeatingElectricMultiStageStageData::autosizedNominalCapacity() const {
    return getImpl<detail::CoilHeatingElectricMultiStageStageData_Impl>()->autosizedNominalCapacity();
  }

  void CoilHeatingElectricMultiStageStageData::autosize() {
    return getImpl<detail::CoilHeatingElectricMultiStageStageData_Impl>()->autosize();
  }

  void CoilHeatingElectricMultiStageStageData::applySizingValues() {
    return getImpl<detail::CoilHeatingElectricMultiStageStageData_Impl>()->applySizingValues();
  }

  boost::optional<CoilHeatingElectricMultiStage> CoilHeatingElectricMultiStageStageData::parentCoil() const {
    return getImpl<detail::CoilHeatingElectricMultiStageStageData_Impl>()->parentCoil();
  }

  boost::optional<std::tuple<int, CoilHeatingElectricMultiStage>> CoilHeatingElectricMultiStageStageData::stageIndexAndParentCoil() const {
    return getImpl<detail::CoilHeatingElectricMultiStageStageData_Impl>()->stageIndexAndParentCoil();
  }

}  // namespace model
}  // namespace openstudio
