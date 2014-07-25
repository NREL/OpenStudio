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

#include "ModelPartitionMaterial.hpp"
#include "ModelPartitionMaterial_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ModelPartitionMaterial_Impl::ModelPartitionMaterial_Impl(const IdfObject& idfObject, 
                                                           Model_Impl* model, 
                                                           bool keepHandle)
    : Material_Impl(idfObject, model, keepHandle)
  {}

  ModelPartitionMaterial_Impl::ModelPartitionMaterial_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
    : Material_Impl(other, model, keepHandle)
  {}
  
  ModelPartitionMaterial_Impl::ModelPartitionMaterial_Impl(
      const ModelPartitionMaterial_Impl& other,Model_Impl* model,bool keepHandle)
    : Material_Impl(other, model, keepHandle)
  {}

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

  bool ModelPartitionMaterial_Impl::setThickness(double value) {
    return false;
  }

  bool ModelPartitionMaterial_Impl::setThermalConductivity(double value) {
    return false;
  }

  bool ModelPartitionMaterial_Impl::setThermalConductance(double value) {
    return false;
  }

  bool ModelPartitionMaterial_Impl::setThermalResistivity(double value) {
    return false;
  }

  bool ModelPartitionMaterial_Impl::setThermalResistance(double value) {
    return false;
  }

  bool ModelPartitionMaterial_Impl::setThermalTransmittance(double value) {
    return false;
  }

  bool ModelPartitionMaterial_Impl::setThermalAbsorptance(double value) {
    return false;
  }

  bool ModelPartitionMaterial_Impl::setThermalReflectance(double value) {
    return false;
  }

  bool ModelPartitionMaterial_Impl::setSolarTransmittance(double value) {
    return false;
  }

  bool ModelPartitionMaterial_Impl::setSolarAbsorptance(double value) {
    return false;
  }

  bool ModelPartitionMaterial_Impl::setSolarReflectance(double value) {
    return false;
  }

  bool ModelPartitionMaterial_Impl::setVisibleTransmittance(double value) {
    return false;
  }

  bool ModelPartitionMaterial_Impl::setVisibleAbsorptance(double value) {
    return false;
  }

  bool ModelPartitionMaterial_Impl::setVisibleReflectance(double value) {
    return false;
  }

} // detail

ModelPartitionMaterial::ModelPartitionMaterial(IddObjectType type,const Model& model)
  : Material(type,model) 
{
  OS_ASSERT(getImpl<detail::ModelPartitionMaterial_Impl>());
}

/// @cond
ModelPartitionMaterial::ModelPartitionMaterial(
    std::shared_ptr<detail::ModelPartitionMaterial_Impl> impl)
  : Material(impl)
{}
/// @endcond

} // model
} // openstudio
