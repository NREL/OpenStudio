/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "UtilityCost_Variable.hpp"
#include "UtilityCost_Variable_Impl.hpp"

#include "Model_Impl.hpp"

#include <utilities/idd/OS_UtilityCost_Variable_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <boost/algorithm/string.hpp>

namespace openstudio {
namespace model  {
namespace detail {

UtilityCost_Variable_Impl::UtilityCost_Variable_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : ParentObject_Impl(idfObject, model, keepHandle)
{
  OS_ASSERT(idfObject.iddObject().type() == UtilityCost_Variable::iddObjectType());
}

UtilityCost_Variable_Impl::UtilityCost_Variable_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
  : ParentObject_Impl(other,model,keepHandle)
{
  OS_ASSERT(other.iddObject().type() == UtilityCost_Variable::iddObjectType());
}

UtilityCost_Variable_Impl::UtilityCost_Variable_Impl(const UtilityCost_Variable_Impl& other,Model_Impl* model,bool keepHandle)
  : ParentObject_Impl(other,model,keepHandle)
{
}

OptionalDouble UtilityCost_Variable_Impl::januaryValue() const
{
  return getDouble(OS_UtilityCost_VariableFields::JanuaryValue);
}

bool UtilityCost_Variable_Impl::setJanuaryValue(double num)
{
  return setDouble(OS_UtilityCost_VariableFields::JanuaryValue,num);;
}

OptionalDouble UtilityCost_Variable_Impl::februaryValue() const
{
  return getDouble(OS_UtilityCost_VariableFields::FebruaryValue);
}

bool UtilityCost_Variable_Impl::setFebruaryValue(double num)
{
  return setDouble(OS_UtilityCost_VariableFields::FebruaryValue,num);;
}

OptionalDouble UtilityCost_Variable_Impl::marchValue() const
{
  return getDouble(OS_UtilityCost_VariableFields::MarchValue);
}

bool UtilityCost_Variable_Impl::setMarchValue(double num)
{
  return setDouble(OS_UtilityCost_VariableFields::MarchValue,num);;
}

OptionalDouble UtilityCost_Variable_Impl::aprilValue() const
{
  return getDouble(OS_UtilityCost_VariableFields::AprilValue);
}

bool UtilityCost_Variable_Impl::setAprilValue(double num)
{
  return setDouble(OS_UtilityCost_VariableFields::AprilValue,num);;
}

OptionalDouble UtilityCost_Variable_Impl::mayValue() const
{
  return getDouble(OS_UtilityCost_VariableFields::MayValue);
}

bool UtilityCost_Variable_Impl::setMayValue(double num)
{
  return setDouble(OS_UtilityCost_VariableFields::MayValue,num);;
}

OptionalDouble UtilityCost_Variable_Impl::juneValue() const
{
  return getDouble(OS_UtilityCost_VariableFields::JuneValue);
}

bool UtilityCost_Variable_Impl::setJuneValue(double num)
{
  return setDouble(OS_UtilityCost_VariableFields::JuneValue,num);;
}

OptionalDouble UtilityCost_Variable_Impl::julyValue() const
{
  return getDouble(OS_UtilityCost_VariableFields::JulyValue);
}

bool UtilityCost_Variable_Impl::setJulyValue(double num)
{
  return setDouble(OS_UtilityCost_VariableFields::JulyValue,num);;
}

OptionalDouble UtilityCost_Variable_Impl::augustValue() const
{
  return getDouble(OS_UtilityCost_VariableFields::AugustValue);
}

bool UtilityCost_Variable_Impl::setAugustValue(double num)
{
  return setDouble(OS_UtilityCost_VariableFields::AugustValue,num);;
}

OptionalDouble UtilityCost_Variable_Impl::septemberValue() const
{
  return getDouble(OS_UtilityCost_VariableFields::SeptemberValue);
}

bool UtilityCost_Variable_Impl::setSeptemberValue(double num)
{
  return setDouble(OS_UtilityCost_VariableFields::SeptemberValue,num);;
}

OptionalDouble UtilityCost_Variable_Impl::octoberValue() const
{
  return getDouble(OS_UtilityCost_VariableFields::OctoberValue);
}

bool UtilityCost_Variable_Impl::setOctoberValue(double num)
{
  return setDouble(OS_UtilityCost_VariableFields::OctoberValue,num);;
}

OptionalDouble UtilityCost_Variable_Impl::novemberValue() const
{
  return getDouble(OS_UtilityCost_VariableFields::NovemberValue);
}

bool UtilityCost_Variable_Impl::setNovemberValue(double num)
{
  return setDouble(OS_UtilityCost_VariableFields::NovemberValue,num);;
}

OptionalDouble UtilityCost_Variable_Impl::decemberValue() const
{
  return getDouble(OS_UtilityCost_VariableFields::DecemberValue);
}

bool UtilityCost_Variable_Impl::setDecemberValue(double num)
{
  return setDouble(OS_UtilityCost_VariableFields::DecemberValue,num);;
}

OptionalString UtilityCost_Variable_Impl::tariffName() const
{
  return getString(OS_UtilityCost_VariableFields::TariffName);
}

bool UtilityCost_Variable_Impl::setTariffName(const std::string& str)
{
  return setString(OS_UtilityCost_VariableFields::TariffName,str);;
}

OptionalString UtilityCost_Variable_Impl::variableType() const
{
  return getString(OS_UtilityCost_VariableFields::VariableType);
}

bool UtilityCost_Variable_Impl::setVariableType(const std::string& str)
{
  return setString(OS_UtilityCost_VariableFields::VariableType,str);;
}

// return the parent object in the hierarchy
boost::optional<ParentObject> UtilityCost_Variable_Impl::parent() const
{
  return boost::optional<ParentObject>();
}

// set the parent, child may have to call methods on the parent
bool UtilityCost_Variable_Impl::setParent(ParentObject& newParent)
{
  //if (newParent.optionalCast<SomeParentClass>() && (newParent.model() == model())) {
  //  return true;
  //}
  return false;
}

// return any children objects in the hierarchy
std::vector<ModelObject> UtilityCost_Variable_Impl::children() const
{
  std::vector<ModelObject> result;
  return result;
}

std::vector<IddObjectType> UtilityCost_Variable_Impl::allowableChildTypes() const {
  IddObjectTypeVector result;
  return result;
}

// Get all output variable names that could be associated with this object.
const std::vector<std::string>& UtilityCost_Variable_Impl::outputVariableNames() const
{
  static std::vector<std::string> result;
  if (result.empty()){
  }
  return result;
}

} // detail

/// constructor
UtilityCost_Variable::UtilityCost_Variable(const Model& model)
  : ParentObject(UtilityCost_Variable::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::UtilityCost_Variable_Impl>());
}

// constructor
UtilityCost_Variable::UtilityCost_Variable(std::shared_ptr<detail::UtilityCost_Variable_Impl> impl)
  : ParentObject(std::move(impl))
{}

OptionalDouble UtilityCost_Variable::januaryValue() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->januaryValue();
}

