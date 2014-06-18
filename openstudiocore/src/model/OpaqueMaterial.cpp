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
  : Material(impl)
{}
/// @endcond

} // model
} // openstudio
