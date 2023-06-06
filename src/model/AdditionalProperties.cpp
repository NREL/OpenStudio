/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <string>
#include <memory>

#include "AdditionalProperties.hpp"
#include "AdditionalProperties_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ModelObject.hpp"
#include "ModelObject_Impl.hpp"
#include "ModelExtensibleGroup.hpp"
#include "ResourceObject.hpp"

#include <utilities/idd/OS_AdditionalProperties_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

namespace openstudio {
namespace model {
  namespace detail {

    AdditionalProperties_Impl::AdditionalProperties_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AdditionalProperties::iddObjectType());
    }

    AdditionalProperties_Impl::AdditionalProperties_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AdditionalProperties::iddObjectType());
    }

    AdditionalProperties_Impl::AdditionalProperties_Impl(const AdditionalProperties_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    ModelObject AdditionalProperties_Impl::modelObject() const {
      boost::optional<ModelObject> result =
        getObject<AdditionalProperties>().getModelObjectTarget<ModelObject>(OS_AdditionalPropertiesFields::ObjectName);
      if (!result) {
        // DLM: should we remove ourself?  probably worse to do that since user may call other methods and cause a crash
        //this->remove();
        LOG_AND_THROW("Cannot retrieve associated ModelObject");
      }
      OS_ASSERT(result);
      return *result;
    }

    const std::vector<std::string>& AdditionalProperties_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType AdditionalProperties_Impl::iddObjectType() const {
      return AdditionalProperties::iddObjectType();
    }

    boost::optional<ParentObject> AdditionalProperties_Impl::parent() const {
      // DLM: should this return the model object? no because model object is not a parent object.
      return {};
    }

    bool AdditionalProperties_Impl::setParent(ParentObject& /*newParent*/) {
      // DLM: should we allow this? no because model object is not a parent object.
      return false;
    }

    std::vector<ResourceObject> AdditionalProperties_Impl::resources() const {
      return {};
    }

    std::vector<std::string> AdditionalProperties_Impl::featureNames() const {
      std::set<std::string> nameSet;
      for (const ModelExtensibleGroup& group : castVector<ModelExtensibleGroup>(extensibleGroups())) {
        boost::optional<std::string> name = group.getString(OS_AdditionalPropertiesExtensibleFields::FeatureName);
        OS_ASSERT(name);
        nameSet.insert(*name);
      }
      std::vector<std::string> names;
      names.assign(nameSet.begin(), nameSet.end());
      return names;
    }

    boost::optional<ModelExtensibleGroup> AdditionalProperties_Impl::getFeatureGroupByName(const std::string& name) const {

      for (ModelExtensibleGroup& group : castVector<ModelExtensibleGroup>(extensibleGroups())) {
        const boost::optional<std::string> featureName(group.getString(OS_AdditionalPropertiesExtensibleFields::FeatureName));
        OS_ASSERT(featureName);
        if (*featureName == name) {
          return group;
        }
      }
      return boost::none;
    }

    bool AdditionalProperties_Impl::hasFeature(const std::string& name) const {
      return getFeatureGroupByName(name).has_value();
    }

    boost::optional<std::string> AdditionalProperties_Impl::getFeatureDataType(const std::string& name) const {
      boost::optional<std::string> dataType;
      boost::optional<ModelExtensibleGroup> group(getFeatureGroupByName(name));
      if (group) {
        dataType = group->getString(OS_AdditionalPropertiesExtensibleFields::FeatureDataType);
        OS_ASSERT(dataType);
      } else {
        dataType = boost::none;
      }
      return dataType;
    }

    boost::optional<std::string> AdditionalProperties_Impl::getFeatureStringAndCheckForType(const std::string& name,
                                                                                            const std::string& expectedDataType) const {
      boost::optional<std::string> value;
      boost::optional<ModelExtensibleGroup> group(getFeatureGroupByName(name));
      if (group) {
        boost::optional<std::string> dataType(group->getString(OS_AdditionalPropertiesExtensibleFields::FeatureDataType));
        OS_ASSERT(dataType);
        if (*dataType == expectedDataType) {
          value = group->getString(OS_AdditionalPropertiesExtensibleFields::FeatureValue);
        } else {
          value = boost::none;
        }
      } else {
        value = boost::none;
      }
      return value;
    }

    boost::optional<std::string> AdditionalProperties_Impl::getFeatureAsString(const std::string& name) const {
      return getFeatureStringAndCheckForType(name, "String");
    }

    boost::optional<double> AdditionalProperties_Impl::getFeatureAsDouble(const std::string& name) const {
      boost::optional<std::string> strValue(getFeatureStringAndCheckForType(name, "Double"));
      boost::optional<double> value;
      if (strValue) {
        try {
          value = boost::lexical_cast<double>(*strValue);
        } catch (boost::bad_lexical_cast&) {
          LOG(Error, "Value: " + *strValue + ", not castable to type double.")
          value = boost::none;
        }
      } else {
        value = boost::none;
      }
      return value;
    }

    boost::optional<int> AdditionalProperties_Impl::getFeatureAsInteger(const std::string& name) const {
      boost::optional<std::string> strValue(getFeatureStringAndCheckForType(name, "Integer"));
      boost::optional<int> value;
      if (strValue) {
        try {
          value = boost::lexical_cast<int>(*strValue);
        } catch (boost::bad_lexical_cast&) {
          LOG(Error, "Value: " + *strValue + ", not castable to type integer.")
          value = boost::none;
        }
      } else {
        value = boost::none;
      }
      return value;
    }

    boost::optional<bool> AdditionalProperties_Impl::getFeatureAsBoolean(const std::string& name) const {
      boost::optional<std::string> strValue(getFeatureStringAndCheckForType(name, "Boolean"));
      boost::optional<bool> value;
      if (strValue) {
        if (*strValue == "false") {
          value = false;
        } else if (*strValue == "true") {
          value = true;
        } else {
          LOG(Error, "Value: " + *strValue + ", not castable to type boolean.")
          value = boost::none;
        }
      } else {
        value = boost::none;
      }
      return value;
    }

    std::vector<std::string> AdditionalProperties_Impl::suggestedFeatureNames() const {
      std::set<std::string> availableFeatureNames;
      // DLM: this should be based on the model object type right?
      // DLM: should we create a virtual method for all model objects similar to outputVariableNames
      // DLM: long term, this should pull from a list that can be updated at run time, possibly related to OpenStudio standards
      for (const AdditionalProperties& addlProps : this->model().getConcreteModelObjects<AdditionalProperties>()) {
        for (const std::string& featureName : addlProps.featureNames()) {
          availableFeatureNames.insert(featureName);
        }
      }
      std::vector<std::string> retvals;
      retvals.assign(availableFeatureNames.begin(), availableFeatureNames.end());
      return retvals;
    }

    bool AdditionalProperties_Impl::setFeatureGroupDataTypeAndValue(const std::string& name, const std::string& dataType, const std::string& value) {
      boost::optional<ModelExtensibleGroup> group(getFeatureGroupByName(name));
      if (group) {
        const bool dataTypeOK = group->setString(OS_AdditionalPropertiesExtensibleFields::FeatureDataType, dataType, false);
        const bool valueOK = group->setString(OS_AdditionalPropertiesExtensibleFields::FeatureValue, value, false);
        // Since we're doing this checking in the public setters, these should always return true.
        OS_ASSERT(dataTypeOK);
        OS_ASSERT(valueOK);
        this->emitChangeSignals();
        return true;
      } else {
        std::vector<std::string> temp;
        temp.push_back(name);
        temp.push_back(dataType);
        temp.push_back(value);
        auto newgroup = pushExtensibleGroup(temp).cast<ModelExtensibleGroup>();
        return (!newgroup.empty());
      }
    }

    bool AdditionalProperties_Impl::setFeature(const std::string& name, const std::string& value) {
      return setFeatureGroupDataTypeAndValue(name, "String", value);
    }

    bool AdditionalProperties_Impl::setFeature(const std::string& name, const char* value) {
      return setFeature(name, std::string(value));
    }

    bool AdditionalProperties_Impl::setFeature(const std::string& name, double value) {
      return setFeatureGroupDataTypeAndValue(name, "Double", boost::lexical_cast<std::string>(value));
    }

    bool AdditionalProperties_Impl::setFeature(const std::string& name, int value) {
      return setFeatureGroupDataTypeAndValue(name, "Integer", boost::lexical_cast<std::string>(value));
    }

    bool AdditionalProperties_Impl::setFeature(const std::string& name, bool value) {
      auto strValue(boost::lexical_cast<std::string>(value));
      if (value) {
        strValue = "true";
      } else {
        strValue = "false";
      }
      return setFeatureGroupDataTypeAndValue(name, "Boolean", strValue);
    }

    bool AdditionalProperties_Impl::resetFeature(const std::string& name) {
      unsigned n_groups = numExtensibleGroups();
      for (unsigned i = 0; i < n_groups; ++i) {
        auto group = getExtensibleGroup(i).cast<ModelExtensibleGroup>();
        const boost::optional<std::string> featureName(group.getString(OS_AdditionalPropertiesExtensibleFields::FeatureName));
        OS_ASSERT(featureName);
        if (*featureName == name) {
          eraseExtensibleGroup(i);
          return true;
        }
      }
      return false;
    }

    void AdditionalProperties_Impl::merge(const AdditionalProperties& other, bool overwrite) {
      if (other.handle() == this->handle()) {
        return;
      }

      for (const auto& featureName : other.featureNames()) {
        // check if we already have this key
        if ((!overwrite) && this->getFeatureDataType(featureName)) {
          continue;
        }

        boost::optional<std::string> dataType = other.getFeatureDataType(featureName);
        OS_ASSERT(dataType);
        if (istringEqual("String", *dataType)) {
          boost::optional<std::string> v = other.getFeatureAsString(featureName);
          OS_ASSERT(v);
          this->setFeature(featureName, *v);
        } else if (istringEqual("Double", *dataType)) {
          boost::optional<double> v = other.getFeatureAsDouble(featureName);
          OS_ASSERT(v);
          this->setFeature(featureName, *v);
        } else if (istringEqual("Boolean", *dataType)) {
          boost::optional<bool> v = other.getFeatureAsBoolean(featureName);
          OS_ASSERT(v);
          this->setFeature(featureName, *v);
        } else if (istringEqual("Integer", *dataType)) {
          boost::optional<int> v = other.getFeatureAsInteger(featureName);
          OS_ASSERT(v);
          this->setFeature(featureName, *v);
        }
      }
    }

  }  // namespace detail

  AdditionalProperties::AdditionalProperties(const ModelObject& modelObject)
    : ModelObject(AdditionalProperties::iddObjectType(), modelObject.model()) {
    OS_ASSERT(getImpl<detail::AdditionalProperties_Impl>());

    if (modelObject.optionalCast<AdditionalProperties>()) {
      this->remove();
      LOG_AND_THROW("Cannot create a AdditionalProperties object for AdditionalProperties object");
    }

    bool ok = setPointer(OS_AdditionalPropertiesFields::ObjectName, modelObject.handle());
    OS_ASSERT(ok);
  }

  ModelObject AdditionalProperties::modelObject() const {
    return getImpl<detail::AdditionalProperties_Impl>()->modelObject();
  }

  IddObjectType AdditionalProperties::iddObjectType() {
    IddObjectType result(IddObjectType::OS_AdditionalProperties);
    return result;
  }

  std::vector<std::string> AdditionalProperties::featureNames() const {
    return getImpl<detail::AdditionalProperties_Impl>()->featureNames();
  }

  bool AdditionalProperties::hasFeature(const std::string& name) const {
    return getImpl<detail::AdditionalProperties_Impl>()->hasFeature(name);
  }

  boost::optional<std::string> AdditionalProperties::getFeatureDataType(const std::string& name) const {
    return getImpl<detail::AdditionalProperties_Impl>()->getFeatureDataType(name);
  }

  boost::optional<std::string> AdditionalProperties::getFeatureAsString(const std::string& name) const {
    return getImpl<detail::AdditionalProperties_Impl>()->getFeatureAsString(name);
  }

  boost::optional<double> AdditionalProperties::getFeatureAsDouble(const std::string& name) const {
    return getImpl<detail::AdditionalProperties_Impl>()->getFeatureAsDouble(name);
  }

  boost::optional<int> AdditionalProperties::getFeatureAsInteger(const std::string& name) const {
    return getImpl<detail::AdditionalProperties_Impl>()->getFeatureAsInteger(name);
  }

  boost::optional<bool> AdditionalProperties::getFeatureAsBoolean(const std::string& name) const {
    return getImpl<detail::AdditionalProperties_Impl>()->getFeatureAsBoolean(name);
  }

  std::vector<std::string> AdditionalProperties::suggestedFeatureNames() const {
    return getImpl<detail::AdditionalProperties_Impl>()->suggestedFeatureNames();
  }

  bool AdditionalProperties::setFeature(const std::string& name, const std::string& value) {
    return getImpl<detail::AdditionalProperties_Impl>()->setFeature(name, value);
  }

  bool AdditionalProperties::setFeature(const std::string& name, const char* value) {
    return getImpl<detail::AdditionalProperties_Impl>()->setFeature(name, value);
  }

  bool AdditionalProperties::setFeature(const std::string& name, double value) {
    return getImpl<detail::AdditionalProperties_Impl>()->setFeature(name, value);
  }

  bool AdditionalProperties::setFeature(const std::string& name, int value) {
    return getImpl<detail::AdditionalProperties_Impl>()->setFeature(name, value);
  }

  bool AdditionalProperties::setFeature(const std::string& name, bool value) {
    return getImpl<detail::AdditionalProperties_Impl>()->setFeature(name, value);
  }

  bool AdditionalProperties::resetFeature(const std::string& name) {
    return getImpl<detail::AdditionalProperties_Impl>()->resetFeature(name);
  }

  void AdditionalProperties::merge(const AdditionalProperties& other, bool overwrite) {
    getImpl<detail::AdditionalProperties_Impl>()->merge(other, overwrite);
  }

  /// @cond
  AdditionalProperties::AdditionalProperties(std::shared_ptr<detail::AdditionalProperties_Impl> impl) : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
