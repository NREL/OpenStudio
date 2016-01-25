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

#include "LifeCycleCostUsePriceEscalation.hpp"
#include "LifeCycleCostUsePriceEscalation_Impl.hpp"

#include "Model_Impl.hpp"

#include "../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/OS_LifeCycleCost_UsePriceEscalation_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <boost/algorithm/string.hpp>

namespace openstudio {
namespace model  {
namespace detail {

LifeCycleCostUsePriceEscalation_Impl::LifeCycleCostUsePriceEscalation_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : ParentObject_Impl(idfObject, model, keepHandle)
{
  OS_ASSERT(idfObject.iddObject().type() == LifeCycleCostUsePriceEscalation::iddObjectType());
}

LifeCycleCostUsePriceEscalation_Impl::LifeCycleCostUsePriceEscalation_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
  : ParentObject_Impl(other,model,keepHandle)
{
  OS_ASSERT(other.iddObject().type() == LifeCycleCostUsePriceEscalation::iddObjectType());
}

LifeCycleCostUsePriceEscalation_Impl::LifeCycleCostUsePriceEscalation_Impl(const LifeCycleCostUsePriceEscalation_Impl& other,Model_Impl* model,bool keepHandle)
  : ParentObject_Impl(other,model,keepHandle)
{
}

OptionalInt LifeCycleCostUsePriceEscalation_Impl::escalationStartYear() const
{
  return getInt(OS_LifeCycleCost_UsePriceEscalationFields::EscalationStartYear);
}

void LifeCycleCostUsePriceEscalation_Impl::setEscalationStartYear(int num)
{
  setInt(OS_LifeCycleCost_UsePriceEscalationFields::EscalationStartYear,num);
}

boost::optional<double> LifeCycleCostUsePriceEscalation_Impl::yearEscalation(unsigned index) const 
{
  IdfExtensibleGroup eg = getExtensibleGroup(index);
  if (!eg.empty()) {
    return eg.getDouble(OS_LifeCycleCost_UsePriceEscalationExtensibleFields::YearEscalation,true);
  }
  return boost::none;
}

bool LifeCycleCostUsePriceEscalation_Impl::setYearEscalation(unsigned index, double num) {
  IdfExtensibleGroup eg = getExtensibleGroup(index);
  if (!eg.empty()) {
    return eg.setDouble(OS_LifeCycleCost_UsePriceEscalationExtensibleFields::YearEscalation,num);
  }
  else {
    StringVector values(1u);
    eg = insertExtensibleGroup(index,values);
    if (!eg.empty()) { 
      return eg.setDouble(OS_LifeCycleCost_UsePriceEscalationExtensibleFields::YearEscalation,num);
    }
  }
  return false;
}

unsigned LifeCycleCostUsePriceEscalation_Impl::numYears() const {
  return numExtensibleGroups();
}

OptionalString LifeCycleCostUsePriceEscalation_Impl::resource() const
{
  return getString(OS_LifeCycleCost_UsePriceEscalationFields::Resource);
}

void LifeCycleCostUsePriceEscalation_Impl::setResource(const std::string& str)
{
  setString(OS_LifeCycleCost_UsePriceEscalationFields::Resource,str);
}

OptionalString LifeCycleCostUsePriceEscalation_Impl::escalationStartMonth() const
{
  return getString(OS_LifeCycleCost_UsePriceEscalationFields::EscalationStartMonth);
}

void LifeCycleCostUsePriceEscalation_Impl::setEscalationStartMonth(const std::string& str)
{
  setString(OS_LifeCycleCost_UsePriceEscalationFields::EscalationStartMonth,str);
}

// return the parent object in the hierarchy
boost::optional<ParentObject> LifeCycleCostUsePriceEscalation_Impl::parent() const
{
  return boost::optional<ParentObject>();
}

// set the parent, child may have to call methods on the parent
bool LifeCycleCostUsePriceEscalation_Impl::setParent(ParentObject& newParent)
{
  //if (newParent.optionalCast<SomeParentClass>() && (newParent.model() == model())) {
  //  return true;
  //}
  return false;
}

// return any children objects in the hierarchy
std::vector<ModelObject> LifeCycleCostUsePriceEscalation_Impl::children() const
{
  std::vector<ModelObject> result;
  return result;
}

std::vector<IddObjectType> LifeCycleCostUsePriceEscalation_Impl::allowableChildTypes() const {
  IddObjectTypeVector result;
  return result;
}

// Get all output variable names that could be associated with this object.
const std::vector<std::string>& LifeCycleCostUsePriceEscalation_Impl::outputVariableNames() const
{
  static std::vector<std::string> result;
  if (result.empty()){
  }
  return result;
}

} // detail

/// constructor
LifeCycleCostUsePriceEscalation::LifeCycleCostUsePriceEscalation(const Model& model)
  : ParentObject(LifeCycleCostUsePriceEscalation::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::LifeCycleCostUsePriceEscalation_Impl>());
}

// constructor
LifeCycleCostUsePriceEscalation::LifeCycleCostUsePriceEscalation(std::shared_ptr<detail::LifeCycleCostUsePriceEscalation_Impl> impl)
  : ParentObject(impl)
{}

OptionalInt LifeCycleCostUsePriceEscalation::escalationStartYear() const
{
  return getImpl<detail::LifeCycleCostUsePriceEscalation_Impl>()->escalationStartYear();
}

void LifeCycleCostUsePriceEscalation::setEscalationStartYear(int num)
{
  getImpl<detail::LifeCycleCostUsePriceEscalation_Impl>()->setEscalationStartYear(num);
}

boost::optional<double> LifeCycleCostUsePriceEscalation::yearEscalation(unsigned index) const {
  return getImpl<detail::LifeCycleCostUsePriceEscalation_Impl>()->yearEscalation(index);
}

bool LifeCycleCostUsePriceEscalation::setYearEscalation(unsigned index, double num) {
  return getImpl<detail::LifeCycleCostUsePriceEscalation_Impl>()->setYearEscalation(index,num);
}

unsigned LifeCycleCostUsePriceEscalation::numYears() const {
  return getImpl<detail::LifeCycleCostUsePriceEscalation_Impl>()->numYears();
}

OptionalString LifeCycleCostUsePriceEscalation::resource() const
{
  return getImpl<detail::LifeCycleCostUsePriceEscalation_Impl>()->resource();
}

void LifeCycleCostUsePriceEscalation::setResource(const std::string& str)
{
  getImpl<detail::LifeCycleCostUsePriceEscalation_Impl>()->setResource(str);
}

OptionalString LifeCycleCostUsePriceEscalation::escalationStartMonth() const
{
  return getImpl<detail::LifeCycleCostUsePriceEscalation_Impl>()->escalationStartMonth();
}

void LifeCycleCostUsePriceEscalation::setEscalationStartMonth(const std::string& str)
{
  getImpl<detail::LifeCycleCostUsePriceEscalation_Impl>()->setEscalationStartMonth(str);
}

IddObjectType LifeCycleCostUsePriceEscalation::iddObjectType()
{
  IddObjectType result(IddObjectType::OS_LifeCycleCost_UsePriceEscalation);
  return result;
}

} // model
} // openstudio
