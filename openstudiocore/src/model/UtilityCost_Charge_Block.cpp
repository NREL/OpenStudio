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

#include "UtilityCost_Charge_Block.hpp"
#include "UtilityCost_Charge_Block_Impl.hpp"

#include "Model_Impl.hpp"

#include "../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/OS_UtilityCost_Charge_Block_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <boost/algorithm/string.hpp>

namespace openstudio {
namespace model  {
namespace detail {

UtilityCost_Charge_Block_Impl::UtilityCost_Charge_Block_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : ParentObject_Impl(idfObject, model, keepHandle)
{
  OS_ASSERT(idfObject.iddObject().type() == UtilityCost_Charge_Block::iddObjectType());
}

UtilityCost_Charge_Block_Impl::UtilityCost_Charge_Block_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
  : ParentObject_Impl(other,model,keepHandle)
{
  OS_ASSERT(other.iddObject().type() == UtilityCost_Charge_Block::iddObjectType());
}

UtilityCost_Charge_Block_Impl::UtilityCost_Charge_Block_Impl(const UtilityCost_Charge_Block_Impl& other,Model_Impl* model,bool keepHandle)
  : ParentObject_Impl(other,model,keepHandle)
{
}

OptionalString UtilityCost_Charge_Block_Impl::tariffName() const
{
  return getString(OS_UtilityCost_Charge_BlockFields::TariffName);
}

void UtilityCost_Charge_Block_Impl::setTariffName(const std::string& str)
{
  setString(OS_UtilityCost_Charge_BlockFields::TariffName,str);
}

OptionalString UtilityCost_Charge_Block_Impl::sourceVariable() const
{
  return getString(OS_UtilityCost_Charge_BlockFields::SourceVariable);
}

void UtilityCost_Charge_Block_Impl::setSourceVariable(const std::string& str)
{
  setString(OS_UtilityCost_Charge_BlockFields::SourceVariable,str);
}

OptionalString UtilityCost_Charge_Block_Impl::season() const
{
  return getString(OS_UtilityCost_Charge_BlockFields::Season);
}

void UtilityCost_Charge_Block_Impl::setSeason(const std::string& str)
{
  setString(OS_UtilityCost_Charge_BlockFields::Season,str);
}

OptionalString UtilityCost_Charge_Block_Impl::categoryVariableName() const
{
  return getString(OS_UtilityCost_Charge_BlockFields::CategoryVariableName);
}

void UtilityCost_Charge_Block_Impl::setCategoryVariableName(const std::string& str)
{
  setString(OS_UtilityCost_Charge_BlockFields::CategoryVariableName,str);
}

OptionalString UtilityCost_Charge_Block_Impl::remainingIntoVariable() const
{
  return getString(OS_UtilityCost_Charge_BlockFields::RemainingIntoVariable);
}

void UtilityCost_Charge_Block_Impl::setRemainingIntoVariable(const std::string& str)
{
  setString(OS_UtilityCost_Charge_BlockFields::RemainingIntoVariable,str);
}

boost::optional<std::string> UtilityCost_Charge_Block_Impl::blockSizeValueOrVariableName(unsigned index) const {
  IdfExtensibleGroup eg = getExtensibleGroup(index);
  if (!eg.empty()) {
    return eg.getString(OS_UtilityCost_Charge_BlockExtensibleFields::BlockSizeValueorVariableName,true);
  }
  return boost::none;
}

bool UtilityCost_Charge_Block_Impl::setBlockSizeValueOrVariableName(unsigned index, const std::string& str) {
  IdfExtensibleGroup eg = getExtensibleGroup(index);
  if (!eg.empty()) {
    return eg.setString(OS_UtilityCost_Charge_BlockExtensibleFields::BlockSizeValueorVariableName,str);
  }
  else {
    StringVector values(2u);
    values[OS_UtilityCost_Charge_BlockExtensibleFields::BlockSizeValueorVariableName] = str;
    return !insertExtensibleGroup(index,values).empty();
  }
  OS_ASSERT(false);
  return false;
}

boost::optional<std::string> UtilityCost_Charge_Block_Impl::blockCostPerUnitValueOrVariableName(unsigned index) const {
  IdfExtensibleGroup eg = getExtensibleGroup(index);
  if (!eg.empty()) {
    return eg.getString(OS_UtilityCost_Charge_BlockExtensibleFields::BlockCostperUnitValueorVariableName,true);
  }
  return boost::none;
}

bool UtilityCost_Charge_Block_Impl::setBlockCostPerUnitValueOrVariableName(unsigned index, const std::string& str) {
  IdfExtensibleGroup eg = getExtensibleGroup(index);
  if (!eg.empty()) {
    return eg.setString(OS_UtilityCost_Charge_BlockExtensibleFields::BlockCostperUnitValueorVariableName,str);
  }
  else {
    StringVector values(2u);
    values[OS_UtilityCost_Charge_BlockExtensibleFields::BlockCostperUnitValueorVariableName] = str;
    return !insertExtensibleGroup(index,values).empty();
  }
  OS_ASSERT(false);
  return false;
}

unsigned UtilityCost_Charge_Block_Impl::numBlocks() const {
  return numExtensibleGroups();
}

unsigned UtilityCost_Charge_Block_Impl::maxNumBlocks() const {
  return maxExtensibleGroups().get();
}

// return the parent object in the hierarchy
boost::optional<ParentObject> UtilityCost_Charge_Block_Impl::parent() const
{
  return boost::optional<ParentObject>();
}

// set the parent, child may have to call methods on the parent
bool UtilityCost_Charge_Block_Impl::setParent(ParentObject& newParent)
{
  //if (newParent.optionalCast<SomeParentClass>() && (newParent.model() == model())) {
  //  return true;
  //}
  return false;
}

// return any children objects in the hierarchy
std::vector<ModelObject> UtilityCost_Charge_Block_Impl::children() const
{
  std::vector<ModelObject> result;
  return result;
}

std::vector<IddObjectType> UtilityCost_Charge_Block_Impl::allowableChildTypes() const {
  IddObjectTypeVector result;
  return result;
}

// Get all output variable names that could be associated with this object.
const std::vector<std::string>& UtilityCost_Charge_Block_Impl::outputVariableNames() const
{
  static std::vector<std::string> result;
  if (result.empty()){
  }
  return result;
}

} // detail

// constructor
UtilityCost_Charge_Block::UtilityCost_Charge_Block(const Model& model)
  : ParentObject(UtilityCost_Charge_Block::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::UtilityCost_Charge_Block_Impl>());
}

// constructor
UtilityCost_Charge_Block::UtilityCost_Charge_Block(std::shared_ptr<detail::UtilityCost_Charge_Block_Impl> impl)
  : ParentObject(std::move(impl))
{}

OptionalString UtilityCost_Charge_Block::tariffName() const
{
  return getImpl<detail::UtilityCost_Charge_Block_Impl>()->tariffName();
}

void UtilityCost_Charge_Block::setTariffName(const std::string& str)
{
  getImpl<detail::UtilityCost_Charge_Block_Impl>()->setTariffName(str);
}

OptionalString UtilityCost_Charge_Block::sourceVariable() const
{
  return getImpl<detail::UtilityCost_Charge_Block_Impl>()->sourceVariable();
}

void UtilityCost_Charge_Block::setSourceVariable(const std::string& str)
{
  getImpl<detail::UtilityCost_Charge_Block_Impl>()->setSourceVariable(str);
}

OptionalString UtilityCost_Charge_Block::season() const
{
  return getImpl<detail::UtilityCost_Charge_Block_Impl>()->season();
}

void UtilityCost_Charge_Block::setSeason(const std::string& str)
{
  getImpl<detail::UtilityCost_Charge_Block_Impl>()->setSeason(str);
}

OptionalString UtilityCost_Charge_Block::categoryVariableName() const
{
  return getImpl<detail::UtilityCost_Charge_Block_Impl>()->categoryVariableName();
}

void UtilityCost_Charge_Block::setCategoryVariableName(const std::string& str)
{
  getImpl<detail::UtilityCost_Charge_Block_Impl>()->setCategoryVariableName(str);
}

OptionalString UtilityCost_Charge_Block::remainingIntoVariable() const
{
  return getImpl<detail::UtilityCost_Charge_Block_Impl>()->remainingIntoVariable();
}

void UtilityCost_Charge_Block::setRemainingIntoVariable(const std::string& str)
{
  getImpl<detail::UtilityCost_Charge_Block_Impl>()->setRemainingIntoVariable(str);
}

boost::optional<std::string> UtilityCost_Charge_Block::blockSizeValueOrVariableName(
    unsigned index) const
{
  return getImpl<detail::UtilityCost_Charge_Block_Impl>()->blockSizeValueOrVariableName(index);
}

bool UtilityCost_Charge_Block::setBlockSizeValueOrVariableName(
    unsigned index, const std::string& str)
{
  return getImpl<detail::UtilityCost_Charge_Block_Impl>()->setBlockSizeValueOrVariableName(index,str);
}

boost::optional<std::string> UtilityCost_Charge_Block::blockCostPerUnitValueOrVariableName(
    unsigned index) const
{
  return getImpl<detail::UtilityCost_Charge_Block_Impl>()->blockCostPerUnitValueOrVariableName(index);
}

bool UtilityCost_Charge_Block::setBlockCostPerUnitValueOrVariableName(
    unsigned index, const std::string& str)
{
  return getImpl<detail::UtilityCost_Charge_Block_Impl>()->setBlockCostPerUnitValueOrVariableName(index,str);
}

unsigned UtilityCost_Charge_Block::numBlocks() const {
  return getImpl<detail::UtilityCost_Charge_Block_Impl>()->numBlocks();
}

unsigned UtilityCost_Charge_Block::maxNumBlocks() const {
  return getImpl<detail::UtilityCost_Charge_Block_Impl>()->maxNumBlocks();
}

IddObjectType UtilityCost_Charge_Block::iddObjectType()
{
  IddObjectType result(IddObjectType::OS_UtilityCost_Charge_Block);
  return result;
}

} // model
} // openstudio
