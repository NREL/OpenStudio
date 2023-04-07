/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
