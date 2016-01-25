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

#include "UtilityCost_Qualify.hpp"
#include "UtilityCost_Qualify_Impl.hpp"

#include "Model_Impl.hpp"

#include <utilities/idd/OS_UtilityCost_Qualify_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <boost/algorithm/string.hpp>

namespace openstudio {
namespace model  {
namespace detail {

UtilityCost_Qualify_Impl::UtilityCost_Qualify_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : ParentObject_Impl(idfObject, model, keepHandle)
{
  OS_ASSERT(idfObject.iddObject().type() == UtilityCost_Qualify::iddObjectType());
}

UtilityCost_Qualify_Impl::UtilityCost_Qualify_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
  : ParentObject_Impl(other,model,keepHandle)
{
  OS_ASSERT(other.iddObject().type() == UtilityCost_Qualify::iddObjectType());
}

UtilityCost_Qualify_Impl::UtilityCost_Qualify_Impl(const UtilityCost_Qualify_Impl& other,Model_Impl* model,bool keepHandle)
  : ParentObject_Impl(other,model,keepHandle)
{
}

OptionalInt UtilityCost_Qualify_Impl::numberOfMonths() const
{
  return getInt(OS_UtilityCost_QualifyFields::NumberofMonths);
}

void UtilityCost_Qualify_Impl::setNumberOfMonths(int num)
{
  setInt(OS_UtilityCost_QualifyFields::NumberofMonths,num);
}

OptionalString UtilityCost_Qualify_Impl::tariffName() const
{
  return getString(OS_UtilityCost_QualifyFields::TariffName);
}

void UtilityCost_Qualify_Impl::setTariffName(const std::string& str)
{
  setString(OS_UtilityCost_QualifyFields::TariffName,str);
}

OptionalString UtilityCost_Qualify_Impl::variableName() const
{
  return getString(OS_UtilityCost_QualifyFields::VariableName);
}

void UtilityCost_Qualify_Impl::setVariableName(const std::string& str)
{
  setString(OS_UtilityCost_QualifyFields::VariableName,str);
}

OptionalString UtilityCost_Qualify_Impl::qualifyType() const
{
  return getString(OS_UtilityCost_QualifyFields::QualifyType);
}

void UtilityCost_Qualify_Impl::setQualifyType(const std::string& str)
{
  setString(OS_UtilityCost_QualifyFields::QualifyType,str);
}

OptionalString UtilityCost_Qualify_Impl::thresholdValueOrVariableName() const
{
  return getString(OS_UtilityCost_QualifyFields::ThresholdValueorVariableName);
}

void UtilityCost_Qualify_Impl::setThresholdValueOrVariableName(const std::string& str)
{
  setString(OS_UtilityCost_QualifyFields::ThresholdValueorVariableName,str);
}

OptionalString UtilityCost_Qualify_Impl::season() const
{
  return getString(OS_UtilityCost_QualifyFields::Season);
}

void UtilityCost_Qualify_Impl::setSeason(const std::string& str)
{
  setString(OS_UtilityCost_QualifyFields::Season,str);
}

OptionalString UtilityCost_Qualify_Impl::thresholdTest() const
{
  return getString(OS_UtilityCost_QualifyFields::ThresholdTest);
}

void UtilityCost_Qualify_Impl::setThresholdTest(const std::string& str)
{
  setString(OS_UtilityCost_QualifyFields::ThresholdTest,str);
}

// return the parent object in the hierarchy
boost::optional<ParentObject> UtilityCost_Qualify_Impl::parent() const
{
  return boost::optional<ParentObject>();
}

// set the parent, child may have to call methods on the parent
bool UtilityCost_Qualify_Impl::setParent(ParentObject& newParent)
{
  //if (newParent.optionalCast<SomeParentClass>() && (newParent.model() == model())) {
  //  return true;
  //}
  return false;
}

// return any children objects in the hierarchy
std::vector<ModelObject> UtilityCost_Qualify_Impl::children() const
{
  std::vector<ModelObject> result;
  return result;
}

std::vector<IddObjectType> UtilityCost_Qualify_Impl::allowableChildTypes() const {
  IddObjectTypeVector result;
  return result;
}

// Get all output variable names that could be associated with this object.
const std::vector<std::string>& UtilityCost_Qualify_Impl::outputVariableNames() const
{
  static std::vector<std::string> result;
  if (result.empty()){
  }
  return result;
}

} // detail

/// constructor
UtilityCost_Qualify::UtilityCost_Qualify(const Model& model)
  : ParentObject(UtilityCost_Qualify::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::UtilityCost_Qualify_Impl>());
}

// constructor
UtilityCost_Qualify::UtilityCost_Qualify(std::shared_ptr<detail::UtilityCost_Qualify_Impl> impl)
  : ParentObject(impl)
{}

OptionalInt UtilityCost_Qualify::numberOfMonths() const
{
  return getImpl<detail::UtilityCost_Qualify_Impl>()->numberOfMonths();
}

void UtilityCost_Qualify::setNumberOfMonths(int num)
{
  getImpl<detail::UtilityCost_Qualify_Impl>()->setNumberOfMonths(num);
}

OptionalString UtilityCost_Qualify::name() const
{
  return getImpl<detail::UtilityCost_Qualify_Impl>()->name();
}

void UtilityCost_Qualify::setName(const std::string& str)
{
  getImpl<detail::UtilityCost_Qualify_Impl>()->setName(str);
}

OptionalString UtilityCost_Qualify::tariffName() const
{
  return getImpl<detail::UtilityCost_Qualify_Impl>()->tariffName();
}

void UtilityCost_Qualify::setTariffName(const std::string& str)
{
  getImpl<detail::UtilityCost_Qualify_Impl>()->setTariffName(str);
}

OptionalString UtilityCost_Qualify::variableName() const
{
  return getImpl<detail::UtilityCost_Qualify_Impl>()->variableName();
}

void UtilityCost_Qualify::setVariableName(const std::string& str)
{
  getImpl<detail::UtilityCost_Qualify_Impl>()->setVariableName(str);
}

OptionalString UtilityCost_Qualify::qualifyType() const
{
  return getImpl<detail::UtilityCost_Qualify_Impl>()->qualifyType();
}

void UtilityCost_Qualify::setQualifyType(const std::string& str)
{
  getImpl<detail::UtilityCost_Qualify_Impl>()->setQualifyType(str);
}

OptionalString UtilityCost_Qualify::thresholdValueOrVariableName() const
{
  return getImpl<detail::UtilityCost_Qualify_Impl>()->thresholdValueOrVariableName();
}

void UtilityCost_Qualify::setThresholdValueOrVariableName(const std::string& str)
{
  getImpl<detail::UtilityCost_Qualify_Impl>()->setThresholdValueOrVariableName(str);
}

OptionalString UtilityCost_Qualify::season() const
{
  return getImpl<detail::UtilityCost_Qualify_Impl>()->season();
}

void UtilityCost_Qualify::setSeason(const std::string& str)
{
  getImpl<detail::UtilityCost_Qualify_Impl>()->setSeason(str);
}

OptionalString UtilityCost_Qualify::thresholdTest() const
{
  return getImpl<detail::UtilityCost_Qualify_Impl>()->thresholdTest();
}

void UtilityCost_Qualify::setThresholdTest(const std::string& str)
{
  getImpl<detail::UtilityCost_Qualify_Impl>()->setThresholdTest(str);
}

IddObjectType UtilityCost_Qualify::iddObjectType()
{
  IddObjectType result(IddObjectType::OS_UtilityCost_Qualify);
  return result;
}

} // model
} // openstudio
