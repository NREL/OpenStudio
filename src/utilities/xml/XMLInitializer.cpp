/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "XMLInitializer.hpp"
#include <libxml/parser.h>
#include <libxslt/transform.h>
#include <libxslt/xslt.h>

namespace openstudio::detail {

XMLInitializer::XMLInitializer() {
  // Initialize on the first **Actual** usage

  // init the parser (keeps libxml2 thread safe)
  xmlInitParser();

  // set some libxml global variables
  xmlIndentTreeOutput = 1;
  xmlKeepBlanksDefaultValue = 0;
  xmlSubstituteEntitiesDefaultValue = 1;
  xmlLoadExtDtdDefaultValue = 1;
  xmlDoValidityCheckingDefaultValue = 0;

  xsltInit();
  xsltSetXIncludeDefault(1);
}

XMLInitializer::~XMLInitializer() {
  // This should be done after we're totally done processing stuff...
  // So that's why it's done by a static object... Note that no cleanup happens in case it was never used, because --reference_counter would be -1
  xsltCleanupGlobals();
  xmlCleanupParser();
}

}  // namespace openstudio::detail
