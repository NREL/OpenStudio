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

#include "CurrencyType.hpp"
#include "CurrencyType_Impl.hpp"

#include "Model_Impl.hpp"

#include <utilities/idd/OS_CurrencyType_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <boost/algorithm/string.hpp>

namespace openstudio {
namespace model  {
namespace detail {

  CurrencyType_Impl::CurrencyType_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ParentObject_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CurrencyType::iddObjectType());
  }

  CurrencyType_Impl::CurrencyType_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
                                         : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CurrencyType::iddObjectType());
  }

  CurrencyType_Impl::CurrencyType_Impl(const CurrencyType_Impl& other,Model_Impl* model,bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
  }

  OptionalString  CurrencyType_Impl::monetaryUnit() const
  {
    return getString(OS_CurrencyTypeFields::MonetaryUnit);
  }

  void CurrencyType_Impl::setMonetaryUnit(const std::string& str)
  {
    setString(OS_CurrencyTypeFields::MonetaryUnit,str);
  }

  // return the parent object in the hierarchy
  boost::optional<ParentObject> CurrencyType_Impl::parent() const
  {
    return boost::optional<ParentObject>();
  }

  // set the parent, child may have to call methods on the parent
  bool CurrencyType_Impl::setParent(ParentObject& newParent)
  {
    //if (newParent.optionalCast<SomeParentClass>() && (newParent.model() == model())) {
    //  return true;
    //}
    return false;
  }

  // return any children objects in the hierarchy
  std::vector<ModelObject> CurrencyType_Impl::children() const
  {
    std::vector<ModelObject> result;
    return result;
  }

  std::vector<IddObjectType> CurrencyType_Impl::allowableChildTypes() const {
    IddObjectTypeVector result;
    return result;
  }

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& CurrencyType_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CurrencyType_Impl::iddObjectType() const {
    return CurrencyType::iddObjectType();
  }

} // detail

/// constructor
CurrencyType::CurrencyType(const Model& model)
  : ParentObject(CurrencyType::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CurrencyType_Impl>());
}

// constructor
CurrencyType::CurrencyType(std::shared_ptr<detail::CurrencyType_Impl> impl)
  : ParentObject(impl)
{}

//int CurrencyType::someInt() const
//{
//  return getImpl<detail::CurrencyType_Impl>()->someInt();
//}
//
//void CurrencyType::setSomeInt(int someInt)
//{
//  getImpl<detail::CurrencyType_Impl>()->setSomeInt(someInt);
//}
//
//std::string CurrencyType::someString() const
//{
//  return getImpl<detail::CurrencyType_Impl>()->someString();
//}
//
//void CurrencyType::setSomeString(const std::string& someString)
//{
//  getImpl<detail::CurrencyType_Impl>()->setSomeString(someString);
//}

IddObjectType CurrencyType::iddObjectType()
{
  IddObjectType result(IddObjectType::OS_CurrencyType);
  return result;
}

} // model
} // openstudio
