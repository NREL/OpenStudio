/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

