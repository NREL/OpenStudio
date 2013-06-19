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

#include <rulesengine/CallbackFunctions.hpp>
#include <rulesengine/RulesEngine.hpp>
#include <rulesengine/ApplyRulesetResult.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/document/DocumentRegex.hpp>

#include <boost/foreach.hpp>

#ifdef CLIPS_WIN32_DLL
  #include <rulesengine/clips/CLIPSWin32.h>
#endif

using namespace openstudio;
using rulesengine::RulesEngine;

rulesengine::RulesEngine *getRulesEngine(void *t_env)
{
  CLIPS::CLIPSCPPEnv *cppenv = static_cast<CLIPS::CLIPSCPPEnv *>(GetEnvironmentContext(t_env));

  rulesengine::RulesEngine *rulesengine = static_cast<rulesengine::RulesEngine *>(cppenv->getUserEnv());

  return rulesengine;
}

void addTextToReport(void *t_env) {
  DATA_OBJECT temp;
  std::stringstream ss;
  std::string sectionName;
  
  // Get string to append to RulesEngine Report.
  if (EnvArgCountCheck(t_env, const_cast<char *>("addTextToReport"),AT_LEAST,2) == -1) { return; }

  bool first(true);
  bool textPrecededByNumber(false);
  for (int i = 1, n = EnvRtnArgCount(t_env); i <= n; ++i) {
    if (first) {
      if (EnvArgTypeCheck(t_env, const_cast<char *>("addTextToReport"),1,STRING,&temp) == 0) { 
        return;
      }
      sectionName = std::string(EnvDOToString(t_env, temp));
      first = false;
      continue;
    }
    // may be number or symbol or string
    RtnUnknown(i,&temp);
    if ((GetType(temp) == STRING) || (GetType(temp) == SYMBOL)) { 
      std::string wstr = std::string(EnvDOToString(t_env,temp));
      if (!textPrecededByNumber) {
        wstr = formatUnderscore(wstr);
      }
      ss << wstr;
      textPrecededByNumber = false;
    }
    else {
      BOOST_ASSERT((GetType(temp) == INTEGER) || (GetType(temp) == FLOAT));
      ss << EnvDOToDouble(t_env,temp);
      textPrecededByNumber = true;
    }
  }

  // add to Report
  getRulesEngine(t_env)->addToReport(sectionName,ss.str());  

  return;
}

void setOverallResultFlag(void *t_env) {
  DATA_OBJECT temp;
  std::string flag;

  if (EnvArgCountCheck(t_env, const_cast<char *>("setOverallResultFlag"),EXACTLY,1) == -1) { 
    return;
  }

  // get model id
  if (EnvArgTypeCheck(t_env, const_cast<char *>("setOverallResultFlag"),1,STRING,&temp) == 0) { 
    return;
  }
  flag = EnvDOToString(t_env, temp);

  getRulesEngine(t_env)->setOverallResultFlag(flag);
  return; 
}

void *getOverallResultFlag(void *t_env) {
  void *returnValue;

  ruleset::StandardsRulesetResult flagEnum = getRulesEngine(t_env)->result().overallResultFlag();
  std::string flagString = flagEnum.valueName();

  returnValue = EnvAddSymbol(t_env, const_cast<char *>(flagString.c_str()));
  return returnValue;
}

