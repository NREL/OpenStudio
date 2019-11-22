/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include <vector>
#include <string>
#include "PerformancePrecisionTradeoffs.hpp"
#include "PerformancePrecisionTradeoffs_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_PerformancePrecisionTradeoffs_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  PerformancePrecisionTradeoffs_Impl::PerformancePrecisionTradeoffs_Impl(const IdfObject& idfObject,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == PerformancePrecisionTradeoffs::iddObjectType());
  }

  PerformancePrecisionTradeoffs_Impl::PerformancePrecisionTradeoffs_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == PerformancePrecisionTradeoffs::iddObjectType());
  }

  PerformancePrecisionTradeoffs_Impl::PerformancePrecisionTradeoffs_Impl(const PerformancePrecisionTradeoffs_Impl& other,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& PerformancePrecisionTradeoffs_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    return result;
  }

  IddObjectType PerformancePrecisionTradeoffs_Impl::iddObjectType() const {
    return PerformancePrecisionTradeoffs::iddObjectType();
  }

  bool PerformancePrecisionTradeoffs_Impl::useCoilDirectSolutions() const {
    boost::optional<std::string> value = getString(OS_PerformancePrecisionTradeoffsFields::UseCoilDirectSolutions,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }
  
  bool PerformancePrecisionTradeoffs_Impl::isUseCoilDirectSolutionsDefaulted() const {
    return isEmpty(OS_PerformancePrecisionTradeoffsFields::UseCoilDirectSolutions);
  }
  
  bool PerformancePrecisionTradeoffs_Impl::setUseCoilDirectSolutions(bool useCoilDirectSolutions) {
    bool result = false;
    if (useCoilDirectSolutions) {
      result = setString(OS_PerformancePrecisionTradeoffsFields::UseCoilDirectSolutions, "Yes");
    } else {
      result = setString(OS_PerformancePrecisionTradeoffsFields::UseCoilDirectSolutions, "No");
    }
    OS_ASSERT(result);
    return result;
  }

  void PerformancePrecisionTradeoffs_Impl::resetUseCoilDirectSolutions() {
    bool result = setString(OS_PerformancePrecisionTradeoffsFields::UseCoilDirectSolutions, "");
    OS_ASSERT(result);
  }

} // detail

IddObjectType PerformancePrecisionTradeoffs::iddObjectType() {
  return IddObjectType(IddObjectType::OS_PerformancePrecisionTradeoffs);
}

bool PerformancePrecisionTradeoffs::useCoilDirectSolutions() const {
  return getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->useCoilDirectSolutions();
}

bool PerformancePrecisionTradeoffs::isUseCoilDirectSolutionsDefaulted() const {
  return getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->isUseCoilDirectSolutionsDefaulted();
}

bool PerformancePrecisionTradeoffs::setUseCoilDirectSolutions(bool useCoilDirectSolutions) {
  return getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->setUseCoilDirectSolutions(useCoilDirectSolutions);
}

void PerformancePrecisionTradeoffs::resetUseCoilDirectSolutions() {
  getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->resetUseCoilDirectSolutions();
}

/// @cond
PerformancePrecisionTradeoffs::PerformancePrecisionTradeoffs(std::shared_ptr<detail::PerformancePrecisionTradeoffs_Impl> impl)
  : ModelObject(std::move(impl))
{}

PerformancePrecisionTradeoffs::PerformancePrecisionTradeoffs(Model& model)
  : ModelObject(PerformancePrecisionTradeoffs::iddObjectType(), model)
{}

/// @endcond

} // model
} // openstudio