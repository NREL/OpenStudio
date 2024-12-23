/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
               "Couldn't find the 'Proj' element in the tree from the supplied element " << "(name()='" << element.name() << "', text()='"
                                                                                         << element.text().as_string() << "').");
      OS_ASSERT(false);
    }
    return result;
  }

}  // namespace sdd
}  // namespace openstudio
