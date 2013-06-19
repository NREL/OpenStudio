/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#ifndef UTILITIES_DOCUMENT_TABLEELEMENT_IMPL_HPP
#define UTILITIES_DOCUMENT_TABLEELEMENT_IMPL_HPP

#include <utilities/UtilitiesAPI.hpp>
#include <utilities/core/Optional.hpp>

#include <boost/variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/serialization/variant.hpp>

#include <string>
#include <vector>
#include <stdexcept>

namespace openstudio {
namespace detail {

  class UTILITIES_API GetIntVisitor : public boost::static_visitor< boost::optional<int> > {
   public:
    boost::optional<int> operator()(const int& i) const { return i; }
    boost::optional<int> operator()(const double& d) const { return boost::none; }
    boost::optional<int> operator()(const std::string& str) const { return boost::none; }
    boost::optional<int> operator()(const bool& b) const { return boost::none; }
  };

  class UTILITIES_API GetDoubleVisitor : public boost::static_visitor< boost::optional<double> > {
   public:
    boost::optional<double> operator()(const int& i) const { return boost::none; }
    boost::optional<double> operator()(const double& d) const { return d; }
    boost::optional<double> operator()(const std::string& str) const { return boost::none; }
    boost::optional<double> operator()(const bool& b) const { return boost::none; }
  };

  class UTILITIES_API GetStringVisitor : public boost::static_visitor< boost::optional<std::string> > 
  {
   public:
    boost::optional<std::string> operator()(const int& i) const { return boost::none; }
    boost::optional<std::string> operator()(const double& d) const { return boost::none; }
    boost::optional<std::string> operator()(const std::string& str) const { return str; }
    boost::optional<std::string> operator()(const bool& b) const { return boost::none; }
  };

  class UTILITIES_API IsBoolVisitor : public boost::static_visitor<bool> {
   public:
    bool operator()(const int& i) const { return false; }
    bool operator()(const double& d) const { return false; }
    bool operator()(const std::string& str) const { return false; }
    bool operator()(const bool& b) const { return true; }
  };

  class UTILITIES_API GetBoolVisitor : public boost::static_visitor<bool> {
   public:
    bool operator()(const int& i) const {
      throw std::runtime_error("Cannot get boolean value from int.");
    }
    bool operator()(const double& d) const {
      throw std::runtime_error("Cannot get boolean value from double.");
    }
    bool operator()(const std::string& str) const {
      throw std::runtime_error("Cannot get boolean value from string.");
    }
    bool operator()(const bool& b) const { return b; }
  };

  class UTILITIES_API EmptyVisitor : public boost::static_visitor<bool> {
   public:
    bool operator()(const int& i) const { return false; }
    bool operator()(const double& d) const { return false; }
    bool operator()(const std::string& str) const { return str.empty(); }
    bool operator()(const bool& b) const { return false; }
  };

} // detail
} // openstudio

#endif // UTILITIES_DOCUMENT_TABLEELEMENT_IMPL_HPP
