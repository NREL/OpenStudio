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

#include "SequentialSearchOptions.hpp"
#include "SequentialSearchOptions_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Json.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  SequentialSearchOptions_Impl::SequentialSearchOptions_Impl(int objectiveToMinimizeFirst)
    : AlgorithmOptions_Impl()
  {
    saveOption(Attribute("objectiveToMinimizeFirst",objectiveToMinimizeFirst));
  }

  SequentialSearchOptions_Impl::SequentialSearchOptions_Impl(const std::vector<Attribute>& options)
    : AlgorithmOptions_Impl(options)
  {}

  AlgorithmOptions SequentialSearchOptions_Impl::clone() const {
    std::shared_ptr<SequentialSearchOptions_Impl> impl(new SequentialSearchOptions_Impl(*this));
    return SequentialSearchOptions(impl);
  }

  int SequentialSearchOptions_Impl::objectiveToMinimizeFirst() const {
    OptionalAttribute option = getOption("objectiveToMinimizeFirst");
    OS_ASSERT(option);
    return option->valueAsInteger();
  }

  SequentialSearchOptions SequentialSearchOptions_Impl::fromVariant(const QVariant& variant,
                                                                const VersionString& version)
  {
    QVariantMap map = variant.toMap();
    AttributeVector attributes = deserializeUnorderedVector(
          map["attributes"].toList(),
          std::function<Attribute (const QVariant&)>(std::bind(openstudio::detail::toAttribute,std::placeholders::_1,version)));
    return SequentialSearchOptions(attributes);
  }

} // detail

SequentialSearchOptions::SequentialSearchOptions(int objectiveToMinimizeFirst)
  : AlgorithmOptions(std::shared_ptr<detail::SequentialSearchOptions_Impl>(
        new detail::SequentialSearchOptions_Impl(objectiveToMinimizeFirst)))
{}

SequentialSearchOptions::SequentialSearchOptions(const std::vector<Attribute>& options)
  : AlgorithmOptions(std::shared_ptr<detail::SequentialSearchOptions_Impl>(
        new detail::SequentialSearchOptions_Impl(options)))
{}

int SequentialSearchOptions::objectiveToMinimizeFirst() const {
  return getImpl<detail::SequentialSearchOptions_Impl>()->objectiveToMinimizeFirst();
}

/// @cond
SequentialSearchOptions::SequentialSearchOptions(std::shared_ptr<detail::SequentialSearchOptions_Impl> impl)
  : AlgorithmOptions(impl)
{}
/// @endcond

} // analysis
} // openstudio

