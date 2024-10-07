/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SURFACEPROPERTYOTHERSIDECOEFFICIENTS_IMPL_HPP
#define MODEL_SURFACEPROPERTYOTHERSIDECOEFFICIENTS_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    /** SurfacePropertyOtherSideCoefficients_Impl is a ResourceObject_Impl that is the implementation class for SurfacePropertyOtherSideCoefficients.*/
    class MODEL_API SurfacePropertyOtherSideCoefficients_Impl : public ResourceObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      SurfacePropertyOtherSideCoefficients_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SurfacePropertyOtherSideCoefficients_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SurfacePropertyOtherSideCoefficients_Impl(const SurfacePropertyOtherSideCoefficients_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SurfacePropertyOtherSideCoefficients_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

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
     private:
      REGISTER_LOGGER("openstudio.model.SurfacePropertyOtherSideCoefficients");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SURFACEPROPERTYOTHERSIDECOEFFICIENTS_IMPL_HPP
