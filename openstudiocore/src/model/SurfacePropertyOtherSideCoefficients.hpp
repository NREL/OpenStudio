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

#ifndef MODEL_SURFACEPROPERTYOTHERSIDECOEFFICIENTS_HPP
#define MODEL_SURFACEPROPERTYOTHERSIDECOEFFICIENTS_HPP

#include <model/ModelAPI.hpp>
#include "ResourceObject.hpp"

namespace openstudio {

namespace model {

class Schedule;

namespace detail {

  class SurfacePropertyOtherSideCoefficients_Impl;

} // detail

/** SurfacePropertyOtherSideCoefficients is a ResourceObject that wraps the OpenStudio IDD object 'OS:SurfaceProperty:OtherSideCoefficients'. */
class MODEL_API SurfacePropertyOtherSideCoefficients : public ResourceObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SurfacePropertyOtherSideCoefficients(const Model& model);

  virtual ~SurfacePropertyOtherSideCoefficients() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  /** If set, this field becomes the exterior convective/radiative film coefficient
      and the other fields are used to calculate the outdoor air temperature
      then exterior surface temperature based on outdoor air and specified coefficient.
      If not set, then remaining fields calculate the outside surface temperature. **/
  boost::optional<double> combinedConvectiveRadiativeFilmCoefficient() const;

  double constantTemperature() const;

  bool isConstantTemperatureDefaulted() const;

  double constantTemperatureCoefficient() const;

  bool isConstantTemperatureCoefficientDefaulted() const;

  double externalDryBulbTemperatureCoefficient() const;

  bool isExternalDryBulbTemperatureCoefficientDefaulted() const;

  double groundTemperatureCoefficient() const;

  bool isGroundTemperatureCoefficientDefaulted() const;

  double windSpeedCoefficient() const;

  bool isWindSpeedCoefficientDefaulted() const;

  double zoneAirTemperatureCoefficient() const;

  bool isZoneAirTemperatureCoefficientDefaulted() const;

  boost::optional<Schedule> constantTemperatureSchedule() const;

  bool sinusoidalVariationofConstantTemperatureCoefficient() const;

  bool isSinusoidalVariationofConstantTemperatureCoefficientDefaulted() const;

  double periodofSinusoidalVariation() const;

  bool isPeriodofSinusoidalVariationDefaulted() const;

  double previousOtherSideTemperatureCoefficient() const;

  bool isPreviousOtherSideTemperatureCoefficientDefaulted() const;

  boost::optional<double> minimumOtherSideTemperatureLimit() const;

  boost::optional<double> maximumOtherSideTemperatureLimit() const;

  //@}
  /** @name Setters */
  //@{

  bool setCombinedConvectiveRadiativeFilmCoefficient(double combinedConvectiveRadiativeFilmCoefficient);

  void resetCombinedConvectiveRadiativeFilmCoefficient();

  bool setConstantTemperature(double constantTemperature);

  void resetConstantTemperature();

  bool setConstantTemperatureCoefficient(double constantTemperatureCoefficient);

  void resetConstantTemperatureCoefficient();

  bool setExternalDryBulbTemperatureCoefficient(double externalDryBulbTemperatureCoefficient);

  void resetExternalDryBulbTemperatureCoefficient();

  bool setGroundTemperatureCoefficient(double groundTemperatureCoefficient);

  void resetGroundTemperatureCoefficient();

  bool setWindSpeedCoefficient(double windSpeedCoefficient);

  void resetWindSpeedCoefficient();

  bool setZoneAirTemperatureCoefficient(double zoneAirTemperatureCoefficient);

  void resetZoneAirTemperatureCoefficient();

  bool setConstantTemperatureSchedule(Schedule& schedule);

  void resetConstantTemperatureSchedule();

  bool setSinusoidalVariationofConstantTemperatureCoefficient(bool sinusoidalVariationofConstantTemperatureCoefficient);

  void resetSinusoidalVariationofConstantTemperatureCoefficient();

  bool setPeriodofSinusoidalVariation(double periodofSinusoidalVariation);

  void resetPeriodofSinusoidalVariation();

  bool setPreviousOtherSideTemperatureCoefficient(double previousOtherSideTemperatureCoefficient);

  void resetPreviousOtherSideTemperatureCoefficient();

  bool setMinimumOtherSideTemperatureLimit(double minimumOtherSideTemperatureLimit);

  void resetMinimumOtherSideTemperatureLimit();

  bool setMaximumOtherSideTemperatureLimit(double maximumOtherSideTemperatureLimit);

  void resetMaximumOtherSideTemperatureLimit();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::SurfacePropertyOtherSideCoefficients_Impl ImplType;

  explicit SurfacePropertyOtherSideCoefficients(std::shared_ptr<detail::SurfacePropertyOtherSideCoefficients_Impl> impl);

  friend class detail::SurfacePropertyOtherSideCoefficients_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.SurfacePropertyOtherSideCoefficients");
};

/** \relates SurfacePropertyOtherSideCoefficients*/
typedef boost::optional<SurfacePropertyOtherSideCoefficients> OptionalSurfacePropertyOtherSideCoefficients;

/** \relates SurfacePropertyOtherSideCoefficients*/
typedef std::vector<SurfacePropertyOtherSideCoefficients> SurfacePropertyOtherSideCoefficientsVector;

} // model
} // openstudio

#endif // MODEL_SURFACEPROPERTYOTHERSIDECOEFFICIENTS_HPP

