/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#ifndef MEASURE_OSMEASURE_HPP
#define MEASURE_OSMEASURE_HPP

#include "MeasureAPI.hpp"

#include "../utilities/core/Logger.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {
namespace measure {

class OSArgument;
class OSOutput;
class OSRunner;

/** OSMeasure is an abstract base class for a user script. There are derived types that operate
 *  on OpenStudio Models and EnergyPlus Workspaces. */
class MEASURE_API OSMeasure {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~OSMeasure();

  //@}
  /** @name Getters */
  //@{

  /// user facing name of this measure, defaults to empty
  virtual std::string name() const;

  /// user facing description of this measure, defaults to empty
  virtual std::string description() const;

  /// user facing BCL taxonomy of this measure, defaults to empty
  virtual std::string taxonomy() const;

  /// user facing technical description of this measure, defaults to empty
  virtual std::string modeler_description() const;

  //@}
  /** @name Actions */
  //@{

  /** Register this script with SketchUp. Deprecated in favor of registerWithApplication().
   *  Base class implementation does nothing and returns false. \deprecated */
  OS_DEPRECATED virtual bool registerWithSketchUp() const;

  /** Register this script with an Application. Base class implementation does nothing and
   *  returns false. \deprecated */
  OS_DEPRECATED virtual bool registerWithApplication() const;

 protected:
  OSMeasure() {}

  //@}
 private:
  REGISTER_LOGGER("openstudio.measure.OSMeasure");
};

/** \relates OSMeasure */
typedef boost::optional<OSMeasure> OptionalOSMeasure;

/** \relates OSMeasure */
typedef std::vector<OSMeasure> OSMeasureVector;

} // measure
} // openstudio

#endif // MEASURE_OSMEASURE_HPP