void getObjects(void *t_env, DATA_OBJECT_PTR returnValuePtr) {
  DATA_OBJECT temp;
  void *multifieldPtr;
  std::vector<std::string> objectIds;

  if (EnvArgCountCheck(t_env, const_cast<char *>("getObjects"),EXACTLY,2) == -1) {
    EnvSetMultifieldErrorValue(t_env, returnValuePtr);
    return;
  }

  // get model id
  if (EnvArgTypeCheck(t_env, const_cast<char *>("getObjects"),1,STRING,&temp) == 0) { 
    EnvSetMultifieldErrorValue(t_env, returnValuePtr);
    return; 
  }
  std::string modelId = EnvDOToString(t_env, temp);

  // get object type
  if (EnvArgTypeCheck(t_env, const_cast<char *>("getObjects"),2,STRING,&temp) == 0) { 
    EnvSetMultifieldErrorValue(t_env, returnValuePtr);
    return; 
  }
  std::string objectType = EnvDOToString(t_env, temp);

  // get objectIds
  objectIds = getRulesEngine(t_env)->getObjectIds(modelId,objectType);

  // return ids
  multifieldPtr = EnvCreateMultifield(t_env, objectIds.size());
  unsigned index = 1;
  BOOST_FOREACH(const std::string& id, objectIds) {
    EnvSetMFType(t_env, multifieldPtr,index,STRING);
    EnvSetMFValue(t_env, multifieldPtr,index,EnvAddSymbol(t_env, const_cast<char *>(id.c_str())));
    ++index;
  }
  EnvSetpType(t_env, returnValuePtr,MULTIFIELD);
  EnvSetpValue(t_env, returnValuePtr,multifieldPtr);
  EnvSetpDOBegin(t_env, returnValuePtr,1);
  EnvSetpDOEnd(t_env, returnValuePtr,objectIds.size());
  return;
}

void *getObjectName(void *t_env) {
  DATA_OBJECT temp;
  void *returnValue;
  std::string objectName;

  if (EnvArgCountCheck(t_env, const_cast<char *>("getObjectName"),EXACTLY,2) == -1) { 
    returnValue = EnvAddSymbol(t_env, const_cast<char *>(objectName.c_str()));
    return returnValue;
  }

  // get model id
  if (EnvArgTypeCheck(t_env, const_cast<char *>("getObjectName"),1,STRING,&temp) == 0) { 
    returnValue = EnvAddSymbol(t_env, const_cast<char *>(objectName.c_str()));
    return returnValue; 
  }
  std::string modelId = EnvDOToString(t_env, temp);

  // get object id
  if (EnvArgTypeCheck(t_env, const_cast<char *>("getObjectName"),2,STRING,&temp) == 0) { 
    returnValue = EnvAddSymbol(t_env, const_cast<char *>(objectName.c_str()));
    return returnValue; 
  }
  std::string objectId = EnvDOToString(t_env, temp);

  // get name
  objectName = getRulesEngine(t_env)->getObjectName(modelId,objectId);

  returnValue = EnvAddSymbol(t_env, const_cast<char *>(objectName.c_str()));
  return returnValue;
}

void *cloneModel(void *t_env) {
  DATA_OBJECT temp;
  void *returnValue;
  std::string clonedModelId;

  if (EnvArgCountCheck(t_env, const_cast<char *>("cloneModel"),EXACTLY,3) == -1) { 
    returnValue = EnvAddSymbol(t_env, const_cast<char *>(clonedModelId.c_str()));
    return returnValue;
  }

  // get model id
  if (EnvArgTypeCheck(t_env, const_cast<char *>("cloneModel"),2,STRING,&temp) == 0) { 
    returnValue = EnvAddSymbol(t_env, const_cast<char *>(clonedModelId.c_str()));
    return returnValue; 
  }
  std::string existingModelId = EnvDOToString(t_env, temp);

  // clone model
  clonedModelId = getRulesEngine(t_env)->cloneModel(existingModelId);

  returnValue = EnvAddSymbol(t_env, const_cast<char *>(clonedModelId.c_str()));
  return returnValue;
}

