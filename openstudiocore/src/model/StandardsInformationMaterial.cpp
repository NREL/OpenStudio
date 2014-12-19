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

#include "StandardsInformationMaterial.hpp"
#include "StandardsInformationMaterial_Impl.hpp"

#include "Model.hpp"
#include "Material.hpp"
#include "Material_Impl.hpp"

#include <utilities/idd/OS_StandardsInformation_Material_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

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
    boost::optional<Material> result = getObject<StandardsInformationMaterial>().getModelObjectTarget<Material>(
      OS_StandardsInformation_MaterialFields::MaterialName);
    OS_ASSERT(result);
    return result.get();
  }

  boost::optional<std::string> StandardsInformationMaterial_Impl::materialStandard() const {
    return getString(OS_StandardsInformation_MaterialFields::MaterialStandard, true);
  }

  std::vector<std::string> StandardsInformationMaterial_Impl::suggestedMaterialStandards() const {
    std::vector<std::string> result;

    boost::optional<std::string> materialStandard = this->materialStandard();

    // include values from json
    parseStandardsMap();

    QMap<QString, QVariant> materials = m_standardsMap["materials"].toMap();
    for (QString material_name : materials.uniqueKeys()) {
      QMap<QString, QVariant> material = materials[material_name].toMap();
      QString tmp = material["material_standard"].toString();
      if (!tmp.isEmpty()){
        result.push_back(toString(tmp));
      }
    }

    // include values from model
    for (const StandardsInformationMaterial& other : this->model().getConcreteModelObjects<StandardsInformationMaterial>()){
      if (other.handle() == this->handle()){
        continue;
      }

      boost::optional<std::string> otherMaterialStandard = other.materialStandard();
      if (otherMaterialStandard){
        result.push_back(*otherMaterialStandard);
      }
    }

    // remove current
    IstringFind finder;
    if (materialStandard){
      finder.addTarget(*materialStandard);
    }
    auto it = std::remove_if(result.begin(), result.end(), finder);
    result.resize(std::distance(result.begin(), it));

    // sort
    std::sort(result.begin(), result.end(), IstringCompare());

    // make unique
    // DLM: have to sort before calling unique, unique only works on consecutive elements
    it = std::unique(result.begin(), result.end(), IstringEqual());
    result.resize(std::distance(result.begin(), it));

    // add current to front
    if (materialStandard){
      result.insert(result.begin(), *materialStandard);
    }

    return result;
  }

  boost::optional<std::string> StandardsInformationMaterial_Impl::materialStandardSource() const {
    return getString(OS_StandardsInformation_MaterialFields::MaterialStandardSource,true);
  }

  std::vector<std::string> StandardsInformationMaterial_Impl::suggestedMaterialStandardSources() const {
    std::vector<std::string> result;

    boost::optional<std::string> materialStandard = this->materialStandard();
    if (!materialStandard){
      return result;
    }

    boost::optional<std::string> materialStandardSource = this->materialStandardSource();

    // include values from json
    parseStandardsMap();

    QMap<QString, QVariant> materials = m_standardsMap["materials"].toMap();
    for (QString material_name : materials.uniqueKeys()) {
      QMap<QString, QVariant> material = materials[material_name].toMap();

      QString tmp = material["material_standard"].toString();
      if (toString(tmp) != *materialStandard){
        continue;
      }

      tmp = material["material_standard_source"].toString();
      if (!tmp.isEmpty()){
        result.push_back(toString(tmp));
      }
    }

    // include values from model
    for (const StandardsInformationMaterial& other : this->model().getConcreteModelObjects<StandardsInformationMaterial>()){
      if (other.handle() == this->handle()){
        continue;
      }

      boost::optional<std::string> otherMaterialStandard = other.materialStandard();

      if (!otherMaterialStandard){
        continue;
      } else if (*materialStandard != *otherMaterialStandard){
        continue;
      }

      boost::optional<std::string> otherMaterialStandardSource = other.materialStandardSource();
      if (otherMaterialStandardSource){
        result.push_back(*otherMaterialStandardSource);
      }
    }

    // remove current
    IstringFind finder;
    if (materialStandardSource){
      finder.addTarget(*materialStandardSource);
    }
    auto it = std::remove_if(result.begin(), result.end(), finder);
    result.resize(std::distance(result.begin(), it));

    // sort
    std::sort(result.begin(), result.end(), IstringCompare());

    // make unique
    // DLM: have to sort before calling unique, unique only works on consecutive elements
    it = std::unique(result.begin(), result.end(), IstringEqual());
    result.resize(std::distance(result.begin(), it));

    // add current to front
    if (materialStandardSource){
      result.insert(result.begin(), *materialStandardSource);
    }

    return result;
  }

  boost::optional<std::string> StandardsInformationMaterial_Impl::standardsCategory() const {
    return getString(OS_StandardsInformation_MaterialFields::StandardsCategory,true);
  }

  std::vector<std::string> StandardsInformationMaterial_Impl::suggestedStandardsCategories() const {
    std::vector<std::string> result;

    boost::optional<std::string> materialStandard = this->materialStandard();

    boost::optional<std::string> standardsCategory = this->standardsCategory();

    // include values from json
    parseStandardsMap();

    QMap<QString, QVariant> materials = m_standardsMap["materials"].toMap();
    for (QString material_name : materials.uniqueKeys()) {
      QMap<QString, QVariant> material = materials[material_name].toMap();

      if (materialStandard){
        QString tmp = material["material_standard"].toString();
        if (toString(tmp) != *materialStandard){
          continue;
        }
      }

      QString tmp = material["code_category"].toString();
      if (!tmp.isEmpty()){
        result.push_back(toString(tmp));
      }
    }

    // include values from model
    for (const StandardsInformationMaterial& other : this->model().getConcreteModelObjects<StandardsInformationMaterial>()){
      if (other.handle() == this->handle()){
        continue;
      }

      boost::optional<std::string> otherMaterialStandard = other.materialStandard();
      if (materialStandard && otherMaterialStandard){
        if (*materialStandard != otherMaterialStandard){
          continue;
        }
      } else if (materialStandard || otherMaterialStandard){
        continue;
      }

      boost::optional<std::string> otherStandardsCategory = other.standardsCategory();
      if (otherStandardsCategory){
        result.push_back(*otherStandardsCategory);
      }
    }

    // remove current 
    IstringFind finder;
    if (standardsCategory){
      finder.addTarget(*standardsCategory);
    }
    auto it = std::remove_if(result.begin(), result.end(), finder);
    result.resize(std::distance(result.begin(), it));

    // sort
    std::sort(result.begin(), result.end(), IstringCompare());

    // make unique
    // DLM: have to sort before calling unique, unique only works on consecutive elements
    it = std::unique(result.begin(), result.end(), IstringEqual());
    result.resize(std::distance(result.begin(), it));

    // add current to front
    if (standardsCategory){
      result.insert(result.begin(), *standardsCategory);
    }

    return result;
  }

  boost::optional<std::string> StandardsInformationMaterial_Impl::standardsIdentifier() const {
    return getString(OS_StandardsInformation_MaterialFields::StandardsIdentifier,true);
  }

  std::vector<std::string> StandardsInformationMaterial_Impl::suggestedStandardsIdentifiers() const {
    std::vector<std::string> result;

    boost::optional<std::string> materialStandard = this->materialStandard();
    boost::optional<std::string> standardsCategory = this->standardsCategory();
    boost::optional<std::string> standardsIdentifier = this->standardsIdentifier();

    if (!standardsCategory){
      return result;
    }

    // include values from json
    parseStandardsMap();

    QMap<QString, QVariant> materials = m_standardsMap["materials"].toMap();
    for (QString material_name : materials.uniqueKeys()) {
      QMap<QString, QVariant> material = materials[material_name].toMap();

      if (materialStandard){
        QString tmp = material["material_standard"].toString();
        if (toString(tmp) != *materialStandard){
          continue;
        }
      }

      QString tmp = material["code_category"].toString();
      if (toString(tmp) != *standardsCategory){
        continue;
      }

      tmp = material["code_identifier"].toString();
      if (!tmp.isEmpty()){
        result.push_back(toString(tmp));
      }
    }

    // include values from model
    for (const StandardsInformationMaterial& other : this->model().getConcreteModelObjects<StandardsInformationMaterial>()){
      if (other.handle() == this->handle()){
        continue;
      }

      boost::optional<std::string> otherMaterialStandard = other.materialStandard();
      if (materialStandard && otherMaterialStandard){
        if (*materialStandard != otherMaterialStandard){
          continue;
        }
      } else if (materialStandard || otherMaterialStandard){
        continue;
      }

      boost::optional<std::string> otherStandardsCategory = other.standardsCategory();
      if (!otherStandardsCategory){
        continue;
      } else if (*standardsCategory != *otherStandardsCategory){
        continue;
      }

      boost::optional<std::string> otherStandardsIdentifier = other.standardsIdentifier();
      if (otherStandardsIdentifier){
        result.push_back(*otherStandardsIdentifier);
      }
    }

    // remove current
    IstringFind finder;
    if (standardsIdentifier){
      finder.addTarget(*standardsIdentifier);
    }
    auto it = std::remove_if(result.begin(), result.end(), finder);
    result.resize(std::distance(result.begin(), it));

    // sort
    std::sort(result.begin(), result.end(), IstringCompare());

    // make unique
    // DLM: have to sort before calling unique, unique only works on consecutive elements
    it = std::unique(result.begin(), result.end(), IstringEqual());
    result.resize(std::distance(result.begin(), it));

    // add current to front
    if (standardsIdentifier){
      result.insert(result.begin(), *standardsIdentifier);
    }

    return result;
  }

  void StandardsInformationMaterial_Impl::setMaterialStandard(const std::string& materialStandard) {
    bool result = setString(OS_StandardsInformation_MaterialFields::MaterialStandard, materialStandard);
    OS_ASSERT(result);
  }

  void StandardsInformationMaterial_Impl::resetMaterialStandard() {
    bool result = setString(OS_StandardsInformation_MaterialFields::MaterialStandard, "");
    OS_ASSERT(result);
  }

  void StandardsInformationMaterial_Impl::setMaterialStandardSource(const std::string& materialStandardSource) {
    bool result = setString(OS_StandardsInformation_MaterialFields::MaterialStandardSource, materialStandardSource);
    OS_ASSERT(result);
  }

  void StandardsInformationMaterial_Impl::resetMaterialStandardSource() {
    bool result = setString(OS_StandardsInformation_MaterialFields::MaterialStandardSource, "");
    OS_ASSERT(result);
  }

  void StandardsInformationMaterial_Impl::setStandardsCategory(const std::string& standardsCategory) {
    bool result = setString(OS_StandardsInformation_MaterialFields::StandardsCategory, standardsCategory);
    OS_ASSERT(result);
  }

  void StandardsInformationMaterial_Impl::resetStandardsCategory() {
    bool result = setString(OS_StandardsInformation_MaterialFields::StandardsCategory, "");
    OS_ASSERT(result);
  }

  void StandardsInformationMaterial_Impl::setStandardsIdentifier(const std::string& standardsIdentifier) {
    bool result = setString(OS_StandardsInformation_MaterialFields::StandardsIdentifier, standardsIdentifier);
    OS_ASSERT(result);
  }

  void StandardsInformationMaterial_Impl::resetStandardsIdentifier() {
    bool result = setString(OS_StandardsInformation_MaterialFields::StandardsIdentifier, "");
    OS_ASSERT(result);
  }


} // detail

