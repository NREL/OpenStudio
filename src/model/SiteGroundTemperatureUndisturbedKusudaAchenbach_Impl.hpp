/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SITEGROUNDTEMPERATUREUNDISTURBEDKUSUDAACHENBACH_IMPL_HPP
#define MODEL_SITEGROUNDTEMPERATUREUNDISTURBEDKUSUDAACHENBACH_IMPL_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl is a ModelObject_Impl that is the implementation class for SiteGroundTemperatureUndisturbedKusudaAchenbach.*/
    class MODEL_API SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl(const SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl& other, Model_Impl* model,
                                                           bool keepHandle);

      virtual ~SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl() override = default;

      //@}

      virtual boost::optional<ParentObject> parent() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /** @name Getters */
      //@{

      double soilThermalConductivity() const;

      double soilDensity() const;

      double soilSpecificHeat() const;

      boost::optional<double> averageSoilSurfaceTemperature() const;

      boost::optional<double> averageAmplitudeofSurfaceTemperature() const;

      boost::optional<double> phaseShiftofMinimumSurfaceTemperature() const;

      //@}
      /** @name Setters */
      //@{

      bool setSoilThermalConductivity(double soilThermalConductivity);

      bool setSoilDensity(double soilDensity);

      bool setSoilSpecificHeat(double soilSpecificHeat);

      bool setAverageSoilSurfaceTemperature(double averageSoilSurfaceTemperature);

      void resetAverageSoilSurfaceTemperature();

      bool setAverageAmplitudeofSurfaceTemperature(double averageAmplitudeofSurfaceTemperature);

      void resetAverageAmplitudeofSurfaceTemperature();

      bool setPhaseShiftofMinimumSurfaceTemperature(double phaseShiftofMinimumSurfaceTemperature);

      void resetPhaseShiftofMinimumSurfaceTemperature();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SiteGroundTemperatureUndisturbedKusudaAchenbach");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SITEGROUNDTEMPERATUREUNDISTURBEDKUSUDAACHENBACH_IMPL_HPP
