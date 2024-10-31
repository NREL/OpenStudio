/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_MATERIALPROPERTYPHASECHANGE_IMPL_HPP
#define MODEL_MATERIALPROPERTYPHASECHANGE_IMPL_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class TemperatureEnthalpy;
  class Material;

  namespace detail {

    /** MaterialPropertyPhaseChange_Impl is a ModelObject_Impl that is the implementation class for MaterialPropertyPhaseChange.*/
    class MODEL_API MaterialPropertyPhaseChange_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      MaterialPropertyPhaseChange_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      MaterialPropertyPhaseChange_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      MaterialPropertyPhaseChange_Impl(const MaterialPropertyPhaseChange_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~MaterialPropertyPhaseChange_Impl() override = default;

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

      double temperatureCoefficientforThermalConductivity() const;

      bool isTemperatureCoefficientforThermalConductivityDefaulted() const;

      std::vector<TemperatureEnthalpy> temperatureEnthalpys() const;

      unsigned int numberofTemperatureEnthalpys() const;

      //@}
      /** @name Setters */
      //@{

      bool setTemperatureCoefficientforThermalConductivity(double temperatureCoefficientforThermalConductivity);

      void resetTemperatureCoefficientforThermalConductivity();

      bool addTemperatureEnthalpy(const TemperatureEnthalpy& temperatureEnthalpy);

      bool addTemperatureEnthalpy(double temperature, double enthalpy);

      bool addTemperatureEnthalpys(const std::vector<TemperatureEnthalpy>& temperatureEnthalpys);

      bool removeTemperatureEnthalpy(unsigned groupIndex);

      void removeAllTemperatureEnthalpys();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.MaterialPropertyPhaseChange");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_MATERIALPROPERTYPHASECHANGE_IMPL_HPP
