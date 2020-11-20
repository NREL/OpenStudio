/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "StandardsInformationMaterial.hpp"
#include "StandardsInformationMaterial_Impl.hpp"

#include "Model.hpp"
#include "Material.hpp"
#include "Material_Impl.hpp"

#include <utilities/idd/OS_StandardsInformation_Material_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <model/embedded_files.hxx>
#include "../utilities/filetypes/StandardsJSON.hpp"
#include "../utilities/core/Json.hpp"
#include <json/json.h>

namespace openstudio {
namespace model {

  namespace detail {

    StandardsInformationMaterial_Impl::StandardsInformationMaterial_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == StandardsInformationMaterial::iddObjectType());
    }

    StandardsInformationMaterial_Impl::StandardsInformationMaterial_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                         bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == StandardsInformationMaterial::iddObjectType());
    }

    StandardsInformationMaterial_Impl::StandardsInformationMaterial_Impl(const StandardsInformationMaterial_Impl& other, Model_Impl* model,
                                                                         bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    boost::optional<ParentObject> StandardsInformationMaterial_Impl::parent() const {
      return material();
    }

    bool StandardsInformationMaterial_Impl::setParent(ParentObject& newParent) {
      return setPointer(OS_StandardsInformation_MaterialFields::MaterialName, newParent.handle());
    }

    const std::vector<std::string>& StandardsInformationMaterial_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType StandardsInformationMaterial_Impl::iddObjectType() const {
      return StandardsInformationMaterial::iddObjectType();
    }

    Material StandardsInformationMaterial_Impl::material() const {
      boost::optional<Material> result =
        getObject<StandardsInformationMaterial>().getModelObjectTarget<Material>(OS_StandardsInformation_MaterialFields::MaterialName);
      OS_ASSERT(result);
      return result.get();
    }

    boost::optional<std::string> StandardsInformationMaterial_Impl::materialStandard() const {
      return getString(OS_StandardsInformation_MaterialFields::MaterialStandard, true, true);
    }

    const Json::Value& StandardsInformationMaterial_Impl::getStandardsJSON() {
      const static Json::Value standards_json = parseStandardsJSON();
      return standards_json;
    }

    Json::Value StandardsInformationMaterial_Impl::parseStandardsJSON() {
      // Embedded file path
      std::string embedded_path = ":/Resources/standards/OpenStudio_Standards_materials_merged.json";
      std::string fileContent = ::openstudiomodel::embedded_files::getFileAsString(embedded_path);

      // Create a StandardsJSON
      StandardsJSON standard(fileContent);

      // Now try to get the primaryKey
      std::string primaryKey = "materials";

      if (boost::optional<Json::Value> _standardsArr = standard.getPrimaryKey(primaryKey)) {
        return _standardsArr.get();
      } else {
        // This should never happen really, until we implement the ability to supply a custom StandardsJSON
        LOG(Error, "Cannot find the primaryKey '" << primaryKey << "' in the StandardsJSON");
        return {};
      }
    }

    std::vector<std::string> StandardsInformationMaterial_Impl::suggestedMaterialStandards() const {
      std::vector<std::string> result;

      boost::optional<std::string> materialStandard = this->materialStandard();

      // an array of hashes (no nested levels)
      for (const auto& v : getStandardsJSON()) {
        const Json::Value _material = v["material_standard"];
        if (_material.isString()) {
          result.push_back(_material.asString());
        }
      }

      // include values from model
      for (const StandardsInformationMaterial& other : this->model().getConcreteModelObjects<StandardsInformationMaterial>()) {
        if (other.handle() == this->handle()) {
          continue;
        }

        boost::optional<std::string> otherMaterialStandard = other.materialStandard();
        if (otherMaterialStandard) {
          result.push_back(*otherMaterialStandard);
        }
      }

      // remove current
      IstringFind finder;
      if (materialStandard) {
        finder.addTarget(*materialStandard);

        auto it = std::remove_if(result.begin(), result.end(), finder);
        result.resize(std::distance(result.begin(), it));
      }

      // sort
      std::sort(result.begin(), result.end(), IstringCompare());

      // make unique
      // DLM: have to sort before calling unique, unique only works on consecutive elements
      auto it = std::unique(result.begin(), result.end(), IstringEqual());
      result.resize(std::distance(result.begin(), it));

      // add current to front
      if (materialStandard) {
        result.insert(result.begin(), *materialStandard);
      }

      return result;
    }

    boost::optional<std::string> StandardsInformationMaterial_Impl::materialStandardSource() const {
      return getString(OS_StandardsInformation_MaterialFields::MaterialStandardSource, true, true);
    }

    std::vector<std::string> StandardsInformationMaterial_Impl::suggestedMaterialStandardSources() const {
      std::vector<std::string> result;

      boost::optional<std::string> materialStandard = this->materialStandard();
      boost::optional<std::string> materialStandardSource = this->materialStandardSource();

      if (!materialStandard) {
        if (materialStandardSource) {
          result.push_back(*materialStandardSource);
        }
        return result;
      }

      std::string thisMaterialStandard;

      for (const auto& v : getStandardsJSON()) {
        // If materialStandard is initialized, then we have to match against it
        if (materialStandard.is_initialized()) {
          // It's fine to call asString directly here because it'll return an empty string
          // if it's null, and therefore won't match against our material standard anyways
          thisMaterialStandard = v["material_standard"].asString();
          if (thisMaterialStandard != materialStandard.get()) {
            continue;
          }
        }

        const Json::Value _tmp = v["material_standard_source"];
        if (_tmp.isString()) {
          result.push_back(_tmp.asString());
        }
      }

      // include values from model
      for (const StandardsInformationMaterial& other : this->model().getConcreteModelObjects<StandardsInformationMaterial>()) {
        if (other.handle() == this->handle()) {
          continue;
        }

        boost::optional<std::string> otherMaterialStandard = other.materialStandard();

        if (!otherMaterialStandard) {
          continue;
        } else if (*materialStandard != *otherMaterialStandard) {
          continue;
        }

        boost::optional<std::string> otherMaterialStandardSource = other.materialStandardSource();
        if (otherMaterialStandardSource) {
          result.push_back(*otherMaterialStandardSource);
        }
      }

      // remove current
      IstringFind finder;
      if (materialStandardSource) {
        finder.addTarget(*materialStandardSource);

        auto it = std::remove_if(result.begin(), result.end(), finder);
        result.resize(std::distance(result.begin(), it));
      }

      // sort
      std::sort(result.begin(), result.end(), IstringCompare());

      // make unique
      // DLM: have to sort before calling unique, unique only works on consecutive elements
      auto it = std::unique(result.begin(), result.end(), IstringEqual());
      result.resize(std::distance(result.begin(), it));

      // add current to front
      if (materialStandardSource) {
        result.insert(result.begin(), *materialStandardSource);
      }

      return result;
    }

    boost::optional<std::string> StandardsInformationMaterial_Impl::standardsCategory() const {
      return getString(OS_StandardsInformation_MaterialFields::StandardsCategory, true, true);
    }

    std::vector<std::string> StandardsInformationMaterial_Impl::suggestedStandardsCategories() const {
      std::vector<std::string> result;

      boost::optional<std::string> materialStandard = this->materialStandard();

      boost::optional<std::string> standardsCategory = this->standardsCategory();

      std::string thisMaterialStandard;

      for (const auto& v : getStandardsJSON()) {
        // If materialStandard is initialized, then we have to match against it
        if (materialStandard.is_initialized()) {
          // It's fine to call asString directly here because it'll return an empty string
          // if it's null, and therefore won't match against our material standard anyways
          thisMaterialStandard = v["material_standard"].asString();
          if (thisMaterialStandard != materialStandard.get()) {
            continue;
          }
        }

        const Json::Value _tmp = v["code_category"];
        if (_tmp.isString()) {
          result.push_back(_tmp.asString());
        }
      }

      // include values from model
      for (const StandardsInformationMaterial& other : this->model().getConcreteModelObjects<StandardsInformationMaterial>()) {
        if (other.handle() == this->handle()) {
          continue;
        }

        boost::optional<std::string> otherMaterialStandard = other.materialStandard();
        if (materialStandard && otherMaterialStandard) {
          if (*materialStandard != otherMaterialStandard) {
            continue;
          }
        } else if (materialStandard || otherMaterialStandard) {
          continue;
        }

        boost::optional<std::string> otherStandardsCategory = other.standardsCategory();
        if (otherStandardsCategory) {
          result.push_back(*otherStandardsCategory);
        }
      }

      // remove current
      IstringFind finder;
      if (standardsCategory) {
        finder.addTarget(*standardsCategory);

        auto it = std::remove_if(result.begin(), result.end(), finder);
        result.resize(std::distance(result.begin(), it));
      }

      // sort
      std::sort(result.begin(), result.end(), IstringCompare());

      // make unique
      // DLM: have to sort before calling unique, unique only works on consecutive elements
      auto it = std::unique(result.begin(), result.end(), IstringEqual());
      result.resize(std::distance(result.begin(), it));

      // add current to front
      if (standardsCategory) {
        result.insert(result.begin(), *standardsCategory);
      }

      return result;
    }

    bool StandardsInformationMaterial_Impl::isCompositeMaterial() const {
      boost::optional<std::string> standardsCategory = this->standardsCategory();
      if (standardsCategory) {
        if (istringEqual("Composite", *standardsCategory) || istringEqual("Metal Framed Wall", *standardsCategory)
            || istringEqual("Metal Framed Floor", *standardsCategory) || istringEqual("Metal Building Roof", *standardsCategory)
            || istringEqual("Wood Framed Wall", *standardsCategory) || istringEqual("Wood Framed Floor", *standardsCategory)
            || istringEqual("Wood Framed Attic Floor", *standardsCategory) || istringEqual("Wood Framed Rafter Roof", *standardsCategory)) {
          return true;
        }
      }

      return false;
    }

    boost::optional<std::string> StandardsInformationMaterial_Impl::standardsIdentifier() const {
      return getString(OS_StandardsInformation_MaterialFields::StandardsIdentifier, true, true);
    }

    std::vector<std::string> StandardsInformationMaterial_Impl::suggestedStandardsIdentifiers() const {
      std::vector<std::string> result;

      boost::optional<std::string> materialStandard = this->materialStandard();
      boost::optional<std::string> standardsCategory = this->standardsCategory();
      boost::optional<std::string> standardsIdentifier = this->standardsIdentifier();

      if (!standardsCategory) {
        if (standardsIdentifier) {
          result.push_back(*standardsIdentifier);
        }
        return result;
      }

      std::string thisMaterialStandard;
      std::string thisStandardsCategory;

      for (const auto& v : getStandardsJSON()) {
        // If materialStandard is initialized, then we have to match against it
        if (materialStandard.is_initialized()) {
          thisMaterialStandard = v["material_standard"].asString();
          if (thisMaterialStandard != materialStandard.get()) {
            continue;
          }
        }

        // If standardsCategory is initialized, then we have to match against it
        if (standardsCategory.is_initialized()) {
          thisStandardsCategory = v["code_category"].asString();
          if (thisStandardsCategory != standardsCategory.get()) {
            continue;
          }
        }

        const Json::Value _tmp = v["code_identifier"];
        if (_tmp.isString()) {
          result.push_back(_tmp.asString());
        }
      }

      // include values from model
      for (const StandardsInformationMaterial& other : this->model().getConcreteModelObjects<StandardsInformationMaterial>()) {
        if (other.handle() == this->handle()) {
          continue;
        }

        boost::optional<std::string> otherMaterialStandard = other.materialStandard();
        if (materialStandard && otherMaterialStandard) {
          if (*materialStandard != otherMaterialStandard) {
            continue;
          }
        } else if (materialStandard || otherMaterialStandard) {
          continue;
        }

        boost::optional<std::string> otherStandardsCategory = other.standardsCategory();
        if (!otherStandardsCategory) {
          continue;
        } else if (*standardsCategory != *otherStandardsCategory) {
          continue;
        }

        boost::optional<std::string> otherStandardsIdentifier = other.standardsIdentifier();
        if (otherStandardsIdentifier) {
          result.push_back(*otherStandardsIdentifier);
        }
      }

      // remove current
      IstringFind finder;
      if (standardsIdentifier) {
        finder.addTarget(*standardsIdentifier);

        auto it = std::remove_if(result.begin(), result.end(), finder);
        result.resize(std::distance(result.begin(), it));
      }

      // sort
      std::sort(result.begin(), result.end(), IstringCompare());

      // make unique
      // DLM: have to sort before calling unique, unique only works on consecutive elements
      auto it = std::unique(result.begin(), result.end(), IstringEqual());
      result.resize(std::distance(result.begin(), it));

      // add current to front
      if (standardsIdentifier) {
        result.insert(result.begin(), *standardsIdentifier);
      }

      return result;
    }

    boost::optional<std::string> StandardsInformationMaterial_Impl::compositeFramingMaterial() const {
      return getString(OS_StandardsInformation_MaterialFields::CompositeFramingMaterial, true, true);
    }

    std::vector<std::string> StandardsInformationMaterial_Impl::suggestedCompositeFramingMaterials() const {
      std::vector<std::string> result;

      boost::optional<std::string> materialStandard = this->materialStandard();

      boost::optional<std::string> standardsCategory = this->standardsCategory();

      boost::optional<std::string> compositeFramingMaterial = this->compositeFramingMaterial();

      std::string thisMaterialStandard;
      std::string thisStandardsCategory;

      for (const auto& v : getStandardsJSON()) {
        // If materialStandard is initialized, then we have to match against it
        if (materialStandard.is_initialized()) {
          thisMaterialStandard = v["material_standard"].asString();
          if (thisMaterialStandard != materialStandard.get()) {
            continue;
          }
        }

        // If standardsCategory is initialized, then we have to match against it
        if (standardsCategory.is_initialized()) {
          thisStandardsCategory = v["code_category"].asString();
          if (thisStandardsCategory != standardsCategory.get()) {
            continue;
          }
        }

        const Json::Value _tmp = v["framing_material"];
        if (_tmp.isString()) {
          result.push_back(_tmp.asString());
        }
      }

      // include values from model
      for (const StandardsInformationMaterial& other : this->model().getConcreteModelObjects<StandardsInformationMaterial>()) {
        if (other.handle() == this->handle()) {
          continue;
        }

        boost::optional<std::string> otherMaterialStandard = other.materialStandard();
        if (materialStandard && otherMaterialStandard) {
          if (*materialStandard != otherMaterialStandard) {
            continue;
          }
        } else if (materialStandard || otherMaterialStandard) {
          continue;
        }

        boost::optional<std::string> otherStandardsCategory = other.standardsCategory();
        if (standardsCategory && otherStandardsCategory) {
          if (*standardsCategory != otherStandardsCategory) {
            continue;
          }
        } else if (standardsCategory || otherStandardsCategory) {
          continue;
        }

        boost::optional<std::string> otherCompositeFramingMaterial = other.compositeFramingMaterial();
        if (otherCompositeFramingMaterial) {
          result.push_back(*otherCompositeFramingMaterial);
        }
      }

      // remove current
      IstringFind finder;
      if (compositeFramingMaterial) {
        finder.addTarget(*compositeFramingMaterial);

        auto it = std::remove_if(result.begin(), result.end(), finder);
        result.resize(std::distance(result.begin(), it));
      }

      // sort
      std::sort(result.begin(), result.end(), IstringCompare());

      // make unique
      // DLM: have to sort before calling unique, unique only works on consecutive elements
      auto it = std::unique(result.begin(), result.end(), IstringEqual());
      result.resize(std::distance(result.begin(), it));

      // add current to front
      if (compositeFramingMaterial) {
        result.insert(result.begin(), *compositeFramingMaterial);
      }

      return result;
    }

    boost::optional<std::string> StandardsInformationMaterial_Impl::compositeFramingConfiguration() const {
      return getString(OS_StandardsInformation_MaterialFields::CompositeFramingConfiguration, true, true);
    }

    std::vector<std::string> StandardsInformationMaterial_Impl::suggestedCompositeFramingConfigurations() const {
      std::vector<std::string> result;

      boost::optional<std::string> materialStandard = this->materialStandard();

      boost::optional<std::string> standardsCategory = this->standardsCategory();

      boost::optional<std::string> compositeFramingConfiguration = this->compositeFramingConfiguration();

      std::string thisMaterialStandard;
      std::string thisStandardsCategory;

      for (const auto& v : getStandardsJSON()) {
        // If materialStandard is initialized, then we have to match against it
        if (materialStandard.is_initialized()) {
          thisMaterialStandard = v["material_standard"].asString();
          if (thisMaterialStandard != materialStandard.get()) {
            continue;
          }
        }

        // If standardsCategory is initialized, then we have to match against it
        if (standardsCategory.is_initialized()) {
          thisStandardsCategory = v["code_category"].asString();
          if (thisStandardsCategory != standardsCategory.get()) {
            continue;
          }
        }

        const Json::Value _tmp = v["framing_configuration"];
        if (_tmp.isString()) {
          result.push_back(_tmp.asString());
        }
      }

      // include values from model
      for (const StandardsInformationMaterial& other : this->model().getConcreteModelObjects<StandardsInformationMaterial>()) {
        if (other.handle() == this->handle()) {
          continue;
        }

        boost::optional<std::string> otherMaterialStandard = other.materialStandard();
        if (materialStandard && otherMaterialStandard) {
          if (*materialStandard != otherMaterialStandard) {
            continue;
          }
        } else if (materialStandard || otherMaterialStandard) {
          continue;
        }

        boost::optional<std::string> otherStandardsCategory = other.standardsCategory();
        if (standardsCategory && otherStandardsCategory) {
          if (*standardsCategory != otherStandardsCategory) {
            continue;
          }
        } else if (standardsCategory || otherStandardsCategory) {
          continue;
        }

        boost::optional<std::string> otherCompositeFramingConfiguration = other.compositeFramingConfiguration();
        if (otherCompositeFramingConfiguration) {
          result.push_back(*otherCompositeFramingConfiguration);
        }
      }

      // remove current
      IstringFind finder;
      if (compositeFramingConfiguration) {
        finder.addTarget(*compositeFramingConfiguration);

        auto it = std::remove_if(result.begin(), result.end(), finder);
        result.resize(std::distance(result.begin(), it));
      }

      // sort
      std::sort(result.begin(), result.end(), IstringCompare());

      // make unique
      // DLM: have to sort before calling unique, unique only works on consecutive elements
      auto it = std::unique(result.begin(), result.end(), IstringEqual());
      result.resize(std::distance(result.begin(), it));

      // add current to front
      if (compositeFramingConfiguration) {
        result.insert(result.begin(), *compositeFramingConfiguration);
      }

      return result;
    }

    boost::optional<std::string> StandardsInformationMaterial_Impl::compositeFramingDepth() const {
      return getString(OS_StandardsInformation_MaterialFields::CompositeFramingDepth, true, true);
    }

    std::vector<std::string> StandardsInformationMaterial_Impl::suggestedCompositeFramingDepths() const {
      std::vector<std::string> result;

      boost::optional<std::string> materialStandard = this->materialStandard();

      boost::optional<std::string> standardsCategory = this->standardsCategory();

      boost::optional<std::string> compositeFramingDepth = this->compositeFramingDepth();

      std::string thisMaterialStandard;
      std::string thisStandardsCategory;

      for (const auto& v : getStandardsJSON()) {
        // If materialStandard is initialized, then we have to match against it
        if (materialStandard.is_initialized()) {
          thisMaterialStandard = v["material_standard"].asString();
          if (thisMaterialStandard != materialStandard.get()) {
            continue;
          }
        }

        // If standardsCategory is initialized, then we have to match against it
        if (standardsCategory.is_initialized()) {
          thisStandardsCategory = v["code_category"].asString();
          if (thisStandardsCategory != standardsCategory.get()) {
            continue;
          }
        }

        const Json::Value _tmp = v["framing_depth"];
        if (_tmp.isString()) {
          result.push_back(_tmp.asString());
        }
      }

      // include values from model
      for (const StandardsInformationMaterial& other : this->model().getConcreteModelObjects<StandardsInformationMaterial>()) {
        if (other.handle() == this->handle()) {
          continue;
        }

        boost::optional<std::string> otherMaterialStandard = other.materialStandard();
        if (materialStandard && otherMaterialStandard) {
          if (*materialStandard != otherMaterialStandard) {
            continue;
          }
        } else if (materialStandard || otherMaterialStandard) {
          continue;
        }

        boost::optional<std::string> otherStandardsCategory = other.standardsCategory();
        if (standardsCategory && otherStandardsCategory) {
          if (*standardsCategory != otherStandardsCategory) {
            continue;
          }
        } else if (standardsCategory || otherStandardsCategory) {
          continue;
        }

        boost::optional<std::string> otherCompositeFramingDepth = other.compositeFramingDepth();
        if (otherCompositeFramingDepth) {
          result.push_back(*otherCompositeFramingDepth);
        }
      }

      // remove current
      IstringFind finder;
      if (compositeFramingDepth) {
        finder.addTarget(*compositeFramingDepth);

        auto it = std::remove_if(result.begin(), result.end(), finder);
        result.resize(std::distance(result.begin(), it));
      }

      // sort
      std::sort(result.begin(), result.end(), IstringCompare());

      // make unique
      // DLM: have to sort before calling unique, unique only works on consecutive elements
      auto it = std::unique(result.begin(), result.end(), IstringEqual());
      result.resize(std::distance(result.begin(), it));

      // add current to front
      if (compositeFramingDepth) {
        result.insert(result.begin(), *compositeFramingDepth);
      }

      return result;
    }

    boost::optional<std::string> StandardsInformationMaterial_Impl::compositeFramingSize() const {
      return getString(OS_StandardsInformation_MaterialFields::CompositeFramingSize, true, true);
    }

    std::vector<std::string> StandardsInformationMaterial_Impl::suggestedCompositeFramingSizes() const {
      std::vector<std::string> result;

      boost::optional<std::string> materialStandard = this->materialStandard();

      boost::optional<std::string> standardsCategory = this->standardsCategory();

      boost::optional<std::string> compositeFramingSize = this->compositeFramingSize();

      std::string thisMaterialStandard;
      std::string thisStandardsCategory;

      for (const auto& v : getStandardsJSON()) {
        // If materialStandard is initialized, then we have to match against it
        if (materialStandard.is_initialized()) {
          thisMaterialStandard = v["material_standard"].asString();
          if (thisMaterialStandard != materialStandard.get()) {
            continue;
          }
        }

        // If standardsCategory is initialized, then we have to match against it
        if (standardsCategory.is_initialized()) {
          thisStandardsCategory = v["code_category"].asString();
          if (thisStandardsCategory != standardsCategory.get()) {
            continue;
          }
        }

        const Json::Value _tmp = v["framing_size"];
        if (_tmp.isString()) {
          result.push_back(_tmp.asString());
        }
      }

      // include values from model
      for (const StandardsInformationMaterial& other : this->model().getConcreteModelObjects<StandardsInformationMaterial>()) {
        if (other.handle() == this->handle()) {
          continue;
        }

        boost::optional<std::string> otherMaterialStandard = other.materialStandard();
        if (materialStandard && otherMaterialStandard) {
          if (*materialStandard != otherMaterialStandard) {
            continue;
          }
        } else if (materialStandard || otherMaterialStandard) {
          continue;
        }

        boost::optional<std::string> otherStandardsCategory = other.standardsCategory();
        if (standardsCategory && otherStandardsCategory) {
          if (*standardsCategory != otherStandardsCategory) {
            continue;
          }
        } else if (standardsCategory || otherStandardsCategory) {
          continue;
        }

        boost::optional<std::string> otherCompositeFramingSize = other.compositeFramingSize();
        if (otherCompositeFramingSize) {
          result.push_back(*otherCompositeFramingSize);
        }
      }

      // remove current
      IstringFind finder;
      if (compositeFramingSize) {
        finder.addTarget(*compositeFramingSize);

        auto it = std::remove_if(result.begin(), result.end(), finder);
        result.resize(std::distance(result.begin(), it));
      }

      // sort
      std::sort(result.begin(), result.end(), IstringCompare());

      // make unique
      // DLM: have to sort before calling unique, unique only works on consecutive elements
      auto it = std::unique(result.begin(), result.end(), IstringEqual());
      result.resize(std::distance(result.begin(), it));

      // add current to front
      if (compositeFramingSize) {
        result.insert(result.begin(), *compositeFramingSize);
      }

      return result;
    }

    boost::optional<std::string> StandardsInformationMaterial_Impl::compositeCavityInsulation() const {
      return getString(OS_StandardsInformation_MaterialFields::CompositeCavityInsulation, true, true);
    }

    std::vector<std::string> StandardsInformationMaterial_Impl::suggestedCompositeCavityInsulations() const {
      std::vector<std::string> result;

      boost::optional<std::string> materialStandard = this->materialStandard();

      boost::optional<std::string> standardsCategory = this->standardsCategory();

      boost::optional<std::string> compositeCavityInsulation = this->compositeCavityInsulation();

      std::string thisMaterialStandard;
      std::string thisStandardsCategory;

      for (const auto& v : getStandardsJSON()) {
        // If materialStandard is initialized, then we have to match against it
        if (materialStandard.is_initialized()) {
          thisMaterialStandard = v["material_standard"].asString();
          if (thisMaterialStandard != materialStandard.get()) {
            continue;
          }
        }

        // If standardsCategory is initialized, then we have to match against it
        if (standardsCategory.is_initialized()) {
          thisStandardsCategory = v["code_category"].asString();
          if (thisStandardsCategory != standardsCategory.get()) {
            continue;
          }
        }

        const Json::Value _tmp = v["cavity_insulation"];
        // Note JM 2018-11-07: cavity_insulation is stored in the JSON as a number (a double), and not a string
        // Json::Value.asString() will not work, so we get it as Double, then convert to a string
        if (_tmp.isDouble()) {
          result.push_back(std::to_string(_tmp.asDouble()));
        }
      }

      // include values from model
      for (const StandardsInformationMaterial& other : this->model().getConcreteModelObjects<StandardsInformationMaterial>()) {
        if (other.handle() == this->handle()) {
          continue;
        }

        boost::optional<std::string> otherMaterialStandard = other.materialStandard();
        if (materialStandard && otherMaterialStandard) {
          if (*materialStandard != otherMaterialStandard) {
            continue;
          }
        } else if (materialStandard || otherMaterialStandard) {
          continue;
        }

        boost::optional<std::string> otherStandardsCategory = other.standardsCategory();
        if (standardsCategory && otherStandardsCategory) {
          if (*standardsCategory != otherStandardsCategory) {
            continue;
          }
        } else if (standardsCategory || otherStandardsCategory) {
          continue;
        }

        boost::optional<std::string> otherCompositeCavityInsulation = other.compositeCavityInsulation();
        if (otherCompositeCavityInsulation) {
          result.push_back(*otherCompositeCavityInsulation);
        }
      }

      // remove current
      IstringFind finder;
      if (compositeCavityInsulation) {
        finder.addTarget(*compositeCavityInsulation);

        auto it = std::remove_if(result.begin(), result.end(), finder);
        result.resize(std::distance(result.begin(), it));
      }

      // sort
      std::sort(result.begin(), result.end(), IstringCompare());

      // make unique
      // DLM: have to sort before calling unique, unique only works on consecutive elements
      auto it = std::unique(result.begin(), result.end(), IstringEqual());
      result.resize(std::distance(result.begin(), it));

      // add current to front
      if (compositeCavityInsulation) {
        result.insert(result.begin(), *compositeCavityInsulation);
      }

      return result;
    }

    bool StandardsInformationMaterial_Impl::setMaterialStandard(const std::string& materialStandard) {
      bool result = setString(OS_StandardsInformation_MaterialFields::MaterialStandard, materialStandard);
      OS_ASSERT(result);
      return result;
    }

    void StandardsInformationMaterial_Impl::resetMaterialStandard() {
      bool result = setString(OS_StandardsInformation_MaterialFields::MaterialStandard, "");
      OS_ASSERT(result);
    }

    bool StandardsInformationMaterial_Impl::setMaterialStandardSource(const std::string& materialStandardSource) {
      bool result = setString(OS_StandardsInformation_MaterialFields::MaterialStandardSource, materialStandardSource);
      OS_ASSERT(result);
      return result;
    }

    void StandardsInformationMaterial_Impl::resetMaterialStandardSource() {
      bool result = setString(OS_StandardsInformation_MaterialFields::MaterialStandardSource, "");
      OS_ASSERT(result);
    }

    bool StandardsInformationMaterial_Impl::setStandardsCategory(const std::string& standardsCategory) {
      bool result = setString(OS_StandardsInformation_MaterialFields::StandardsCategory, standardsCategory);
      OS_ASSERT(result);

      if (!isCompositeMaterial()) {
        resetCompositeFramingMaterial();
        resetCompositeFramingConfiguration();
        resetCompositeFramingDepth();
        resetCompositeFramingSize();
        resetCompositeCavityInsulation();
      }
      return result;
    }

    void StandardsInformationMaterial_Impl::resetStandardsCategory() {
      bool result = setString(OS_StandardsInformation_MaterialFields::StandardsCategory, "");
      OS_ASSERT(result);
    }

    bool StandardsInformationMaterial_Impl::setStandardsIdentifier(const std::string& standardsIdentifier) {
      bool result = setString(OS_StandardsInformation_MaterialFields::StandardsIdentifier, standardsIdentifier);
      OS_ASSERT(result);
      return result;
    }

    void StandardsInformationMaterial_Impl::resetStandardsIdentifier() {
      bool result = setString(OS_StandardsInformation_MaterialFields::StandardsIdentifier, "");
      OS_ASSERT(result);
    }

    bool StandardsInformationMaterial_Impl::setCompositeFramingMaterial(const std::string& compositeFramingMaterial) {
      bool result = setString(OS_StandardsInformation_MaterialFields::CompositeFramingMaterial, compositeFramingMaterial);
      OS_ASSERT(result);
      return result;
    }

    void StandardsInformationMaterial_Impl::resetCompositeFramingMaterial() {
      bool result = setString(OS_StandardsInformation_MaterialFields::CompositeFramingMaterial, "");
      OS_ASSERT(result);
    }

    bool StandardsInformationMaterial_Impl::setCompositeFramingConfiguration(const std::string& compositeFramingConfiguration) {
      bool result = setString(OS_StandardsInformation_MaterialFields::CompositeFramingConfiguration, compositeFramingConfiguration);
      OS_ASSERT(result);
      return result;
    }

    void StandardsInformationMaterial_Impl::resetCompositeFramingConfiguration() {
      bool result = setString(OS_StandardsInformation_MaterialFields::CompositeFramingConfiguration, "");
      OS_ASSERT(result);
    }

    bool StandardsInformationMaterial_Impl::setCompositeFramingDepth(const std::string& compositeFramingDepth) {
      bool result = setString(OS_StandardsInformation_MaterialFields::CompositeFramingDepth, compositeFramingDepth);
      OS_ASSERT(result);
      return result;
    }

    void StandardsInformationMaterial_Impl::resetCompositeFramingDepth() {
      bool result = setString(OS_StandardsInformation_MaterialFields::CompositeFramingDepth, "");
      OS_ASSERT(result);
    }

    bool StandardsInformationMaterial_Impl::setCompositeFramingSize(const std::string& compositeFramingSize) {
      bool result = setString(OS_StandardsInformation_MaterialFields::CompositeFramingSize, compositeFramingSize);
      OS_ASSERT(result);
      return result;
    }

    void StandardsInformationMaterial_Impl::resetCompositeFramingSize() {
      bool result = setString(OS_StandardsInformation_MaterialFields::CompositeFramingSize, "");
      OS_ASSERT(result);
    }

    bool StandardsInformationMaterial_Impl::setCompositeCavityInsulation(const std::string& compositeFramingInsulation) {
      bool result = setString(OS_StandardsInformation_MaterialFields::CompositeCavityInsulation, compositeFramingInsulation);
      OS_ASSERT(result);
      return result;
    }

    void StandardsInformationMaterial_Impl::resetCompositeCavityInsulation() {
      bool result = setString(OS_StandardsInformation_MaterialFields::CompositeCavityInsulation, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  StandardsInformationMaterial::StandardsInformationMaterial(const Material& material)
    : ModelObject(StandardsInformationMaterial::iddObjectType(), material.model()) {
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

  bool StandardsInformationMaterial::isCompositeMaterial() const {
    return getImpl<detail::StandardsInformationMaterial_Impl>()->isCompositeMaterial();
  }

  boost::optional<std::string> StandardsInformationMaterial::standardsIdentifier() const {
    return getImpl<detail::StandardsInformationMaterial_Impl>()->standardsIdentifier();
  }

  std::vector<std::string> StandardsInformationMaterial::suggestedStandardsIdentifiers() const {
    return getImpl<detail::StandardsInformationMaterial_Impl>()->suggestedStandardsIdentifiers();
  }

  boost::optional<std::string> StandardsInformationMaterial::compositeFramingMaterial() const {
    return getImpl<detail::StandardsInformationMaterial_Impl>()->compositeFramingMaterial();
  }

  std::vector<std::string> StandardsInformationMaterial::suggestedCompositeFramingMaterials() const {
    return getImpl<detail::StandardsInformationMaterial_Impl>()->suggestedCompositeFramingMaterials();
  }

  boost::optional<std::string> StandardsInformationMaterial::compositeFramingConfiguration() const {
    return getImpl<detail::StandardsInformationMaterial_Impl>()->compositeFramingConfiguration();
  }

  std::vector<std::string> StandardsInformationMaterial::suggestedCompositeFramingConfigurations() const {
    return getImpl<detail::StandardsInformationMaterial_Impl>()->suggestedCompositeFramingConfigurations();
  }

  boost::optional<std::string> StandardsInformationMaterial::compositeFramingDepth() const {
    return getImpl<detail::StandardsInformationMaterial_Impl>()->compositeFramingDepth();
  }

  std::vector<std::string> StandardsInformationMaterial::suggestedCompositeFramingDepths() const {
    return getImpl<detail::StandardsInformationMaterial_Impl>()->suggestedCompositeFramingDepths();
  }

  boost::optional<std::string> StandardsInformationMaterial::compositeFramingSize() const {
    return getImpl<detail::StandardsInformationMaterial_Impl>()->compositeFramingSize();
  }

  std::vector<std::string> StandardsInformationMaterial::suggestedCompositeFramingSizes() const {
    return getImpl<detail::StandardsInformationMaterial_Impl>()->suggestedCompositeFramingSizes();
  }

  boost::optional<std::string> StandardsInformationMaterial::compositeCavityInsulation() const {
    return getImpl<detail::StandardsInformationMaterial_Impl>()->compositeCavityInsulation();
  }

  std::vector<std::string> StandardsInformationMaterial::suggestedCompositeCavityInsulations() const {
    return getImpl<detail::StandardsInformationMaterial_Impl>()->suggestedCompositeCavityInsulations();
  }

  bool StandardsInformationMaterial::setMaterialStandard(const std::string& materialStandard) {
    return getImpl<detail::StandardsInformationMaterial_Impl>()->setMaterialStandard(materialStandard);
  }

  void StandardsInformationMaterial::resetMaterialStandard() {
    getImpl<detail::StandardsInformationMaterial_Impl>()->resetMaterialStandard();
  }

  bool StandardsInformationMaterial::setMaterialStandardSource(const std::string& materialStandardSource) {
    return getImpl<detail::StandardsInformationMaterial_Impl>()->setMaterialStandardSource(materialStandardSource);
  }

  void StandardsInformationMaterial::resetMaterialStandardSource() {
    getImpl<detail::StandardsInformationMaterial_Impl>()->resetMaterialStandardSource();
  }

  bool StandardsInformationMaterial::setStandardsCategory(const std::string& standardsCategory) {
    return getImpl<detail::StandardsInformationMaterial_Impl>()->setStandardsCategory(standardsCategory);
  }

  void StandardsInformationMaterial::resetStandardsCategory() {
    getImpl<detail::StandardsInformationMaterial_Impl>()->resetStandardsCategory();
  }

  bool StandardsInformationMaterial::setStandardsIdentifier(const std::string& standardsIdentifier) {
    return getImpl<detail::StandardsInformationMaterial_Impl>()->setStandardsIdentifier(standardsIdentifier);
  }

  void StandardsInformationMaterial::resetStandardsIdentifier() {
    getImpl<detail::StandardsInformationMaterial_Impl>()->resetStandardsIdentifier();
  }

  bool StandardsInformationMaterial::setCompositeFramingMaterial(const std::string& compositeFramingMaterial) {
    return getImpl<detail::StandardsInformationMaterial_Impl>()->setCompositeFramingMaterial(compositeFramingMaterial);
  }

  void StandardsInformationMaterial::resetCompositeFramingMaterial() {
    getImpl<detail::StandardsInformationMaterial_Impl>()->resetCompositeFramingMaterial();
  }

  bool StandardsInformationMaterial::setCompositeFramingConfiguration(const std::string& compositeFramingConfiguration) {
    return getImpl<detail::StandardsInformationMaterial_Impl>()->setCompositeFramingConfiguration(compositeFramingConfiguration);
  }

  void StandardsInformationMaterial::resetCompositeFramingConfiguration() {
    getImpl<detail::StandardsInformationMaterial_Impl>()->resetCompositeFramingConfiguration();
  }

  bool StandardsInformationMaterial::setCompositeFramingDepth(const std::string& compositeFramingDepth) {
    return getImpl<detail::StandardsInformationMaterial_Impl>()->setCompositeFramingDepth(compositeFramingDepth);
  }

  void StandardsInformationMaterial::resetCompositeFramingDepth() {
    getImpl<detail::StandardsInformationMaterial_Impl>()->resetCompositeFramingDepth();
  }

  bool StandardsInformationMaterial::setCompositeFramingSize(const std::string& compositeFramingSize) {
    return getImpl<detail::StandardsInformationMaterial_Impl>()->setCompositeFramingSize(compositeFramingSize);
  }

  void StandardsInformationMaterial::resetCompositeFramingSize() {
    getImpl<detail::StandardsInformationMaterial_Impl>()->resetCompositeFramingSize();
  }

  bool StandardsInformationMaterial::setCompositeCavityInsulation(const std::string& compositeCavityInsulation) {
    return getImpl<detail::StandardsInformationMaterial_Impl>()->setCompositeCavityInsulation(compositeCavityInsulation);
  }

  void StandardsInformationMaterial::resetCompositeCavityInsulation() {
    getImpl<detail::StandardsInformationMaterial_Impl>()->resetCompositeCavityInsulation();
  }

  /// @cond
  StandardsInformationMaterial::StandardsInformationMaterial(std::shared_ptr<detail::StandardsInformationMaterial_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
