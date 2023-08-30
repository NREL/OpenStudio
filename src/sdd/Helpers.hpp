/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef SDD_HELPERS_HPP
#define SDD_HELPERS_HPP

#include "../utilities/core/Optional.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/Assert.hpp"

namespace pugi {
class xml_node;
class xml_attribute;
}  // namespace pugi

namespace openstudio {
namespace sdd {

  // Helper to make a vector of pugi::xml_node of all children under an element (regardless of their tag)
  std::vector<pugi::xml_node> makeVectorOfChildren(const pugi::xml_node& root);

  // Helper to make a vector of pugi::xml_node of children matching a specific tag (first-descendants only)
  std::vector<pugi::xml_node> makeVectorOfChildren(const pugi::xml_node& root, const char* tagName);

  // Helper to make a vector of pugi::xml_node of children matching a specific tag (any level descendants)
  std::vector<pugi::xml_node> makeVectorOfChildrenRecursive(const pugi::xml_node& root, const std::string& tagName);

  // Lexical cast the text() of a node as a double
  // Checks if the element actually exists, then if it can be converted to a double.
  // (pugi::xml_node::text().as_double() will return a default value in all cases, which is too permissive)
  boost::optional<double> lexicalCastToDouble(const pugi::xml_node& element);

  // Lexical cast an attribute's value() as a double
  // Checks if the attribute actually exists, then if it can be converted to a double
  boost::optional<double> lexicalCastToDouble(const pugi::xml_attribute& attr);

  // Lexical cast the text() of a node as an integer
  // Checks if the element actually exists, then if it can be converted to an int.
  boost::optional<int> lexicalCastToInt(const pugi::xml_node& element);

  // Lexical cast an attribute's value() as an integer
  // Checks if the attribute actually exists, then if it can be converted to an integer
  boost::optional<int> lexicalCastToInt(const pugi::xml_attribute& attr);

  // Lexical cast an attribute's value() as an Unsigned
  // Checks if the attribute actually exists, then if it can be converted to an Unsigned
  // (boost::lexical_cast<unsigned>(-1) would not throw but return gibberish)
  boost::optional<unsigned> lexicalCastToUnsigned(const pugi::xml_attribute& attr);

  // Returns the 'Proj' element from any element in the tree
  pugi::xml_node getProjectElement(const pugi::xml_node& element);

}  // namespace sdd
}  // namespace openstudio

#endif  // SDD_HELPERS_HPP
