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

#include <model/StandardsInformationMaterial.hpp>
#include <model/StandardsInformationMaterial_Impl.hpp>

// TODO: Check the following class names against object getters and setters.
#include <model/Material.hpp>
#include <model/Material_Impl.hpp>

#include <utilities/idd/OS_StandardsInformation_Material_FieldEnums.hxx>

#include <utilities/core/Assert.hpp>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

namespace openstudio {
namespace model {

namespace detail {

  QMap<QString, QVariant> StandardsInformationMaterial_Impl::m_standardsMap;

  StandardsInformationMaterial_Impl::StandardsInformationMaterial_Impl(const IdfObject& idfObject,
    Model_Impl* model,
    bool keepHandle)
    : ModelObject_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == StandardsInformationMaterial::iddObjectType());
  }

  StandardsInformationMaterial_Impl::StandardsInformationMaterial_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
    Model_Impl* model,
    bool keepHandle)
    : ModelObject_Impl(other, model, keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == StandardsInformationMaterial::iddObjectType());
  }

  StandardsInformationMaterial_Impl::StandardsInformationMaterial_Impl(const StandardsInformationMaterial_Impl& other,
    Model_Impl* model,
    bool keepHandle)
    : ModelObject_Impl(other, model, keepHandle)
  {}

  void StandardsInformationMaterial_Impl::parseStandardsMap() const
  {
    if (m_standardsMap.empty()){
      QFile file(":/resources/standards/OpenStudio_Standards.json");
      if (file.open(QFile::ReadOnly)) {
        QJsonParseError parseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll(), &parseError);
        file.close();
        if (QJsonParseError::NoError == parseError.error) {
          m_standardsMap = jsonDoc.object().toVariantMap();
        }
      }
    }
  }

  const std::vector<std::string>& StandardsInformationMaterial_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType StandardsInformationMaterial_Impl::iddObjectType() const {
    return StandardsInformationMaterial::iddObjectType();
  }

  Material StandardsInformationMaterial_Impl::material() const {
    boost::optional<Material> value = optionalMaterial();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Material attached.");
    }
    return value.get();
  }

  boost::optional<std::string> StandardsInformationMaterial_Impl::materialStandard() const {
    return getString(OS_StandardsInformation_MaterialFields::MaterialStandard, true);
  }

  std::vector<std::string> StandardsInformationMaterial_Impl::suggestedMaterialStandard() const {
    std::vector<std::string> results;

    boost::optional<std::string> materialStandard = this->materialStandard();

    // include values from json
    parseStandardsMap();

    QMap<QString, QVariant> templates = m_standardsMap["materials"].toMap();
    for (QString template_name : templates.uniqueKeys()) {
      QMap<QString, QVariant> material_types = templates[template_name].toMap();
      for (QString material_type_name : material_types.uniqueKeys()) {
        results.push_back(toString(material_type_name));
      }
    }

    return results;
  }

  boost::optional<std::string> StandardsInformationMaterial_Impl::materialStandardSource() const {
    return getString(OS_StandardsInformation_MaterialFields::MaterialStandardSource,true);
  }

  std::vector<std::string> StandardsInformationMaterial_Impl::suggestedMaterialStandardSource() const {
    std::vector<std::string> results;

    boost::optional<std::string> materialStandardSource = this->materialStandardSource();

    return results;
  }

  boost::optional<std::string> StandardsInformationMaterial_Impl::standardsCategory() const {
    return getString(OS_StandardsInformation_MaterialFields::StandardsCategory,true);
  }

  std::vector<std::string> StandardsInformationMaterial_Impl::suggestedStandardsCategory() const {
    std::vector<std::string> results;

    boost::optional<std::string> standardsCategory = this->standardsCategory();

    return results;
  }

  boost::optional<std::string> StandardsInformationMaterial_Impl::standardsIdentifier() const {
    return getString(OS_StandardsInformation_MaterialFields::StandardsIdentifier,true);
  }

  std::vector<std::string> StandardsInformationMaterial_Impl::suggestedStandardsIdentifier() const {
    std::vector<std::string> results;

    boost::optional<std::string> standardsIdentifier = this->standardsIdentifier();

    return results;
  }

  bool StandardsInformationMaterial_Impl::setMaterial(const Material& material) {
    bool result = setPointer(OS_StandardsInformation_MaterialFields::MaterialName, material.handle());
    return result;
  }

  void StandardsInformationMaterial_Impl::setMaterialStandard(boost::optional<std::string> materialStandard) {
    bool result(false);
    if (materialStandard) {
      result = setString(OS_StandardsInformation_MaterialFields::MaterialStandard, materialStandard.get());
    }
    else {
      resetMaterialStandard();
      result = true;
    }
    OS_ASSERT(result);
  }

  void StandardsInformationMaterial_Impl::resetMaterialStandard() {
    bool result = setString(OS_StandardsInformation_MaterialFields::MaterialStandard, "");
    OS_ASSERT(result);
  }

  void StandardsInformationMaterial_Impl::setMaterialStandardSource(boost::optional<std::string> materialStandardSource) {
    bool result(false);
    if (materialStandardSource) {
      result = setString(OS_StandardsInformation_MaterialFields::MaterialStandardSource, materialStandardSource.get());
    }
    else {
      resetMaterialStandardSource();
      result = true;
    }
    OS_ASSERT(result);
  }

  void StandardsInformationMaterial_Impl::resetMaterialStandardSource() {
    bool result = setString(OS_StandardsInformation_MaterialFields::MaterialStandardSource, "");
    OS_ASSERT(result);
  }

  void StandardsInformationMaterial_Impl::setStandardsCategory(boost::optional<std::string> standardsCategory) {
    bool result(false);
    if (standardsCategory) {
      result = setString(OS_StandardsInformation_MaterialFields::StandardsCategory, standardsCategory.get());
    }
    else {
      resetStandardsCategory();
      result = true;
    }
    OS_ASSERT(result);
  }

  void StandardsInformationMaterial_Impl::resetStandardsCategory() {
    bool result = setString(OS_StandardsInformation_MaterialFields::StandardsCategory, "");
    OS_ASSERT(result);
  }

  void StandardsInformationMaterial_Impl::setStandardsIdentifier(boost::optional<std::string> standardsIdentifier) {
    bool result(false);
    if (standardsIdentifier) {
      result = setString(OS_StandardsInformation_MaterialFields::StandardsIdentifier, standardsIdentifier.get());
    }
    else {
      resetStandardsIdentifier();
      result = true;
    }
    OS_ASSERT(result);
  }

  void StandardsInformationMaterial_Impl::resetStandardsIdentifier() {
    bool result = setString(OS_StandardsInformation_MaterialFields::StandardsIdentifier, "");
    OS_ASSERT(result);
  }

  boost::optional<Material> StandardsInformationMaterial_Impl::optionalMaterial() const {
    return getObject<ModelObject>().getModelObjectTarget<Material>(OS_StandardsInformation_MaterialFields::MaterialName);
  }

} // detail

