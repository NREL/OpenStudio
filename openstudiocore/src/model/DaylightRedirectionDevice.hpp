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

#ifndef MODEL_DAYLIGHTREDIRECTIONDEVICE_HPP
#define MODEL_DAYLIGHTREDIRECTIONDEVICE_HPP

#include "ModelAPI.hpp"
#include "ShadingMaterial.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

namespace detail {

  class DaylightRedirectionDevice_Impl;

} // detail

/** DaylightRedirectionDevice is a ShadingMaterial that wraps the OpenStudio IDD object 'OS:WindowMaterial:DaylightRedirectionDevice'. 
 ** This object is not in EnergyPlus, this represents a film or louver which redirects daylighting. 
 **/
class MODEL_API DaylightRedirectionDevice : public ShadingMaterial {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit DaylightRedirectionDevice(const Model& model);

  virtual ~DaylightRedirectionDevice() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> daylightRedirectionDeviceTypeValues();

  /** @name Getters */
  //@{

  std::string daylightRedirectionDeviceType() const;
  
  bool isDaylightRedirectionDeviceTypeDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setDaylightRedirectionDeviceType(const std::string& daylightRedirectionDeviceType);

  void resetDaylightRedirectionDeviceType();
  
  //@}
  /** @name Other */
  //@{

  //@}
  
 protected:
 
  /// @cond
  typedef detail::DaylightRedirectionDevice_Impl ImplType;

  explicit DaylightRedirectionDevice(std::shared_ptr<detail::DaylightRedirectionDevice_Impl> impl);

  friend class detail::DaylightRedirectionDevice_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.DaylightRedirectionDevice");
};

/** \relates DaylightRedirectionDevice*/
typedef boost::optional<DaylightRedirectionDevice> OptionalDaylightRedirectionDevice;

/** \relates DaylightRedirectionDevice*/
typedef std::vector<DaylightRedirectionDevice> DaylightRedirectionDeviceVector;

} // model
} // openstudio

#endif // MODEL_DAYLIGHTREDIRECTIONDEVICE_HPP

