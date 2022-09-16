/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "OutputConstructions.hpp"
#include "OutputConstructions_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Output_Constructions_FieldEnums.hxx>

#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    OutputConstructions_Impl::OutputConstructions_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == OutputConstructions::iddObjectType());
    }

    OutputConstructions_Impl::OutputConstructions_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == OutputConstructions::iddObjectType());
    }

    OutputConstructions_Impl::OutputConstructions_Impl(const OutputConstructions_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& OutputConstructions_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType OutputConstructions_Impl::iddObjectType() const {
      return OutputConstructions::iddObjectType();
    }

    std::string OutputConstructions_Impl::detailsType1() const {
      boost::optional<std::string> value = getString(OS_Output_ConstructionsFields::DetailsType1, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string OutputConstructions_Impl::detailsType2() const {
      boost::optional<std::string> value = getString(OS_Output_ConstructionsFields::DetailsType2, true);
      OS_ASSERT(value);
      return value.get();
    }
    
    bool OutputConstructions_Impl::setDetailsType1(std::string detailsType1) {
      bool result = setString(OS_Output_ConstructionsFields::DetailsType1, detailsType1);
      return result;
    }

    bool OutputConstructions_Impl::setDetailsType2(std::string detailsType2) {
      bool result = setString(OS_Output_ConstructionsFields::DetailsType2, detailsType2);
      return result;
    }

  }  // namespace detail

  IddObjectType OutputConstructions::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Output_Constructions);
  }

  std::vector<std::string> OutputConstructions::detailsType1Values() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Output_ConstructionsFields::DetailsType1);
  }

  std::vector<std::string> OutputConstructions::validDetailsType1Values() {
    return detailsType1Values();
  }

  std::vector<std::string> OutputConstructions::detailsType2Values() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Output_ConstructionsFields::DetailsType2);
  }

  std::vector<std::string> OutputConstructions::validDetailsType2Values() {
    return detailsType2Values();
  }

  std::string OutputConstructions::detailsType1() const {
    return getImpl<detail::OutputConstructions_Impl>()->detailsType1();
  }
  
  bool OutputConstructions::setDetailsType1(std::string detailsType1) {
    return getImpl<detail::OutputConstructions_Impl>()->setDetailsType1(detailsType1);
  }

  std::string OutputConstructions::detailsType2() const {
    return getImpl<detail::OutputConstructions_Impl>()->detailsType2();
  }
  
  bool OutputConstructions::setDetailsType2(std::string detailsType2) {
    return getImpl<detail::OutputConstructions_Impl>()->setDetailsType2(detailsType2);
  }

  /// @cond
  OutputConstructions::OutputConstructions(std::shared_ptr<detail::OutputConstructions_Impl> impl) : ModelObject(impl) {}
  OutputConstructions::OutputConstructions(Model& model) : ModelObject(OutputConstructions::iddObjectType(), model) {}

  /// @endcond

}  // namespace model
}  // namespace openstudio
