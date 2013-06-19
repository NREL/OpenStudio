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

#include <rulesengine/RulesEngine.hpp>

#include <ruleset/StandardsRulesetEnums.hpp>
#include <ruleset/StandardsRulesetOption.hpp>
#include <ruleset/StandardsRule.hpp>

#include <utilities/document/Section.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/UUID.hpp>
#include <utilities/core/Checksum.hpp>

#include <utilities/idd/IddEnums.hxx>

#include <boost/foreach.hpp>

namespace openstudio {
namespace rulesengine {

UUID RulesEngine::currentUuid() const {
  return m_currentUuid;
}

ApplyRulesetResult RulesEngine::result() const {
  return m_result;
}

ruleset::StandardsRuleset RulesEngine::ruleset() const {
  return m_ruleset;
}

std::string RulesEngine::printInternalConstructs(
    const std::vector<ruleset::StandardsRulesetOption>& options) const 
{
  std::stringstream ss;
  ss << "RulesEngine contains the following CLIPS constructs: "
     << std::endl << std::endl 
     << mf_printDeftemplates() 
     << mf_printDefrules(options) 
     << mf_printDeffacts(options);
  return ss.str();
}

/// @cond

void RulesEngine::setOverallResultFlag(const std::string& flag) {
  m_result.setOverallResultFlag(ruleset::StandardsRulesetResult(flag));
}

void RulesEngine::addToReport(const std::string& sectionName, const std::string& text) {
  m_result.addToReport(sectionName,text);
}

bool RulesEngine::getBooleanAttribute(const std::string& modelId,
                                      const std::string& objectId,
                                      const std::string& attributeName)
{
  std::string mapKey = mf_mapKey(modelId,objectId,attributeName);
  std::map<std::string,bool>::iterator it = m_booleanAttributeMap.find(mapKey);
  BOOST_ASSERT(it != m_booleanAttributeMap.end());
  return it->second;
}

double RulesEngine::getDoubleAttribute(const std::string& modelId,
                                       const std::string& objectId,
                                       const std::string& attributeName)
{
  std::string mapKey = mf_mapKey(modelId,objectId,attributeName);
  std::map<std::string,double>::iterator it = m_doubleAttributeMap.find(mapKey);
  BOOST_ASSERT(it != m_doubleAttributeMap.end());
  return it->second;
}

std::string RulesEngine::getEnumAttribute(const std::string& modelId,
                                          const std::string& objectId,
                                          const std::string& attributeName)
{
  std::string mapKey = mf_mapKey(modelId,objectId,attributeName);
  std::map<std::string,std::string>::iterator it = m_enumAttributeMap.find(mapKey);
  BOOST_ASSERT(it != m_enumAttributeMap.end());
  return it->second;
}

int RulesEngine::getIntegerAttribute(const std::string& modelId,
                                     const std::string& objectId,
                                     const std::string& attributeName)
{
  std::string mapKey = mf_mapKey(modelId,objectId,attributeName);
  std::map<std::string,int>::iterator it = m_integerAttributeMap.find(mapKey);
  BOOST_ASSERT(it != m_integerAttributeMap.end());
  return it->second;
}

/// @endcond

RulesEngine::RulesEngine()
  : m_clipsEnvironment(this), m_ruleset("Empty Ruleset"), m_result("")
{}

void RulesEngine::mf_initializeRun(const ruleset::StandardsRuleset& ruleset,
                                   const std::vector<ruleset::StandardsRulesetOption>& options) 
{
  // report title
  std::stringstream ss;
  ss << "RulesEngine Run " << checksum(toString(m_currentUuid)) << " --- Report for Model '" 
     << m_originalModelName << "'";

  // "Construct" this run.
  m_ruleset = ruleset;
  m_currentUuid = createUUID();
  m_result = ApplyRulesetResult(ss.str());
  m_clipsEnvironment.Clear();
  m_clipsEnvironment.Watch(const_cast<char *>("facts"));
  m_clipsEnvironment.Watch(const_cast<char *>("rules"));
  m_clipsEnvironment.Watch(const_cast<char *>("activations"));
  m_clipsEnvironment.Watch(const_cast<char *>("statistics"));

  // load ruleset options
  mf_buildRulesetOptions();

  // load deftemplates and defrules
  mf_buildDeftemplates();
  mf_buildDefrules(options);

  // load ruleset rules
  mf_buildRulesetRules();

  // load deffacts
  mf_buildDeffacts(options);
}

void RulesEngine::mf_buildRulesetOptions() {
  bool ok(true);
  ruleset::StandardsRulesetOptionVector options = m_ruleset.options();
  BOOST_FOREACH(const ruleset::StandardsRulesetOption& option,options) {
    ok = m_clipsEnvironment.Build(const_cast<char *>(option.printDefinition().c_str()));
    if (!ok) {
      std::stringstream msg;
      msg << "Unable to load ruleset::StandardsRulesetOption '" << option.name() 
          << "' into CLIPS. Actual text:" << std::endl << option.printDefinition() << std::endl;
      mf_handleErrorMessage(msg.str());
    }
  }
}

void RulesEngine::mf_buildDeftemplates() {
  bool ok(true);

  ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDeftemplateApplyRulesetResult().c_str()));
  if (!ok) {
    mf_handleErrorMessage("Unable to load ApplyRulesetResult deftemplate into CLIPS.");
  }
  ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDeftemplateSimulationRequired().c_str()));
  if (!ok) {
    mf_handleErrorMessage("Unable to load SimulationRequired deftemplate into CLIPS.");
  }
  ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDeftemplateSimulationComplete().c_str()));
  if (!ok) {
    mf_handleErrorMessage("Unable to load SimulationComplete deftemplate into CLIPS.");
  }

  ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDeftemplateAvailableModel().c_str()));
  if (!ok) {
    mf_handleErrorMessage("Unable to load AvailableModel deftemplate into CLIPS.");
  }
  ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDeftemplateGetObjects().c_str()));
  if (!ok) {
    mf_handleErrorMessage("Unable to load GetObjects deftemplate into CLIPS.");
  }
  ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDeftemplateAvailableObjects().c_str()));
  if (!ok) {
    mf_handleErrorMessage("Unable to load AvailableObjects deftemplate into CLIPS.");
  }
  ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDeftemplateAvailableObject().c_str()));
  if (!ok) {
    mf_handleErrorMessage("Unable to load AvailableObject deftemplate into CLIPS.");
  }

  ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDeftemplateGetAttribute().c_str()));
  if (!ok) {
    mf_handleErrorMessage("Unable to load GetAttribute deftemplate into CLIPS.");
  }
  ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDeftemplateBooleanAttribute().c_str()));
  if (!ok) {
    mf_handleErrorMessage("Unable to load BooleanAttribute deftemplate into CLIPS.");
  }
  ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDeftemplateDoubleAttribute().c_str()));
  if (!ok) {
    mf_handleErrorMessage("Unable to load DoubleAttribute deftemplate into CLIPS.");
  }
  ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDeftemplateEnumAttribute().c_str()));
  if (!ok) {
    mf_handleErrorMessage("Unable to load EnumAttribute deftemplate into CLIPS.");
  }
  ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDeftemplateIntegerAttribute().c_str()));
  if (!ok) {
    mf_handleErrorMessage("Unable to load IntegerAttribute deftemplate into CLIPS.");
  }
}

