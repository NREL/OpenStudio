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

#include <xercesc/framework/MemBufInputSource.hpp>

namespace openstudio {

class WStr
{
 private:
  XMLCh* wStr;

 public:
  WStr(const char* str) {
    wStr = xercesc::XMLString::transcode(str);
  }
  /* 
    ~WStr() {
      xercesc::XMLString::release(&amp;wStr);
    } */

  operator const XMLCh*() const {
    return wStr;
  }
};

Validator::Validator(const openstudio::path& xsdPath) : m_xsdPath(openstudio::filesystem::system_complete(xsdPath)) {
  if (!openstudio::filesystem::exists(xsdPath)) {
    LOG_AND_THROW("'" << toString(xsdPath) << "' does not exist");
  } else if (!openstudio::filesystem::is_regular_file(xsdPath)) {
    LOG_AND_THROW("'" << toString(xsdPath) << "' cannot be opened for reading XSD data");
  }

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
    LOG_AND_THROW("'" << toString(xmlPath) << "' does not exist");
  } else if (!openstudio::filesystem::is_regular_file(xmlPath)) {
    LOG_AND_THROW("'" << toString(xmlPath) << "' cannot be opened for reading XML data");
  }

  xercesc::XercesDOMParser parser;

  /*   char *str = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" elementFormDefault=\"qualified\" attributeFormDefault=\"unqualified\"> \r\n </xs:schema>";
  std::string strContent = str;
  xercesc::MemBufInputSource pMemBufIS((XMLByte*)strContent.c_str(), strContent.size(), "xsd");
  if (parser.loadGrammar(pMemBufIS, xercesc::Grammar::SchemaGrammarType) == NULL) {
    LOG_AND_THROW("Could not load.");
  } */

  /*   std::string xsdFile = "";
  xercesc::MemBufInputSource inMemorySchemaSource(reinterpret_cast<const XMLByte*>(xsdFile.c_str()), xsdFile.size (), "/schema.xsd");
  xercesc::Grammar* grammar = parser.loadGrammar(inMemorySchemaSource, xercesc::Grammar::SchemaGrammarType, true); */

  // Let's preparse the schema grammar (.xsd) and cache it.
  xercesc::Grammar* grammar = parser.loadGrammar(m_xsdPath, xercesc::Grammar::SchemaGrammarType, true);

  // Enable schema processing.
  parser.setDoSchema(true);
  parser.setDoNamespaces(true);

  // Enable grammar caching
  parser.cacheGrammarFromParse(true);

  parser.parse(toString(xmlPath));
}

}  // namespace openstudio