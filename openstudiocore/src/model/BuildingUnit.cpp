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

  boost::optional<std::string> BuildingUnit_Impl::buildingUnitType() const
  {
    return getString(OS_BuildingUnitFields::BuildingUnitType, true, true);
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
    std::vector<std::string> names;
    for (const ModelExtensibleGroup& group : castVector<ModelExtensibleGroup>(extensibleGroups())) {
      boost::optional<std::string> name = group.getString(OS_BuildingUnitExtensibleFields::BuildingUnitFeatureName);
      OS_ASSERT(name);
      names.push_back(*name);
    }
    return names;
  }

  boost::optional<ModelExtensibleGroup> BuildingUnit_Impl::getFeatureGroupByName(const std::string &name) const {

    for (ModelExtensibleGroup& group : castVector<ModelExtensibleGroup>(extensibleGroups())) {
      const boost::optional<std::string> featureName(group.getString(OS_BuildingUnitExtensibleFields::BuildingUnitFeatureName));
      OS_ASSERT(featureName);
      if (*featureName == name) {
        boost::optional<ModelExtensibleGroup> modelGroup(group);
        return modelGroup;
      }
    }
    boost::optional<ModelExtensibleGroup> nullModelGroup(boost::none);
    return nullModelGroup;
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
    return getFeatureStringAndCheckForType(name, "alpha");
  }

  boost::optional<double> BuildingUnit_Impl::getFeatureAsDouble(const std::string& name) const
  {
    boost::optional<std::string> strValue(getFeatureStringAndCheckForType(name, "real"));
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
    boost::optional<std::string> strValue(getFeatureStringAndCheckForType(name, "integer"));
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
    boost::optional<std::string> strValue(getFeatureStringAndCheckForType(name, "boolean"));
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
    std::vector<std::string> retvals;
    retvals.push_back("NumberOfBedrooms");
    retvals.push_back("NumberOfBathrooms");
    // TODO: Look at other Building Unit feature names and suggest those too.
    return retvals;
  }

  bool BuildingUnit_Impl::setFeatureGroupDataTypeAndValue(const std::string& name, const std::string& dataType, const std::string& value)
  {
    boost::optional<ModelExtensibleGroup> group(getFeatureGroupByName(name));
    if (group) {
      const bool dataTypeOK = group->setString(OS_BuildingUnitExtensibleFields::BuildingUnitFeatureDataType, dataType);
      const bool valueOK = group->setString(OS_BuildingUnitExtensibleFields::BuildingUnitFeatureValue, value);
      return (dataTypeOK and valueOK);
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
    return setFeatureGroupDataTypeAndValue(name, "alpha", value);
  }

  bool BuildingUnit_Impl::setFeature(const std::string& name, const char* value)
  {
    return setFeature(name, std::string(value));
  }

  bool BuildingUnit_Impl::setFeature(const std::string& name, double value)
  {
    return setFeatureGroupDataTypeAndValue(name, "real", boost::lexical_cast<std::string>(value));
  }

  bool BuildingUnit_Impl::setFeature(const std::string& name, int value)
  {
    return setFeatureGroupDataTypeAndValue(name, "integer", boost::lexical_cast<std::string>(value));
  }

  bool BuildingUnit_Impl::setFeature(const std::string& name, bool value)
  {
    std::string strValue(boost::lexical_cast<std::string>(value));
    if (value) {
      strValue = "true";
    } else {
      strValue = "false";
    }
    return setFeatureGroupDataTypeAndValue(name, "boolean", strValue);
  }

  bool BuildingUnit_Impl::resetFeature(const std::string& name)
  {
    unsigned n_groups = numExtensibleGroups();
    std::vector<std::string> featureNames(this->featureNames());
    for (unsigned i=0; i < n_groups; ++i) {
      if (featureNames[i] == name) {
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

boost::optional<std::string> BuildingUnit::buildingUnitType() const
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
  : ModelObject(impl)
{}
/// @endcond

} //model
} //openstudio