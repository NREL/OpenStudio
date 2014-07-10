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

#ifndef MODEL_SHADINGSURFACE_HPP
#define MODEL_SHADINGSURFACE_HPP

#include "ModelAPI.hpp"
#include "PlanarSurface.hpp"

namespace openstudio {
namespace model {

class ShadingSurfaceGroup;
class Schedule;
class DaylightingDeviceShelf;

namespace detail {

  class ShadingSurface_Impl;

} // detail

/** ShadingSurface is a PlanarSurface that wraps the OpenStudio IDD object
 *  'OS:ShadingSurface'. */
class MODEL_API ShadingSurface : public PlanarSurface {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ShadingSurface(const std::vector<Point3d>& vertices, const Model& model);

  virtual ~ShadingSurface() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  /// get the shading group
  boost::optional<ShadingSurfaceGroup> shadingSurfaceGroup() const;

  // get the transmittance schedule
  boost::optional<Schedule> transmittanceSchedule() const;

  boost::optional<double> numberofVertices() const;

  bool isNumberofVerticesDefaulted() const;

  bool isNumberofVerticesAutocalculated() const;

  //@}
  /** @name Setters */
  //@{

  /// set the exterior shading group
  bool setShadingSurfaceGroup(const ShadingSurfaceGroup& shadingSurfaceGroup);

  void resetShadingSurfaceGroup();

  /// set the transmittance schedule
  bool setTransmittanceSchedule(Schedule& transmittanceSchedule);

  /// reset the transmittance schedule
  void resetTransmittanceSchedule();

  bool setNumberofVertices(boost::optional<double> numberofVertices);

  bool setNumberofVertices(double numberofVertices);

  void resetNumberofVertices();

  void autocalculateNumberofVertices();

  //@}

  boost::optional<DaylightingDeviceShelf> daylightingDeviceShelf() const;

 protected:
  /// @cond
  typedef detail::ShadingSurface_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  explicit ShadingSurface(std::shared_ptr<detail::ShadingSurface_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.ShadingSurface");
};

/** \relates ShadingSurface*/
typedef boost::optional<ShadingSurface> OptionalShadingSurface;

/** \relates ShadingSurface*/
typedef std::vector<ShadingSurface> ShadingSurfaceVector;

} // model
} // openstudio

#endif // MODEL_SHADINGSURFACE_HPP