void *checkAttribute(void *t_env) {
  DATA_OBJECT temp;
  void *returnValue;
  // default error message for bad call from CLIPS
  std::string message = "Bad call from CLIPS to checkAttribute.";

  if (EnvArgCountCheck(t_env, const_cast<char *>("checkAttribute"),EXACTLY,5) == -1) { 
    getRulesEngine(t_env)->addToReport("Errors",message);
    returnValue = EnvAddSymbol(t_env, const_cast<char *>(message.c_str()));
    return returnValue;
  }

  // get modelId
  if (EnvArgTypeCheck(t_env, const_cast<char *>("checkAttribute"),1,STRING,&temp) == 0) { 
    getRulesEngine(t_env)->addToReport("Errors",message);
    returnValue = EnvAddSymbol(t_env, const_cast<char *>(message.c_str()));
    return returnValue;
  }
  std::string modelId = EnvDOToString(t_env, temp);

  // get objectType
  if (EnvArgTypeCheck(t_env, const_cast<char *>("checkAttribute"),2,STRING,&temp) == 0) { 
    getRulesEngine(t_env)->addToReport("Errors",message);
    returnValue = EnvAddSymbol(t_env, const_cast<char *>(message.c_str()));
    return returnValue;
  }
  std::string objectType = EnvDOToString(t_env, temp);

  // get objectId
  if (EnvArgTypeCheck(t_env, const_cast<char *>("checkAttribute"),3,STRING,&temp) == 0) { 
    getRulesEngine(t_env)->addToReport("Errors",message);
    returnValue = EnvAddSymbol(t_env, const_cast<char *>(message.c_str()));
    return returnValue;
  }
  std::string objectId = EnvDOToString(t_env, temp);

  // get attributeType
  if (EnvArgTypeCheck(t_env, const_cast<char *>("checkAttribute"),4,SYMBOL,&temp) == 0) { 
    getRulesEngine(t_env)->addToReport("Errors",message);
    returnValue = EnvAddSymbol(t_env, const_cast<char *>(message.c_str()));
    return returnValue;
  }
  standardsinterface::PartsOfSpeech attributeType(EnvDOToString(t_env, temp));

  // get attributeName
  if (EnvArgTypeCheck(t_env, const_cast<char *>("checkAttribute"),5,STRING,&temp) == 0) { 
    getRulesEngine(t_env)->addToReport("Errors",message);
    returnValue = EnvAddSymbol(t_env, const_cast<char *>(message.c_str()));
    return returnValue;
  }
  std::string attributeName = EnvDOToString(t_env, temp);

  // have arguments, get real message
  message = getRulesEngine(t_env)->checkAttribute(modelId,
                                                  objectType,
                                                  objectId,
                                                  attributeType,
                                                  attributeName);

  returnValue = EnvAddSymbol(t_env, const_cast<char *>(message.c_str()));
  return returnValue;
}

void *getBooleanAttribute(void *t_env) {
  DATA_OBJECT temp;
  void *returnValue;
  bool result;
  std::string booleanValue;

  if (EnvArgCountCheck(t_env, const_cast<char *>("getBooleanAttribute"),EXACTLY,4) == -1) { 
    returnValue = EnvAddSymbol(t_env, const_cast<char *>(booleanValue.c_str()));
    return returnValue;
  }

  // get model id
  if (EnvArgTypeCheck(t_env, const_cast<char *>("getBooleanAttribute"),1,STRING,&temp) == 0) { 
    returnValue = EnvAddSymbol(t_env, const_cast<char *>(booleanValue.c_str()));
    return returnValue; 
  }
  std::string modelId = EnvDOToString(t_env, temp);

  // get object id
  if (EnvArgTypeCheck(t_env, const_cast<char *>("getBooleanAttribute"),3,STRING,&temp) == 0) { 
    returnValue = EnvAddSymbol(t_env, const_cast<char *>(booleanValue.c_str()));
    return returnValue; 
  }
  std::string objectId = EnvDOToString(t_env, temp);

  // get attribute name
  if (EnvArgTypeCheck(t_env, const_cast<char *>("getBooleanAttribute"),4,STRING,&temp) == 0) { 
    returnValue = EnvAddSymbol(t_env, const_cast<char *>(booleanValue.c_str()));
    return returnValue; 
  }
  std::string attributeName = EnvDOToString(t_env, temp);

  result = getRulesEngine(t_env)->getBooleanAttribute(modelId,objectId,attributeName);
  if (result) { booleanValue = "TRUE"; }
  else { booleanValue = "FALSE"; }

  returnValue = EnvAddSymbol(t_env, const_cast<char *>(booleanValue.c_str()));
  return returnValue;
}

