/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "Helpers.hpp"
#include <boost/lexical_cast.hpp>
#include <pugixml.hpp>

namespace openstudio {

namespace sdd {

  // Helper to make a vector of pugi::xml_node of all children
  std::vector<pugi::xml_node> makeVectorOfChildren(const pugi::xml_node& root) {
    std::vector<pugi::xml_node> result;
    for (const pugi::xml_node& child : root.children()) {
      result.push_back(child);
    }
    return result;
  }

  // Helper to make a vector of pugi::xml_node matching a tag
  std::vector<pugi::xml_node> makeVectorOfChildren(const pugi::xml_node& root, const char* tagName) {
    std::vector<pugi::xml_node> result;
    for (const pugi::xml_node& child : root.children(tagName)) {
      result.push_back(child);
    }
    return result;
  }

  // Helper to make a vector of pugi::xml_node matching a tag: any level
  std::vector<pugi::xml_node> makeVectorOfChildrenRecursive(const pugi::xml_node& root, const std::string& tagName) {
    std::vector<pugi::xml_node> result;
    // We use xpath '//tagName'
    for (const pugi::xpath_node& n : root.select_nodes((std::string("//") + tagName).c_str())) {
      result.push_back(n.node());
    }
    return result;
  }

  // Lexical cast the text() of a node as a double
  boost::optional<double> lexicalCastToDouble(const pugi::xml_node& element) {
    boost::optional<double> result;
    if (element) {
      try {
        result = boost::lexical_cast<double>(element.text().as_string());
      } catch (const boost::bad_lexical_cast&) {
        // LOG(Error, "Cannot convert element text to double");
      }
    }
    return result;
  }

  // Lexical cast the text() of a node as an int
  boost::optional<int> lexicalCastToInt(const pugi::xml_node& element) {
    boost::optional<int> result;
    if (element) {
      try {
        result = boost::lexical_cast<int>(element.text().as_string());
      } catch (const boost::bad_lexical_cast&) {
        // LOG(Error, "Cannot convert element text to double");
      }
    }
    return result;
  }

  // Lexical cast the text() of an attribute as a double
  boost::optional<double> lexicalCastToDouble(const pugi::xml_attribute& attr) {
    boost::optional<double> result;
    if (attr) {
      try {
        result = boost::lexical_cast<double>(attr.value());
      } catch (const boost::bad_lexical_cast&) {
        // LOG(Error, "Cannot convert attribute value to double");
      }
    }
    return result;
  }

  // Lexical cast the text() of an attribute as an int
  boost::optional<int> lexicalCastToInt(const pugi::xml_attribute& attr) {
    boost::optional<int> result;
    if (attr) {
      try {
        result = boost::lexical_cast<int>(attr.value());
      } catch (const boost::bad_lexical_cast&) {
        // LOG(Error, "Cannot convert attribute value to integer");
      }
    }
    return result;
  }

  // Lexical cast the text() of an attribute as an unsigned int (and don't return if a negative int is there)
  boost::optional<unsigned> lexicalCastToUnsigned(const pugi::xml_attribute& attr) {
    boost::optional<unsigned> result;
    if (attr) {
      try {
        boost::optional<int> _int = boost::lexical_cast<int>(attr.value());
        if (_int && (_int.get() >= 0)) {
          result = static_cast<unsigned>(_int.get());
        }
      } catch (const boost::bad_lexical_cast&) {
        // LOG(Error, "Cannot convert attribute value to integer");
      }
    }
    return result;
  }

  pugi::xml_node getProjectElement(const pugi::xml_node& element) {

    // root() returns the root that has the SDDXML element as the only child, which is the one we want (has 'Proj')
    pugi::xml_node result = element.root().first_child().child("Proj");
    if (!result) {
      LOG_FREE(Error, "openstudio.sdd.Helpers",
               "Couldn't find the 'Proj' element in the tree from the supplied element "
                 << "(name()='" << element.name() << "', text()='" << element.text().as_string() << "').");
      OS_ASSERT(false);
    }
    return result;
  }

}  // namespace sdd
}  // namespace openstudio