void RulesEngine::mf_buildDefrules(const std::vector<ruleset::StandardsRulesetOption>& options) {
  bool ok(true);
  std::stringstream msg;
  BOOST_FOREACH(const ruleset::StandardsRulesetOption& option, options) {
    ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDefrule(option).c_str()));
    if (!ok) {
      msg << "Unable to load Reporting rule related to ruleset::StandardsRulesetOption '" << option.name() 
          << "' into CLIPS. Rule: " << std::endl
          << mf_printDefrule(option) << std::endl;
      mf_handleErrorMessage(msg.str()); msg.str("");
    }
  }

  ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDefruleBasicChangePhase().c_str()));
  if (!ok) { 
    mf_handleErrorMessage("Unable to load basic change phase rule into CLIPS.");
  }
  ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDefruleChangePhaseToSimulation().c_str()));
  if (!ok) { 
    mf_handleErrorMessage("Unable to load change phase to simulation rule into CLIPS.");
  }

  ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDefruleGetModelIdsForObjects().c_str()));
  if (!ok) {
    mf_handleErrorMessage("Unable to load get model ids for objects into CLIPS.");
  }
  ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDefruleGetObjects().c_str()));
  if (!ok) { 
    mf_handleErrorMessage("Unable to load get objects rule into CLIPS.");
  }
  ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDefruleBreakUpAvailableObjects().c_str()));
  if (!ok) { 
    mf_handleErrorMessage("Unable to load break up available objects rule into CLIPS.");
  }

  ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDefruleCheckAttribute().c_str()));
  if (!ok) { 
    mf_handleErrorMessage("Unable to load check attribute rule into CLIPS.");
  }
  ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDefruleQueueSimulation().c_str()));
  if (!ok) { 
    mf_handleErrorMessage("Unable to load queue simulation rule into CLIPS.");
  }
  ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDefruleCheckAttributeAfterSimulation().c_str()));
  if (!ok) { 
    mf_handleErrorMessage("Unable to load check attribute after simulation rule into CLIPS.");
  }
  ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDefruleUpdateApplyRulesetResult().c_str()));
  if (!ok) { 
    mf_handleErrorMessage("Unable to load update apply ruleset rule into CLIPS.");
  }
  ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDefruleGetBooleanAttribute().c_str()));
  if (!ok) { 
    mf_handleErrorMessage("Unable to load get boolean attribute rule into CLIPS.");
  }
  ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDefruleGetDoubleAttribute().c_str()));
  if (!ok) { 
    mf_handleErrorMessage("Unable to load get double attribute rule into CLIPS.");
  }
  ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDefruleGetEnumAttribute().c_str()));
  if (!ok) { 
    mf_handleErrorMessage("Unable to load get enum attribute rule into CLIPS.");
  }
  ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDefruleGetIntegerAttribute().c_str()));
  if (!ok) { 
    mf_handleErrorMessage("Unable to load get integer attribute rule into CLIPS.");
  }

  ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDefruleReportApplyRulesetResult().c_str()));
  if (!ok) { 
    mf_handleErrorMessage("Unable to load report apply ruleset result rule into CLIPS.");
  }
}

