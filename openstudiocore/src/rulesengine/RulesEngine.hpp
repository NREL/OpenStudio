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

#ifndef RULESENGINE_RULESENGINE_HPP
#define RULESENGINE_RULESENGINE_HPP

#include <rulesengine/RulesEngineAPI.hpp>
#include <clips/clipscpp.h>
#include <rulesengine/ApplyRulesetResult.hpp>

#include <ruleset/StandardsRuleset.hpp>

#include <standardsinterface/AgnosticInterface.hpp>
#include <standardsinterface/DataDictionary.hpp>

#include <utilities/document/Document.hpp>
#include <utilities/document/Text.hpp>

#include <utilities/core/UUID.hpp>

namespace openstudio {
namespace rulesengine {

/** RulesEngine harnesses a public domain, third party, expert systems library, 
 *  CLIPS (http://clipsrules.sourceforge.net/) to apply standards rulesets to building models
 *  wrapped in an AgnosticInterface. RulesEngine is an abstract class that must be implemented 
 *  by a derived class that manages its own building models wrapped in an instatiation of the 
 *  AgnosticInterface. OpenStudio provides once such concrete RulesEngine class: 
 *  OpenStudioRulesEngine. */
class RULESENGINE_API RulesEngine {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~RulesEngine() {}
  
  //@}
  /** @name Getters */
  //@{

  /** Returns the current UUID. A new UUID is created for each rules engine run. */
  UUID currentUuid() const;

  /** Returns the current result. A new result object is constructed for each rules engine run. 
   *  Old results are not archived by this class. */
  ApplyRulesetResult result() const;

  /** Returns the ruleset used by the last run. */
  ruleset::StandardsRuleset ruleset() const;

  /** Prints the internal CLIPS constructs of the RulesEngine. Primarily used for debugging. */
  std::string printInternalConstructs(
      const std::vector<ruleset::StandardsRulesetOption>& options) const;

  //@}

  /// @cond
  /** @name Callback Methods */
  //@{

  void setOverallResultFlag(const std::string& flag);

  void addToReport(const std::string& sectionName, const std::string& text);

  virtual std::string cloneModel(const std::string& id) = 0;

  /** modelId is guaranteed to be non-empty string. */
  virtual std::vector<std::string> getObjectIds(const std::string& modelId,
                                                const std::string& objectType) = 0;

  template <typename T>
  standardsinterface::Result<typename std::vector<T> > 
  getObjects(typename standardsinterface::AgnosticInterface<T>& model,
             const std::string& ddObjectType)
  {
    return model.getObjects(ddObjectType);
  }

  virtual std::string getObjectName(const std::string& modelId,
                                    const std::string& objectId) = 0;

  virtual std::string checkAttribute(const std::string& modelId,
                                     const std::string& objectType,
                                     const std::string& objectId,
                                     const standardsinterface::PartsOfSpeech& attributeType,
                                     const std::string& attributeName) = 0;

  template <typename T>
  std::string checkAttribute(const std::string& modelId,
                             typename standardsinterface::AgnosticInterface<T>& model,
                             const std::string& ddObjectType,
                             const std::string& objectId,
                             const T& object,
                             const standardsinterface::PartsOfSpeech& ddAttributeType,
                             const std::string& ddAttributeName)
  {
    std::string result;
    std::string mapKey = mf_mapKey(modelId,objectId,ddAttributeName);

    standardsinterface::BoolResult br;
    standardsinterface::DoubleResult dr;
    standardsinterface::StringResult sr;
    standardsinterface::IntegerResult ir;

    switch (ddAttributeType.value()) {
      case (standardsinterface::PartsOfSpeech::Boolean) :
        br = model.booleanAttribute(ddObjectType,object,ddAttributeName);
        if (br) { 
          m_booleanAttributeMap.insert(std::map<std::string,bool>::value_type(mapKey,*br)); 
        }
        else { result = mf_handleAttributeCodeAndMessage(br.code(),br.what()); }
       break;
      case (standardsinterface::PartsOfSpeech::Double) :
        dr = model.doubleAttribute(ddObjectType,object,ddAttributeName);
        if (dr) { 
          m_doubleAttributeMap.insert(std::map<std::string,double>::value_type(mapKey,*dr)); 
        }
        else { result = mf_handleAttributeCodeAndMessage(dr.code(),dr.what()); }
       break;
      case (standardsinterface::PartsOfSpeech::Enum) :
        sr = model.enumAttribute(ddObjectType,object,ddAttributeName);
        if (sr) {
          m_enumAttributeMap.insert(std::map<std::string,std::string>::value_type(mapKey,*sr));
        }
        else { result = mf_handleAttributeCodeAndMessage(sr.code(),sr.what()); }
       break;
      case (standardsinterface::PartsOfSpeech::Integer) :
        ir = model.integerAttribute(ddObjectType,object,ddAttributeName);
        if (ir) {
          m_integerAttributeMap.insert(std::map<std::string,int>::value_type(mapKey,*ir));
        }
        else { result = mf_handleAttributeCodeAndMessage(ir.code(),ir.what()); }
       break;
      default:
       BOOST_ASSERT(false);
    };
    return result;
  }

