/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "Validator.hpp"

#include <xercesc/util/XMLString.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/validators/common/Grammar.hpp>

namespace openstudio {

Validator::Validator(const openstudio::path& xsdPath) : m_xsdPath(openstudio::filesystem::system_complete(xsdPath)) {
  if (!openstudio::filesystem::exists(xsdPath)) {
    LOG_AND_THROW("'" << toString(xsdPath) << "' does not exist");
  } else if (!openstudio::filesystem::is_regular_file(xsdPath)) {
    LOG_AND_THROW("'" << toString(xsdPath) << "' XSD cannot be opened");
  }

  xercesc::XMLPlatformUtils::Initialize();

  // create the parser here?
}

Validator::Validator(const std::string& xsdString) : m_xsdString(xsdString) {
  // create the parser here?
}

openstudio::path Validator::xsdPath() const {
  return m_xsdPath;
}

std::vector<std::string> Validator::errors() const {
  return m_errors;
}

std::vector<std::string> Validator::warnings() const {
  return m_warnings;
}

bool Validator::isValid() const {
  return (m_errors.size() == 0u);
}

bool Validator::validate(const openstudio::path& xmlPath) const {
  if (!openstudio::filesystem::exists(xmlPath)) {
    //LOG_AND_THROW("'" << toString(xmlPath) << "' does not exist");
  } else if (!openstudio::filesystem::is_regular_file(xmlPath)) {
    //LOG_AND_THROW("'" << toString(xmlPath) << "' XML cannot be opened");
  }

  xercesc::XercesDOMParser parser;

  // Let's preparse the schema grammar (.xsd) and cache it.
  if (parser.loadGrammar(toString(m_xsdPath).c_str(), xercesc::Grammar::SchemaGrammarType) == NULL) {
    LOG_AND_THROW("'" << toString(m_xsdPath) << "' XSD cannot be loaded");
  }

  // Enable schema processing.
  parser.setDoSchema(true);
  parser.setDoNamespaces(true);

  // Enable grammar caching
  parser.cacheGrammarFromParse(true);

  parser.parse(toString(openstudio::filesystem::system_complete(xmlPath)).c_str());

  if (parser.getErrorCount() > 0) {
    return false;
  }

  return true;
}

bool Validator::validate(const std::string& xmlString) const {
  return true;
}

}  // namespace openstudio