void RulesEngine::mf_buildRulesetRules() {
  bool ok(true);
  std::stringstream msg;
  ruleset::StandardsRuleVector rules = m_ruleset.rules(false); // only return active rules
  BOOST_FOREACH(const ruleset::StandardsRule& rule,rules) {
    ok = m_clipsEnvironment.Build(const_cast<char *>(rule.print(ruleset::StandardsRulesetPrintMode::Engine).c_str()));
    if (!ok) {
      msg << "Unable to load Rule '" << rule.name() << "' into CLIPS. Actual text: " << std::endl
          << rule.print(ruleset::StandardsRulesetPrintMode::Engine) << std::endl;
      mf_handleErrorMessage(msg.str()); msg.str("");
    }
  }  
  ruleset::StandardsRuleVector impliedRules = m_ruleset.impliedRules(); // excludes disabled rules by default
  BOOST_FOREACH(const ruleset::StandardsRule& rule,impliedRules) {
    ok = m_clipsEnvironment.Build(const_cast<char *>(rule.print(ruleset::StandardsRulesetPrintMode::Engine).c_str()));
    if (!ok) {
      msg << "Unable to load Rule '" << rule.name() << "' into CLIPS. Actual text: " << std::endl
          << rule.print(ruleset::StandardsRulesetPrintMode::Engine) << std::endl;
      mf_handleErrorMessage(msg.str()); msg.str("");
    }
  }
}

void RulesEngine::mf_buildDeffacts(const std::vector<ruleset::StandardsRulesetOption>& options) {
  bool ok = m_clipsEnvironment.Build(const_cast<char *>(mf_printDeffacts(options).c_str()));
  if (!ok) { 
    mf_handleErrorMessage("Unable to load deffacts into CLIPS.");
  }
}

std::string RulesEngine::mf_printDeftemplates() const {
  std::stringstream ss;

  ss << mf_printDeftemplateApplyRulesetResult();
  ss << mf_printDeftemplateSimulationRequired();
  ss << mf_printDeftemplateSimulationComplete();

  ss << mf_printDeftemplateAvailableModel();
  ss << mf_printDeftemplateGetObjects();
  ss << mf_printDeftemplateAvailableObjects();
  ss << mf_printDeftemplateAvailableObject();

  ss << mf_printDeftemplateGetAttribute();
  ss << mf_printDeftemplateBooleanAttribute();
  ss << mf_printDeftemplateIntegerAttribute();
  ss << mf_printDeftemplateDoubleAttribute();
  ss << mf_printDeftemplateEnumAttribute();

  return ss.str();
}

std::string RulesEngine::mf_printDefrules(const std::vector<ruleset::StandardsRulesetOption>& options) const
{
  std::stringstream ss;

  BOOST_FOREACH(const ruleset::StandardsRulesetOption& option, options) {
    ss << mf_printDefrule(option);
  }

  ss << mf_printDefruleBasicChangePhase();
  ss << mf_printDefruleChangePhaseToSimulation();

  ss << mf_printDefruleGetModelIdsForObjects();
  ss << mf_printDefruleGetObjects();
  ss << mf_printDefruleBreakUpAvailableObjects();

  ss << mf_printDefruleCheckAttribute();
  ss << mf_printDefruleQueueSimulation();
  ss << mf_printDefruleCheckAttributeAfterSimulation();
  ss << mf_printDefruleGetBooleanAttribute();
  ss << mf_printDefruleGetDoubleAttribute();
  ss << mf_printDefruleGetEnumAttribute();
  ss << mf_printDefruleGetIntegerAttribute();

  ss << mf_printDefruleReportApplyRulesetResult();

  return ss.str();
}

