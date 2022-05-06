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

#include "XMLValidator.hpp"

#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/validators/common/Grammar.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>

namespace openstudio {

class ParserErrorHandler : public xercesc::ErrorHandler
{
 private:
  void reportParseException(const xercesc::SAXParseException& ex) {
    char* msg = xercesc::XMLString::transcode(ex.getMessage());
    fprintf(stderr, "at line %llu column %llu, %s\n", ex.getLineNumber(), ex.getColumnNumber(), msg);
    xercesc::XMLString::release(&msg);
  }

 public:
  void warning(const xercesc::SAXParseException& ex) {
    reportParseException(ex);
  }

  void error(const xercesc::SAXParseException& ex) {
    reportParseException(ex);
  }

  void fatalError(const xercesc::SAXParseException& ex) {
    reportParseException(ex);
  }

  void resetErrors() {}
};

XMLValidator::XMLValidator(const openstudio::path& xsdPath) : m_xsdPath(openstudio::filesystem::system_complete(xsdPath)) {
  if (!openstudio::filesystem::exists(xsdPath)) {
    LOG_AND_THROW("'" << toString(xsdPath) << "' does not exist");
  } else if (!openstudio::filesystem::is_regular_file(xsdPath)) {
    LOG_AND_THROW("'" << toString(xsdPath) << "' XSD cannot be opened");
  }

  xercesc::XMLPlatformUtils::Initialize();

  // Let's preparse the schema grammar (.xsd) and cache it.
  if (m_parser.loadGrammar(toString(m_xsdPath.get()).c_str(), xercesc::Grammar::SchemaGrammarType) == NULL) {
    LOG_AND_THROW("'" << toString(m_xsdPath.get()) << "' XSD cannot be loaded");
  }

  setParser();
}

XMLValidator::XMLValidator(const std::string& xsdString) : m_xsdString(xsdString) {
  xercesc::XMLPlatformUtils::Initialize();

  xercesc::MemBufInputSource _xsdString(reinterpret_cast<const XMLByte*>(m_xsdString.get().c_str()), m_xsdString.get().size(), "xsd");

  // Let's preparse the schema grammar (.xsd) and cache it.
  if (m_parser.loadGrammar(_xsdString, xercesc::Grammar::SchemaGrammarType) == NULL) {
    LOG_AND_THROW("xmlString XSD cannot be loaded");
  }

  setParser();
}

boost::optional<openstudio::path> XMLValidator::xsdPath() const {
  return m_xsdPath;
}

boost::optional<std::string> XMLValidator::xsdString() const {
  return m_xsdString;
}

std::vector<std::string> XMLValidator::errors() const {
  return m_errors;
}

std::vector<std::string> XMLValidator::warnings() const {
  return m_warnings;
}

bool XMLValidator::isValid() const {
  return (m_errors.size() == 0u);
}

bool XMLValidator::validate(const openstudio::path& xmlPath) {
  if (!openstudio::filesystem::exists(xmlPath)) {
    LOG(Error, "'" << toString(xmlPath) << "' does not exist");
    return false;
  } else if (!openstudio::filesystem::is_regular_file(xmlPath)) {
    LOG(Error, "'" << toString(xmlPath) << "' XML cannot be opened");
    return false;
  }

  // Let's parse the XML document. The parser will cache any grammars encountered.
  m_parser.parse(toString(openstudio::filesystem::system_complete(xmlPath)).c_str());

  unsigned int errorCount = m_parser.getErrorCount();
  if (errorCount > 0) {
    LOG(Error, "'" << toString(xmlPath) << "' has " << errorCount << " error(s)");
    return false;
  }

  return true;
}

bool XMLValidator::validate(const std::string& xmlString) {
  xercesc::MemBufInputSource _xmlString(reinterpret_cast<const XMLByte*>(xmlString.c_str()), xmlString.size(), "xml");

  // Let's parse the XML document. The parser will cache any grammars encountered.
  m_parser.parse(_xmlString);

  unsigned int errorCount = m_parser.getErrorCount();
  if (errorCount > 0) {
    LOG(Error, "xmlString has " << errorCount << " error(s)");
    return false;
  }

  return true;
}

void XMLValidator::setParser() {
  // Enable schema processing.
  m_parser.setDoSchema(true);
  m_parser.setDoNamespaces(true);
  m_parser.setValidationConstraintFatal(true);
  m_parser.setValidationSchemaFullChecking(true);
  m_parser.setValidationScheme(xercesc::XercesDOMParser::Val_Always);

  ParserErrorHandler errorHandler;
  m_parser.setErrorHandler(&errorHandler);

  // Enable grammar caching
  m_parser.cacheGrammarFromParse(true);
}

}  // namespace openstudio
