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
  : ResourceObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