std::string RulesEngine::mf_printDeffacts(
    const std::vector<ruleset::StandardsRulesetOption>& options) const 
{
  std::stringstream ss;
  ss << "(deffacts RulesEngine_" << checksum(toString(m_currentUuid)) << std::endl;
  ss << "  (phase StartUp)" << std::endl;
  ss << "  (phase-after StartUp Query)" << std::endl;
  ss << "  (phase-after Simulation Query)" << std::endl;
  ss << "  (phase-after Query ShutDown)" << std::endl;
  BOOST_FOREACH(const ruleset::StandardsRulesetOption& option, options) {
    ss << "  " << option.printValue() << std::endl;
  }
  ss << "  (AvailableModel (name \"" << m_originalModelName << "\")" << std::endl;
  ss << "                  (id   \"" << 0 << "\")" << std::endl;
  ss << "                  (modelType " 
         << ruleset::StandardsRulesetModelType(ruleset::StandardsRulesetModelType::Original).valueName() 
         << "))" << std::endl;
  ss << ")" << std::endl;
  ss << std::endl;
  return ss.str();
}

std::string RulesEngine::mf_printDeftemplateApplyRulesetResult() const {
  std::stringstream ss;

  ss << "(deftemplate ApplyRulesetResult" << std::endl;
  ss << "  (slot modelName (type STRING))" << std::endl;
  ss << "  (slot result (type SYMBOL)" << std::endl;
  ss << "               (allowed-symbols";
  BOOST_FOREACH(int value,ruleset::StandardsRulesetResult::getValues()) {
    ss << " " << ruleset::StandardsRulesetResult(value).valueName();
  }
  ss << "))" << std::endl;
  ss << ")" << std::endl;
  ss << std::endl;

  return ss.str();
}

std::string RulesEngine::mf_printDeftemplateSimulationRequired() const {
  std::stringstream ss;

  ss << "(deftemplate SimulationRequired" << std::endl;
  ss << "  (slot modelId (type STRING))" << std::endl;
  ss << ")" << std::endl;
  ss << std::endl;

  return ss.str();
}

std::string RulesEngine::mf_printDeftemplateSimulationComplete() const {
  std::stringstream ss;

  ss << "(deftemplate SimulationComplete" << std::endl;
  ss << "  (slot modelId (type STRING))" << std::endl;
  ss << ")" << std::endl;
  ss << std::endl;

  return ss.str();
}

std::string RulesEngine::mf_printDeftemplateAvailableModel() const {
  std::stringstream ss;

  ss << "(deftemplate AvailableModel " << std::endl;
  ss << "  (slot name (type STRING))" << std::endl;
  ss << "  (slot id (type STRING))" << std::endl;
  ss << "  (slot modelType (type SYMBOL)" << std::endl;
  ss << "                  (allowed-symbols";
  BOOST_FOREACH(int value,ruleset::StandardsRulesetModelType::getValues()) {
    ss << " " << ruleset::StandardsRulesetModelType(value).valueName();
  }
  ss << "))" << std::endl;
  ss << ")" << std::endl;
  ss << std::endl;

  return ss.str();
}

std::string RulesEngine::mf_printDeftemplateGetObjects() const {
  std::stringstream ss;

  ss << "(deftemplate GetObjects" << std::endl;
  ss << "    (slot modelId (type STRING))" << std::endl;
  ss << "    (slot objectType (type STRING))" << std::endl;
  ss << ")" << std::endl;
  ss << std::endl;

  return ss.str();
}

std::string RulesEngine::mf_printDeftemplateAvailableObjects() const {
  std::stringstream ss;

  ss << "(deftemplate AvailableObjects" << std::endl;
  ss << "    (slot modelId (type STRING))" << std::endl;
  ss << "    (slot objectType (type STRING))" << std::endl;
  ss << "    (multislot objectIds (type STRING))" << std::endl;
  ss << ")" << std::endl;
  ss << std::endl;

  return ss.str();
}

std::string RulesEngine::mf_printDeftemplateAvailableObject() const {
  std::stringstream ss;

  ss << "(deftemplate AvailableObject" << std::endl;
  ss << "    (slot modelId (type STRING))" << std::endl;
  ss << "    (slot objectType (type STRING))" << std::endl;
  ss << "    (slot objectName (type STRING))" << std::endl;
  ss << "    (slot objectId (type STRING))" << std::endl;
  ss << ")" << std::endl;
  ss << std::endl;

  return ss.str();
}

