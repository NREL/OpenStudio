/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SITEGROUNDTEMPERATUREUNDISTURBEDXING_IMPL_HPP
#define MODEL_SITEGROUNDTEMPERATUREUNDISTURBEDXING_IMPL_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"
#include "../utilities/time/Date.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** SiteGroundTemperatureUndisturbedXing_Impl is a ModelObject_Impl that is the implementation class for SiteGroundTemperatureUndisturbedXing.*/
    class MODEL_API SiteGroundTemperatureUndisturbedXing_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      SiteGroundTemperatureUndisturbedXing_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SiteGroundTemperatureUndisturbedXing_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SiteGroundTemperatureUndisturbedXing_Impl(const SiteGroundTemperatureUndisturbedXing_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SiteGroundTemperatureUndisturbedXing_Impl() override = default;

      //@}

      virtual boost::optional<ParentObject> parent() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /** @name Getters */
      //@{

      double soilThermalConductivity() const;

      double soilDensity() const;

      double soilSpecificHeat() const;

      double averageSoilSurfaceTemperature() const;

      double soilSurfaceTemperatureAmplitude1() const;

      double soilSurfaceTemperatureAmplitude2() const;

      double phaseShiftofTemperatureAmplitude1() const;

      double phaseShiftofTemperatureAmplitude2() const;

      //@}
      /** @name Setters */
      //@{

      bool setSoilThermalConductivity(double soilThermalConductivity);

      bool setSoilDensity(double soilDensity);

      bool setSoilSpecificHeat(double soilSpecificHeat);

      bool setAverageSoilSurfaceTemperature(double averageSoilSurfaceTemperature);

      bool setSoilSurfaceTemperatureAmplitude1(double soilSurfaceTemperatureAmplitude1);

      bool setSoilSurfaceTemperatureAmplitude2(double soilSurfaceTemperatureAmplitude2);

      bool setPhaseShiftofTemperatureAmplitude1(double phaseShiftofTemperatureAmplitude1);

      bool setPhaseShiftofTemperatureAmplitude2(double phaseShiftofTemperatureAmplitude2);

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SiteGroundTemperatureUndisturbedXing");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SITEGROUNDTEMPERATUREUNDISTURBEDXING_IMPL_HPP
