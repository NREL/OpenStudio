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

#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>  // BAD_CAST

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

  // schema parser
  xmlSchema* schema = xmlSchemaParse(parser_ctxt);
  xmlSchemaValidCtxt* ctxt = xmlSchemaNewValidCtxt(schema);

  // set valid errors
  detail::ErrorCollector schemaValidErrorCollector;
  xmlSchemaSetValidErrors(ctxt, detail::callback_messages_error, detail::callback_messages_warning, &schemaValidErrorCollector);

  detail::ErrorCollector parseFileErrorCollector;
  xmlSetStructuredErrorFunc(&parseFileErrorCollector, detail::callback_messages_structured_error);
  xmlSetGenericErrorFunc(&parseFileErrorCollector, detail::callback_messages_error);

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

  for (auto& logMessage : schemaValidErrorCollector.logMessages) {
    LOG(logMessage.logLevel(), "xsdValidate.schemaValidError: " + logMessage.logMessage())
  }

  for (auto& logMessage : schemaParserErrorCollector.logMessages) {
    LOG(logMessage.logLevel(), "xsdValidate.schemaParserError: " + logMessage.logMessage())
  }

  for (auto& logMessage : parseFileErrorCollector.logMessages) {
    LOG(logMessage.logLevel(), "xsdValidate.parseFileError: " + logMessage.logMessage())
  }

  // free
  xmlSchemaFreeParserCtxt(parser_ctxt);
  xmlSchemaFreeValidCtxt(ctxt);
  xmlSchemaFree(schema);

  xmlFreeDoc(doc);
  xmlCleanupParser();

  return result;
}

std::vector<std::string> processXSLTApplyResult(xmlDoc* res) {

  xmlXPathContext* xpathCtx = nullptr;
  xmlXPathObject* xpathObj = nullptr;

  const char* xpathExpr = "//svrl:failed-assert";
  /* Create xpath evaluation context */
  xpathCtx = xmlXPathNewContext(res);
  if (xpathCtx == nullptr) {
    throw std::runtime_error("Error: unable to create new XPath context");
  }

  // namesapce
  // xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
  if (xmlXPathRegisterNs(xpathCtx, BAD_CAST "svrl", BAD_CAST "http://purl.oclc.org/dsdl/svrl") != 0) {
    throw std::runtime_error("Error: unable to register NS svrl with prefix");
  }

  /* Evaluate xpath expression */
  xpathObj = xmlXPathEvalExpression((const xmlChar*)xpathExpr, xpathCtx);
  if (xpathObj == nullptr) {
    xmlXPathFreeContext(xpathCtx);
    throw std::runtime_error(fmt::format("Error: unable to evaluate xpath expression '{}'\n", xpathExpr));
  }

  std::vector<std::string> result;

  if (xmlXPathNodeSetIsEmpty(xpathObj->nodesetval)) {
    fmt::print("No errors\n");
    return {};
  } else {

    xmlNodeSet* nodeset = xpathObj->nodesetval;
    for (int i = 0; i < nodeset->nodeNr; i++) {
      xmlNode* error_node = nodeset->nodeTab[i];
      error_node = error_node->xmlChildrenNode;
      detail::xmlchar_helper error_message(xmlNodeListGetString(res, error_node->xmlChildrenNode, 1));

      result.emplace_back(error_message.get());
      // while (error_node != nullptr) {
      //   if ((xmlStrcmp(error_node->name, (const xmlChar*)"text") == 0)) {
      //     keyword = xmlNodeListGetString(doc, error_node->xmlChildrenNode, 1);
      //     printf("text: %s\n", keyword);
      //   }

      //   error_node = error_node->next;
      // }
    }
  }

  /* Cleanup of XPath data */
  xmlXPathFreeObject(xpathObj);
  xmlXPathFreeContext(xpathCtx);

  return result;
}

std::string dumpXSLTApplyResultToString(xmlDoc* res, xsltStylesheet* style) {

  xmlChar* xml_string = nullptr;
  int xml_string_length = 0;

  std::string result;

  if (xsltSaveResultToString(&xml_string, &xml_string_length, res, style) == 0) {

    detail::xmlchar_helper helper(xml_string);
    if (xml_string_length > 0) {
      result.assign(helper.get(), detail::checked_size_t_cast(xml_string_length));
    }
  }

  // std::string result((char*)xml_string);
  // xmlFree(xml_string);
  return result;
}

bool XMLValidator::xsltValidate() const {

  const char* params[16 + 1];
  int nbparams = 0;
  params[nbparams] = nullptr;
  xmlSubstituteEntitiesDefault(1);
  xmlLoadExtDtdDefaultValue = 1;

  auto schematron_filename_str = openstudio::toString(m_schemaPath);
  const auto* schematron_filename = schematron_filename_str.c_str();
  xsltStylesheet* style = xsltParseStylesheetFile((const xmlChar*)schematron_filename);

  auto filename_str = openstudio::toString(m_xmlPath);
  const auto* filename = filename_str.c_str();
  xmlDoc* doc = xmlParseFile(filename);
  xmlDoc* res = xsltApplyStylesheet(style, doc, params);

  // Dump result of xlstApply
  m_fullValidationReport = dumpXSLTApplyResultToString(res, style);
  fmt::print("\n====== Full Validation Report =====\n\n{}", m_fullValidationReport);
  // xsltSaveResultToFile(stdout, res, style);

  auto m_errors = processXSLTApplyResult(res);
  for (const auto& error : m_errors) {
    // m_logMessages.emplace_back(LogLevel::Error, "processXSLTApplyResult", error);
    LOG(Error, "processXSLTApplyResult: " << error);
  }

  /* dump the resulting document */
  // xmlDocDump(stdout, res);

  xsltFreeStylesheet(style);
  xmlFreeDoc(res);
  xmlFreeDoc(doc);

  xsltCleanupGlobals();
  xmlCleanupParser();

  return true;
}

boost::optional<std::string> XMLValidator::fullValidationReport() const {

  if (m_validatorType == XMLValidatorType::Schematron) {
    return m_fullValidationReport;
  }

  return boost::none;
}

}  // namespace openstudio
