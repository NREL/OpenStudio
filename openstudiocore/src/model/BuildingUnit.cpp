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

#include <string>
#include <memory>

#include "BuildingUnit.hpp"
#include "BuildingUnit_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "RenderingColor.hpp"
#include "RenderingColor_Impl.hpp"
#include "ModelExtensibleGroup.hpp"

#include <utilities/idd/OS_BuildingUnit_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

namespace openstudio {
namespace model {
namespace detail {

  BuildingUnit_Impl::BuildingUnit_Impl(const IdfObject &idfObject, Model_Impl *model, bool keepHandle)
    : ModelObject_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == BuildingUnit::iddObjectType());
  }

  BuildingUnit_Impl::BuildingUnit_Impl(const openstudio::detail::WorkspaceObject_Impl &other,
                                       Model_Impl *model,
                                       bool keepHandle)
    : ModelObject_Impl(other, model, keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == BuildingUnit::iddObjectType());
  }

  BuildingUnit_Impl::BuildingUnit_Impl(const BuildingUnit_Impl &other,
                                       Model_Impl *model,
                                       bool keepHandle)
    : ModelObject_Impl(other, model, keepHandle)
  {}

  const std::vector<std::string>& BuildingUnit_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType BuildingUnit_Impl::iddObjectType() const {
    return BuildingUnit::iddObjectType();
  }

  boost::optional<RenderingColor> BuildingUnit_Impl::renderingColor() const
  {
    return getObject<ModelObject>().getModelObjectTarget<RenderingColor>(OS_BuildingUnitFields::RenderingColor);
  }

  bool BuildingUnit_Impl::setRenderingColor(const RenderingColor& renderingColor)
  {
    return setPointer(OS_BuildingUnitFields::RenderingColor, renderingColor.handle());
  }

  void BuildingUnit_Impl::resetRenderingColor()
  {
    bool ok = setString(OS_BuildingUnitFields::RenderingColor, "");
    OS_ASSERT(ok);
  }

  std::string BuildingUnit_Impl::buildingUnitType() const
  {
    boost::optional<std::string> retval = getString(OS_BuildingUnitFields::BuildingUnitType, true, true);
    OS_ASSERT(retval);
    return *retval;
  }

  bool BuildingUnit_Impl::setBuildingUnitType(const std::string& buildingUnitType)
  {
    bool ok = setString(OS_BuildingUnitFields::BuildingUnitType, buildingUnitType);
    return ok;
  }

  void BuildingUnit_Impl::resetBuildingUnitType()
  {
    bool ok = setString(OS_BuildingUnitFields::BuildingUnitType, "");
    OS_ASSERT(ok);
  }

  std::vector<Space> BuildingUnit_Impl::spaces() const
  {
    return getObject<ModelObject>().getModelObjectSources<Space>(Space::iddObjectType());
  }

  std::vector<std::string> BuildingUnit_Impl::featureNames() const
  {
    std::set<std::string> nameSet;
    for (const ModelExtensibleGroup& group : castVector<ModelExtensibleGroup>(extensibleGroups())) {
      boost::optional<std::string> name = group.getString(OS_BuildingUnitExtensibleFields::BuildingUnitFeatureName);
      OS_ASSERT(name);
      nameSet.insert(*name);
    }
    std::vector<std::string> names;
    names.assign(nameSet.begin(), nameSet.end());
    return names;
  }

  boost::optional<ModelExtensibleGroup> BuildingUnit_Impl::getFeatureGroupByName(const std::string &name) const {

    for (ModelExtensibleGroup& group : castVector<ModelExtensibleGroup>(extensibleGroups())) {
      const boost::optional<std::string> featureName(group.getString(OS_BuildingUnitExtensibleFields::BuildingUnitFeatureName));
      OS_ASSERT(featureName);
      if (*featureName == name) {
        return group;
      }
    }
    return boost::none;
  }

  boost::optional<std::string> BuildingUnit_Impl::getFeatureDataType(const std::string &name) const
  {
    boost::optional<std::string> dataType;
    boost::optional<ModelExtensibleGroup> group(getFeatureGroupByName(name));
    if (group) {
      dataType = group->getString(OS_BuildingUnitExtensibleFields::BuildingUnitFeatureDataType);
      OS_ASSERT(dataType);
    } else {
      dataType = boost::none;
    }
    return dataType;
  }

  boost::optional<std::string> BuildingUnit_Impl::getFeatureStringAndCheckForType(const std::string& name, const std::string& expectedDataType) const
  {
    boost::optional<std::string> value;
    boost::optional<ModelExtensibleGroup> group(getFeatureGroupByName(name));
    if (group) {
      boost::optional<std::string> dataType(group->getString(OS_BuildingUnitExtensibleFields::BuildingUnitFeatureDataType));
      OS_ASSERT(dataType);
      if (*dataType == expectedDataType) {
        value = group->getString(OS_BuildingUnitExtensibleFields::BuildingUnitFeatureValue);
      } else {
        value = boost::none;
      }
    } else {
      value = boost::none;
    }
    return value;
  }

  boost::optional<std::string> BuildingUnit_Impl::getFeatureAsString(const std::string& name) const
  {
    return getFeatureStringAndCheckForType(name, "String");
  }

  boost::optional<double> BuildingUnit_Impl::getFeatureAsDouble(const std::string& name) const
  {
    boost::optional<std::string> strValue(getFeatureStringAndCheckForType(name, "Double"));
    boost::optional<double> value;
    if (strValue) {
      try {
        value = boost::lexical_cast<double>(*strValue);
      } catch (boost::bad_lexical_cast) {
        LOG(Error, "Value: " + *strValue + ", not castable to type double.")
        value = boost::none;
      }
    } else {
      value = boost::none;
    }
    return value;
  }

  boost::optional<int> BuildingUnit_Impl::getFeatureAsInteger(const std::string& name) const
  {
    boost::optional<std::string> strValue(getFeatureStringAndCheckForType(name, "Integer"));
    boost::optional<int> value;
    if (strValue) {
      try {
        value = boost::lexical_cast<int>(*strValue);
      } catch (boost::bad_lexical_cast) {
        LOG(Error, "Value: " + *strValue + ", not castable to type integer.")
        value = boost::none;
      }
    } else {
      value = boost::none;
    }
    return value;
  }

  boost::optional<bool> BuildingUnit_Impl::getFeatureAsBoolean(const std::string& name) const
  {
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

  std::vector<std::string> BuildingUnit_Impl::suggestedFeatures() const
  {
    std::set<std::string> availableFeatureNames;
    availableFeatureNames.insert("NumberOfBedrooms");
    availableFeatureNames.insert("NumberOfBathrooms");
    for (const BuildingUnit& bldgUnit : this->model().getConcreteModelObjects<BuildingUnit>()) {
      for (const std::string& featureName : bldgUnit.featureNames()) {
        availableFeatureNames.insert(featureName);
      }
    }
    std::vector<std::string> retvals;
    retvals.assign(availableFeatureNames.begin(), availableFeatureNames.end());
    return retvals;
  }

  bool BuildingUnit_Impl::setFeatureGroupDataTypeAndValue(const std::string& name, const std::string& dataType, const std::string& value)
  {
    boost::optional<ModelExtensibleGroup> group(getFeatureGroupByName(name));
    if (group) {
      const bool dataTypeOK = group->setString(OS_BuildingUnitExtensibleFields::BuildingUnitFeatureDataType, dataType, false);
      const bool valueOK = group->setString(OS_BuildingUnitExtensibleFields::BuildingUnitFeatureValue, value, false);
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
      ModelExtensibleGroup newgroup = pushExtensibleGroup(temp).cast<ModelExtensibleGroup>();
      return (!newgroup.empty());
    }
  }

  bool BuildingUnit_Impl::setFeature(const std::string& name, const std::string& value)
  {
    return setFeatureGroupDataTypeAndValue(name, "String", value);
  }

  bool BuildingUnit_Impl::setFeature(const std::string& name, const char* value)
  {
    return setFeature(name, std::string(value));
  }

  bool BuildingUnit_Impl::setFeature(const std::string& name, double value)
  {
    return setFeatureGroupDataTypeAndValue(name, "Double", boost::lexical_cast<std::string>(value));
  }

  bool BuildingUnit_Impl::setFeature(const std::string& name, int value)
  {
    return setFeatureGroupDataTypeAndValue(name, "Integer", boost::lexical_cast<std::string>(value));
  }

  bool BuildingUnit_Impl::setFeature(const std::string& name, bool value)
  {
    std::string strValue(boost::lexical_cast<std::string>(value));
    if (value) {
      strValue = "true";
    } else {
      strValue = "false";
    }
    return setFeatureGroupDataTypeAndValue(name, "Boolean", strValue);
  }

  bool BuildingUnit_Impl::resetFeature(const std::string& name)
  {
    unsigned n_groups = numExtensibleGroups();
    for (unsigned i=0; i < n_groups; ++i) {
      ModelExtensibleGroup group = getExtensibleGroup(i).cast<ModelExtensibleGroup>();
      const boost::optional<std::string> featureName(group.getString(OS_BuildingUnitExtensibleFields::BuildingUnitFeatureName));
      OS_ASSERT(featureName);
      if (*featureName == name) {
        eraseExtensibleGroup(i);
        return true;
      }
    }
    return false;
  }

} //detail

