/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ModelPartitionMaterial.hpp"
#include "ModelPartitionMaterial_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ModelPartitionMaterial_Impl::ModelPartitionMaterial_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : Material_Impl(idfObject, model, keepHandle) {}

    ModelPartitionMaterial_Impl::ModelPartitionMaterial_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                             bool keepHandle)
      : Material_Impl(other, model, keepHandle) {}

    ModelPartitionMaterial_Impl::ModelPartitionMaterial_Impl(const ModelPartitionMaterial_Impl& other, Model_Impl* model, bool keepHandle)
      : Material_Impl(other, model, keepHandle) {}

    double ModelPartitionMaterial_Impl::thickness() const {
      return 0.0;
    }

    double ModelPartitionMaterial_Impl::thermalConductivity() const {
      LOG_AND_THROW("Thermal conductivity is not defined for ModelPartitionMaterials.");
      return 0.0;
    }

    double ModelPartitionMaterial_Impl::thermalConductance() const {
      LOG_AND_THROW("Thermal conductance is not defined for ModelPartitionMaterials.");
      return 0.0;
    }

    double ModelPartitionMaterial_Impl::thermalResistivity() const {
      LOG_AND_THROW("Thermal resistivity is not defined for ModelPartitionMaterials.");
      return 0.0;
    }

    double ModelPartitionMaterial_Impl::thermalResistance() const {
      LOG_AND_THROW("Thermal resistance is not defined for ModelPartitionMaterials.");
      return 0.0;
    }

    double ModelPartitionMaterial_Impl::thermalTransmittance() const {
      return 1.0;
    }

    double ModelPartitionMaterial_Impl::thermalAbsorptance() const {
      return 0.0;
    }

    double ModelPartitionMaterial_Impl::thermalReflectance() const {
      return 0.0;
    }

    double ModelPartitionMaterial_Impl::solarTransmittance() const {
      return 1.0;
    }

    double ModelPartitionMaterial_Impl::solarAbsorptance() const {
      return 0.0;
    }

    double ModelPartitionMaterial_Impl::solarReflectance() const {
      return 0.0;
    }

    OptionalDouble ModelPartitionMaterial_Impl::getVisibleTransmittance() const {
      OptionalDouble od(1.0);
      return od;
    }

    double ModelPartitionMaterial_Impl::visibleAbsorptance() const {
      return 0.0;
    }

    double ModelPartitionMaterial_Impl::visibleReflectance() const {
      return 0.0;
    }

    bool ModelPartitionMaterial_Impl::setThickness(double /*value*/) {
      return false;
    }

    bool ModelPartitionMaterial_Impl::setThermalConductivity(double /*value*/) {
      return false;
    }

    bool ModelPartitionMaterial_Impl::setThermalConductance(double /*value*/) {
      return false;
    }

    bool ModelPartitionMaterial_Impl::setThermalResistivity(double /*value*/) {
      return false;
    }

    bool ModelPartitionMaterial_Impl::setThermalResistance(double /*value*/) {
      return false;
    }

    bool ModelPartitionMaterial_Impl::setThermalTransmittance(double /*value*/) {
      return false;
    }

    bool ModelPartitionMaterial_Impl::setThermalAbsorptance(double /*value*/) {
      return false;
    }

    bool ModelPartitionMaterial_Impl::setThermalReflectance(double /*value*/) {
      return false;
    }

    bool ModelPartitionMaterial_Impl::setSolarTransmittance(double /*value*/) {
      return false;
    }

    bool ModelPartitionMaterial_Impl::setSolarAbsorptance(double /*value*/) {
      return false;
    }

    bool ModelPartitionMaterial_Impl::setSolarReflectance(double /*value*/) {
      return false;
    }

    bool ModelPartitionMaterial_Impl::setVisibleTransmittance(double /*value*/) {
      return false;
    }

    bool ModelPartitionMaterial_Impl::setVisibleAbsorptance(double /*value*/) {
      return false;
    }

    bool ModelPartitionMaterial_Impl::setVisibleReflectance(double /*value*/) {
      return false;
    }

  }  // namespace detail

  ModelPartitionMaterial::ModelPartitionMaterial(IddObjectType type, const Model& model) : Material(type, model) {
    OS_ASSERT(getImpl<detail::ModelPartitionMaterial_Impl>());
  }

  /// @cond
  ModelPartitionMaterial::ModelPartitionMaterial(std::shared_ptr<detail::ModelPartitionMaterial_Impl> impl) : Material(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
