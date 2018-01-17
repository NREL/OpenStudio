/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "Variable.hpp"
#include "Variable_Impl.hpp"

#include "MeasureGroup.hpp"
#include "MeasureGroup_Impl.hpp"
#include "OutputAttributeVariable.hpp"
#include "OutputAttributeVariable_Impl.hpp"
#include "RubyContinuousVariable.hpp"
#include "RubyContinuousVariable_Impl.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  Variable_Impl::Variable_Impl(const std::string& name)
    : AnalysisObject_Impl(name)
  {}

  Variable_Impl::Variable_Impl(const UUID& uuid,
                               const UUID& versionUUID,
                               const std::string& name,
                               const std::string& displayName,
                               const std::string& description)
    : AnalysisObject_Impl(uuid,versionUUID,name,displayName,description)
  {}

  Variable_Impl::Variable_Impl(const Variable_Impl &other)
    : AnalysisObject_Impl(other)
  {}

  Variable Variable_Impl::factoryFromVariant(const QVariant& variant, const VersionString& version) {
    QVariantMap map = variant.toMap();

    std::string variableType;
    if (map.contains("variable_type")) {
      variableType = map["variable_type"].toString().toStdString();
    }
    else if (map.contains("workflow_step_type")) {
      variableType = map["workflow_step_type"].toString().toStdString();
    }
    else {
      LOG_AND_THROW("Unable to find Variable in expected location.");
    }

    if (variableType == "MeasureGroup") {
      return MeasureGroup_Impl::fromVariant(variant,version);
    }
    if (variableType == "OutputAttributeVariable") {
      return OutputAttributeVariable_Impl::fromVariant(variant,version);
    }
    if (variableType == "RubyContinuousVariable") {
      return RubyContinuousVariable_Impl::fromVariant(variant,version);
    }

    LOG_AND_THROW("Unexpected variable_type or workflow_step_type " << variableType << ".");
    return OptionalVariable().get();
  }

} // detail

double Variable::getValue(const DataPoint& dataPoint) const {
  return getImpl<detail::Variable_Impl>()->getValue(dataPoint);
}

/// @cond
Variable::Variable(std::shared_ptr<detail::Variable_Impl> impl)
  : AnalysisObject(impl)
{}
/// @endcond

} // analysis
} // openstudio

