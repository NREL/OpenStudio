/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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

bool UtilityCost_Charge_Simple_Impl::setTariffName(const std::string& str)
{
  return setString(OS_UtilityCost_Charge_SimpleFields::TariffName,str);;
}

OptionalString UtilityCost_Charge_Simple_Impl::sourceVariable() const
{
  return getString(OS_UtilityCost_Charge_SimpleFields::SourceVariable);
}

bool UtilityCost_Charge_Simple_Impl::setSourceVariable(const std::string& str)
{
  return setString(OS_UtilityCost_Charge_SimpleFields::SourceVariable,str);;
}

OptionalString UtilityCost_Charge_Simple_Impl::season() const
{
  return getString(OS_UtilityCost_Charge_SimpleFields::Season);
}

bool UtilityCost_Charge_Simple_Impl::setSeason(const std::string& str)
{
  return setString(OS_UtilityCost_Charge_SimpleFields::Season,str);;
}

OptionalString UtilityCost_Charge_Simple_Impl::categoryVariableName() const
{
  return getString(OS_UtilityCost_Charge_SimpleFields::CategoryVariableName);
}

bool UtilityCost_Charge_Simple_Impl::setCategoryVariableName(const std::string& str)
{
  return setString(OS_UtilityCost_Charge_SimpleFields::CategoryVariableName,str);;
}

OptionalString UtilityCost_Charge_Simple_Impl::costPerUnitValueOrVariableName() const
{
  return getString(OS_UtilityCost_Charge_SimpleFields::CostperUnitValueorVariableName);
}

bool UtilityCost_Charge_Simple_Impl::setCostPerUnitValueOrVariableName(const std::string& str)
{
  return setString(OS_UtilityCost_Charge_SimpleFields::CostperUnitValueorVariableName,str);;
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
  : ParentObject(std::move(impl))
{}

OptionalString UtilityCost_Charge_Simple::tariffName() const
{
  return getImpl<detail::UtilityCost_Charge_Simple_Impl>()->tariffName();
}

bool UtilityCost_Charge_Simple::setTariffName(const std::string& str)
{
  return getImpl<detail::UtilityCost_Charge_Simple_Impl>()->setTariffName(str);
}

OptionalString UtilityCost_Charge_Simple::sourceVariable() const
{
  return getImpl<detail::UtilityCost_Charge_Simple_Impl>()->sourceVariable();
}

bool UtilityCost_Charge_Simple::setSourceVariable(const std::string& str)
{
  return getImpl<detail::UtilityCost_Charge_Simple_Impl>()->setSourceVariable(str);
}

OptionalString UtilityCost_Charge_Simple::season() const
{
  return getImpl<detail::UtilityCost_Charge_Simple_Impl>()->season();
}

bool UtilityCost_Charge_Simple::setSeason(const std::string& str)
{
  return getImpl<detail::UtilityCost_Charge_Simple_Impl>()->setSeason(str);
}

OptionalString UtilityCost_Charge_Simple::categoryVariableName() const
{
  return getImpl<detail::UtilityCost_Charge_Simple_Impl>()->categoryVariableName();
}

bool UtilityCost_Charge_Simple::setCategoryVariableName(const std::string& str)
{
  return getImpl<detail::UtilityCost_Charge_Simple_Impl>()->setCategoryVariableName(str);
}

OptionalString UtilityCost_Charge_Simple::costPerUnitValueOrVariableName() const
{
  return getImpl<detail::UtilityCost_Charge_Simple_Impl>()->costPerUnitValueOrVariableName();
}

bool UtilityCost_Charge_Simple::setCostPerUnitValueOrVariableName(const std::string& str)
{
  return getImpl<detail::UtilityCost_Charge_Simple_Impl>()->setCostPerUnitValueOrVariableName(str);
}

IddObjectType UtilityCost_Charge_Simple::iddObjectType()
{
  IddObjectType result(IddObjectType::OS_UtilityCost_Charge_Simple);
  return result;
}

} // model
} // openstudio
