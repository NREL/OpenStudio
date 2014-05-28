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

#ifndef MODEL_SHADINGCONTROL_HPP
#define MODEL_SHADINGCONTROL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {

namespace model {

class Construction;
class ShadingMaterial;
class Schedule;

namespace detail {

  class ShadingControl_Impl;

} // detail

/** ShadingControl is a ResourceObject that wraps the OpenStudio IDD object 'OS:ShadingControl'. */
class MODEL_API ShadingControl : public ResourceObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ShadingControl(const Construction& construction);

  explicit ShadingControl(const ShadingMaterial& shadingMaterial);

  virtual ~ShadingControl() {}

  //@}

  static IddObjectType iddObjectType();
  
  static std::vector<std::string> shadingTypeValues();
  
  static std::vector<std::string> shadingControlTypeValues();

  /** @name Getters */
  //@{
  
  boost::optional<Construction> construction() const;

  boost::optional<ShadingMaterial> shadingMaterial() const;
  
  std::string shadingType() const;
  
  std::string shadingControlType() const;

  boost::optional<Schedule> schedule() const;
  
  //@}
  /** @name Setters */
  //@{
  
  bool setShadingType(const std::string& shadingType);
  
  bool setShadingControlType(const std::string& shadingControlType);
  
  bool setSchedule(const Schedule& schedule);
  
  void resetSchedule();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ShadingControl_Impl ImplType;

  explicit ShadingControl(std::shared_ptr<detail::ShadingControl_Impl> impl);

  friend class detail::ShadingControl_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ShadingControl");
};

/** \relates ShadingControl*/
typedef boost::optional<ShadingControl> OptionalShadingControl;

/** \relates ShadingControl*/
typedef std::vector<ShadingControl> ShadingControlVector;

} // model
} // openstudio

#endif // MODEL_SHADINGCONTROL_HPP