std::string RulesEngine::mf_printDeftemplateGetAttribute() const {
  std::stringstream ss;

  ss << "(deftemplate GetAttribute" << std::endl;
  ss << "    (slot modelId (type STRING))" << std::endl;
  ss << "    (slot objectType (type STRING))" << std::endl;
  ss << "    (slot objectId (type STRING))" << std::endl;
  ss << "    (slot attributeType (type SYMBOL)" << std::endl;
  ss << "                        (allowed-symbols Boolean Double Enum Integer))" << std::endl;
  ss << "    (slot attributeName (type STRING))" << std::endl;
  ss << "    (slot checked (type SYMBOL)" << std::endl;
  ss << "                  (allowed-symbols TRUE FALSE)" << std::endl;
  ss << "                  (default FALSE))" << std::endl;
  ss << "    (slot message (type STRING))" << std::endl;
  ss << ")" << std::endl;
  ss << std::endl;

  return ss.str();
}

std::string RulesEngine::mf_printDeftemplateBooleanAttribute() const {
  std::stringstream ss;

  ss << "(deftemplate BooleanAttribute" << std::endl;
  ss << "    (slot modelId (type STRING))" << std::endl;
  ss << "    (slot objectId (type STRING))" << std::endl;
  ss << "    (slot name (type STRING))" << std::endl;
  ss << "    (slot value (type SYMBOL)" << std::endl;
  ss << "                (allowed-symbols TRUE FALSE))" << std::endl;
  ss << ")" << std::endl;
  ss << std::endl;

  return ss.str();
}

std::string RulesEngine::mf_printDeftemplateDoubleAttribute() const {
  std::stringstream ss;

  ss << "(deftemplate DoubleAttribute" << std::endl;
  ss << "    (slot modelId (type STRING))" << std::endl;
  ss << "    (slot objectId (type STRING))" << std::endl;
  ss << "    (slot name (type STRING))" << std::endl;
  ss << "    (slot value (type NUMBER))" << std::endl;
  ss << ")" << std::endl;
  ss << std::endl;

  return ss.str();
}

std::string RulesEngine::mf_printDeftemplateEnumAttribute() const {
  std::stringstream ss;

  ss << "(deftemplate EnumAttribute" << std::endl;
  ss << "    (slot modelId (type STRING))" << std::endl;
  ss << "    (slot objectId (type STRING))" << std::endl;
  ss << "    (slot name (type STRING))" << std::endl;
  ss << "    (slot value (type STRING))" << std::endl;
  ss << ")" << std::endl;
  ss << std::endl;

  return ss.str();
}

std::string RulesEngine::mf_printDeftemplateIntegerAttribute() const {
  std::stringstream ss;

  ss << "(deftemplate IntegerAttribute" << std::endl;
  ss << "    (slot modelId (type STRING))" << std::endl;
  ss << "    (slot objectId (type STRING))" << std::endl;
  ss << "    (slot name (type STRING))" << std::endl;
  ss << "    (slot value (type INTEGER))" << std::endl;
  ss << ")" << std::endl;
  ss << std::endl;

  return ss.str();
}

std::string RulesEngine::mf_printDefrule(const ruleset::StandardsRulesetOption& option) const {
  std::stringstream ss;
  ss << std::endl;
  ss << "(defrule Report_" << option.name() << std::endl;
  ss << "    (phase StartUp)" << std::endl;
  ss << "    (" << option.factName() << " (value ?value))" << std::endl;
  ss << "  =>" << std::endl;
  ss << "    (addTextToReport \"Summary\" \"" << option.name() << ": \" ?value )" << std::endl;
  ss << ")" << std::endl;
  return ss.str();
}

std::string RulesEngine::mf_printDefruleBasicChangePhase() const {
  std::stringstream ss;
  ss << "(defrule change-phase-basic" << std::endl;
  ss << "    (declare (salience -10))" << std::endl;
  ss << "    ?phase <- (phase ?current-phase)" << std::endl;
  ss << "    (phase-after ?current-phase ?next-phase)" << std::endl;
  ss << "  => " << std::endl;
  ss << "    (retract ?phase)" << std::endl;
  ss << "    (assert (phase ?next-phase))" << std::endl;
  ss << ")" << std::endl;
  ss << std::endl;
  return ss.str();
}

