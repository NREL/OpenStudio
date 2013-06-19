/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef RULESENGINE_APPLYRULESETRESULT_HPP
#define RULESENGINE_APPLYRULESETRESULT_HPP

#include <rulesengine/RulesEngineAPI.hpp>

#include <ruleset/StandardsRulesetEnums.hpp>

#include <utilities/document/Document.hpp>

#include <vector>
#include <string>

namespace openstudio {
namespace rulesengine {

class RulesEngine;

/** Contains information about a single rules engine run. */
class RULESENGINE_API ApplyRulesetResult {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Constructor sets the title of the report(). */
  ApplyRulesetResult(const std::string& reportTitle);

  //@}
  /** @name Getters */
  //@{

  /** Evaluates to false if the rules engine run is not yet complete. */
  bool runComplete() const;

  /** Returns the overall result of the rules engine run: Pass, Incomplete, Fail, or Error. 
   *  The listed order of the flags matters. For instance, if there is missing information and
   *  an error, Error will be returned (rather than Incomplete). */
  ruleset::StandardsRulesetResult overallResultFlag() const;

  /** Detailed run report. Document (see utilities documentation) may be printed to screen, html, 
   *  or LaTeX. */
  Document report() const;

  //@}
 protected:
  /// @cond
  friend class RulesEngine;

  /** @name Setters */
  //@{

  void setRunComplete();

  void setOverallResultFlag(const ruleset::StandardsRulesetResult& flag);

  void addToReport(const std::string& sectionName, const std::string& text);

  //@}
  /// @endcond
 private:
  bool m_runComplete;
  ruleset::StandardsRulesetResult m_overallResultFlag;
  Document m_report;
};

} // rulesengine
} // openstudio

#endif // RULESENGINE_APPLYRULESETRESULT_HPP
