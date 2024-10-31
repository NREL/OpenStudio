/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_MATERIALPROPERTYPHASECHANGEHYSTERESIS_IMPL_HPP
#define MODEL_MATERIALPROPERTYPHASECHANGEHYSTERESIS_IMPL_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Material;

  namespace detail {

    /** MaterialPropertyPhaseChangeHysteresis_Impl is a ModelObject_Impl that is the implementation class for MaterialPropertyPhaseChangeHysteresis.*/
    class MODEL_API MaterialPropertyPhaseChangeHysteresis_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      MaterialPropertyPhaseChangeHysteresis_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      MaterialPropertyPhaseChangeHysteresis_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      MaterialPropertyPhaseChangeHysteresis_Impl(const MaterialPropertyPhaseChangeHysteresis_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~MaterialPropertyPhaseChangeHysteresis_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string materialName() const;

      Material material() const;

      double latentHeatduringtheEntirePhaseChangeProcess() const;

      double liquidStateThermalConductivity() const;

      double liquidStateDensity() const;

      double liquidStateSpecificHeat() const;

      double highTemperatureDifferenceofMeltingCurve() const;

      double peakMeltingTemperature() const;

      double lowTemperatureDifferenceofMeltingCurve() const;

      double solidStateThermalConductivity() const;

      double solidStateDensity() const;

      double solidStateSpecificHeat() const;

      double highTemperatureDifferenceofFreezingCurve() const;

      double peakFreezingTemperature() const;

      double lowTemperatureDifferenceofFreezingCurve() const;

      //@}
      /** @name Setters */
      //@{

      bool setLatentHeatduringtheEntirePhaseChangeProcess(double latentHeatduringtheEntirePhaseChangeProcess);

      bool setLiquidStateThermalConductivity(double liquidStateThermalConductivity);

      bool setLiquidStateDensity(double liquidStateDensity);

      bool setLiquidStateSpecificHeat(double liquidStateSpecificHeat);

      bool setHighTemperatureDifferenceofMeltingCurve(double highTemperatureDifferenceofMeltingCurve);

      bool setPeakMeltingTemperature(double peakMeltingTemperature);

      bool setLowTemperatureDifferenceofMeltingCurve(double lowTemperatureDifferenceofMeltingCurve);

      bool setSolidStateThermalConductivity(double solidStateThermalConductivity);

      bool setSolidStateDensity(double solidStateDensity);

      bool setSolidStateSpecificHeat(double solidStateSpecificHeat);

      bool setHighTemperatureDifferenceofFreezingCurve(double highTemperatureDifferenceofFreezingCurve);

      bool setPeakFreezingTemperature(double peakFreezingTemperature);

      bool setLowTemperatureDifferenceofFreezingCurve(double lowTemperatureDifferenceofFreezingCurve);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.MaterialPropertyPhaseChangeHysteresis");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_MATERIALPROPERTYPHASECHANGEHYSTERESIS_IMPL_HPP
