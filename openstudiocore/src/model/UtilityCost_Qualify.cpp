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
  : ParentObject(std::move(impl))
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
