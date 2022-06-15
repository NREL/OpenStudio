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

XMLValidator::XMLValidator(const openstudio::path& schemaPath, const openstudio::path& xmlPath)
  : m_schemaPath(openstudio::filesystem::system_complete(schemaPath)), m_xmlPath(openstudio::filesystem::system_complete(xmlPath)) {

  if (!openstudio::filesystem::exists(schemaPath)) {
    LOG_AND_THROW("'" << toString(schemaPath) << "' does not exist");
  } else if (!openstudio::filesystem::is_regular_file(schemaPath)) {
    LOG_AND_THROW("'" << toString(schemaPath) << "' cannot be opened");
  }

  if (!openstudio::filesystem::exists(xmlPath)) {
    LOG_AND_THROW("'" << toString(xmlPath) << "' does not exist");
  } else if (!openstudio::filesystem::is_regular_file(xmlPath)) {
    LOG_AND_THROW("'" << toString(xmlPath) << "' cannot be opened");
  }

  if (schemaPath.extension() == ".xsd") {
    m_xsdPath = schemaPath;
  } else if (schemaPath.extension() == ".xml") {
    m_schematronPath = schemaPath;
  } else {
    LOG_AND_THROW("Schema path extension '" << toString(schemaPath.extension()) << "' not supported.");
  }

  setParser();
}

openstudio::path XMLValidator::schemaPath() const {
  return m_schemaPath;
}

openstudio::path XMLValidator::xmlPath() const {
  return m_xmlPath;
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

bool XMLValidator::validate() const {

  bool ok;
  if (m_xsdPath) {
    ok = xsdValidate();
  } else if (m_schematronPath) {
    ok = schematronValidate();
  }

  return ok;
}

void err(void* ctx, const char* msg) {
  // TODO
}

void warn(void* ctx, const char* msg) {
  // TODO
}

bool XMLValidator::xsdValidate() const {

  // schema path
  auto schema_filename_str = toString(m_xsdPath.value());
  const auto* schema_filename = schema_filename_str.c_str();

  // xml path
  auto xml_filename_str = toString(m_xmlPath);
  const auto* xml_filename = xml_filename_str.c_str();

  // schema parser ptr
  xmlSchemaParserCtxtPtr parser_ctxt = xmlSchemaNewParserCtxt(schema_filename);

  // set parser errors
  // https://stackoverflow.com/questions/11901206/libxml2-suppress-the-debug-output-on-console
  // https://cpp.hotexamples.com/examples/-/-/xmlSchemaNewMemParserCtxt/cpp-xmlschemanewmemparserctxt-function-examples.html
  xmlSchemaSetParserErrors(parser_ctxt, (xmlSchemaValidityErrorFunc)err, (xmlSchemaValidityWarningFunc)warn, NULL);

  // schema parser
  xmlSchemaPtr schema = xmlSchemaParse(parser_ctxt);
  xmlSchemaValidCtxtPtr ctxt = xmlSchemaNewValidCtxt(schema);

  // set valid errors
  xmlSchemaSetValidErrors(ctxt, (xmlSchemaValidityErrorFunc)err, (xmlSchemaValidityWarningFunc)warn, NULL);

  // xml doc ptr
  xmlDocPtr doc = xmlParseFile(xml_filename);

  // validate doc
  int ret = xmlSchemaValidateDoc(ctxt, doc);

  // free
  xmlSchemaFreeParserCtxt(parser_ctxt);
  xmlSchemaFreeValidCtxt(ctxt);
  xmlSchemaFree(schema);

  xmlFreeDoc(doc);
  xmlCleanupParser();

  return true;
}

bool XMLValidator::schematronValidate() const {

  auto schema_filename_str = toString(m_schematronPath.value());
  const auto* schema_filename = schema_filename_str.c_str();

  return true;
}

void XMLValidator::setParser() {
  // TODO
}

}  // namespace openstudio
