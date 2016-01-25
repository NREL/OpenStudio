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

#include "UtilityCost_Computation.hpp"
#include "UtilityCost_Computation_Impl.hpp"

#include "Model_Impl.hpp"

#include "../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/OS_UtilityCost_Computation_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <boost/algorithm/string.hpp>

namespace openstudio {
namespace model  {
namespace detail {

UtilityCost_Computation_Impl::UtilityCost_Computation_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : ParentObject_Impl(idfObject, model, keepHandle)
{
  OS_ASSERT(idfObject.iddObject().type() == UtilityCost_Computation::iddObjectType());
}

UtilityCost_Computation_Impl::UtilityCost_Computation_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
  : ParentObject_Impl(other,model,keepHandle)
{
  OS_ASSERT(other.iddObject().type() == UtilityCost_Computation::iddObjectType());
}

UtilityCost_Computation_Impl::UtilityCost_Computation_Impl(const UtilityCost_Computation_Impl& other,Model_Impl* model,bool keepHandle)
  : ParentObject_Impl(other,model,keepHandle)
{
}

OptionalString UtilityCost_Computation_Impl::tariffName() const
{
  return getString(OS_UtilityCost_ComputationFields::TariffName);
}

void UtilityCost_Computation_Impl::setTariffName(const std::string& str)
{
  setString(OS_UtilityCost_ComputationFields::TariffName,str);
}

boost::optional<std::string> UtilityCost_Computation_Impl::computeStep(unsigned index) const {
  IdfExtensibleGroup eg = getExtensibleGroup(index);
  if (!eg.empty()) {
    return eg.getString(OS_UtilityCost_ComputationExtensibleFields::ComputeStep,true);
  }
  return boost::none;
}

bool UtilityCost_Computation_Impl::setComputeStep(unsigned index, const std::string& str) {
  IdfExtensibleGroup eg = getExtensibleGroup(index);
  if (!eg.empty()) {
    return eg.setString(OS_UtilityCost_ComputationExtensibleFields::ComputeStep,str);
  }
  else {
    StringVector values(1u,str);
    return !insertExtensibleGroup(index,values).empty();
  }
  OS_ASSERT(false);
  return false;
}

// return the parent object in the hierarchy
boost::optional<ParentObject> UtilityCost_Computation_Impl::parent() const
{
  return boost::optional<ParentObject>();
}

// set the parent, child may have to call methods on the parent
bool UtilityCost_Computation_Impl::setParent(ParentObject& newParent)
{
  //if (newParent.optionalCast<SomeParentClass>() && (newParent.model() == model())) {
  //  return true;
  //}
  return false;
}

// return any children objects in the hierarchy
std::vector<ModelObject> UtilityCost_Computation_Impl::children() const
{
  std::vector<ModelObject> result;
  return result;
}

std::vector<IddObjectType> UtilityCost_Computation_Impl::allowableChildTypes() const {
  IddObjectTypeVector result;
  return result;
}

// Get all output variable names that could be associated with this object.
const std::vector<std::string>& UtilityCost_Computation_Impl::outputVariableNames() const
{
  static std::vector<std::string> result;
  if (result.empty()){
  }
  return result;
}

unsigned UtilityCost_Computation_Impl::numComputeSteps() const {
  return numExtensibleGroups();
}
unsigned UtilityCost_Computation_Impl::maxComputeSteps() const {
  return maxExtensibleGroups().get();
}

} // detail

/// constructor
UtilityCost_Computation::UtilityCost_Computation(const Model& model)
  : ParentObject(UtilityCost_Computation::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::UtilityCost_Computation_Impl>());
}

// constructor
UtilityCost_Computation::UtilityCost_Computation(std::shared_ptr<detail::UtilityCost_Computation_Impl> impl)
  : ParentObject(impl)
{}

OptionalString UtilityCost_Computation::name() const
{
  return getImpl<detail::UtilityCost_Computation_Impl>()->name();
}

void UtilityCost_Computation::setName(const std::string& str)
{
  getImpl<detail::UtilityCost_Computation_Impl>()->setName(str);
}

OptionalString UtilityCost_Computation::tariffName() const
{
  return getImpl<detail::UtilityCost_Computation_Impl>()->tariffName();
}

void UtilityCost_Computation::setTariffName(const std::string& str)
{
  getImpl<detail::UtilityCost_Computation_Impl>()->setTariffName(str);
}

boost::optional<std::string> UtilityCost_Computation::computeStep(unsigned index) const {
  return getImpl<detail::UtilityCost_Computation_Impl>()->computeStep(index);
}
bool UtilityCost_Computation::setComputeStep(unsigned index, const std::string& str) {
  return getImpl<detail::UtilityCost_Computation_Impl>()->setComputeStep(index,str);
}

unsigned UtilityCost_Computation::numComputeSteps() const {
  return getImpl<detail::UtilityCost_Computation_Impl>()->numComputeSteps();
}
unsigned UtilityCost_Computation::maxComputeSteps() const {
  return getImpl<detail::UtilityCost_Computation_Impl>()->maxComputeSteps();
}

IddObjectType UtilityCost_Computation::iddObjectType()
{
  IddObjectType result(IddObjectType::OS_UtilityCost_Computation);
  return result;
}

} // model
} // openstudio
