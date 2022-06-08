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

#include <libxml/xmlversion.h>
#include <libxml/xmlreader.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/schematron.h>

namespace openstudio {

XMLValidator::XMLValidator(const openstudio::path& xsdPath) : m_xsdPath(openstudio::filesystem::system_complete(xsdPath)) {
  if (!openstudio::filesystem::exists(xsdPath)) {
    LOG_AND_THROW("'" << toString(xsdPath) << "' does not exist");
  } else if (!openstudio::filesystem::is_regular_file(xsdPath)) {
    LOG_AND_THROW("'" << toString(xsdPath) << "' XSD cannot be opened");
  }

  // TODO

  setParser();
}

XMLValidator::XMLValidator(const std::string& xsdString) : m_xsdString(xsdString) {
  // TODO

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
  return m_errors.empty();
}

bool XMLValidator::xsdValidate(const openstudio::path& xmlPath) {
  if (!openstudio::filesystem::exists(xmlPath)) {
    LOG(Error, "'" << toString(xmlPath) << "' does not exist");
    return false;
  } else if (!openstudio::filesystem::is_regular_file(xmlPath)) {
    LOG(Error, "'" << toString(xmlPath) << "' XML cannot be opened");
    return false;
  }

  auto schema_filename_str = openstudio::toString(m_xsdPath.value());
  const auto* schema_filename = schema_filename_str.c_str();

  xmlSchemaParserCtxt* parser_ctxt = nullptr;
  parser_ctxt = xmlSchemaNewParserCtxt(schema_filename);
  if (parser_ctxt == nullptr) {
    //throw std::runtime_error("Memory error reading schema in xmlSchematronNewParserCtxt");
  }

  xmlSchema* schema = nullptr;
  schema = xmlSchemaParse(parser_ctxt);
  xmlSchemaFreeParserCtxt(parser_ctxt);

  // Start on the document to validate side
  auto filename_str = openstudio::toString(xmlPath);
  const auto* filename = filename_str.c_str();

  xmlDoc* doc = nullptr;
  //doc = xmlReadFile(filename, nullptr, 0);
  doc = xmlParseFile(filename);

  xmlSchemaValidCtxt* ctxt = nullptr;
  ctxt = xmlSchemaNewValidCtxt(schema);
  if (ctxt == nullptr) {
    //throw std::runtime_error("Memory error reading schema in xmlSchematronNewValidCtxt");
  }

  int ret = xmlSchemaValidateDoc(ctxt, doc);
  //xmlTextReaderSchemaValidateCtxt(doc, ctxt, 0);

  //if (ret == 0) {
  //fmt::print(stderr, "{} validates\n", filename);
  //} else if (ret > 0) {
  //fmt::print(stderr, "{} fails to validate\n", filename);
  //} else {
  //fmt::print(stderr, "{} validation generated an internal error, ret = {}\n", filename, ret);
  //}
  xmlSchemaFreeValidCtxt(ctxt);
  xmlSchemaFree(schema);

  xmlFreeDoc(doc);     // free document
  xmlCleanupParser();  // free globals

  return true;
}

bool XMLValidator::validate(const std::string& xmlString) {
  // TODO

  return true;
}

void XMLValidator::setParser() {
  // TODO
}

}  // namespace openstudio