BuildingUnit::BuildingUnit(const Model &model)
  : ModelObject(BuildingUnit::iddObjectType(), model)
{
  OS_ASSERT(getImpl<detail::BuildingUnit_Impl>());
}

IddObjectType BuildingUnit::iddObjectType() {
  IddObjectType result(IddObjectType::OS_BuildingUnit);
  return result;
}

boost::optional<RenderingColor> BuildingUnit::renderingColor() const
{
  return getImpl<detail::BuildingUnit_Impl>()->renderingColor();
}

bool BuildingUnit::setRenderingColor(const RenderingColor& renderingColor)
{
  return getImpl<detail::BuildingUnit_Impl>()->setRenderingColor(renderingColor);
}

void BuildingUnit::resetRenderingColor()
{
  getImpl<detail::BuildingUnit_Impl>()->resetRenderingColor();
}

std::vector<std::string> BuildingUnit::buildingUnitTypeValues()
{
  return getIddKeyNames(
    IddFactory::instance().getObject(iddObjectType()).get(),
    OS_BuildingUnitFields::BuildingUnitType
  );
}

std::string BuildingUnit::buildingUnitType() const
{
  return getImpl<detail::BuildingUnit_Impl>()->buildingUnitType();
}

bool BuildingUnit::setBuildingUnitType(const std::string& buildingUnitType)
{
  return getImpl<detail::BuildingUnit_Impl>()->setBuildingUnitType(buildingUnitType);
}

