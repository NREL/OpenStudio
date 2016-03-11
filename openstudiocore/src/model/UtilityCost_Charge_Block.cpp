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
  : ParentObject(impl)
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