StandardsInformationMaterial::StandardsInformationMaterial(const Material& material)
  : ModelObject(StandardsInformationMaterial::iddObjectType(), material.model())
{
  OS_ASSERT(getImpl<detail::StandardsInformationMaterial_Impl>());
  bool ok = setPointer(OS_StandardsInformation_MaterialFields::MaterialName, material.handle());
  OS_ASSERT(ok);
}

IddObjectType StandardsInformationMaterial::iddObjectType() {
  return IddObjectType(IddObjectType::OS_StandardsInformation_Material);
}

Material StandardsInformationMaterial::material() const {
  return getImpl<detail::StandardsInformationMaterial_Impl>()->material();
}

boost::optional<std::string> StandardsInformationMaterial::materialStandard() const {
  return getImpl<detail::StandardsInformationMaterial_Impl>()->materialStandard();
}

std::vector<std::string> StandardsInformationMaterial::suggestedMaterialStandards() const {
  return getImpl<detail::StandardsInformationMaterial_Impl>()->suggestedMaterialStandards();
}

boost::optional<std::string> StandardsInformationMaterial::materialStandardSource() const {
  return getImpl<detail::StandardsInformationMaterial_Impl>()->materialStandardSource();
}

std::vector<std::string> StandardsInformationMaterial::suggestedMaterialStandardSources() const {
  return getImpl<detail::StandardsInformationMaterial_Impl>()->suggestedMaterialStandardSources();
}

