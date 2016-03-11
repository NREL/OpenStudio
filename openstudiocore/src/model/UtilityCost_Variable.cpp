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

void UtilityCost_Variable_Impl::setJanuaryValue(double num)
{
  setDouble(OS_UtilityCost_VariableFields::JanuaryValue,num);
}

OptionalDouble UtilityCost_Variable_Impl::februaryValue() const
{
  return getDouble(OS_UtilityCost_VariableFields::FebruaryValue);
}

void UtilityCost_Variable_Impl::setFebruaryValue(double num)
{
  setDouble(OS_UtilityCost_VariableFields::FebruaryValue,num);
}

OptionalDouble UtilityCost_Variable_Impl::marchValue() const
{
  return getDouble(OS_UtilityCost_VariableFields::MarchValue);
}

void UtilityCost_Variable_Impl::setMarchValue(double num)
{
  setDouble(OS_UtilityCost_VariableFields::MarchValue,num);
}

OptionalDouble UtilityCost_Variable_Impl::aprilValue() const
{
  return getDouble(OS_UtilityCost_VariableFields::AprilValue);
}

void UtilityCost_Variable_Impl::setAprilValue(double num)
{
  setDouble(OS_UtilityCost_VariableFields::AprilValue,num);
}

OptionalDouble UtilityCost_Variable_Impl::mayValue() const
{
  return getDouble(OS_UtilityCost_VariableFields::MayValue);
}

void UtilityCost_Variable_Impl::setMayValue(double num)
{
  setDouble(OS_UtilityCost_VariableFields::MayValue,num);
}

OptionalDouble UtilityCost_Variable_Impl::juneValue() const
{
  return getDouble(OS_UtilityCost_VariableFields::JuneValue);
}

void UtilityCost_Variable_Impl::setJuneValue(double num)
{
  setDouble(OS_UtilityCost_VariableFields::JuneValue,num);
}

OptionalDouble UtilityCost_Variable_Impl::julyValue() const
{
  return getDouble(OS_UtilityCost_VariableFields::JulyValue);
}

void UtilityCost_Variable_Impl::setJulyValue(double num)
{
  setDouble(OS_UtilityCost_VariableFields::JulyValue,num);
}

OptionalDouble UtilityCost_Variable_Impl::augustValue() const
{
  return getDouble(OS_UtilityCost_VariableFields::AugustValue);
}

void UtilityCost_Variable_Impl::setAugustValue(double num)
{
  setDouble(OS_UtilityCost_VariableFields::AugustValue,num);
}

OptionalDouble UtilityCost_Variable_Impl::septemberValue() const
{
  return getDouble(OS_UtilityCost_VariableFields::SeptemberValue);
}

void UtilityCost_Variable_Impl::setSeptemberValue(double num)
{
  setDouble(OS_UtilityCost_VariableFields::SeptemberValue,num);
}

OptionalDouble UtilityCost_Variable_Impl::octoberValue() const
{
  return getDouble(OS_UtilityCost_VariableFields::OctoberValue);
}

void UtilityCost_Variable_Impl::setOctoberValue(double num)
{
  setDouble(OS_UtilityCost_VariableFields::OctoberValue,num);
}

OptionalDouble UtilityCost_Variable_Impl::novemberValue() const
{
  return getDouble(OS_UtilityCost_VariableFields::NovemberValue);
}

void UtilityCost_Variable_Impl::setNovemberValue(double num)
{
  setDouble(OS_UtilityCost_VariableFields::NovemberValue,num);
}

OptionalDouble UtilityCost_Variable_Impl::decemberValue() const
{
  return getDouble(OS_UtilityCost_VariableFields::DecemberValue);
}

void UtilityCost_Variable_Impl::setDecemberValue(double num)
{
  setDouble(OS_UtilityCost_VariableFields::DecemberValue,num);
}

OptionalString UtilityCost_Variable_Impl::tariffName() const
{
  return getString(OS_UtilityCost_VariableFields::TariffName);
}

void UtilityCost_Variable_Impl::setTariffName(const std::string& str)
{
  setString(OS_UtilityCost_VariableFields::TariffName,str);
}

