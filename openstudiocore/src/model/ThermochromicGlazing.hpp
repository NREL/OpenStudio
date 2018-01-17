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

#ifndef MODEL_THERMOCHROMICGLAZING_HPP
#define MODEL_THERMOCHROMICGLAZING_HPP

#include "ModelAPI.hpp"
#include "Glazing.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class ThermochromicGlazing_Impl;

} // detail

/** ThermochromicGlazing is a Glazing that wraps the OpenStudio IDD object 'OS:WindowMaterial:GlazingGroup:Thermochromic'. */
class MODEL_API ThermochromicGlazing : public Glazing {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ThermochromicGlazing(const Model& model,
    double opticalDataTemperature = 80.0);

  virtual ~ThermochromicGlazing() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  double opticalDataTemperature() const;

  // TODO: Handle this object's extensible fields.

  //@}
  /** @name Setters */
  //@{

  bool setOpticalDataTemperature(double value);

  // TODO: Handle this object's extensible fields.

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ThermochromicGlazing_Impl ImplType;

  explicit ThermochromicGlazing(std::shared_ptr<detail::ThermochromicGlazing_Impl> impl);

  friend class detail::ThermochromicGlazing_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ThermochromicGlazing");
};

/** \relates ThermochromicGlazing*/
typedef boost::optional<ThermochromicGlazing> OptionalThermochromicGlazing;

/** \relates ThermochromicGlazing*/
typedef std::vector<ThermochromicGlazing> ThermochromicGlazingVector;

} // model
} // openstudio

#endif // MODEL_THERMOCHROMICGLAZING_HPP