std::string RulesEngine::mf_printDefruleChangePhaseToSimulation() const {
  std::stringstream ss;
  ss << "(defrule change-phase-simulation" << std::endl;
  ss << "    (declare (salience -5))" << std::endl;
  ss << "    ?phase <- (phase Query)" << std::endl;
  ss << "    ?sim <- (SimulationRequired)" << std::endl;
  ss << "  => " << std::endl;
  ss << "    (retract ?phase)" << std::endl;
  ss << "    (retract ?sim)" << std::endl;
  ss << "    (assert (phase Simulation))" << std::endl;
  ss << ")" << std::endl;
  ss << std::endl;
  return ss.str();
}

std::string RulesEngine::mf_printDefruleGetModelIdsForObjects() const {
  std::stringstream ss;

  ss << "(defrule GetModelIdsForObjects" << std::endl;
  ss << "    (phase Query)" << std::endl;
  ss << "    ?availableModel <- (AvailableModel (id ?modelId))" << std::endl;
  ss << "    ?getObjects <- (GetObjects (modelId \"\")" << std::endl;
  ss << "                               (objectType ?objectType))" << std::endl;
  ss << " =>" << std::endl;
  ss << "    (assert (GetObjects (modelId ?modelId)" << std::endl;
  ss << "                        (objectType ?objectType)))" << std::endl;
  ss << ")" << std::endl;
  ss << std::endl;

  return ss.str();
}

std::string RulesEngine::mf_printDefruleGetObjects() const {
  std::stringstream ss;

  ss << "(defrule GetObjects" << std::endl;
  ss << "    (phase Query)" << std::endl;
  ss << "    ?getObjects <- (GetObjects (modelId ?modelId&~\"\")" << std::endl;
  ss << "                               (objectType ?objectType))" << std::endl;
  ss << "  =>" << std::endl;
  ss << "    (retract ?getObjects)" << std::endl;
  ss << "    (bind ?ids (getObjects ?modelId ?objectType))" << std::endl;
  ss << "    (assert (AvailableObjects (modelId ?modelId)" << std::endl;
  ss << "                              (objectType ?objectType)" << std::endl;
  ss << "                              (objectIds ?ids)))" << std::endl;
  ss << ")" << std::endl;
  ss << std::endl;
  
  return ss.str();
}

std::string RulesEngine::mf_printDefruleBreakUpAvailableObjects() const {
  std::stringstream ss;

  ss << "(defrule BreakUpAvailableObjects" << std::endl;
  ss << "    (phase Query)" << std::endl;
  ss << "    ?availableObjects <- (AvailableObjects (modelId ?modelId)" << std::endl;
  ss << "                                           (objectType ?objectType)" << std::endl;
  ss << "                                           (objectIds $?before ?current $?after))" << std::endl;
  ss << "  =>" << std::endl;
  ss << "    (bind ?objectName (getObjectName ?modelId ?current))" << std::endl;
  ss << "    (assert (AvailableObject (modelId ?modelId)" << std::endl;
  ss << "                             (objectType ?objectType)" << std::endl;
  ss << "                             (objectName ?objectName)" << std::endl; // HERE -- No objectName available!
  ss << "                             (objectId ?current)))" << std::endl; 
  ss << ")" << std::endl;
  ss << std::endl;

  return ss.str();
}

std::string RulesEngine::mf_printDefruleCheckAttribute() const {
  std::stringstream ss;

  ss << "(defrule CheckAttribute" << std::endl;
  ss << "    (phase Query)" << std::endl;
  ss << "    ?getAttribute <- (GetAttribute (modelId ?modelId)" << std::endl;
  ss << "                                   (objectType ?objectType)" << std::endl;
  ss << "                                   (objectId ?objectId)" << std::endl;
  ss << "                                   (attributeType ?attributeType)" << std::endl;
  ss << "                                   (attributeName ?attributeName)" << std::endl;
  ss << "                                   (checked FALSE))" << std::endl;
  ss << "  => " << std::endl;
  ss << "    (bind ?message (checkAttribute ?modelId ?objectType ?objectId ?attributeType ?attributeName))" << std::endl;
  ss << "    (modify ?getAttribute (checked TRUE)" << std::endl;
  ss << "                          (message ?message))" << std::endl;
  ss << ")" << std::endl;
  ss << std::endl;
  
  return ss.str();
}

std::string RulesEngine::mf_printDefruleQueueSimulation() const {
  std::stringstream ss;

  ss << "(defrule QueueSimulation" << std::endl;
  ss << "    (phase Query)" << std::endl;
  ss << "    (GetAttribute (modelId ?modelId)" << std::endl;
  ss << "                  (message \"SimulationRequired\"))" << std::endl;
  ss << "    (not (SimulationRequired (modelId ?modelId)))" << std::endl;
  ss << "  =>" << std::endl;
  ss << "    (assert (SimulationRequired (modelId ?modelId)))" << std::endl;
  ss << ")";
  ss << std::endl;
  
  return ss.str();
}

