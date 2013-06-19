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

#ifndef RULESENGINE_OPENSTUDIORULESENGINE_HPP
#define RULESENGINE_OPENSTUDIORULESENGINE_HPP

#include <rulesengine/RulesEngineAPI.hpp>
#include <rulesengine/RulesEngine.hpp>

#include <standardsinterface/OpenStudioStandardsInterface.hpp>

#include <project/ProjectDatabase.hpp>

#include <utilities/core/Logger.hpp>

namespace openstudio {
namespace rulesengine {

/** Concrete instantiation of base class RulesEngine. Manages OpenStudioStandardsInterface models
 *  and objects. Provides driver method applyRules. Usage:
 *
 *  \code
 *  OpenStudioRulesEngine engine;
 *  standardsinterface::OpenStudioStandardsInterface wrappedModel(model);
 *  engine.applyRules(ruleset,myOptions,model);
 *  ApplyRulesetResult result = engine.result();
 *  BOOST_ASSERT(result.runComplete());
 *  ruleset::StandardsRulesetResult flag = result.overallResultFlag();
 *  Document report = result.report(); // can print this as html
 *  \endcode
 */
class RULESENGINE_API OpenStudioRulesEngine : public RulesEngine {
 public:
  /** @name Constructors and Destructors */
  //@{

  OpenStudioRulesEngine();

  virtual ~OpenStudioRulesEngine() {}

  //@}
  /** @name Start a Run */
  //@{
  
  /** Drives a run of RulesEngine. Creates a new RulesEngine UUID and a new ApplyRulesetResult, 
   *  discarding any previous data. Also discards previous models. After running, numModels() 
   *  should be greater than zero. */
  void applyRules(const ruleset::StandardsRuleset& ruleset,
                  const std::vector<ruleset::StandardsRulesetOption>& options,
                  standardsinterface::OpenStudioStandardsInterface& model,
                  const boost::optional<project::ProjectDatabase>& componentLibrary=boost::none);

  //@}

  /// @cond
  /** @name Callback Methods */
  //@{

  virtual std::string cloneModel(const std::string& id);

  virtual std::vector<std::string> getObjectIds(const std::string& modelId,
                                                const std::string& objectType);

  virtual std::string getObjectName(const std::string& modelId,
                                    const std::string& objectId);

  virtual std::string checkAttribute(const std::string& modelId,
                                     const std::string& objectType,
                                     const std::string& objectId,
                                     const standardsinterface::PartsOfSpeech& attributeType,
                                     const std::string& attributeName);

  // Note: Not sure if this is the best way? Should both of these be public or should one be protected and named differently so this
  // isn't necessary?
  using RulesEngine::checkAttribute;

  //@}
  /// @endcond

  /** @name Queries */
  //@{

  /** The number of models currently stored in the object. Should be zero immediately after 
   *  construction, and greater than zero after a call to applyRules. */
  unsigned numModels() const;

  //@}
 private:
  boost::optional<project::ProjectDatabase> m_componentLibrary;
  std::vector<standardsinterface::OpenStudioStandardsInterface> m_models; // original will always be at position 0

  REGISTER_LOGGER("rulesengine.OpenStudioRulesEngine");
};



} // rulesengine
} // openstudio

#endif // RULESENGINE_OPENSTUDIORULESENGINE_HPP