OptionalString UtilityCost_Variable_Impl::variableType() const
{
  return getString(OS_UtilityCost_VariableFields::VariableType);
}

void UtilityCost_Variable_Impl::setVariableType(const std::string& str)
{
  setString(OS_UtilityCost_VariableFields::VariableType,str);
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
  : ParentObject(impl)
{}

OptionalDouble UtilityCost_Variable::januaryValue() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->januaryValue();
}

void UtilityCost_Variable::setJanuaryValue(double num)
{
  getImpl<detail::UtilityCost_Variable_Impl>()->setJanuaryValue(num);
}

OptionalDouble UtilityCost_Variable::februaryValue() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->februaryValue();
}

void UtilityCost_Variable::setFebruaryValue(double num)
{
  getImpl<detail::UtilityCost_Variable_Impl>()->setFebruaryValue(num);
}

OptionalDouble UtilityCost_Variable::marchValue() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->marchValue();
}

void UtilityCost_Variable::setMarchValue(double num)
{
  getImpl<detail::UtilityCost_Variable_Impl>()->setMarchValue(num);
}

OptionalDouble UtilityCost_Variable::aprilValue() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->aprilValue();
}

void UtilityCost_Variable::setAprilValue(double num)
{
  getImpl<detail::UtilityCost_Variable_Impl>()->setAprilValue(num);
}

OptionalDouble UtilityCost_Variable::mayValue() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->mayValue();
}

void UtilityCost_Variable::setMayValue(double num)
{
  getImpl<detail::UtilityCost_Variable_Impl>()->setMayValue(num);
}

OptionalDouble UtilityCost_Variable::juneValue() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->juneValue();
}

void UtilityCost_Variable::setJuneValue(double num)
{
  getImpl<detail::UtilityCost_Variable_Impl>()->setJuneValue(num);
}

OptionalDouble UtilityCost_Variable::julyValue() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->julyValue();
}

void UtilityCost_Variable::setJulyValue(double num)
{
  getImpl<detail::UtilityCost_Variable_Impl>()->setJulyValue(num);
}

OptionalDouble UtilityCost_Variable::augustValue() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->augustValue();
}

void UtilityCost_Variable::setAugustValue(double num)
{
  getImpl<detail::UtilityCost_Variable_Impl>()->setAugustValue(num);
}

OptionalDouble UtilityCost_Variable::septemberValue() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->septemberValue();
}

void UtilityCost_Variable::setSeptemberValue(double num)
{
  getImpl<detail::UtilityCost_Variable_Impl>()->setSeptemberValue(num);
}

OptionalDouble UtilityCost_Variable::octoberValue() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->octoberValue();
}

void UtilityCost_Variable::setOctoberValue(double num)
{
  getImpl<detail::UtilityCost_Variable_Impl>()->setOctoberValue(num);
}

OptionalDouble UtilityCost_Variable::novemberValue() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->novemberValue();
}

void UtilityCost_Variable::setNovemberValue(double num)
{
  getImpl<detail::UtilityCost_Variable_Impl>()->setNovemberValue(num);
}

OptionalDouble UtilityCost_Variable::decemberValue() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->decemberValue();
}

void UtilityCost_Variable::setDecemberValue(double num)
{
  getImpl<detail::UtilityCost_Variable_Impl>()->setDecemberValue(num);
}

OptionalString UtilityCost_Variable::name() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->name();
}

void UtilityCost_Variable::setName(const std::string& str)
{
  getImpl<detail::UtilityCost_Variable_Impl>()->setName(str);
}

OptionalString UtilityCost_Variable::tariffName() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->tariffName();
}

void UtilityCost_Variable::setTariffName(const std::string& str)
{
  getImpl<detail::UtilityCost_Variable_Impl>()->setTariffName(str);
}

OptionalString UtilityCost_Variable::variableType() const
{
  return getImpl<detail::UtilityCost_Variable_Impl>()->variableType();
}

void UtilityCost_Variable::setVariableType(const std::string& str)
{
  getImpl<detail::UtilityCost_Variable_Impl>()->setVariableType(str);
}

IddObjectType UtilityCost_Variable::iddObjectType()
{
  IddObjectType result(IddObjectType::OS_UtilityCost_Variable);
  return result;
}

} // model
} // openstudio