std::string RulesEngine::mf_printDefruleCheckAttributeAfterSimulation() const {
  std::stringstream ss;

  ss << "(defrule CheckAttributeAfterSimulation" << std::endl;
  ss << "    (phase Query)" << std::endl;
  ss << "    ?getAttribute <- (GetAttribute (modelId ?modelId)" << std::endl;
  ss << "                                   (message \"SimulationRequired\"))" << std::endl;
  ss << "    (SimulationComplete (modelId ?modelId))" << std::endl;
  ss << "  => " << std::endl;
  ss << "    (modify ?getAttribute (checked FALSE)" << std::endl;
  ss << "                          (message \"\"))" << std::endl;
  ss << ")" << std::endl;
  ss << std::endl;
  
  return ss.str();
}

std::string RulesEngine::mf_printDefruleUpdateApplyRulesetResult() const {
  std::stringstream ss;

  ss << "(defrule UpdateApplyRulesetResult" << std::endl;
  ss << "    (phase ShutDown)" << std::endl;
  ss << "    (AvailableModel (name ?modelName)" << std::endl;
  ss << "                    (id ?modelId))" << std::endl;
  ss << "    (not (ApplyRulesetResult (modelName ?modelName)))" << std::endl;
  ss << "  =>" << std::endl;
  ss << "    (bind ?result (getOverallResultFlag ?modelName))" << std::endl;
  ss << "    (assert (ApplyRulesetResult (modelName ?modelName)" << std::endl;
  ss << "                                (result ?result)))" << std::endl;
  ss << ")";
  ss << std::endl;
  
  return ss.str();
}

std::string RulesEngine::mf_printDefruleGetBooleanAttribute() const {
  std::stringstream ss;

  ss << "(defrule GetBooleanAttribute" << std::endl;
  ss << "    (phase Query)" << std::endl;
  ss << "    ?getAttribute <- (GetAttribute (modelId ?modelId)" << std::endl;
  ss << "                                   (objectType ?objectType)" << std::endl;
  ss << "                                   (objectId ?objectId)" << std::endl;
  ss << "                                   (attributeType Boolean)" << std::endl;
  ss << "                                   (attributeName ?attributeName)" << std::endl;
  ss << "                                   (checked TRUE)" << std::endl;
  ss << "                                   (message \"\"))" << std::endl;
  ss << "  =>" << std::endl;
  ss << "    (retract ?getAttribute)" << std::endl;
  ss << "    (bind ?attributeValue (getBooleanAttribute ?modelId ?objectType ?objectId ?attributeName))" << std::endl;
  ss << "    (assert (BooleanAttribute (modelId ?modelId)" << std::endl;
  ss << "                              (objectId ?objectId)" << std::endl;
  ss << "                              (name ?attributeName)" << std::endl;
  ss << "                              (value ?attributeValue)))" << std::endl;
  ss << ")" << std::endl;
  ss << std::endl;
  
  return ss.str();
}

std::string RulesEngine::mf_printDefruleGetDoubleAttribute() const {
  std::stringstream ss;

  ss << "(defrule GetDoubleAttribute" << std::endl;
  ss << "    (phase Query)" << std::endl;
  ss << "    ?getAttribute <- (GetAttribute (modelId ?modelId)" << std::endl;
  ss << "                                   (objectType ?objectType)" << std::endl;
  ss << "                                   (objectId ?objectId)" << std::endl;
  ss << "                                   (attributeType Double)" << std::endl;
  ss << "                                   (attributeName ?attributeName)" << std::endl;
  ss << "                                   (checked TRUE)" << std::endl;
  ss << "                                   (message \"\"))" << std::endl;
  ss << "  =>" << std::endl;
  ss << "    (retract ?getAttribute)" << std::endl;
  ss << "    (bind ?attributeValue (getDoubleAttribute ?modelId ?objectType ?objectId ?attributeName))" << std::endl;
  ss << "    (assert (DoubleAttribute (modelId ?modelId)" << std::endl;
  ss << "                              (objectId ?objectId)" << std::endl;
  ss << "                              (name ?attributeName)" << std::endl;
  ss << "                              (value ?attributeValue)))" << std::endl;
  ss << ")" << std::endl;
  ss << std::endl;
  
  return ss.str();
}

