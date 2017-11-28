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
  : Material(std::move(impl))
{}
/// @endcond

} // model
} // openstudio
