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
#include "XMLInitializer.hpp"

#include "utilities/core/Filesystem.hpp"
#include "utilities/core/FilesystemHelpers.hpp"

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

#include <src/utilities/embedded_files.hxx>

#include <fmt/format.h>

#include <algorithm>
#include <iterator>

namespace openstudio {

xmlDoc* applyEmbeddedXSLT(const std::string& embedded_path, xmlDoc* curdoc, const char** params) {

  std::string xlstString = ::openstudio::embedded_files::getFileAsString(embedded_path);
  xmlDoc* xlstDoc = xmlParseDoc(detail::xml_string(xlstString));
  xsltStylesheet* style = xsltParseStylesheetDoc(xlstDoc);
  xmlDoc* res = xsltApplyStylesheet(style, curdoc, params);

  xsltFreeStylesheet(style);
  // xmlFreeDoc(xlstDoc); // This is already freed via xlstFreeStyleSheet because we used xsltParseStylesheetDoc
  xmlFreeDoc(curdoc);
  return res;
}

xmlDoc* applyEmbeddedXSLTWithImports(xmlDoc* curdoc, const openstudio::path& outputDir, const char** params) {

  // Extract the two files we need
  bool quiet = true;

  ::openstudio::embedded_files::extractFile(":/xml/resources/iso_svrl_for_xslt1.xsl", openstudio::toString(outputDir), quiet);
  ::openstudio::embedded_files::extractFile(":/xml/resources/iso_schematron_skeleton_for_xslt1.xsl", openstudio::toString(outputDir), quiet);

  auto schematron_filename_str = openstudio::toString(outputDir / "iso_svrl_for_xslt1.xsl");
  xsltStylesheet* style = xsltParseStylesheetFile(detail::xml_string(schematron_filename_str));

  xmlDoc* res = xsltApplyStylesheet(style, curdoc, params);

  xsltFreeStylesheet(style);
  xmlFreeDoc(curdoc);

  return res;
}

openstudio::path XMLValidator::schematronToXslt(const openstudio::path& schemaPath, const openstudio::path& outputDir) {

  // We need to potentially initialize the libxml2/libxslt stylesheets since this is a static function
  xmlInitializerInstance();

  // This replicates what happens when you do:
  // from lxml.isoschematron import Schematron;
  // s = Schematron(file='schematron.sct', store_xslt=True)
  // with open('schematron.xslt', 'w') as f:
  //     f.write(str(s._validator_xslt))

  auto filename_str = openstudio::toString(schemaPath);
  const auto* filename = filename_str.c_str();
  xmlDoc* schematronXmlDoc = xmlParseFile(filename);

  // For debugging
  constexpr bool saveIntermediates = false;

  if (!openstudio::filesystem::exists(outputDir)) {
    openstudio::filesystem::create_directories(outputDir);
  } else if (!openstudio::filesystem::is_directory(outputDir)) {
    LOG_AND_THROW("Path '" << toString(outputDir) << "' already exists but is not a directory.");
  }

  auto saveXmlDocToFile = [](const openstudio::path& outputPath, xmlDoc* doc) {
    auto save_filename_str = openstudio::toString(outputPath);
    const auto* save_filename = save_filename_str.c_str();
    const int result = xmlSaveFormatFileEnc(save_filename, doc, "UTF-8", 1);  // 1 means format=true
    if (result == -1) {
      // xmlSaveFormatFileEnc returns the number of bytes written or -1 if it failed
      LOG_AND_THROW("Writing to file failed for " << openstudio::toString(outputPath));
    }
  };

  // Extract

  // If we wanted to pass params, it's in this form where i=name and i+1 is the value, and it's null-terminated
  // constexpr int nbpairs = 1;
  // constexpr int nbparams = 2 * nbpairs;
  // const char * params[nbparams+1]:
  // params[0] = "name";
  // params[1] = "value";
  // params[nbparams] = nullptr;
  xmlSubstituteEntitiesDefault(1);
  xmlLoadExtDtdDefaultValue = 1;

  // include
  xmlDoc* withIncludes = applyEmbeddedXSLT(":/xml/resources/iso_dsdl_include.xsl", schematronXmlDoc, nullptr);
  if constexpr (saveIntermediates) {
    saveXmlDocToFile(outputDir / "1_withIncludes.xslt", withIncludes);
  }

  // expand
  xmlDoc* withExpand = applyEmbeddedXSLT(":/xml/resources/iso_dsdl_include.xsl", withIncludes, nullptr);
  if constexpr (saveIntermediates) {
    saveXmlDocToFile(outputDir / "2_withExpand.xslt", withIncludes);
  }

  // compile: this one uses an xsl:import ... not sure how to merge them...
  // xmlDoc* compiled = applyEmbeddedXSLT(":/xml/resources/iso_svrl_for_xslt1.xsl", withExpand, nullptr);
  xmlDoc* compiled = applyEmbeddedXSLTWithImports(withExpand, outputDir, nullptr);

  openstudio::path xsltPath = outputDir / openstudio::toPath(openstudio::toString(schemaPath.stem()) + "_stylesheet.xslt");

  if (openstudio::filesystem::is_regular_file(xsltPath)) {
    if (!openstudio::filesystem::remove(xsltPath)) {
      LOG_AND_THROW("Cannot remove existing file '" << toString(xsltPath) << "'.");
    }
  }

  // xsltPath.replace_extension(".xslt");
  saveXmlDocToFile(xsltPath, compiled);

  xmlFreeDoc(compiled);
  LOG(Info, "Saved transformed XSLT Stylesheet at '" << toString(xsltPath) << "'.");

  return xsltPath;
}

detail::XMLInitializer& XMLValidator::xmlInitializerInstance() {
  static detail::XMLInitializer xmlInitializer;
  return xmlInitializer;
}

XMLValidator::XMLValidator(const openstudio::path& schemaPath) : m_schemaPath(openstudio::filesystem::system_complete(schemaPath)) {

  xmlInitializerInstance();

  if (!openstudio::filesystem::exists(schemaPath)) {
    std::string logMessage = fmt::format("Schema '{}' does not exist", toString(schemaPath));
    m_logMessages.emplace_back(Fatal, "openstudio.XMLValidator", logMessage);
    LOG_AND_THROW(logMessage);
  } else if (!openstudio::filesystem::is_regular_file(schemaPath)) {
    std::string logMessage = fmt::format("Schema '{}' cannot be opened", toString(schemaPath));
    m_logMessages.emplace_back(Fatal, "openstudio.XMLValidator", logMessage);
    LOG_AND_THROW(logMessage);
  }

  if (schemaPath.extension() == ".xsd") {
    m_validatorType = XMLValidatorType::XSD;
    logAndStore(Info, "Treating schema as a regular XSD");
  } else if (schemaPath.extension() == ".xslt") {
    m_validatorType = XMLValidatorType::XSLTSchematron;
    logAndStore(Info, "Treating schema as a XLST StyleSheet that derives from a Schematron.");
  } else if ((schemaPath.extension() == ".xml") || (schemaPath.extension() == ".sct")) {
    m_validatorType = XMLValidatorType::Schematron;
    logAndStore(Info, "Treating schema as a Schematron, converting to an XSLT StyleSheet.");
    // Let's use a temporary directory for this, so we avoid having two instances trying to write to the same file and we avoid issues where the
    // directory is write protected.
    const auto tmpDir = openstudio::filesystem::create_temporary_directory("xmlvalidation");
    if (tmpDir.empty()) {
      LOG_AND_THROW(fmt::format("Failed to create a temporary directory for extracting the stylesheets need to transform the Schematron '{}'",
                                toString(schemaPath)));
    }
    m_schemaPath = schematronToXslt(m_schemaPath, tmpDir);
    logAndStore(Info, fmt::format("Transformed Schematron to an XSLT Stylesheet and saved it at {}.", toString(m_schemaPath)));
  } else {
    std::string logMessage = fmt::format("Schema path extension '{}' not supported.", toString(schemaPath.extension()));
    m_logMessages.emplace_back(Fatal, "openstudio.XMLValidator", logMessage);
    LOG_AND_THROW(logMessage);
  }
}

// XMLValidator::~XMLValidator() {
//   if (m_tempDir.empty()) {
//     try {
//       const auto count = openstudio::filesystem::remove_all(m_tempDir);
//       LOG(Debug, "XMLValidator", "Removed temporary directory with " << count << " files");
//     } catch (const std::exception& e) {
//       LOG(Warn, "Error removing temporary directory at '" << toString(m_tempDir) << "', Description: " << e.what());
//     }
//   }
// }

openstudio::path XMLValidator::schemaPath() const {
  return m_schemaPath;
}

boost::optional<openstudio::path> XMLValidator::xmlPath() const {
  return m_xmlPath;
}

void XMLValidator::logAndStore(LogLevel logLevel, const std::string& logMessage) const {
  m_logMessages.emplace_back(logLevel, "openstudio.XMLValidator", logMessage);
  LOG(logLevel, logMessage);
}

std::vector<LogMessage> XMLValidator::errors() const {

  std::vector<LogMessage> result;
  std::copy_if(m_logMessages.cbegin(), m_logMessages.cend(), std::back_inserter(result),
               [](const auto& logMessage) { return logMessage.logLevel() > LogLevel::Warn; });

  return result;
}

std::vector<LogMessage> XMLValidator::warnings() const {

  std::vector<LogMessage> result;
  std::copy_if(m_logMessages.cbegin(), m_logMessages.cend(), std::back_inserter(result),
               [](const auto& logMessage) { return logMessage.logLevel() == LogLevel::Warn; });

  return result;
}

bool XMLValidator::isValid() const {
  if (!m_xmlPath) {
    logAndStore(Warn, fmt::format("Nothing has yet been validated against '{}'", toString(m_schemaPath)));
    return false;
  }
  return errors().empty();
}

void XMLValidator::reset() {

  m_logMessages.clear(),

    m_xmlPath = boost::none;

  m_fullValidationReport.clear();
}

bool XMLValidator::validate(const openstudio::path& xmlPath) {

  reset();

  if (!openstudio::filesystem::exists(xmlPath)) {
    std::string logMessage = fmt::format("XML File '{}' does not exist.", toString(xmlPath));
    m_logMessages.emplace_back(Fatal, "openstudio.XMLValidator", logMessage);
    LOG_AND_THROW(logMessage);
  } else if (!openstudio::filesystem::is_regular_file(xmlPath)) {
    std::string logMessage = fmt::format("XML File '{}' cannot be opened.", toString(xmlPath));
    m_logMessages.emplace_back(Fatal, "openstudio.XMLValidator", logMessage);
    LOG_AND_THROW(logMessage);
  }

  if (xmlPath.extension() == ".xml") {
    auto t_xmlPath = openstudio::filesystem::system_complete(xmlPath);
    m_xmlPath = t_xmlPath;
  } else {
    std::string logMessage = fmt::format("XML path extension '{}' not supported.", toString(xmlPath.extension()));
    m_logMessages.emplace_back(Fatal, "openstudio.XMLValidator", logMessage);
    LOG_AND_THROW(logMessage);
  }

  if (m_validatorType == XMLValidatorType::XSD) {
    return xsdValidate();
  } else if ((m_validatorType == XMLValidatorType::XSLTSchematron) || (m_validatorType == XMLValidatorType::Schematron)) {
    return xsltValidate();
  }

  return false;
}

bool XMLValidator::xsdValidate() const {

  // schema path
  auto schema_filename_str = toString(m_schemaPath);
  const auto* schema_filename = schema_filename_str.c_str();

  // xml path
  auto xml_filename_str = toString(m_xmlPath.get());
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
    // LOG(Fatal, "Valid instance " << toString(m_xmlPath.get()) << " failed to validate against " << toString(m_schemaPath));
    result = false;
  } else if (ret < 0) {
    logAndStore(Fatal,
                fmt::format("Valid instance '{}' got internal error validating against '{}'", toString(m_xmlPath.get()), toString(m_schemaPath)));
    result = false;
  } else {
    result = true;
  }