  bool getBooleanAttribute(const std::string& modelId,
                           const std::string& objectId,
                           const std::string& attributeName);

  double getDoubleAttribute(const std::string& modelId,
                            const std::string& objectId,
                            const std::string& attributeName);

  std::string getEnumAttribute(const std::string& modelId,
                               const std::string& objectId,
                               const std::string& attributeName);

  int getIntegerAttribute(const std::string& modelId,
                          const std::string& objectId,
                          const std::string& attributeName);

  //@}
  /// @endcond
 protected:
  /// @cond

  RulesEngine();

  /** Logs msg to Logger and m_result. */
  void mf_handleErrorMessage(const std::string& msg);  

  template <typename T>
  void mf_applyRules(const ruleset::StandardsRuleset& ruleset,
                     const std::vector<ruleset::StandardsRulesetOption>& options,
                     typename standardsinterface::AgnosticInterface<T>& model) 
  {
    mf_registerOriginalModel(model);
    mf_initializeRun(ruleset,options);
    mf_completeRun();
  }

  /// @endcond
 private:
  UUID m_currentUuid;
  CLIPS::CLIPSCPPEnv m_clipsEnvironment;
  ruleset::StandardsRuleset m_ruleset;
  std::string m_originalModelName;
  ApplyRulesetResult m_result;

  template <typename T>
  void mf_registerOriginalModel(typename standardsinterface::AgnosticInterface<T>& model) {
    std::string name = model.name();
    if (name.empty()) {
      std::string candidate = "Original Model";
      standardsinterface::VoidResult ok = model.setName(candidate);
      if (ok) { name = candidate; }
    }
    m_originalModelName = name;
  }

  /** Establish templates, load facts, load rules. */
  void mf_initializeRun(const ruleset::StandardsRuleset& ruleset,const std::vector<ruleset::StandardsRulesetOption>& options);

  void mf_buildRulesetOptions();
  void mf_buildDeftemplates();
  void mf_buildDefrules(const std::vector<ruleset::StandardsRulesetOption>& options);
  void mf_buildRulesetRules();
  void mf_buildDeffacts(const std::vector<ruleset::StandardsRulesetOption>& options);

  std::string mf_printDeftemplates() const;
  std::string mf_printDefrules(const std::vector<ruleset::StandardsRulesetOption>& options) const;
  std::string mf_printDeffacts(const std::vector<ruleset::StandardsRulesetOption>& options) const;

  std::string mf_printDeftemplateApplyRulesetResult() const;
  std::string mf_printDeftemplateSimulationRequired() const;
  std::string mf_printDeftemplateSimulationComplete() const;

  std::string mf_printDeftemplateAvailableModel() const;
  std::string mf_printDeftemplateGetObjects() const;
  std::string mf_printDeftemplateAvailableObjects() const;
  std::string mf_printDeftemplateAvailableObject() const;

  std::string mf_printDeftemplateGetAttribute() const;
  std::string mf_printDeftemplateBooleanAttribute() const;
  std::string mf_printDeftemplateIntegerAttribute() const;
  std::string mf_printDeftemplateDoubleAttribute() const;
  std::string mf_printDeftemplateEnumAttribute() const;

  std::string mf_printDefrule(const ruleset::StandardsRulesetOption& option) const;
  
  std::string mf_printDefruleBasicChangePhase() const;
  std::string mf_printDefruleChangePhaseToSimulation() const;

  std::string mf_printDefruleGetModelIdsForObjects() const;
  std::string mf_printDefruleGetObjects() const;
  std::string mf_printDefruleBreakUpAvailableObjects() const;

  std::string mf_printDefruleCheckAttribute() const;
  std::string mf_printDefruleQueueSimulation() const;
  std::string mf_printDefruleCheckAttributeAfterSimulation() const;
  std::string mf_printDefruleUpdateApplyRulesetResult() const;
  std::string mf_printDefruleGetBooleanAttribute() const;
  std::string mf_printDefruleGetDoubleAttribute() const;
  std::string mf_printDefruleGetEnumAttribute() const;
  std::string mf_printDefruleGetIntegerAttribute() const;

  std::string mf_printDefruleReportApplyRulesetResult() const;

  /** Run the RulesEngine. */
  void mf_completeRun();

  // Maps to efficiently handle two-step attribute getting process
  std::map<std::string,bool> m_booleanAttributeMap;
  std::map<std::string,double> m_doubleAttributeMap;
  std::map<std::string,std::string> m_enumAttributeMap;
  std::map<std::string,int> m_integerAttributeMap;

  /** Creates key for any of the above maps. */
  std::string mf_mapKey(const std::string& modelId,
                        const std::string& objectId,
                        const std::string& ddAttributeName);

  /** Returns "SimulationRequired" or an error message. If the latter, also pushes error
   *  message onto the report. */
  std::string mf_handleAttributeCodeAndMessage(standardsinterface::result::Code code,
                                               const std::string& msg);

  REGISTER_LOGGER("rulesengine.RulesEngine");
};


} // rulesengine
} // openstudio

#endif // RULESENGINE_RULESENGINE_HPP
