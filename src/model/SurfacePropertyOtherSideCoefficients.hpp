/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SURFACEPROPERTYOTHERSIDECOEFFICIENTS_HPP
#define MODEL_SURFACEPROPERTYOTHERSIDECOEFFICIENTS_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class SurfacePropertyOtherSideCoefficients_Impl;

  }  // namespace detail

  /** SurfacePropertyOtherSideCoefficients is a ResourceObject that wraps the OpenStudio IDD object 'OS:SurfaceProperty:OtherSideCoefficients'. */
  class MODEL_API SurfacePropertyOtherSideCoefficients : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SurfacePropertyOtherSideCoefficients(const Model& model);

    virtual ~SurfacePropertyOtherSideCoefficients() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SurfacePropertyOtherSideCoefficients(const SurfacePropertyOtherSideCoefficients& other) = default;
    SurfacePropertyOtherSideCoefficients(SurfacePropertyOtherSideCoefficients&& other) = default;
    SurfacePropertyOtherSideCoefficients& operator=(const SurfacePropertyOtherSideCoefficients&) = default;
    SurfacePropertyOtherSideCoefficients& operator=(SurfacePropertyOtherSideCoefficients&&) = default;

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
    using ImplType = detail::SurfacePropertyOtherSideCoefficients_Impl;

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
  using OptionalSurfacePropertyOtherSideCoefficients = boost::optional<SurfacePropertyOtherSideCoefficients>;

  /** \relates SurfacePropertyOtherSideCoefficients*/
  using SurfacePropertyOtherSideCoefficientsVector = std::vector<SurfacePropertyOtherSideCoefficients>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SURFACEPROPERTYOTHERSIDECOEFFICIENTS_HPP
