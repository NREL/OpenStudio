/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKCONSTANTPRESSUREDROP_HPP
#define MODEL_AIRFLOWNETWORKCONSTANTPRESSUREDROP_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

namespace detail {

  class AirflowNetworkConstantPressureDrop_Impl;

} // detail

/** AirflowNetworkConstantPressureDrop is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetwork:ConstantPressureDrop'. */
class MODEL_API AirflowNetworkConstantPressureDrop : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  //explicit AirflowNetworkConstantPressureDrop(const Model& model);
  /** Construct a constant pressure drop object. */
  AirflowNetworkConstantPressureDrop(const Model& model, double pressureDrop);

  virtual ~AirflowNetworkConstantPressureDrop() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{
  /** Returns the pressure drop across the component. */
  double pressureDrop() const;

  //@}
  /** @name Setters */
  //@{
  /** Sets the pressure drop across the component. */
  bool setPressureDrop(double pressureDrop);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::AirflowNetworkConstantPressureDrop_Impl ImplType;

  explicit AirflowNetworkConstantPressureDrop(std::shared_ptr<detail::AirflowNetworkConstantPressureDrop_Impl> impl);

  friend class detail::AirflowNetworkConstantPressureDrop_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AirflowNetworkConstantPressureDrop");
};

/** \relates AirflowNetworkConstantPressureDrop*/
typedef boost::optional<AirflowNetworkConstantPressureDrop> OptionalAirflowNetworkConstantPressureDrop;

/** \relates AirflowNetworkConstantPressureDrop*/
typedef std::vector<AirflowNetworkConstantPressureDrop> AirflowNetworkConstantPressureDropVector;

} // model
} // openstudio

#endif // MODEL_AIRFLOWNETWORKCONSTANTPRESSUREDROP_HPP

