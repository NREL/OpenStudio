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
  : ParentObject(std::move(impl))
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
