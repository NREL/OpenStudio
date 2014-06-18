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

#ifndef MODEL_DAYLIGHTINGDEVICESHELF_HPP
#define MODEL_DAYLIGHTINGDEVICESHELF_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

class SubSurface;
class InteriorPartitionSurface;
class ShadingSurface;

namespace detail {

  class DaylightingDeviceShelf_Impl;

} // detail

/** DaylightingDeviceShelf is a ModelObject that wraps the OpenStudio IDD object 'OS_DaylightingDevice_Shelf'. */
class MODEL_API DaylightingDeviceShelf : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit DaylightingDeviceShelf(const SubSurface& subSurface);

  virtual ~DaylightingDeviceShelf() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  boost::optional<InteriorPartitionSurface> insideShelf() const;

  boost::optional<ShadingSurface> outsideShelf() const;

  boost::optional<double> viewFactortoOutsideShelf() const;

  //@}
  /** @name Setters */
  //@{

  bool setInsideShelf(const InteriorPartitionSurface& insideShelf);

  bool setOutsideShelf(const ShadingSurface& outsideShelf);

  bool setViewFactortoOutsideShelf(double viewFactortoOutsideShelf);

  void resetViewFactortoOutsideShelf();

  //@}

  /// Get the related sub surface.
  SubSurface subSurface() const;

 protected:
  /// @cond
  typedef detail::DaylightingDeviceShelf_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  explicit DaylightingDeviceShelf(std::shared_ptr<detail::DaylightingDeviceShelf_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.DaylightingDeviceShelf");
};

/** \relates DaylightingDeviceShelf*/
typedef boost::optional<DaylightingDeviceShelf> OptionalDaylightingDeviceShelf;

/** \relates DaylightingDeviceShelf*/
typedef std::vector<DaylightingDeviceShelf> DaylightingDeviceShelfVector;

} // model
} // openstudio

#endif // MODEL_DAYLIGHTINGDEVICESHELF_HPP

