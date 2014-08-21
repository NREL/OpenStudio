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

#ifndef RULESET_RUBYUSERSCRIPTINFOGETTER_HPP
#define RULESET_RUBYUSERSCRIPTINFOGETTER_HPP

#include "RulesetAPI.hpp"

#include "OSArgument.hpp"

#include "../utilities/core/Optional.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"

#include <vector>
#include <string>

namespace openstudio {

class Workspace;

namespace model {
  class Model;
}

namespace ruleset {

/** BCLMeasureInfo contains information that can be extracted from a by the Ruby parser.
 */
class RULESET_API RubyUserScriptInfo {
public:

  RubyUserScriptInfo(const MeasureType& measureType,
                     const std::string& className,
                     const std::string& name,
                     const std::string& description,
                     const std::string& modelerDescription,
                     const std::vector<OSArgument>& arguments);

  MeasureType measureType() const;
  std::string className() const;
  std::string name() const;
  std::string description() const;
  std::string modelerDescription() const;
  std::vector<OSArgument> arguments() const;

  /// Compares the measure's information from the xml with information extracted from the ruby measure
  /// Updates the measure if needed, returns true if an update was performed
  bool update(BCLMeasure& measure) const;

private:
  REGISTER_LOGGER("openstudio.ruleset.RubyUserScriptInfo");

  MeasureType m_measureType;
  std::string m_className;
  std::string m_name;
  std::string m_description;
  std::string m_modelerDescription;
  std::vector<OSArgument> m_arguments;
};

/** Interface class for using embedded Ruby to extract arguments from a Ruby measure (UserScript).
 *  See the documentation for detail::RubyUserScriptArgumentGetter_Impl and 
 *  src/ruleset/test/EmbeddedRuby_GTest.cpp for this use case. If your application is written in 
 *  Ruby, this functionality is available through non-member helper function 
 *  OpenStudio::Ruleset::getArguments(BCLMeasure), see 
 *  ruby/openstudio/ruleset/test/RubyUserScriptArgumentGetter_Test.rb for an example. */
class RULESET_API RubyUserScriptInfoGetter {
 public:
   virtual ~RubyUserScriptInfoGetter() {}

   virtual RubyUserScriptInfo getInfo(const BCLMeasure& measure) = 0;

   /** Use this method to pass in a (EnergyPlus) Workspace or a (OpenStudio) Model. */
   virtual RubyUserScriptInfo getInfo(const BCLMeasure& measure, const Workspace& workspace) = 0;

   virtual RubyUserScriptInfo getInfo(const BCLMeasure& measure,
                                      const model::Model& model,
                                      const Workspace& workspace) = 0;

   RubyUserScriptInfo getInfo(const BCLMeasure& measure,
                              const boost::optional<model::Model>& model,
                              const boost::optional<Workspace>& workspace);
};

/** Non-member function that returns Ruby code text for retrieving info from a BCLMeasure. This
 *  function is used by both the embedded Ruby (RubyScriptArgumentGetter) and native Ruby 
 *  (OpenStudio::Ruleset::getArguments) implementations of the functionality, such that either usage
 *  pathway should produce identical results. \relates RubyUserScriptInfoGetter */
RULESET_API std::string infoExtractorRubyFunction();

} // ruleset
} // openstudio

#endif // RULESET_RUBYUSERSCRIPTINFOGETTER_HPP
