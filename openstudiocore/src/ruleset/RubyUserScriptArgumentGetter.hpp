/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef RULESET_RUBYUSERSCRIPTARGUMENTGETTER_HPP
#define RULESET_RUBYUSERSCRIPTARGUMENTGETTER_HPP

#include "RulesetAPI.hpp"

#include "../utilities/core/Optional.hpp"

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
 *  ruby/openstudio/ruleset/test/RubyUserScriptArgumentGetter_Test.rb for an example. */
class RULESET_API RubyUserScriptArgumentGetter {
 public:
  virtual ~RubyUserScriptArgumentGetter() {}

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
