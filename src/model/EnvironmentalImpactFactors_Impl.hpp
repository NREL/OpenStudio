/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ENVIRONMENTALIMPACTFACTORS_IMPL_HPP
#define MODEL_ENVIRONMENTALIMPACTFACTORS_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** EnvironmentalImpactFactors_Impl is a ModelObject_Impl that is the implementation class for EnvironmentalImpactFactors.*/
    class MODEL_API EnvironmentalImpactFactors_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      EnvironmentalImpactFactors_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      EnvironmentalImpactFactors_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      EnvironmentalImpactFactors_Impl(const EnvironmentalImpactFactors_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~EnvironmentalImpactFactors_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      double districtHeatingWaterEfficiency() const;

      double districtCoolingCOP() const;

      double districtHeatingSteamConversionEfficiency() const;

      double totalCarbonEquivalentEmissionFactorFromN2O() const;

      double totalCarbonEquivalentEmissionFactorFromCH4() const;

      double totalCarbonEquivalentEmissionFactorFromCO2() const;

      //@}
      /** @name Setters */
      //@{

      bool setDistrictHeatingWaterEfficiency(double districtHeatingWaterEfficiency);

      bool setDistrictCoolingCOP(double districtCoolingCOP);

      bool setDistrictHeatingSteamConversionEfficiency(double districtHeatingSteamConversionEfficiency);

      bool setTotalCarbonEquivalentEmissionFactorFromN2O(double totalCarbonEquivalentEmissionFactorFromN2O);

      bool setTotalCarbonEquivalentEmissionFactorFromCH4(double totalCarbonEquivalentEmissionFactorFromCH4);

      bool setTotalCarbonEquivalentEmissionFactorFromCO2(double totalCarbonEquivalentEmissionFactorFromCO2);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.EnvironmentalImpactFactors");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENVIRONMENTALIMPACTFACTORS_IMPL_HPP
