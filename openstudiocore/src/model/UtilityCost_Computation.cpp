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
  : ParentObject(std::move(impl))
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
