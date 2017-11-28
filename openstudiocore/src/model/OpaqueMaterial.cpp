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

#include "OpaqueMaterial.hpp"
#include "OpaqueMaterial_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  OpaqueMaterial_Impl::OpaqueMaterial_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : Material_Impl(idfObject, model, keepHandle)
  {}

  OpaqueMaterial_Impl::OpaqueMaterial_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : Material_Impl(other, model, keepHandle)
  {}

  OpaqueMaterial_Impl::OpaqueMaterial_Impl(const OpaqueMaterial_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : Material_Impl(other, model, keepHandle)
  {}

  boost::optional<double> OpaqueMaterial_Impl::getVisibleTransmittance() const { return 0.0; }

  boost::optional<double> OpaqueMaterial_Impl::interiorVisibleAbsorptance() const {
    return this->visibleAbsorptance();
  }

  boost::optional<double> OpaqueMaterial_Impl::exteriorVisibleAbsorptance() const {
    return this->visibleAbsorptance();
  }

  boost::optional<double> OpaqueMaterial_Impl::heatCapacity() const { return boost::none; }

} // detail

OpaqueMaterial::OpaqueMaterial(IddObjectType type,const Model& model)
  : Material(type,model)
{
  OS_ASSERT(getImpl<detail::OpaqueMaterial_Impl>());
}

double OpaqueMaterial::thermalConductivity() const {
  return getImpl<detail::OpaqueMaterial_Impl>()->thermalConductivity();
}

double OpaqueMaterial::thermalConductance() const {
  return getImpl<detail::OpaqueMaterial_Impl>()->thermalConductance();
}

double OpaqueMaterial::thermalResistivity() const {
  return getImpl<detail::OpaqueMaterial_Impl>()->thermalResistivity();
}

double OpaqueMaterial::thermalResistance() const {
  return getImpl<detail::OpaqueMaterial_Impl>()->thermalResistance();
}

double OpaqueMaterial::thermalAbsorptance() const {
  return getImpl<detail::OpaqueMaterial_Impl>()->thermalAbsorptance();
}

boost::optional<double> OpaqueMaterial::thermalReflectance() const {
  return getImpl<detail::OpaqueMaterial_Impl>()->thermalReflectance();
}

double OpaqueMaterial::solarAbsorptance() const {
  return getImpl<detail::OpaqueMaterial_Impl>()->solarAbsorptance();
}

boost::optional<double> OpaqueMaterial::solarReflectance() const {
  return getImpl<detail::OpaqueMaterial_Impl>()->solarReflectance();
}

double OpaqueMaterial::visibleAbsorptance() const {
  return getImpl<detail::OpaqueMaterial_Impl>()->visibleAbsorptance();
}

boost::optional<double> OpaqueMaterial::visibleReflectance() const {
  return getImpl<detail::OpaqueMaterial_Impl>()->visibleReflectance();
}

bool OpaqueMaterial::setThermalConductivity(double value) {
  return getImpl<detail::OpaqueMaterial_Impl>()->setThermalConductivity(value);
}

bool OpaqueMaterial::setThermalConductance(double value) {
  return getImpl<detail::OpaqueMaterial_Impl>()->setThermalConductance(value);
}

bool OpaqueMaterial::setThermalResistivity(double value) {
  return getImpl<detail::OpaqueMaterial_Impl>()->setThermalResistivity(value);
}

bool OpaqueMaterial::setThermalResistance(double value) {
  return getImpl<detail::OpaqueMaterial_Impl>()->setThermalResistance(value);
}

bool OpaqueMaterial::setThermalAbsorptance(double value) {
  return getImpl<detail::OpaqueMaterial_Impl>()->setThermalAbsorptance(value);
}

bool OpaqueMaterial::setThermalReflectance(boost::optional<double> value) {
  return getImpl<detail::OpaqueMaterial_Impl>()->setThermalReflectance(value);
}

bool OpaqueMaterial::setSolarAbsorptance(boost::optional<double> value) {
  return getImpl<detail::OpaqueMaterial_Impl>()->setSolarAbsorptance(value);
}

bool OpaqueMaterial::setSolarReflectance(boost::optional<double> value) {
  return getImpl<detail::OpaqueMaterial_Impl>()->setSolarReflectance(value);
}

bool OpaqueMaterial::setVisibleAbsorptance(boost::optional<double> value) {
  return getImpl<detail::OpaqueMaterial_Impl>()->setVisibleAbsorptance(value);
}

bool OpaqueMaterial::setVisibleReflectance(boost::optional<double> value) {
  return getImpl<detail::OpaqueMaterial_Impl>()->setVisibleReflectance(value);
}

boost::optional<double> OpaqueMaterial::heatCapacity() const {
  return getImpl<detail::OpaqueMaterial_Impl>()->heatCapacity();
}

/// @cond
OpaqueMaterial::OpaqueMaterial(std::shared_ptr<detail::OpaqueMaterial_Impl> impl)
  : Material(std::move(impl))
{}
/// @endcond

} // model
} // openstudio
