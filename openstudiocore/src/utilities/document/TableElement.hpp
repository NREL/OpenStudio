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

#ifndef UTILITIES_DOCUMENT_TABLEELEMENT_HPP
#define UTILITIES_DOCUMENT_TABLEELEMENT_HPP

#include <utilities/UtilitiesAPI.hpp>
#include <utilities/core/Optional.hpp>
#include <utilities/core/Logger.hpp>

#include <QVariant>

#include <boost/variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/serialization/variant.hpp>

#include <string>
#include <vector>
#include <stdexcept>

namespace openstudio {

/** Specifies whether the TableElement constructor from string should try to convert the string
 *  to bool, int, or double. By default, it tries all three conversions. */
struct UTILITIES_API TableLoadOptions {
  bool lookForBools;
  bool lookForInts;
  bool lookForDoubles;

  TableLoadOptions(bool t_lookForBools = true,
                   bool t_lookForInts = true,
                   bool t_lookForDoubles = true);
};

class UTILITIES_API TableElement {
 public:  
  TableElement(const std::string& data = std::string(),
               const TableLoadOptions& options = TableLoadOptions());

  TableElement(const char* data, const TableLoadOptions& options = TableLoadOptions());

  TableElement(double data);

  TableElement(int data);

  TableElement(bool data);

  TableElement(QVariant data);

  bool empty() const;

  bool isString() const;

  bool isDouble() const;

  bool isInt() const;

  bool isBool() const;

  /** Returns string version of this element. Never fails. */
  std::string toString() const;

  /** Throws if not isDouble() || isInt(). */
  double toDouble() const;

  /** Throws if not isInt(). */
  int toInt() const;

  /** Throws if not isBool(). */
  bool toBool() const;

  bool operator==(const TableElement& other) const;

 private:
  typedef boost::variant<std::string, double, int, bool> ElementType;
  ElementType m_element;

  friend UTILITIES_API std::ostream& operator<<(std::ostream& os,const TableElement& e);

  void createElement(const std::string& data,const TableLoadOptions& options);

  REGISTER_LOGGER("openstudio.document.TableElement");

  // serialization support
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_NVP(m_element);
  }

};

/** \relates TableElement */
typedef std::vector<TableElement> TableRow;

/** \relates TableElement */
typedef std::vector<TableElement> TableElementVector;

/** \relates TableElement */
UTILITIES_API bool operator!=(const TableElement& le, const TableElement& re);

/** \relates TableElement */
UTILITIES_API std::ostream& operator<<(std::ostream& os,const TableElement& e);

/** \relates TableElement */
UTILITIES_API std::string print(const TableElement& e);

/** \relates TableElement */
UTILITIES_API TableElement formatUnderscore(const TableElement& e);

} // openstudio

#endif // UTILITIES_DOCUMENT_TABLEELEMENT_HPP
