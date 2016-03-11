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

  /// Create a ShadingControl object which references the passed in construction for the shaded state.
  /// This constructor should be used to model switchable glazing only at this time, other configurations 
  /// will not be supported by the Radiance ForwardTranslator.  Note that changes to the construction made
  /// after this constructor will not be reflected in this ShadingControl object.  If changes to the construction
  /// are made a new ShadingControl object should be constructed.
  explicit ShadingControl(const Construction& construction);

  /// Create a ShadingControl object which references the passed in material for the shading layer.
  /// This constructor is preferred for all configurations other than switchable glazing.  
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

  bool isShadingControlTypeDefaulted() const;

  boost::optional<Schedule> schedule() const;

  boost::optional<double> setpoint() const;

  bool isSetpointDefaulted() const;
  
  //@}
  /** @name Setters */
  //@{
  
  // DLM: should this be removed?
  /// Sets the shading type field.  There is no error checking to ensure that shadingType is correct
  /// given the shaded construction or shading material.  
  bool setShadingType(const std::string& shadingType);
  
  bool setShadingControlType(const std::string& shadingControlType);

  void resetShadingControlType();
  
  bool setSchedule(const Schedule& schedule);
  
  void resetSchedule();

  bool setSetpoint(double setpoint);

  void resetSetpoint();

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

