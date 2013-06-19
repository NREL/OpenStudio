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

#ifndef RULESET_RULESETOBJECT_HPP
#define RULESET_RULESETOBJECT_HPP

#include <ruleset/RulesetAPI.hpp>

#include <utilities/core/Logger.hpp>
#include <utilities/core/UUID.hpp>

#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>

#include <string>
#include <ostream>

#include <QDomElement>
#include <QDomDocument>

namespace openstudio {
namespace ruleset {

namespace detail {
  class RulesetObject_Impl;
} // detail

/** RulesetObject is the base class for the ruleset library. \link RulesetObject RulesetObjects 
 *  \endlink have a uuid(), a versionUUID(), and are shared data (impl-pattern) objects. (The 
 *  default copy constructor and assignment shares data between the original and the new object.
 *  Both objects will be affected by any setters.) RulesetObject supports exact comparisons 
 *  (by impl), data-based comparisons (equivalentData), and type-casting. Serialization to XML
 *  is stubbed out, but not fully supported. \link RulesetObject RulesetObjects \endlink can be 
 *  serialized to project::ProjectDatabases. */
class RULESET_API RulesetObject {
 public:
  /** @name Constructors and Destructors */
  //@{
  
  virtual ~RulesetObject() {}
  
  //@}
  /** @name Getters */
  //@{

  UUID uuid() const;

  UUID versionUUID() const;

  //@}
  /** @name Serialization */
  //@{

  /// write object and all children to xml
  QDomDocument toXml() const;

  //@}
  /** @name Queries */
  //@{

  /** Equality as determined by deep comparison of data members. */
  bool equivalentData(const RulesetObject& other) const;

  /** Exact equality as determined by Impls. */
  bool operator==(const RulesetObject& other) const;

  /** Negation of operator==. */
  bool operator!=(const RulesetObject& other) const;

  /** Order by Impls so can use std::set, etc. */
  bool operator<(const RulesetObject& other) const;

  //@}
  /** @name Type Casting */
  //@{

  /** Cast to type T. Can throw std::bad_cast. */
  template<typename T>
  T cast() const {
    boost::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (!impl) { throw(std::bad_cast()); }
    return T(impl);
  }

  /** Cast to boost::optional<T>. */
  template<typename T>
  boost::optional<T> optionalCast() const {
    boost::optional<T> result;
    boost::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (impl) { result = T(impl); }
    return result;
  }

  //@}
 protected:
  /// @cond
  typedef detail::RulesetObject_Impl ImplType;

  friend class detail::RulesetObject_Impl;
  friend struct RulesetObjectNonEquivalentDataLess;

  /** Protected constructor from impl. */
  RulesetObject(boost::shared_ptr<detail::RulesetObject_Impl> impl);

  template<typename T>
  boost::shared_ptr<T> getImpl() const
  {  return boost::dynamic_pointer_cast<T>(m_impl); }

  /// @endcond
 private:
  boost::shared_ptr<detail::RulesetObject_Impl> m_impl;

  REGISTER_LOGGER("Ruleset.RulesetObject");
};

/** Writes out rulesetObject.toXml(). \relates RulesetObject */
RULESET_API std::ostream& operator<<(std::ostream& os, const RulesetObject& rulesetObject);

} // ruleset
} // openstudio

#endif // RULESET_RULESETOBJECT_HPP