  m_logMessages.reserve(m_logMessages.size() + schemaValidErrorCollector.logMessages.size() + schemaParserErrorCollector.logMessages.size()
                        + parseFileErrorCollector.logMessages.size());

  for (auto& logMessage : schemaValidErrorCollector.logMessages) {
    logAndStore(logMessage.logLevel(), "xsdValidate.schemaValidError: " + logMessage.logMessage());
  }

  for (auto& logMessage : schemaParserErrorCollector.logMessages) {
    logAndStore(logMessage.logLevel(), "xsdValidate.schemaParserError: " + logMessage.logMessage());
  }

  for (auto& logMessage : parseFileErrorCollector.logMessages) {
    logAndStore(logMessage.logLevel(), "xsdValidate.parseFileError: " + logMessage.logMessage());
  }

  // free
  xmlSchemaFreeParserCtxt(parser_ctxt);
  xmlSchemaFreeValidCtxt(ctxt);
  xmlSchemaFree(schema);

  xmlFreeDoc(doc);

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

  return result;
}

bool XMLValidator::xsltValidate() const {

  xmlSubstituteEntitiesDefault(1);
  xmlLoadExtDtdDefaultValue = 1;

  auto schematron_filename_str = openstudio::toString(m_schemaPath);
  xsltStylesheet* style = xsltParseStylesheetFile(detail::xml_string(schematron_filename_str));

  auto filename_str = openstudio::toString(m_xmlPath.get());
  const auto* filename = filename_str.c_str();
  xmlDoc* doc = xmlParseFile(filename);
  xmlDoc* res = xsltApplyStylesheet(style, doc, nullptr);

  // Dump result of xlstApply
  m_fullValidationReport = dumpXSLTApplyResultToString(res, style);
  // fmt::print("\n====== Full Validation Report =====\n\n{}", m_fullValidationReport);
  // xsltSaveResultToFile(stdout, res, style);

  std::vector<std::string> errors = processXSLTApplyResult(res);
  for (const auto& error : errors) {
    logAndStore(Error, "xsltValidate: " + error);
  }

  /* dump the resulting document */
  // xmlDocDump(stdout, res);

  xsltFreeStylesheet(style);
  xmlFreeDoc(res);
  xmlFreeDoc(doc);

  return errors.empty();
}

boost::optional<std::string> XMLValidator::fullValidationReport() const {

  if ((m_validatorType == XMLValidatorType::Schematron) || (m_validatorType == XMLValidatorType::XSLTSchematron)) {
    return m_fullValidationReport;
  }

  return boost::none;
}

std::vector<LogMessage> XMLValidator::logMessages() const {
  return m_logMessages;
}

XMLValidator XMLValidator::gbxmlValidator() {
  const auto tmpDir = openstudio::filesystem::create_temporary_directory("xmlvalidation");
  if (tmpDir.empty()) {
    LOG_AND_THROW("Failed to create a temporary directory for extracting the embedded path");
  }
  bool quiet = true;
  ::openstudio::embedded_files::extractFile(":/xml/resources/GreenBuildingXML_Ver6.01.xsd", openstudio::toString(tmpDir), quiet);
  return XMLValidator(tmpDir / "GreenBuildingXML_Ver6.01.xsd");
}

}  // namespace openstudio
