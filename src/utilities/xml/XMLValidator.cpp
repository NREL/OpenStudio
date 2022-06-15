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
#include "XMLErrors.hpp"
#include "XMLUtils.hpp"

#include <libxml/xmlversion.h>
#include <libxml/xmlreader.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/schematron.h>

#include <fmt/format.h>

#include <algorithm>
#include <iterator>

namespace openstudio {

XMLValidator::XMLValidator(const openstudio::path& schemaPath, const openstudio::path& xmlPath)
  : m_schemaPath(openstudio::filesystem::system_complete(schemaPath)), m_xmlPath(openstudio::filesystem::system_complete(xmlPath)) {

  m_logSink.setLogLevel(Warn);
  m_logSink.setChannelRegex(boost::regex("openstudio\\.XMLValidator"));
  m_logSink.setThreadId(std::this_thread::get_id());

  if (!openstudio::filesystem::exists(schemaPath)) {
    LOG_AND_THROW("Schema '" << toString(schemaPath) << "' does not exist");
  } else if (!openstudio::filesystem::is_regular_file(schemaPath)) {
    LOG_AND_THROW("Schema '" << toString(schemaPath) << "' cannot be opened");
  }

  if (!openstudio::filesystem::exists(xmlPath)) {
    LOG_AND_THROW("XML File '" << toString(xmlPath) << "' does not exist");
  } else if (!openstudio::filesystem::is_regular_file(xmlPath)) {
    LOG_AND_THROW("XML File '" << toString(xmlPath) << "' cannot be opened");
  }

  if (schemaPath.extension() == ".xsd") {
    m_validatorType = XMLValidatorType::XSD;
    LOG(Info, "Treating schema as a regular XSD");
  } else if ((schemaPath.extension() == ".xml") || (schemaPath.extension() == ".sct")) {
    LOG(Info, "Treating schema as a Schematron");
    m_validatorType = XMLValidatorType::Schematron;
  } else {
    LOG_AND_THROW("Schema path extension '" << toString(schemaPath.extension()) << "' not supported.");
  }
}

openstudio::path XMLValidator::schemaPath() const {
  return m_schemaPath;
}

openstudio::path XMLValidator::xmlPath() const {
  return m_xmlPath;
}

std::vector<LogMessage> XMLValidator::errors() const {
  auto logMessages = m_logSink.logMessages();

  std::vector<LogMessage> result;
  std::copy_if(logMessages.cbegin(), logMessages.cend(), std::back_inserter(result),
               [](const auto& logMessage) { return logMessage.logLevel() > LogLevel::Warn; });

  return result;
}

std::vector<LogMessage> XMLValidator::warnings() const {
  auto logMessages = m_logSink.logMessages();

  std::vector<LogMessage> result;
  std::copy_if(logMessages.cbegin(), logMessages.cend(), std::back_inserter(result),
               [](const auto& logMessage) { return logMessage.logLevel() == LogLevel::Warn; });

  return result;
}

bool XMLValidator::isValid() const {
  return errors().empty();
}

bool XMLValidator::validate() const {

  if (m_validatorType == XMLValidatorType::XSD) {
    return xsdValidate();
  } else {  // if (m_validatorType == XMLValidatorType::Schematron)
    return xsltValidate();
  }
}

bool XMLValidator::xsdValidate() const {

  // schema path
  auto schema_filename_str = toString(m_schemaPath);
  const auto* schema_filename = schema_filename_str.c_str();

  // xml path
  auto xml_filename_str = toString(m_xmlPath);
  const auto* xml_filename = xml_filename_str.c_str();

  // schema parser ptr
  xmlSchemaParserCtxt* parser_ctxt = xmlSchemaNewParserCtxt(schema_filename);

  // set parser errors
  detail::ErrorCollector schemaParserErrorCollector;
  xmlSchemaSetParserErrors(parser_ctxt, detail::callback_messages_error, detail::callback_messages_warning, &schemaParserErrorCollector);
  for (auto& logMessage : schemaParserErrorCollector.logMessages) {
    LOG(logMessage.logLevel(), "xsdValidate.schemaParserError: " + logMessage.logMessage())
  }

  // schema parser
  xmlSchema* schema = xmlSchemaParse(parser_ctxt);
  xmlSchemaValidCtxt* ctxt = xmlSchemaNewValidCtxt(schema);

  // set valid errors
  detail::ErrorCollector schemaValidErrorCollector;
  xmlSchemaSetValidErrors(ctxt, detail::callback_messages_error, detail::callback_messages_warning, &schemaValidErrorCollector);
  for (auto& logMessage : schemaValidErrorCollector.logMessages) {
    LOG(logMessage.logLevel(), "xsdValidate.schemaValidError: " + logMessage.logMessage())
  }

  // xml doc ptr
  xmlDoc* doc = xmlParseFile(xml_filename);

  // validate doc
  int ret = xmlSchemaValidateDoc(ctxt, doc);
  bool result = false;
  if (ret > 0) {
    LOG(Fatal, "Valid instance " << toString(m_xmlPath) << " failed to validate against " << toString(m_schemaPath));
    result = false;
  } else if (ret < 0) {
    LOG(Fatal, "Valid instance " << toString(m_xmlPath) << " got internal error validating against " << toString(m_schemaPath));
    result = true;
  } else {
    result = true;
  }

  // free
  xmlSchemaFreeParserCtxt(parser_ctxt);
  xmlSchemaFreeValidCtxt(ctxt);
  xmlSchemaFree(schema);

  xmlFreeDoc(doc);
  xmlCleanupParser();

  return result;
}

bool XMLValidator::schematronValidate() const {

  auto schema_filename_str = toString(m_schematronPath.value());
  const auto* schema_filename = schema_filename_str.c_str();

  return true;
}

}  // namespace openstudio