void BuildingUnit::resetBuildingUnitType()
{
  getImpl<detail::BuildingUnit_Impl>()->resetBuildingUnitType();
}

std::vector<Space> BuildingUnit::spaces() const
{
  return getImpl<detail::BuildingUnit_Impl>()->spaces();
}

std::vector<std::string> BuildingUnit::featureNames() const
{
  return getImpl<detail::BuildingUnit_Impl>()->featureNames();
}

boost::optional<std::string> BuildingUnit::getFeatureDataType(const std::string& name) const
{
  return getImpl<detail::BuildingUnit_Impl>()->getFeatureDataType(name);
}

boost::optional<std::string> BuildingUnit::getFeatureAsString(const std::string& name) const
{
  return getImpl<detail::BuildingUnit_Impl>()->getFeatureAsString(name);
}

boost::optional<double> BuildingUnit::getFeatureAsDouble(const std::string& name) const
{
  return getImpl<detail::BuildingUnit_Impl>()->getFeatureAsDouble(name);
}

boost::optional<int> BuildingUnit::getFeatureAsInteger(const std::string& name) const
{
  return getImpl<detail::BuildingUnit_Impl>()->getFeatureAsInteger(name);
}

boost::optional<bool> BuildingUnit::getFeatureAsBoolean(const std::string& name) const
{
  return getImpl<detail::BuildingUnit_Impl>()->getFeatureAsBoolean(name);
}

std::vector<std::string> BuildingUnit::suggestedFeatures() const
{
  return getImpl<detail::BuildingUnit_Impl>()->suggestedFeatures();
}

double BuildingUnit::floorArea() const
{
  double totalFloorArea(0.0);
  for (const auto &space : this->spaces()) {
    totalFloorArea += space.floorArea();
  }
  return totalFloorArea;
}

bool BuildingUnit::setFeature(const std::string& name, const std::string& value)
{
  return getImpl<detail::BuildingUnit_Impl>()->setFeature(name, value);
}

bool BuildingUnit::setFeature(const std::string& name, const char* value)
{
  return getImpl<detail::BuildingUnit_Impl>()->setFeature(name, value);
}

bool BuildingUnit::setFeature(const std::string& name, double value)
{
  return getImpl<detail::BuildingUnit_Impl>()->setFeature(name, value);
}

bool BuildingUnit::setFeature(const std::string& name, int value)
{
  return getImpl<detail::BuildingUnit_Impl>()->setFeature(name, value);
}

bool BuildingUnit::setFeature(const std::string& name, bool value)
{
  return getImpl<detail::BuildingUnit_Impl>()->setFeature(name, value);
}

bool BuildingUnit::resetFeature(const std::string& name)
{
  return getImpl<detail::BuildingUnit_Impl>()->resetFeature(name);
}

/// @cond
BuildingUnit::BuildingUnit(std::shared_ptr<detail::BuildingUnit_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} //model
} //openstudio