double getDoubleAttribute(void *t_env) {
  DATA_OBJECT temp;
  double doubleValue(0.0);

  if (EnvArgCountCheck(t_env, const_cast<char *>("getDoubleAttribute"),EXACTLY,4) == -1) { 
    BOOST_ASSERT(false);
  }

  // get model id
  if (EnvArgTypeCheck(t_env, const_cast<char *>("getDoubleAttribute"),1,STRING,&temp) == 0) { 
    BOOST_ASSERT(false);
  }
  std::string modelId = EnvDOToString(t_env, temp);

  // get object id
  if (EnvArgTypeCheck(t_env, const_cast<char *>("getDoubleAttribute"),3,STRING,&temp) == 0) { 
    BOOST_ASSERT(false);
  }
  std::string objectId = EnvDOToString(t_env, temp);

  // get attribute name
  if (EnvArgTypeCheck(t_env, const_cast<char *>("getDoubleAttribute"),4,STRING,&temp) == 0) { 
    BOOST_ASSERT(false);
  }
  std::string attributeName = EnvDOToString(t_env, temp);

  doubleValue = getRulesEngine(t_env)->getDoubleAttribute(modelId,objectId,attributeName);
  return doubleValue;
}

void *getEnumAttribute(void *t_env) {
  DATA_OBJECT temp;
  void *returnValue;
  std::string enumValue;

  if (EnvArgCountCheck(t_env, const_cast<char *>("getEnumAttribute"),EXACTLY,4) == -1) { 
    returnValue = EnvAddSymbol(t_env, const_cast<char *>(enumValue.c_str()));
    return returnValue;
  }

  // get model id
  if (EnvArgTypeCheck(t_env, const_cast<char *>("getEnumAttribute"),1,STRING,&temp) == 0) { 
    returnValue = EnvAddSymbol(t_env, const_cast<char *>(enumValue.c_str()));
    return returnValue; 
  }
  std::string modelId = EnvDOToString(t_env, temp);

  // get object id
  if (EnvArgTypeCheck(t_env, const_cast<char *>("getEnumAttribute"),3,STRING,&temp) == 0) { 
    returnValue = EnvAddSymbol(t_env, const_cast<char *>(enumValue.c_str()));
    return returnValue; 
  }
  std::string objectId = EnvDOToString(t_env, temp);

  // get attribute name
  if (EnvArgTypeCheck(t_env, const_cast<char *>("getEnumAttribute"),4,STRING,&temp) == 0) { 
    returnValue = EnvAddSymbol(t_env, const_cast<char *>(enumValue.c_str()));
    return returnValue; 
  }
  std::string attributeName = EnvDOToString(t_env, temp);

  enumValue = getRulesEngine(t_env)->getEnumAttribute(modelId,objectId,attributeName);

  returnValue = EnvAddSymbol(t_env, const_cast<char *>(enumValue.c_str()));
  return returnValue;
}

int getIntegerAttribute(void *t_env) {
  DATA_OBJECT temp;
  int integerValue(0);

  if (EnvArgCountCheck(t_env, const_cast<char *>("getIntegerAttribute"),EXACTLY,4) == -1) { 
    BOOST_ASSERT(false);
  }

  // get model id
  if (EnvArgTypeCheck(t_env, const_cast<char *>("getIntegerAttribute"),1,STRING,&temp) == 0) { 
    BOOST_ASSERT(false);
  }
  std::string modelId = EnvDOToString(t_env, temp);

  // get object id
  if (EnvArgTypeCheck(t_env, const_cast<char *>("getIntegerAttribute"),3,STRING,&temp) == 0) { 
    BOOST_ASSERT(false);
  }
  std::string objectId = EnvDOToString(t_env, temp);

  // get attribute name
  if (EnvArgTypeCheck(t_env, const_cast<char *>("getIntegerAttribute"),4,STRING,&temp) == 0) { 
    BOOST_ASSERT(false);
  }
  std::string attributeName = EnvDOToString(t_env, temp);

  integerValue = getRulesEngine(t_env)->getIntegerAttribute(modelId,objectId,attributeName);
  return integerValue;
}

