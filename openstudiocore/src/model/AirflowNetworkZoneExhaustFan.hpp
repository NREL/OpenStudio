/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_AIRFLOWNETWORKZONEEXHAUSTFAN_HPP
#define MODEL_AIRFLOWNETWORKZONEEXHAUSTFAN_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

class FanZoneExhaust;
class AirflowNetworkReferenceCrackConditions;

namespace detail {

class AirflowNetworkZoneExhaustFan_Impl;

} // detail

/** AirflowNetworkZoneExhaustFan is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetwork:ZoneExhaustFan'. */
class MODEL_API AirflowNetworkZoneExhaustFan : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{
  /** Construct a zone exhaust fan object with default values and a specified mass flow coefficient. */
  AirflowNetworkZoneExhaustFan(const Model& model, const FanZoneExhaust &fanZoneExhaust, double massFlowCoefficient);
  /** Construct a zone exhaust fan object with a possibly defaulted reference conditions object. */
  AirflowNetworkZoneExhaustFan(const Model& model, const FanZoneExhaust &fanZoneExhaust, double massFlowCoefficient,
    double massFlowExponent);
  /** Construct a zone exhaust fan object. */
  AirflowNetworkZoneExhaustFan(const Model& model, const FanZoneExhaust &fanZoneExhaust, double massFlowCoefficient,
    double massFlowExponent, const AirflowNetworkReferenceCrackConditions &referenceCrackConditions);

  virtual ~AirflowNetworkZoneExhaustFan() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{
  /** Returns the FanZoneExhaust object associated with this object. */
  boost::optional<FanZoneExhaust> fanZoneExhaust() const;
  /** Returns the mass flow coefficient in kg/s. */
  double airMassFlowCoefficientWhentheZoneExhaustFanisOff() const;
  /** Returns the mass flow exponent. */
  double airMassFlowExponentWhentheZoneExhaustFanisOff() const;
  /** Returns true if the mass flow exponent is defaulted. */
  bool isAirMassFlowExponentWhentheZoneExhaustFanisOffDefaulted() const;
  /** Returns a reference crack conditions object if available. */
  boost::optional<AirflowNetworkReferenceCrackConditions> referenceCrackConditions() const;

  //@}
  /** @name Setters */
  //@{
  /** Sets the FanZoneExhaust object associated with this object. */
  bool setFanZoneExhaust(const FanZoneExhaust& fanZoneExhaust);
  /** Sets the mass flow coefficient in kg/s.  */
  bool setAirMassFlowCoefficientWhentheZoneExhaustFanisOff(double airMassFlowCoefficientWhentheZoneExhaustFanisOffatReferenceConditions);
  /** Sets the mass flow exponent. */
  bool setAirMassFlowExponentWhentheZoneExhaustFanisOff(double airMassFlowExponentWhentheZoneExhaustFanisOff);
  /** Resets the mass flow exponent. */
  void resetAirMassFlowExponentWhentheZoneExhaustFanisOff();
  /** Sets the the reference crack conditions. */
  bool setReferenceCrackConditions(const AirflowNetworkReferenceCrackConditions& referenceCrackConditions);
  /** Resets the the reference crack conditions. */
  void resetReferenceCrackConditions();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::AirflowNetworkZoneExhaustFan_Impl ImplType;

  explicit AirflowNetworkZoneExhaustFan(std::shared_ptr<detail::AirflowNetworkZoneExhaustFan_Impl> impl);

  friend class detail::AirflowNetworkZoneExhaustFan_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AirflowNetworkZoneExhaustFan");
};

/** \relates AirflowNetworkZoneExhaustFan*/
typedef boost::optional<AirflowNetworkZoneExhaustFan> OptionalAirflowNetworkZoneExhaustFan;

/** \relates AirflowNetworkZoneExhaustFan*/
typedef std::vector<AirflowNetworkZoneExhaustFan> AirflowNetworkZoneExhaustFanVector;

} // model
} // openstudio

#endif // MODEL_AIRFLOWNETWORKZONEEXHAUSTFAN_HPP

