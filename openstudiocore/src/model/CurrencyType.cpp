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
  : ParentObject(std::move(impl))
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
