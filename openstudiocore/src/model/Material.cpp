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

#include "Material.hpp"
#include "Material_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  Material_Impl::Material_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ResourceObject_Impl(idfObject, model, keepHandle)
  {}

  Material_Impl::Material_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                               Model_Impl* model, 
                               bool keepHandle)
    : ResourceObject_Impl(other, model, keepHandle)
  {}
  
  Material_Impl::Material_Impl(const Material_Impl& other,Model_Impl* model,bool keepHandle)
    : ResourceObject_Impl(other, model, keepHandle)
  {}

  double Material_Impl::thickness() const { return 0.0; }

  boost::optional<double> Material_Impl::interiorVisibleAbsorptance() const { return boost::none; }

  boost::optional<double> Material_Impl::exteriorVisibleAbsorptance() const { return boost::none; }

  bool Material_Impl::setThickness(double value) { return false; }

} // detail

Material::Material(IddObjectType type,const Model& model)
  : ResourceObject(type,model) 
{
  OS_ASSERT(getImpl<detail::Material_Impl>());
}

double Material::thickness() const {
  return getImpl<detail::Material_Impl>()->thickness();
}

OptionalDouble Material::getVisibleTransmittance() const {
  return getImpl<detail::Material_Impl>()->getVisibleTransmittance();
}

boost::optional<double> Material::interiorVisibleAbsorptance() const {
  return getImpl<detail::Material_Impl>()->interiorVisibleAbsorptance();
}

boost::optional<double> Material::exteriorVisibleAbsorptance() const {
  return getImpl<detail::Material_Impl>()->exteriorVisibleAbsorptance();
}

bool Material::setThickness(double value) {
  return getImpl<detail::Material_Impl>()->setThickness(value);
}

/// @cond
Material::Material(std::shared_ptr<detail::Material_Impl> impl)
  : ResourceObject(impl)
{}
/// @endcond

} // model
} // openstudio
