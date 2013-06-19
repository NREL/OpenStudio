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

#ifndef RULESENGINE_AGNOSTICINTERFACE_HPP
#define RULESENGINE_AGNOSTICINTERFACE_HPP

extern "C"
{
  #include <clips/clips.h>
}

/// @cond

/** Push text onto the report. 
 *
 *  \param[in] text - string - text to be added to the report.
 */ 
extern "C" void addTextToReport(void *t_env);

/** Set the overall result flag. May or may not change depending on current value.
 *  
 *  \param[in] flag - Pass, Incomplete, Fail, or Error, see ruleset::StandardsRulesetResult. */
extern "C" void setOverallResultFlag(void *t_env);

/** Get the overall result flag. 
 *
 *  \param[in] modelName - string - not currently used. stubbed out for future multi-model uses
 *  \param[out] flag - Pass, Incomplete, Fail or Error, see ruleset::StandardsRulesetResult. */
extern "C" void *getOverallResultFlag(void *t_env);

/** Get objects from an AgnosticInterface. 
 *  
 *  \param[in] modelId - string - unique identifier of the model
 *  \param[in] objectType - string - used to construct standardsinterface::ObjectType
 *  \param[out] ids - multifield string - unique identifiers of objects in modelId of objectType
 */
extern "C" void getObjects(void *t_env, DATA_OBJECT_PTR returnValuePtr);

/** Get the name of an object. 
 *  
 *  \param[in] modelId - string - unique identifier of the model
 *  \param[in] objectId - string - unique identifier of the object
 *  \param[out] objectName - string - name of the object. */
extern "C" void *getObjectName(void *t_env);

/** Clone an AgnosticInterface model from CLIPS.
 * 
 *  \param[in] modelName - string - the name of the model being cloned
 *  \param[in] modelId - string - unique identifier of the model being cloned
 *  \param[in] modelType - symbol - RulesetModelType of model being cloned
 *  \param[out] clonedModelId - string - unique identifier of the model that was cloned
 */
extern "C" void *cloneModel(void *t_env);

/** Determine if a particular attribute is available. 
 *
 *  \param[in] modelId - string - unique identifier of the model containing the object
 *  \param[in] objectType - string - used to construct standardsinterface::ObjectType for the object
 *  \param[in] objectId - string - unique identifier of the object within the model
 *  \param[in] attributeType - symbol - Boolean, Double, Enum, or Integer
 *  \param[in] attributeName - string - used to construct standardsinterface::[attributeType]Attribute
 *  \param[out] message - string - empty if available, "SimulationRequired", or error message
 */
extern "C" void *checkAttribute(void *t_env);

/** Get a boolean attribute of an object. 
 *  
 *  \param[in] modelId - string - unique identifier of the model containing the object
 *  \param[in] objectType - string - used to construct standardsinterface::ObjectType for the object
 *  \param[in] objectId - string - unique identifier of the object within the model
 *  \param[in] attributeName - string - used to construct standardsinterface::BooleanAttribute
 *  \param[out] value - symbol - TRUE or FALSE */
extern "C" void *getBooleanAttribute(void *t_env);

/** Get a double attribute of an object. 
 *  
 *  \param[in] modelId - string - unique identifier of the model containing the object
 *  \param[in] objectType - string - used to construct standardsinterface::ObjectType for the object
 *  \param[in] objectId - string - unique identifier of the object within the model
 *  \param[in] attributeName - string - used to construct standardsinterface::DoubleAttribute
 *  \param[out] value - number */
extern "C" double getDoubleAttribute(void *t_env);

/** Get an enum attribute of an object. 
 *  
 *  \param[in] modelId - string - unique identifier of the model containing the object
 *  \param[in] objectType - string - used to construct standardsinterface::ObjectType for the object
 *  \param[in] objectId - string - unique identifier of the object within the model
 *  \param[in] attributeName - string - used to construct standardsinterface::EnumAttribute
 *  \param[out] value - string */
extern "C" void *getEnumAttribute(void *t_env);

/** Get an integer attribute of an object. 
 *  
 *  \param[in] modelId - string - unique identifier of the model containing the object
 *  \param[in] objectType - string - used to construct standardsinterface::ObjectType for the object
 *  \param[in] objectId - string - unique identifier of the object within the model
 *  \param[in] attributeName - string - used to construct standardsinterface::IntegerAttribute
 *  \param[out] value - integer */
extern "C" int getIntegerAttribute(void *t_env);

/// @endcond

#endif // RULESENGINE_AGNOSTICINTERFACE_HPP
