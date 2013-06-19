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

#ifndef RULESET_RULESET_HPP
#define RULESET_RULESET_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/RulesetObject.hpp>
#include <ruleset/RulesetEnums.hpp>

#include <utilities/core/Path.hpp>

namespace openstudio {
namespace ruleset {
  
class Rule;

namespace detail {
  class Ruleset_Impl;
} // detail

/** A Ruleset RulesetObject has \link Rule Rules \endlink and \link RulesetOption RulesetOptions 
 *  \endlink. It also has a name, and can be serialized to a project::ProjectDatabase. Once 
 *  constructed, \link Ruleset Rulesets\endlink are typically applied to model::Models. Ruleset 
 *  is essentially an abstract object, as it has no public constructors for general use. */
class RULESET_API Ruleset : public RulesetObject {
 public:
  /// @cond
  typedef detail::Ruleset_Impl ImplType;
  /// @endcond

  /** @name Constructors and Destructors */
  //@{

  /** Constructor provided for deserialization; not for general use. */
  static boost::optional<Ruleset> factoryFromXml(const QDomElement& element);
  
  virtual ~Ruleset() {}

  //@}
  /** @name Getters */
  //@{

  std::string name() const;

  //@}
  /** @name Setters */
  //@{

  bool setName(const std::string& name);

  //@}
  /** @name Serialization */
  //@{

  /// write object and all children out as xml to path. 
  bool saveToXml(const openstudio::path& path) const;

  //@}
 protected:
  /// @cond
  friend class RulesetObject;
  friend class detail::RulesetObject_Impl;
  friend class detail::Ruleset_Impl;

  /** Protected constructor from impl. */
  Ruleset(boost::shared_ptr<detail::Ruleset_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("Ruleset.Ruleset");

};

} // ruleset
} // openstudio

#endif // RULESET_RULESET_HPP
