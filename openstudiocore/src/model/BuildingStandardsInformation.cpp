/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#include <model/BuildingStandardsInformation.hpp>
#include <model/BuildingStandardsInformation_Impl.hpp>

#include <model/Model.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>

#include <utilities/idd/OS_StandardsInformation_Building_FieldEnums.hxx>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  BuildingStandardsInformation_Impl::BuildingStandardsInformation_Impl(const IdfObject& idfObject, 
                                                                       Model_Impl* model, 
                                                                       bool keepHandle)
    : ModelObject_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == BuildingStandardsInformation::iddObjectType());
  }

  BuildingStandardsInformation_Impl::BuildingStandardsInformation_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == BuildingStandardsInformation::iddObjectType());
  }

  BuildingStandardsInformation_Impl::BuildingStandardsInformation_Impl(
      const BuildingStandardsInformation_Impl& other,Model_Impl* model,bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  boost::optional<ParentObject> BuildingStandardsInformation_Impl::parent() const {
    boost::optional<ParentObject> result;
    boost::optional<Building> building = this->model().building();
    if (building){
      result = boost::optional<ParentObject>(*building);
    }
    return result;
  }

  boost::optional<int> BuildingStandardsInformation_Impl::numberOfStories() const {
    return getInt(OS_StandardsInformation_BuildingFields::NumberofStories,true);
  }

  boost::optional<int> BuildingStandardsInformation_Impl::numberOfAboveGroundStories() const {
    return getInt(OS_StandardsInformation_BuildingFields::NumberofAboveGroundStories,true);
  }

  const std::vector<std::string>& BuildingStandardsInformation_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType BuildingStandardsInformation_Impl::iddObjectType() const {
    return BuildingStandardsInformation::iddObjectType();
  }

  bool BuildingStandardsInformation_Impl::setNumberOfStories(boost::optional<int> value) {
    bool ok(true);
    if (value) {
      ok = setInt(OS_StandardsInformation_BuildingFields::NumberofStories,*value);
    }
    else {
      ok = setString(OS_StandardsInformation_BuildingFields::NumberofStories,std::string());
    }
    return ok;
  }

  bool BuildingStandardsInformation_Impl::setNumberOfAboveGroundStories(boost::optional<int> value) {
    bool ok(true);
    if (value) {
      ok = setInt(OS_StandardsInformation_BuildingFields::NumberofAboveGroundStories,*value);
    }
    else {
      ok = setString(OS_StandardsInformation_BuildingFields::NumberofAboveGroundStories,std::string());
    }
    return ok;
  }

} // detail

BuildingStandardsInformation::BuildingStandardsInformation(Model& model) 
  : ModelObject(BuildingStandardsInformation::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::BuildingStandardsInformation_Impl>());
}

IddObjectType BuildingStandardsInformation::iddObjectType() {
  IddObjectType result(IddObjectType::OS_StandardsInformation_Building);
  return result;
}

boost::optional<int> BuildingStandardsInformation::numberOfStories() const {
  return getImpl<detail::BuildingStandardsInformation_Impl>()->numberOfStories();
}

boost::optional<int> BuildingStandardsInformation::numberOfAboveGroundStories() const {
  return getImpl<detail::BuildingStandardsInformation_Impl>()->numberOfAboveGroundStories();
}

bool BuildingStandardsInformation::setNumberOfStories(boost::optional<int> value) {
  return getImpl<detail::BuildingStandardsInformation_Impl>()->setNumberOfStories(value);
}

bool BuildingStandardsInformation::setNumberOfAboveGroundStories(boost::optional<int> value) {
  return getImpl<detail::BuildingStandardsInformation_Impl>()->setNumberOfAboveGroundStories(value);
}

/// @cond
BuildingStandardsInformation::BuildingStandardsInformation(
    boost::shared_ptr<detail::BuildingStandardsInformation_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