std::string RulesEngine::mf_printDefruleGetEnumAttribute() const {
  std::stringstream ss;

  ss << "(defrule GetEnumAttribute" << std::endl;
  ss << "    (phase Query)" << std::endl;
  ss << "    ?getAttribute <- (GetAttribute (modelId ?modelId)" << std::endl;
  ss << "                                   (objectType ?objectType)" << std::endl;
  ss << "                                   (objectId ?objectId)" << std::endl;
  ss << "                                   (attributeType Enum)" << std::endl;
  ss << "                                   (attributeName ?attributeName)" << std::endl;
  ss << "                                   (checked TRUE)" << std::endl;
  ss << "                                   (message \"\"))" << std::endl;
  ss << "  =>" << std::endl;
  ss << "    (retract ?getAttribute)" << std::endl;
  ss << "    (bind ?attributeValue (getEnumAttribute ?modelId ?objectType ?objectId ?attributeName))" << std::endl;
  ss << "    (assert (EnumAttribute (modelId ?modelId)" << std::endl;
  ss << "                              (objectId ?objectId)" << std::endl;
  ss << "                              (name ?attributeName)" << std::endl;
  ss << "                              (value ?attributeValue)))" << std::endl;
  ss << ")" << std::endl;
  ss << std::endl;
  
  return ss.str();
}

std::string RulesEngine::mf_printDefruleGetIntegerAttribute() const {
  std::stringstream ss;

  ss << "(defrule GetIntegerAttribute" << std::endl;
  ss << "    (phase Query)" << std::endl;
  ss << "    ?getAttribute <- (GetAttribute (modelId ?modelId)" << std::endl;
  ss << "                                   (objectType ?objectType)" << std::endl;
  ss << "                                   (objectId ?objectId)" << std::endl;
  ss << "                                   (attributeType Integer)" << std::endl;
  ss << "                                   (attributeName ?attributeName)" << std::endl;
  ss << "                                   (checked TRUE)" << std::endl;
  ss << "                                   (message \"\"))" << std::endl;
  ss << "  =>" << std::endl;
  ss << "    (retract ?getAttribute)" << std::endl;
  ss << "    (bind ?attributeValue (getIntegerAttribute ?modelId ?objectType ?objectId ?attributeName))" << std::endl;
  ss << "    (assert (IntegerAttribute (modelId ?modelId)" << std::endl;
  ss << "                              (objectId ?objectId)" << std::endl;
  ss << "                              (name ?attributeName)" << std::endl;
  ss << "                              (value ?attributeValue)))" << std::endl;
  ss << ")" << std::endl;
  ss << std::endl;
  
  return ss.str();
}

std::string RulesEngine::mf_printDefruleReportApplyRulesetResult() const {
  std::stringstream ss;

  ss << "(defrule ReportApplyRulesetResult" << std::endl;
  ss << "    (phase ShutDown)" << std::endl;
  ss << "    (ApplyRulesetResult (modelName ?modelName)" << std::endl;
  ss << "                        (result ?result))" << std::endl;
  ss << "  =>" << std::endl;
  ss << "    (addTextToReport \"Summary\" \"Result: \" ?result )" << std::endl;
  ss << ")" << std::endl;
  ss << std::endl;
  
  return ss.str();
}

void RulesEngine::mf_completeRun() {
  // run
  m_clipsEnvironment.Reset();
  m_clipsEnvironment.Run();
  m_result.setRunComplete();
}

std::string RulesEngine::mf_mapKey(const std::string& modelId,
                                            const std::string& objectId,
                                            const std::string& ddAttributeName)
{
  std::stringstream ss;
  ss << modelId << "_" << objectId << "_" << ddAttributeName;
  return ss.str();
}

/// @cond

void RulesEngine::mf_handleErrorMessage(const std::string& msg) {
  LOG(Error,msg);
  m_result.setOverallResultFlag(ruleset::StandardsRulesetResult::Error);
  m_result.addToReport("Errors",msg);
}

/// @endcond

std::string RulesEngine::mf_handleAttributeCodeAndMessage(
  standardsinterface::result::Code code,const std::string& msg)
{
  std::string result;
  switch (code) {
    case standardsinterface::result::DataDictionaryError :
      BOOST_ASSERT(!msg.empty());
      result = msg;
      mf_handleErrorMessage(msg);
    case standardsinterface::result::ModelError :
    case standardsinterface::result::RequestUnmapped :
      BOOST_ASSERT(!msg.empty());
      result = msg;
      LOG(Info,msg);
      m_result.setOverallResultFlag(ruleset::StandardsRulesetResult::Incomplete);
      m_result.addToReport("Missing Information",msg);
     break;
    case standardsinterface::result::Good :
      BOOST_ASSERT(msg.empty());
     break;
    case standardsinterface::result::SimulationRequired :
      result = "SimulationRequired";
     break;
    default:
      BOOST_ASSERT(false);
  };
  return result;
}

} // rulesengine
} // openstudio
