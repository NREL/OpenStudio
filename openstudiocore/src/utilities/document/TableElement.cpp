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

#include <utilities/document/TableElement.hpp>
#include <utilities/document/TableElement_Impl.hpp>
#include <utilities/document/DocumentRegex.hpp>

#include <utilities/core/Compare.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

namespace openstudio {

TableLoadOptions::TableLoadOptions(bool t_lookForBools,
                                   bool t_lookForInts,
                                   bool t_lookForDoubles)
  : lookForBools(t_lookForBools),
    lookForInts(t_lookForInts),
    lookForDoubles(t_lookForDoubles)
{}

TableElement::TableElement(const std::string& data,const TableLoadOptions& options)
  : m_element(data)
{
  createElement(data,options);
}

TableElement::TableElement(const char* data, const TableLoadOptions& options)
  : m_element(std::string(data))
{
  createElement(std::string(data),options);
}

TableElement::TableElement(double data)
  : m_element(data)
{}

TableElement::TableElement(int data)
  : m_element(data)
{}

TableElement::TableElement(bool data)
  : m_element(data)
{}

TableElement::TableElement(QVariant data)
{
  switch (data.type()) {
  case QVariant::Double :
    m_element = data.toDouble();
    break;
  case QVariant::Int :
    m_element = data.toInt();
    break;
  case QVariant::Bool :
    m_element = data.toBool();
    break;
  case QVariant::String :
    m_element = data.toString().toStdString();
    break;
  default :
    LOG_AND_THROW("The TableElement constructor currently only supports QVariant Double, Int, "
                  << "Bool, and String. This QVariant is " << data.typeName() << ".");
  }
}

bool TableElement::empty() const {
  return boost::apply_visitor(detail::EmptyVisitor(),m_element);
}

bool TableElement::isString() const {
  OptionalString os = boost::apply_visitor(detail::GetStringVisitor(),m_element);
  if (os) {
    return true;
  }
  return false;
}

bool TableElement::isDouble() const {
  OptionalDouble od = boost::apply_visitor(detail::GetDoubleVisitor(),m_element);
  if (od) {
    return true;
  }
  return false;
}

bool TableElement::isInt() const {
  OptionalInt oi = boost::apply_visitor(detail::GetIntVisitor(),m_element);
  if (oi) {
    return true;
  }
  return false;
}

bool TableElement::isBool() const {
  return boost::apply_visitor(detail::IsBoolVisitor(),m_element);
}

std::string TableElement::toString() const {
  std::stringstream ss;
  ss << m_element;
  return ss.str();
}

double TableElement::toDouble() const {
  OptionalDouble od = boost::apply_visitor(detail::GetDoubleVisitor(),m_element);
  if (od) {
    return *od;
  }
  OptionalInt oi = boost::apply_visitor(detail::GetIntVisitor(),m_element);
  if (oi) {
    return double(*oi);
  }
  LOG_AND_THROW("Cannot return double from TableElement '" << m_element << "'.");
}

int TableElement::toInt() const {
  return boost::apply_visitor(detail::GetIntVisitor(),m_element).get();
}

bool TableElement::toBool() const {
  return boost::apply_visitor(detail::GetBoolVisitor(),m_element);
}

bool TableElement::operator==(const TableElement& other) const {
  return m_element == other.m_element;
}

void TableElement::createElement(const std::string& data,const TableLoadOptions& options) {
  std::string trimmedData(data);
  boost::algorithm::trim(trimmedData);

  if (options.lookForInts) {
    boost::regex intRegex("[-+]?[0-9]+");
    if (boost::regex_match(trimmedData,intRegex)) {
      try {
        int result = boost::lexical_cast<int>(trimmedData);
        m_element = result;
        return;
      } catch (...) {}
    }
  }

  if (options.lookForDoubles) {
    try {
      double result = boost::lexical_cast<double>(trimmedData);
      m_element = result;
      return;
    } catch (...) {}
  }

  if (options.lookForBools) {
    if (openstudio::istringEqual(trimmedData,"true") || openstudio::istringEqual(trimmedData,"t") ||
        openstudio::istringEqual(trimmedData,"yes") || openstudio::istringEqual(trimmedData,"y")) {
      m_element = true;
      return;
    }
    if (openstudio::istringEqual(trimmedData,"false") || openstudio::istringEqual(trimmedData,"f") ||
        openstudio::istringEqual(trimmedData,"no") || openstudio::istringEqual(trimmedData,"n")) {
      m_element = false;
      return;
    }
  }

  if (trimmedData != data) {
    m_element = trimmedData;
  }
}

bool operator!=(const TableElement& le, const TableElement& re) {
  return !(le == re);
}

std::ostream& operator<<(std::ostream& os,const TableElement& e) {
  os << e.m_element;
  return os;
}

std::string print(const TableElement& e) {
  return e.toString();
}

TableElement formatUnderscore(const TableElement& e) {
  TableElement result(e);
  if (e.isString()) {
    result = formatUnderscore(e.toString());
  }
  return result;
}

} // openstudio
