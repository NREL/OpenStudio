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

#include "UtilityCost_Charge_Simple.hpp"
#include "UtilityCost_Charge_Simple_Impl.hpp"

#include "Model_Impl.hpp"

#include <utilities/idd/OS_UtilityCost_Charge_Simple_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <boost/algorithm/string.hpp>

namespace openstudio {
namespace model  {
namespace detail {

UtilityCost_Charge_Simple_Impl::UtilityCost_Charge_Simple_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : ParentObject_Impl(idfObject, model, keepHandle)
{
  OS_ASSERT(idfObject.iddObject().type() == UtilityCost_Charge_Simple::iddObjectType());
}

UtilityCost_Charge_Simple_Impl::UtilityCost_Charge_Simple_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
  : ParentObject_Impl(other,model,keepHandle)
{
  OS_ASSERT(other.iddObject().type() == UtilityCost_Charge_Simple::iddObjectType());
}

UtilityCost_Charge_Simple_Impl::UtilityCost_Charge_Simple_Impl(const UtilityCost_Charge_Simple_Impl& other,Model_Impl* model,bool keepHandle)
  : ParentObject_Impl(other,model,keepHandle)
{
}

OptionalString UtilityCost_Charge_Simple_Impl::tariffName() const
{
  return getString(OS_UtilityCost_Charge_SimpleFields::TariffName);
}

void UtilityCost_Charge_Simple_Impl::setTariffName(const std::string& str)
{
  setString(OS_UtilityCost_Charge_SimpleFields::TariffName,str);
}

OptionalString UtilityCost_Charge_Simple_Impl::sourceVariable() const
{
  return getString(OS_UtilityCost_Charge_SimpleFields::SourceVariable);
}

void UtilityCost_Charge_Simple_Impl::setSourceVariable(const std::string& str)
{
  setString(OS_UtilityCost_Charge_SimpleFields::SourceVariable,str);
}

OptionalString UtilityCost_Charge_Simple_Impl::season() const
{
  return getString(OS_UtilityCost_Charge_SimpleFields::Season);
}

void UtilityCost_Charge_Simple_Impl::setSeason(const std::string& str)
{
  setString(OS_UtilityCost_Charge_SimpleFields::Season,str);
}

OptionalString UtilityCost_Charge_Simple_Impl::categoryVariableName() const
{
  return getString(OS_UtilityCost_Charge_SimpleFields::CategoryVariableName);
}

void UtilityCost_Charge_Simple_Impl::setCategoryVariableName(const std::string& str)
{
  setString(OS_UtilityCost_Charge_SimpleFields::CategoryVariableName,str);
}

OptionalString UtilityCost_Charge_Simple_Impl::costPerUnitValueOrVariableName() const
{
  return getString(OS_UtilityCost_Charge_SimpleFields::CostperUnitValueorVariableName);
}

void UtilityCost_Charge_Simple_Impl::setCostPerUnitValueOrVariableName(const std::string& str)
{
  setString(OS_UtilityCost_Charge_SimpleFields::CostperUnitValueorVariableName,str);
}

// return the parent object in the hierarchy
boost::optional<ParentObject> UtilityCost_Charge_Simple_Impl::parent() const
{
  return boost::optional<ParentObject>();
}

// set the parent, child may have to call methods on the parent
bool UtilityCost_Charge_Simple_Impl::setParent(ParentObject& newParent)
{
  //if (newParent.optionalCast<SomeParentClass>() && (newParent.model() == model())) {
  //  return true;
  //}
  return false;
}

// return any children objects in the hierarchy
std::vector<ModelObject> UtilityCost_Charge_Simple_Impl::children() const
{
  std::vector<ModelObject> result;
  return result;
}

std::vector<IddObjectType> UtilityCost_Charge_Simple_Impl::allowableChildTypes() const {
  IddObjectTypeVector result;
  return result;
}

// Get all output variable names that could be associated with this object.
const std::vector<std::string>& UtilityCost_Charge_Simple_Impl::outputVariableNames() const
{
  static std::vector<std::string> result;
  if (result.empty()){
  }
  return result;
}

} // detail

/// constructor
UtilityCost_Charge_Simple::UtilityCost_Charge_Simple(const Model& model)
  : ParentObject(UtilityCost_Charge_Simple::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::UtilityCost_Charge_Simple_Impl>());
}

// constructor
UtilityCost_Charge_Simple::UtilityCost_Charge_Simple(std::shared_ptr<detail::UtilityCost_Charge_Simple_Impl> impl)
  : ParentObject(impl)
{}

OptionalString UtilityCost_Charge_Simple::tariffName() const
{
  return getImpl<detail::UtilityCost_Charge_Simple_Impl>()->tariffName();
}

void UtilityCost_Charge_Simple::setTariffName(const std::string& str)
{
  getImpl<detail::UtilityCost_Charge_Simple_Impl>()->setTariffName(str);
}

OptionalString UtilityCost_Charge_Simple::sourceVariable() const
{
  return getImpl<detail::UtilityCost_Charge_Simple_Impl>()->sourceVariable();
}

void UtilityCost_Charge_Simple::setSourceVariable(const std::string& str)
{
  getImpl<detail::UtilityCost_Charge_Simple_Impl>()->setSourceVariable(str);
}

OptionalString UtilityCost_Charge_Simple::season() const
{
  return getImpl<detail::UtilityCost_Charge_Simple_Impl>()->season();
}

void UtilityCost_Charge_Simple::setSeason(const std::string& str)
{
  getImpl<detail::UtilityCost_Charge_Simple_Impl>()->setSeason(str);
}

OptionalString UtilityCost_Charge_Simple::categoryVariableName() const
{
  return getImpl<detail::UtilityCost_Charge_Simple_Impl>()->categoryVariableName();
}

void UtilityCost_Charge_Simple::setCategoryVariableName(const std::string& str)
{
  getImpl<detail::UtilityCost_Charge_Simple_Impl>()->setCategoryVariableName(str);
}

OptionalString UtilityCost_Charge_Simple::costPerUnitValueOrVariableName() const
{
  return getImpl<detail::UtilityCost_Charge_Simple_Impl>()->costPerUnitValueOrVariableName();
}

void UtilityCost_Charge_Simple::setCostPerUnitValueOrVariableName(const std::string& str)
{
  getImpl<detail::UtilityCost_Charge_Simple_Impl>()->setCostPerUnitValueOrVariableName(str);
}

IddObjectType UtilityCost_Charge_Simple::iddObjectType()
{
  IddObjectType result(IddObjectType::OS_UtilityCost_Charge_Simple);
  return result;
}

} // model
} // openstudio