StandardsInformationMaterial::StandardsInformationMaterial(const Model& model)
  : ModelObject(StandardsInformationMaterial::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::StandardsInformationMaterial_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_StandardsInformation_MaterialFields::MaterialName
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setMaterial();
  OS_ASSERT(ok);
}

IddObjectType StandardsInformationMaterial::iddObjectType() {
  return IddObjectType(0l); // TODO
  //return IddObjectType(IddObjectType::OS_StandardsInformation_Material);
}

Material StandardsInformationMaterial::material() const {
  return getImpl<detail::StandardsInformationMaterial_Impl>()->material();
}

boost::optional<std::string> StandardsInformationMaterial::materialStandard() const {
  return getImpl<detail::StandardsInformationMaterial_Impl>()->materialStandard();
}

std::vector<std::string> StandardsInformationMaterial::suggestedMaterialStandard() const {
  return getImpl<detail::StandardsInformationMaterial_Impl>()->suggestedMaterialStandard();
}

boost::optional<std::string> StandardsInformationMaterial::materialStandardSource() const {
  return getImpl<detail::StandardsInformationMaterial_Impl>()->materialStandardSource();
}

std::vector<std::string> StandardsInformationMaterial::suggestedMaterialStandardSource() const {
  return getImpl<detail::StandardsInformationMaterial_Impl>()->suggestedMaterialStandardSource();
}

boost::optional<std::string> StandardsInformationMaterial::standardsCategory() const {
  return getImpl<detail::StandardsInformationMaterial_Impl>()->standardsCategory();
}

std::vector<std::string> StandardsInformationMaterial::suggestedStandardsCategory() const {
  return getImpl<detail::StandardsInformationMaterial_Impl>()->suggestedStandardsCategory();
}

boost::optional<std::string> StandardsInformationMaterial::standardsIdentifier() const {
  return getImpl<detail::StandardsInformationMaterial_Impl>()->standardsIdentifier();
}

std::vector<std::string> StandardsInformationMaterial::suggestedStandardsIdentifier() const {
  return getImpl<detail::StandardsInformationMaterial_Impl>()->suggestedStandardsIdentifier();
}

bool StandardsInformationMaterial::setMaterial(const Material& material) {
  return getImpl<detail::StandardsInformationMaterial_Impl>()->setMaterial(material);
}

void StandardsInformationMaterial::setMaterialStandard(std::string materialStandard) {
  getImpl<detail::StandardsInformationMaterial_Impl>()->setMaterialStandard(materialStandard);
}

void StandardsInformationMaterial::resetMaterialStandard() {
  getImpl<detail::StandardsInformationMaterial_Impl>()->resetMaterialStandard();
}

void StandardsInformationMaterial::setMaterialStandardSource(std::string materialStandardSource) {
  getImpl<detail::StandardsInformationMaterial_Impl>()->setMaterialStandardSource(materialStandardSource);
}

void StandardsInformationMaterial::resetMaterialStandardSource() {
  getImpl<detail::StandardsInformationMaterial_Impl>()->resetMaterialStandardSource();
}

void StandardsInformationMaterial::setStandardsCategory(std::string standardsCategory) {
  getImpl<detail::StandardsInformationMaterial_Impl>()->setStandardsCategory(standardsCategory);
}

void StandardsInformationMaterial::resetStandardsCategory() {
  getImpl<detail::StandardsInformationMaterial_Impl>()->resetStandardsCategory();
}

void StandardsInformationMaterial::setStandardsIdentifier(std::string standardsIdentifier) {
  getImpl<detail::StandardsInformationMaterial_Impl>()->setStandardsIdentifier(standardsIdentifier);
}

void StandardsInformationMaterial::resetStandardsIdentifier() {
  getImpl<detail::StandardsInformationMaterial_Impl>()->resetStandardsIdentifier();
}

/// @cond
StandardsInformationMaterial::StandardsInformationMaterial(std::shared_ptr<detail::StandardsInformationMaterial_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

