/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Model.hpp"
#include "Model_Impl.hpp"

#include "GeneratorFuelCellElectricalStorage.hpp"
#include "GeneratorFuelCellElectricalStorage_Impl.hpp"
#include "GeneratorFuelCell.hpp"
#include "GeneratorFuelCell_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Generator_FuelCell_ElectricalStorage_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    GeneratorFuelCellElectricalStorage_Impl::GeneratorFuelCellElectricalStorage_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == GeneratorFuelCellElectricalStorage::iddObjectType());
    }

    GeneratorFuelCellElectricalStorage_Impl::GeneratorFuelCellElectricalStorage_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == GeneratorFuelCellElectricalStorage::iddObjectType());
    }

    GeneratorFuelCellElectricalStorage_Impl::GeneratorFuelCellElectricalStorage_Impl(const GeneratorFuelCellElectricalStorage_Impl& other,
                                                                                     Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& GeneratorFuelCellElectricalStorage_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType GeneratorFuelCellElectricalStorage_Impl::iddObjectType() const {
      return GeneratorFuelCellElectricalStorage::iddObjectType();
    }

    // Get the parent GeneratorFuelCell
    boost::optional<GeneratorFuelCell> GeneratorFuelCellElectricalStorage_Impl::fuelCell() const {

      boost::optional<GeneratorFuelCell> fc;
      // We use getModelObjectSources to check if more than one
      std::vector<GeneratorFuelCell> fcs = getObject<ModelObject>().getModelObjectSources<GeneratorFuelCell>(GeneratorFuelCell::iddObjectType());

      if (!fcs.empty()) {
        if (fcs.size() > 1u) {
          LOG(Error, briefDescription() << " is referenced by more than one GeneratorFuelCell, returning the first");
        }
        fc = fcs[0];
      }
      return fc;
    }

    std::string GeneratorFuelCellElectricalStorage_Impl::choiceofModel() const {
      boost::optional<std::string> value = getString(OS_Generator_FuelCell_ElectricalStorageFields::ChoiceofModel, true);
      if (!value) {
        LOG_AND_THROW(" does not have choiceofModel");
      }
      return value.get();
    }

    double GeneratorFuelCellElectricalStorage_Impl::nominalChargingEnergeticEfficiency() const {
      boost::optional<double> value = getDouble(OS_Generator_FuelCell_ElectricalStorageFields::NominalChargingEnergeticEfficiency, true);
      if (!value) {
        LOG_AND_THROW(" does not have nominalChargingEnergeticEfficiency");
      }
      return value.get();
    }

    double GeneratorFuelCellElectricalStorage_Impl::nominalDischargingEnergeticEfficiency() const {
      boost::optional<double> value = getDouble(OS_Generator_FuelCell_ElectricalStorageFields::NominalDischargingEnergeticEfficiency, true);
      if (!value) {
        LOG_AND_THROW(" does not have nominalDischargingEnergeticEfficiency");
      }
      return value.get();
    }

    double GeneratorFuelCellElectricalStorage_Impl::simpleMaximumCapacity() const {
      boost::optional<double> value = getDouble(OS_Generator_FuelCell_ElectricalStorageFields::SimpleMaximumCapacity, true);
      if (!value) {
        LOG_AND_THROW(" does not have simpleMaximumCapacity");
      }
      return value.get();
    }

    double GeneratorFuelCellElectricalStorage_Impl::simpleMaximumPowerDraw() const {
      boost::optional<double> value = getDouble(OS_Generator_FuelCell_ElectricalStorageFields::SimpleMaximumPowerDraw, true);
      if (!value) {
        LOG_AND_THROW(" does not have simpleMaximumPowerDraw");
      }
      return value.get();
    }

    double GeneratorFuelCellElectricalStorage_Impl::simpleMaximumPowerStore() const {
      boost::optional<double> value = getDouble(OS_Generator_FuelCell_ElectricalStorageFields::SimpleMaximumPowerStore, true);
      if (!value) {
        LOG_AND_THROW(" does not have simpleMaximumPowerStore.");
      }
      return value.get();
    }

    double GeneratorFuelCellElectricalStorage_Impl::initialChargeState() const {
      boost::optional<double> value = getDouble(OS_Generator_FuelCell_ElectricalStorageFields::InitialChargeState, true);
      if (!value) {
        LOG_AND_THROW(" does not have initialChargeState.");
      }
      return value.get();
    }

    bool GeneratorFuelCellElectricalStorage_Impl::setChoiceofModel(const std::string& choiceofModel) {
      bool result = setString(OS_Generator_FuelCell_ElectricalStorageFields::ChoiceofModel, choiceofModel);
      return result;
    }

    void GeneratorFuelCellElectricalStorage_Impl::resetChoiceofModel() {
      bool result = setString(OS_Generator_FuelCell_ElectricalStorageFields::ChoiceofModel, "SimpleEfficiencyWithConstraints");
      OS_ASSERT(result);
    }

    bool GeneratorFuelCellElectricalStorage_Impl::setNominalChargingEnergeticEfficiency(double nominalChargingEnergeticEfficiency) {
      bool result = setDouble(OS_Generator_FuelCell_ElectricalStorageFields::NominalChargingEnergeticEfficiency, nominalChargingEnergeticEfficiency);
      return result;
    }

    void GeneratorFuelCellElectricalStorage_Impl::resetNominalChargingEnergeticEfficiency() {
      bool result = setDouble(OS_Generator_FuelCell_ElectricalStorageFields::NominalChargingEnergeticEfficiency, 1.0);
      OS_ASSERT(result);
    }

    bool GeneratorFuelCellElectricalStorage_Impl::setNominalDischargingEnergeticEfficiency(double nominalDischargingEnergeticEfficiency) {
      bool result =
        setDouble(OS_Generator_FuelCell_ElectricalStorageFields::NominalDischargingEnergeticEfficiency, nominalDischargingEnergeticEfficiency);
      return result;
    }

    void GeneratorFuelCellElectricalStorage_Impl::resetNominalDischargingEnergeticEfficiency() {
      bool result = setDouble(OS_Generator_FuelCell_ElectricalStorageFields::NominalDischargingEnergeticEfficiency, 1.0);
      OS_ASSERT(result);
    }

    bool GeneratorFuelCellElectricalStorage_Impl::setSimpleMaximumCapacity(double simpleMaximumCapacity) {
      bool result = setDouble(OS_Generator_FuelCell_ElectricalStorageFields::SimpleMaximumCapacity, simpleMaximumCapacity);
      OS_ASSERT(result);
      return result;
    }

    void GeneratorFuelCellElectricalStorage_Impl::resetSimpleMaximumCapacity() {
      bool result = setDouble(OS_Generator_FuelCell_ElectricalStorageFields::SimpleMaximumCapacity, 0);
      OS_ASSERT(result);
    }

    bool GeneratorFuelCellElectricalStorage_Impl::setSimpleMaximumPowerDraw(double simpleMaximumPowerDraw) {
      bool result = setDouble(OS_Generator_FuelCell_ElectricalStorageFields::SimpleMaximumPowerDraw, simpleMaximumPowerDraw);
      OS_ASSERT(result);
      return result;
    }

    void GeneratorFuelCellElectricalStorage_Impl::resetSimpleMaximumPowerDraw() {
      bool result = setDouble(OS_Generator_FuelCell_ElectricalStorageFields::SimpleMaximumPowerDraw, 0);
      OS_ASSERT(result);
    }

    bool GeneratorFuelCellElectricalStorage_Impl::setSimpleMaximumPowerStore(double simpleMaximumPowerStore) {
      bool result = setDouble(OS_Generator_FuelCell_ElectricalStorageFields::SimpleMaximumPowerStore, simpleMaximumPowerStore);
      OS_ASSERT(result);
      return result;
    }

    void GeneratorFuelCellElectricalStorage_Impl::resetSimpleMaximumPowerStore() {
      bool result = setDouble(OS_Generator_FuelCell_ElectricalStorageFields::SimpleMaximumPowerStore, 0);
      OS_ASSERT(result);
    }

    bool GeneratorFuelCellElectricalStorage_Impl::setInitialChargeState(double initialChargeState) {
      bool result = setDouble(OS_Generator_FuelCell_ElectricalStorageFields::InitialChargeState, initialChargeState);
      OS_ASSERT(result);
      return result;
    }

    void GeneratorFuelCellElectricalStorage_Impl::resetInitialChargeState() {
      bool result = setDouble(OS_Generator_FuelCell_ElectricalStorageFields::InitialChargeState, 0);
      OS_ASSERT(result);
    }

  }  // namespace detail

  GeneratorFuelCellElectricalStorage::GeneratorFuelCellElectricalStorage(const Model& model)
    : ModelObject(GeneratorFuelCellElectricalStorage::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>());

    setChoiceofModel("SimpleEfficiencyWithConstraints");
    setNominalChargingEnergeticEfficiency(1.0);
    setNominalDischargingEnergeticEfficiency(1.0);
    setSimpleMaximumCapacity(0);
    setSimpleMaximumPowerDraw(0);
    setSimpleMaximumPowerStore(0);
    setInitialChargeState(0);
  }

  IddObjectType GeneratorFuelCellElectricalStorage::iddObjectType() {
    return {IddObjectType::OS_Generator_FuelCell_ElectricalStorage};
  }

  std::vector<std::string> GeneratorFuelCellElectricalStorage::choiceofModelValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Generator_FuelCell_ElectricalStorageFields::ChoiceofModel);
  }

  boost::optional<GeneratorFuelCell> GeneratorFuelCellElectricalStorage::fuelCell() const {
    return getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->fuelCell();
  }

  std::string GeneratorFuelCellElectricalStorage::choiceofModel() const {
    return getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->choiceofModel();
  }

  double GeneratorFuelCellElectricalStorage::nominalChargingEnergeticEfficiency() const {
    return getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->nominalChargingEnergeticEfficiency();
  }

  double GeneratorFuelCellElectricalStorage::nominalDischargingEnergeticEfficiency() const {
    return getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->nominalDischargingEnergeticEfficiency();
  }

  double GeneratorFuelCellElectricalStorage::simpleMaximumCapacity() const {
    return getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->simpleMaximumCapacity();
  }

  double GeneratorFuelCellElectricalStorage::simpleMaximumPowerDraw() const {
    return getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->simpleMaximumPowerDraw();
  }

  double GeneratorFuelCellElectricalStorage::simpleMaximumPowerStore() const {
    return getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->simpleMaximumPowerStore();
  }

  double GeneratorFuelCellElectricalStorage::initialChargeState() const {
    return getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->initialChargeState();
  }

  bool GeneratorFuelCellElectricalStorage::setChoiceofModel(const std::string& choiceofModel) {
    return getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->setChoiceofModel(choiceofModel);
  }

  void GeneratorFuelCellElectricalStorage::resetChoiceofModel() {
    getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->resetChoiceofModel();
  }

  bool GeneratorFuelCellElectricalStorage::setNominalChargingEnergeticEfficiency(double nominalChargingEnergeticEfficiency) {
    return getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->setNominalChargingEnergeticEfficiency(nominalChargingEnergeticEfficiency);
  }

  void GeneratorFuelCellElectricalStorage::resetNominalChargingEnergeticEfficiency() {
    getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->resetNominalChargingEnergeticEfficiency();
  }

  bool GeneratorFuelCellElectricalStorage::setNominalDischargingEnergeticEfficiency(double nominalDischargingEnergeticEfficiency) {
    return getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->setNominalDischargingEnergeticEfficiency(
      nominalDischargingEnergeticEfficiency);
  }

  void GeneratorFuelCellElectricalStorage::resetNominalDischargingEnergeticEfficiency() {
    getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->resetNominalDischargingEnergeticEfficiency();
  }

  bool GeneratorFuelCellElectricalStorage::setSimpleMaximumCapacity(double simpleMaximumCapacity) {
    return getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->setSimpleMaximumCapacity(simpleMaximumCapacity);
  }

  void GeneratorFuelCellElectricalStorage::resetSimpleMaximumCapacity() {
    getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->resetSimpleMaximumCapacity();
  }

  bool GeneratorFuelCellElectricalStorage::setSimpleMaximumPowerDraw(double simpleMaximumPowerDraw) {
    return getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->setSimpleMaximumPowerDraw(simpleMaximumPowerDraw);
  }

  void GeneratorFuelCellElectricalStorage::resetSimpleMaximumPowerDraw() {
    getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->resetSimpleMaximumPowerDraw();
  }

  bool GeneratorFuelCellElectricalStorage::setSimpleMaximumPowerStore(double simpleMaximumPowerStore) {
    return getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->setSimpleMaximumPowerStore(simpleMaximumPowerStore);
  }

  void GeneratorFuelCellElectricalStorage::resetSimpleMaximumPowerStore() {
    getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->resetSimpleMaximumPowerStore();
  }

  bool GeneratorFuelCellElectricalStorage::setInitialChargeState(double initialChargeState) {
    return getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->setInitialChargeState(initialChargeState);
  }

  void GeneratorFuelCellElectricalStorage::resetInitialChargeState() {
    getImpl<detail::GeneratorFuelCellElectricalStorage_Impl>()->resetInitialChargeState();
  }

  /// @cond
  GeneratorFuelCellElectricalStorage::GeneratorFuelCellElectricalStorage(std::shared_ptr<detail::GeneratorFuelCellElectricalStorage_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
