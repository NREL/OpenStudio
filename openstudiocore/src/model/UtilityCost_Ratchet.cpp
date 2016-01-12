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
  : ParentObject(impl)
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