bool UtilityCost_Variable::setJanuaryValue(double num)
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->setJanuaryValue(num);
}

OptionalDouble UtilityCost_Variable::februaryValue() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->februaryValue();
}

bool UtilityCost_Variable::setFebruaryValue(double num)
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->setFebruaryValue(num);
}

OptionalDouble UtilityCost_Variable::marchValue() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->marchValue();
}

bool UtilityCost_Variable::setMarchValue(double num)
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->setMarchValue(num);
}

OptionalDouble UtilityCost_Variable::aprilValue() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->aprilValue();
}

bool UtilityCost_Variable::setAprilValue(double num)
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->setAprilValue(num);
}

OptionalDouble UtilityCost_Variable::mayValue() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->mayValue();
}

bool UtilityCost_Variable::setMayValue(double num)
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->setMayValue(num);
}

OptionalDouble UtilityCost_Variable::juneValue() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->juneValue();
}

bool UtilityCost_Variable::setJuneValue(double num)
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->setJuneValue(num);
}

OptionalDouble UtilityCost_Variable::julyValue() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->julyValue();
}

bool UtilityCost_Variable::setJulyValue(double num)
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->setJulyValue(num);
}

OptionalDouble UtilityCost_Variable::augustValue() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->augustValue();
}

bool UtilityCost_Variable::setAugustValue(double num)
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->setAugustValue(num);
}

OptionalDouble UtilityCost_Variable::septemberValue() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->septemberValue();
}

bool UtilityCost_Variable::setSeptemberValue(double num)
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->setSeptemberValue(num);
}

OptionalDouble UtilityCost_Variable::octoberValue() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->octoberValue();
}

bool UtilityCost_Variable::setOctoberValue(double num)
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->setOctoberValue(num);
}

OptionalDouble UtilityCost_Variable::novemberValue() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->novemberValue();
}

bool UtilityCost_Variable::setNovemberValue(double num)
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->setNovemberValue(num);
}

OptionalDouble UtilityCost_Variable::decemberValue() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->decemberValue();
}

bool UtilityCost_Variable::setDecemberValue(double num)
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->setDecemberValue(num);
}

OptionalString UtilityCost_Variable::tariffName() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->tariffName();
}

bool UtilityCost_Variable::setTariffName(const std::string& str)
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->setTariffName(str);
}

OptionalString UtilityCost_Variable::variableType() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->variableType();
}

bool UtilityCost_Variable::setVariableType(const std::string& str)
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->setVariableType(str);
}

IddObjectType UtilityCost_Variable::iddObjectType()
{
  IddObjectType result(IddObjectType::OS_UtilityCost_Variable);
  return result;
}

} // model
} // openstudio
