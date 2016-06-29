/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "MaterialPropertyGlazingSpectralData.hpp"
#include "MaterialPropertyGlazingSpectralData_Impl.hpp"
#include "ModelExtensibleGroup.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_MaterialProperty_GlazingSpectralData_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  MaterialPropertyGlazingSpectralData_Impl::MaterialPropertyGlazingSpectralData_Impl(const IdfObject& idfObject,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ResourceObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == MaterialPropertyGlazingSpectralData::iddObjectType());
  }

  MaterialPropertyGlazingSpectralData_Impl::MaterialPropertyGlazingSpectralData_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == MaterialPropertyGlazingSpectralData::iddObjectType());
  }

  MaterialPropertyGlazingSpectralData_Impl::MaterialPropertyGlazingSpectralData_Impl(const MaterialPropertyGlazingSpectralData_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& MaterialPropertyGlazingSpectralData_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType MaterialPropertyGlazingSpectralData_Impl::iddObjectType() const {
    return MaterialPropertyGlazingSpectralData::iddObjectType();
  }

  std::vector<SpectralDataField> MaterialPropertyGlazingSpectralData_Impl::spectralDataFields() const
  {
    std::vector<SpectralDataField> result;

    for (const ModelExtensibleGroup& group : castVector<ModelExtensibleGroup>(extensibleGroups()))
    {
      OptionalDouble wavelength = group.getDouble(OS_MaterialProperty_GlazingSpectralDataExtensibleFields::Wavelength);
      OptionalDouble transmittance = group.getDouble(OS_MaterialProperty_GlazingSpectralDataExtensibleFields::Transmittance);
      OptionalDouble frontReflectance = group.getDouble(OS_MaterialProperty_GlazingSpectralDataExtensibleFields::FrontReflectance);
      OptionalDouble backReflectance = group.getDouble(OS_MaterialProperty_GlazingSpectralDataExtensibleFields::BackReflectance);

      if (wavelength && transmittance && frontReflectance && backReflectance){
        result.push_back(SpectralDataField(*wavelength, *transmittance, *frontReflectance, *backReflectance));
      }else{
        LOG(Error, "Could not read SpectralDataField " << group.groupIndex() << " in " << briefDescription() << "." );
      }
    }

    return result;
  }

  bool MaterialPropertyGlazingSpectralData_Impl::setSpectralDataFields(const std::vector<SpectralDataField>& spectralDataFields)
  {
    std::vector<SpectralDataField> originalFields = this->spectralDataFields();

    bool result = true;

    clearExtensibleGroups(false);
    for (const auto& spectralDataField : spectralDataFields) {
      result = result && addSpectralDataField(spectralDataField);
    }

    if (!result){
      clearExtensibleGroups(false);
      for (const auto& spectralDataField : originalFields) {
        addSpectralDataField(spectralDataField);
      }
    }

    return result;
  }

  bool MaterialPropertyGlazingSpectralData_Impl::addSpectralDataField(const SpectralDataField& spectralDataField)
  {
    std::vector<std::string> values;
    values.push_back(toString(spectralDataField.wavelength()));
    values.push_back(toString(spectralDataField.transmittance()));
    values.push_back(toString(spectralDataField.frontReflectance()));
    values.push_back(toString(spectralDataField.backReflectance()));

    ModelExtensibleGroup group = pushExtensibleGroup(values, false).cast<ModelExtensibleGroup>();

    return (!group.empty());
  }

  bool MaterialPropertyGlazingSpectralData_Impl::addSpectralDataField(double wavelength, double transmittance, double frontReflectance, double backReflectance)
  {
    return addSpectralDataField(SpectralDataField(wavelength, transmittance, frontReflectance, backReflectance));
  }

  void MaterialPropertyGlazingSpectralData_Impl::resetSpectralDataFields()
  {
    clearExtensibleGroups(false);
  }

} // detail

SpectralDataField::SpectralDataField(double wavelength, double transmittance, double frontReflectance, double backReflectance)
: m_wavelength(wavelength), m_transmittance(transmittance), m_frontReflectance(frontReflectance), m_backReflectance(backReflectance)
{}

double SpectralDataField::wavelength() const
{
  return m_wavelength;
}

double SpectralDataField::transmittance() const
{
  return m_transmittance;
}

double SpectralDataField::frontReflectance() const
{
  return m_frontReflectance;
}

double SpectralDataField::backReflectance() const
{
  return m_backReflectance;
}

MaterialPropertyGlazingSpectralData::MaterialPropertyGlazingSpectralData(const Model& model)
  : ResourceObject(MaterialPropertyGlazingSpectralData::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::MaterialPropertyGlazingSpectralData_Impl>());
}

IddObjectType MaterialPropertyGlazingSpectralData::iddObjectType() {
  return IddObjectType(IddObjectType::OS_MaterialProperty_GlazingSpectralData);
}

std::vector<SpectralDataField> MaterialPropertyGlazingSpectralData::spectralDataFields() const
{
  return getImpl<detail::MaterialPropertyGlazingSpectralData_Impl>()->spectralDataFields();
}

bool MaterialPropertyGlazingSpectralData::setSpectralDataFields(const std::vector<SpectralDataField>& spectralDataFields)
{
  return getImpl<detail::MaterialPropertyGlazingSpectralData_Impl>()->setSpectralDataFields(spectralDataFields);
}

bool MaterialPropertyGlazingSpectralData::addSpectralDataField(const SpectralDataField& spectralDataField)
{
  return getImpl<detail::MaterialPropertyGlazingSpectralData_Impl>()->addSpectralDataField(spectralDataField);
}

bool MaterialPropertyGlazingSpectralData::addSpectralDataField(double wavelength, double transmittance, double frontReflectance, double backReflectance)
{
  return getImpl<detail::MaterialPropertyGlazingSpectralData_Impl>()->addSpectralDataField(wavelength, transmittance, frontReflectance, backReflectance);
}

void MaterialPropertyGlazingSpectralData::resetSpectralDataFields()
{
  getImpl<detail::MaterialPropertyGlazingSpectralData_Impl>()->resetSpectralDataFields();
}

/// @cond
MaterialPropertyGlazingSpectralData::MaterialPropertyGlazingSpectralData(std::shared_ptr<detail::MaterialPropertyGlazingSpectralData_Impl> impl)
  : ResourceObject(impl)
{}
/// @endcond

} // model
} // openstudio

