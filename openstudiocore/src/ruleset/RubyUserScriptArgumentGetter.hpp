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

#ifndef RULESET_RUBYUSERSCRIPTARGUMENTGETTER_HPP
#define RULESET_RUBYUSERSCRIPTARGUMENTGETTER_HPP

#include "RulesetAPI.hpp"

#include "../utilities/core/Optional.hpp"
#include "../utilities/core/Deprecated.hpp"

#include <vector>
#include <string>

namespace openstudio {

class BCLMeasure;
class Workspace;

namespace model {
  class Model;
}

namespace ruleset {

class OSArgument;

/** Interface class for using embedded Ruby to extract arguments from a Ruby measure (UserScript).
 *  See the documentation for detail::RubyUserScriptArgumentGetter_Impl and 
 *  src/ruleset/test/EmbeddedRuby_GTest.cpp for this use case. If your application is written in 
 *  Ruby, this functionality is available through non-member helper function 
 *  OpenStudio::Ruleset::getArguments(BCLMeasure), see 
 *  ruby/openstudio/ruleset/test/RubyUserScriptArgumentGetter_Test.rb for an example. 
 *  \deprecated in preference to RubyUserScriptInfoGetter */
class RULESET_API RubyUserScriptArgumentGetter {
 public:
  virtual ~RubyUserScriptArgumentGetter();

  virtual std::vector<OSArgument> getArguments(const BCLMeasure& measure)=0;

  /** Use this method to pass in a (EnergyPlus) Workspace or a (OpenStudio) Model. */
  virtual std::vector<OSArgument> getArguments(const BCLMeasure& measure, const Workspace& workspace)=0;

  virtual std::vector<OSArgument> getArguments(const BCLMeasure& measure,
                                               const model::Model& model,
                                               const Workspace& workspace)=0;

  std::vector<OSArgument> getArguments(const BCLMeasure& measure,
                                       const boost::optional<model::Model>& model,
                                       const boost::optional<Workspace>& workspace);
};

/** Non-member function that returns Ruby code text for retrieving arguments from a BCLMeasure. This
 *  function is used by both the embedded Ruby (RubyScriptArgumentGetter) and native Ruby 
 *  (OpenStudio::Ruleset::getArguments) implementations of the functionality, such that either usage
 *  pathway should produce identical results. \relates RubyUserScriptArgumentGetter */
RULESET_API std::string argumentExtractorRubyFunction();

} // ruleset
} // openstudio

#endif // RULESET_RUBYUSERSCRIPTARGUMENTGETTER_HPP
