/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef RULESET_USERSCRIPT_HPP
#define RULESET_USERSCRIPT_HPP

#include "RulesetAPI.hpp"

#include "../utilities/core/Logger.hpp"

namespace openstudio {
namespace ruleset {

class OSArgument;
class OSRunner;

/** UserScript is an abstract base class for a user script. There are derived types that operate
 *  on OpenStudio Models and EnergyPlus Workspaces. */
class RULESET_API UserScript {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~UserScript();

  //@}
  /** @name Getters */
  //@{

  /// name of this script
  virtual std::string name() const = 0;

  //@}
  /** @name Actions */
  //@{

  /** Register this script with SketchUp. Deprecated in favor of registerWithApplication().
   *  Base class implementation does nothing and returns false. \deprecated */
  virtual bool registerWithSketchUp() const;

  /** Register this script with an Application. Base class implementation does nothing and
   *  returns false. */
  virtual bool registerWithApplication() const;

 protected:
  UserScript() {}

  //@}
 private:
  REGISTER_LOGGER("openstudio.ruleset.UserScript");
};

/** \relates UserScript */
typedef boost::optional<UserScript> OptionalUserScript;

/** \relates UserScript */
typedef std::vector<UserScript> UserScriptVector;

} // ruleset
} // openstudio

#endif // RULESET_USERSCRIPT_HPP
