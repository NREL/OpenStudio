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

#ifndef MODEL_DAYLIGHTINGDEVICESHELF_HPP
#define MODEL_DAYLIGHTINGDEVICESHELF_HPP

#include <model/ModelAPI.hpp>
#include <model/ModelObject.hpp>

namespace openstudio {
namespace model {

class SubSurface;

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

  // TODO: Handle Non-Extensible IddField Window Name.

  // TODO: Handle Non-Extensible IddField Inside Shelf Name.

  // TODO: Handle Non-Extensible IddField Outside Shelf Name.

  boost::optional<double> viewFactortoOutsideShelf() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Handle Non-Extensible IddField Window Name.

  // TODO: Handle Non-Extensible IddField Inside Shelf Name.

  // TODO: Handle Non-Extensible IddField Outside Shelf Name.

  bool setViewFactortoOutsideShelf(boost::optional<double> viewFactortoOutsideShelf);

  bool setViewFactortoOutsideShelf(double viewFactortoOutsideShelf);

  void resetViewFactortoOutsideShelf();

  //@}

  /// Get the related sub surface.
  SubSurface subSurface() const;

  /// Set the related sub surface.
  bool setSubSurface(const SubSurface& subSurface);

 protected:
  /// @cond
  typedef detail::DaylightingDeviceShelf_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit DaylightingDeviceShelf(boost::shared_ptr<detail::DaylightingDeviceShelf_Impl> impl);

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

