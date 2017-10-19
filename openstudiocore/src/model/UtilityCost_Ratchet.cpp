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

#include "UtilityCost_Ratchet.hpp"
#include "UtilityCost_Ratchet_Impl.hpp"

#include "Model_Impl.hpp"

#include <utilities/idd/OS_UtilityCost_Ratchet_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <boost/algorithm/string.hpp>

namespace openstudio {
namespace model  {
namespace detail {

UtilityCost_Ratchet_Impl::UtilityCost_Ratchet_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : ParentObject_Impl(idfObject, model, keepHandle)
{
  OS_ASSERT(idfObject.iddObject().type() == UtilityCost_Ratchet::iddObjectType());
}

UtilityCost_Ratchet_Impl::UtilityCost_Ratchet_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
  : ParentObject_Impl(other,model,keepHandle)
{
  OS_ASSERT(other.iddObject().type() == UtilityCost_Ratchet::iddObjectType());
}

UtilityCost_Ratchet_Impl::UtilityCost_Ratchet_Impl(const UtilityCost_Ratchet_Impl& other,Model_Impl* model,bool keepHandle)
  : ParentObject_Impl(other,model,keepHandle)
{
}

OptionalString UtilityCost_Ratchet_Impl::tariffName() const
{
  return getString(OS_UtilityCost_RatchetFields::TariffName);
}

void UtilityCost_Ratchet_Impl::setTariffName(const std::string& str)
{
  setString(OS_UtilityCost_RatchetFields::TariffName,str);
}

OptionalString UtilityCost_Ratchet_Impl::baselineSourceVariable() const
{
  return getString(OS_UtilityCost_RatchetFields::BaselineSourceVariable);
}

void UtilityCost_Ratchet_Impl::setBaselineSourceVariable(const std::string& str)
{
  setString(OS_UtilityCost_RatchetFields::BaselineSourceVariable,str);
}

OptionalString UtilityCost_Ratchet_Impl::adjustmentSourceVariable() const
{
  return getString(OS_UtilityCost_RatchetFields::AdjustmentSourceVariable);
}

void UtilityCost_Ratchet_Impl::setAdjustmentSourceVariable(const std::string& str)
{
  setString(OS_UtilityCost_RatchetFields::AdjustmentSourceVariable,str);
}

OptionalString UtilityCost_Ratchet_Impl::seasonFrom() const
{
  return getString(OS_UtilityCost_RatchetFields::SeasonFrom);
}

void UtilityCost_Ratchet_Impl::setSeasonFrom(const std::string& str)
{
  setString(OS_UtilityCost_RatchetFields::SeasonFrom,str);
}

OptionalString UtilityCost_Ratchet_Impl::seasonTo() const
{
  return getString(OS_UtilityCost_RatchetFields::SeasonTo);
}

void UtilityCost_Ratchet_Impl::setSeasonTo(const std::string& str)
{
  setString(OS_UtilityCost_RatchetFields::SeasonTo,str);
}

OptionalString UtilityCost_Ratchet_Impl::multiplierValueOrVariableName() const
{
  return getString(OS_UtilityCost_RatchetFields::MultiplierValueorVariableName);
}

void UtilityCost_Ratchet_Impl::setMultiplierValueOrVariableName(const std::string& str)
{
  setString(OS_UtilityCost_RatchetFields::MultiplierValueorVariableName,str);
}

OptionalString UtilityCost_Ratchet_Impl::offsetValueOrVariableName() const
{
  return getString(OS_UtilityCost_RatchetFields::OffsetValueorVariableName);
}

void UtilityCost_Ratchet_Impl::setOffsetValueOrVariableName(const std::string& str)
{
  setString(OS_UtilityCost_RatchetFields::OffsetValueorVariableName,str);
}

// return the parent object in the hierarchy
boost::optional<ParentObject> UtilityCost_Ratchet_Impl::parent() const
{
  return boost::optional<ParentObject>();
}

// set the parent, child may have to call methods on the parent
bool UtilityCost_Ratchet_Impl::setParent(ParentObject& newParent)
{
  //if (newParent.optionalCast<SomeParentClass>() && (newParent.model() == model())) {
  //  return true;
  //}
  return false;
}

// return any children objects in the hierarchy
std::vector<ModelObject> UtilityCost_Ratchet_Impl::children() const
{
  std::vector<ModelObject> result;
  return result;
}

std::vector<IddObjectType> UtilityCost_Ratchet_Impl::allowableChildTypes() const {
  IddObjectTypeVector result;
  return result;
}

// Get all output variable names that could be associated with this object.
const std::vector<std::string>& UtilityCost_Ratchet_Impl::outputVariableNames() const
{
  static std::vector<std::string> result;
  if (result.empty()){
  }
  return result;
}

} // detail

/// constructor
UtilityCost_Ratchet::UtilityCost_Ratchet(const Model& model)
  : ParentObject(UtilityCost_Ratchet::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::UtilityCost_Ratchet_Impl>());
}

// constructor
UtilityCost_Ratchet::UtilityCost_Ratchet(std::shared_ptr<detail::UtilityCost_Ratchet_Impl> impl)
  : ParentObject(std::move(impl))
{}

OptionalString UtilityCost_Ratchet::tariffName() const
{
  return getImpl<detail::UtilityCost_Ratchet_Impl>()->tariffName();
}

void UtilityCost_Ratchet::setTariffName(const std::string& str)
{
  getImpl<detail::UtilityCost_Ratchet_Impl>()->setTariffName(str);
}

OptionalString UtilityCost_Ratchet::baselineSourceVariable() const
{
  return getImpl<detail::UtilityCost_Ratchet_Impl>()->baselineSourceVariable();
}

void UtilityCost_Ratchet::setBaselineSourceVariable(const std::string& str)
{
  getImpl<detail::UtilityCost_Ratchet_Impl>()->setBaselineSourceVariable(str);
}

OptionalString UtilityCost_Ratchet::adjustmentSourceVariable() const
{
  return getImpl<detail::UtilityCost_Ratchet_Impl>()->adjustmentSourceVariable();
}

void UtilityCost_Ratchet::setAdjustmentSourceVariable(const std::string& str)
{
  getImpl<detail::UtilityCost_Ratchet_Impl>()->setAdjustmentSourceVariable(str);
}

OptionalString UtilityCost_Ratchet::seasonFrom() const
{
  return getImpl<detail::UtilityCost_Ratchet_Impl>()->seasonFrom();
}

void UtilityCost_Ratchet::setSeasonFrom(const std::string& str)
{
  getImpl<detail::UtilityCost_Ratchet_Impl>()->setSeasonFrom(str);
}

OptionalString UtilityCost_Ratchet::seasonTo() const
{
  return getImpl<detail::UtilityCost_Ratchet_Impl>()->seasonTo();
}

void UtilityCost_Ratchet::setSeasonTo(const std::string& str)
{
  getImpl<detail::UtilityCost_Ratchet_Impl>()->setSeasonTo(str);
}

OptionalString UtilityCost_Ratchet::multiplierValueOrVariableName() const
{
  return getImpl<detail::UtilityCost_Ratchet_Impl>()->multiplierValueOrVariableName();
}

void UtilityCost_Ratchet::setMultiplierValueOrVariableName(const std::string& str)
{
  getImpl<detail::UtilityCost_Ratchet_Impl>()->setMultiplierValueOrVariableName(str);
}

OptionalString UtilityCost_Ratchet::offsetValueOrVariableName() const
{
  return getImpl<detail::UtilityCost_Ratchet_Impl>()->offsetValueOrVariableName();
}

void UtilityCost_Ratchet::setOffsetValueOrVariableName(const std::string& str)
{
  getImpl<detail::UtilityCost_Ratchet_Impl>()->setOffsetValueOrVariableName(str);
}

IddObjectType UtilityCost_Ratchet::iddObjectType()
{
  IddObjectType result(IddObjectType::OS_UtilityCost_Ratchet);
  return result;
}

} // model
} // openstudio