boost::optional<std::string> StandardsInformationMaterial::standardsCategory() const {
  return getImpl<detail::StandardsInformationMaterial_Impl>()->standardsCategory();
}

std::vector<std::string> StandardsInformationMaterial::suggestedStandardsCategories() const {
  return getImpl<detail::StandardsInformationMaterial_Impl>()->suggestedStandardsCategories();
}

boost::optional<std::string> StandardsInformationMaterial::standardsIdentifier() const {
  return getImpl<detail::StandardsInformationMaterial_Impl>()->standardsIdentifier();
}

std::vector<std::string> StandardsInformationMaterial::suggestedStandardsIdentifiers() const {
  return getImpl<detail::StandardsInformationMaterial_Impl>()->suggestedStandardsIdentifiers();
}

void StandardsInformationMaterial::setMaterialStandard(const std::string& materialStandard) {
  getImpl<detail::StandardsInformationMaterial_Impl>()->setMaterialStandard(materialStandard);
}

void StandardsInformationMaterial::resetMaterialStandard() {
  getImpl<detail::StandardsInformationMaterial_Impl>()->resetMaterialStandard();
}

void StandardsInformationMaterial::setMaterialStandardSource(const std::string& materialStandardSource) {
  getImpl<detail::StandardsInformationMaterial_Impl>()->setMaterialStandardSource(materialStandardSource);
}

void StandardsInformationMaterial::resetMaterialStandardSource() {
  getImpl<detail::StandardsInformationMaterial_Impl>()->resetMaterialStandardSource();
}

void StandardsInformationMaterial::setStandardsCategory(const std::string& standardsCategory) {
  getImpl<detail::StandardsInformationMaterial_Impl>()->setStandardsCategory(standardsCategory);
}

void StandardsInformationMaterial::resetStandardsCategory() {
  getImpl<detail::StandardsInformationMaterial_Impl>()->resetStandardsCategory();
}

void StandardsInformationMaterial::setStandardsIdentifier(const std::string& standardsIdentifier) {
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

