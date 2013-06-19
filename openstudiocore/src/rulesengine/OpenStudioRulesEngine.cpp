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

#include <rulesengine/RulesEngineAPI.hpp>
#include <rulesengine/OpenStudioRulesEngine.hpp>

#include <ruleset/StandardsRulesetOption.hpp>

#include <model/ModelObject.hpp>
#include <model/ModelObject_Impl.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/UUID.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

namespace openstudio {
namespace rulesengine {

void OpenStudioRulesEngine::applyRules(
    const ruleset::StandardsRuleset& ruleset,
    const std::vector<ruleset::StandardsRulesetOption>& options,
    standardsinterface::OpenStudioStandardsInterface& model,
    const boost::optional<project::ProjectDatabase>& componentLibrary) 
{
  m_componentLibrary = componentLibrary;
  m_models.clear();
  m_models.push_back(model);
  mf_applyRules(ruleset,options,model);
}

/// @cond

std::string OpenStudioRulesEngine::cloneModel(const std::string& id) {
  std::string result;
  unsigned index(0);
  try { index = boost::lexical_cast<unsigned>(id); }
  catch (...) { BOOST_ASSERT(false); }

  BOOST_ASSERT(m_models.size() > index);
  m_models.push_back(m_models[index].clone());
  std::stringstream ss;
  ss << m_models.size()-1;
  return ss.str();
}

std::vector<std::string> OpenStudioRulesEngine::getObjectIds(
    const std::string& modelId,const std::string& objectType)
{
  StringVector result;
  std::stringstream msg;
  unsigned index(0);
  try { index = boost::lexical_cast<unsigned>(modelId); }
  catch (...) { BOOST_ASSERT(false); }
 
  if (index >= m_models.size()) { BOOST_ASSERT(false); }

  standardsinterface::ModelObjectVectorResult objectsResult = getObjects(m_models[index],objectType);
  if (objectsResult) {
    model::ModelObjectVector objects = *objectsResult;
    BOOST_FOREACH(const model::ModelObject& object,objects) {
      result.push_back(toString(object.handle()));
    }
  }

  return result;
}

std::string OpenStudioRulesEngine::getObjectName(const std::string& modelId,
                                                 const std::string& objectId)
{
  std::string result;
  std::stringstream msg;
  unsigned index(0);
  try { index = boost::lexical_cast<unsigned>(modelId); }
  catch (...) { BOOST_ASSERT(false); }

  if (index >= m_models.size()) { BOOST_ASSERT(false); }

  standardsinterface::OpenStudioStandardsInterface model = m_models[index];

  Handle handle = toUUID(objectId);
  BOOST_ASSERT(!handle.isNull());
  model::OptionalModelObject object = model.model().getModelObject<model::ModelObject>(handle);
  BOOST_ASSERT(object);

  OptionalString oName = object->name();
  if (oName) { result = *oName; }
  return result;
}

std::string OpenStudioRulesEngine::checkAttribute(
    const std::string& modelId,
    const std::string& objectType,
    const std::string& objectId,
    const standardsinterface::PartsOfSpeech& attributeType,
    const std::string& attributeName)
{
  unsigned index(0);
  std::stringstream msg;
  try { index = boost::lexical_cast<unsigned>(modelId); }
  catch (...) { BOOST_ASSERT(false); }

  if (index >= m_models.size()) { BOOST_ASSERT(false); }

  standardsinterface::OpenStudioStandardsInterface model = m_models[index];

  Handle handle = toUUID(objectId);
  BOOST_ASSERT(!handle.isNull());
  model::OptionalModelObject object = model.model().getModelObject<model::ModelObject>(handle);
  BOOST_ASSERT(object);

  return checkAttribute(modelId,
                        model,
                        objectType,
                        objectId,
                        *object,
                        attributeType,
                        attributeName);
}

/// @endcond

unsigned OpenStudioRulesEngine::numModels() const {
  return m_models.size();
}

OpenStudioRulesEngine::OpenStudioRulesEngine() {}

} // rulesengine
} // openstudio

