/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_THERMOCHROMICGLAZING_IMPL_HPP
#define MODEL_THERMOCHROMICGLAZING_IMPL_HPP

#include "ModelAPI.hpp"
#include "Glazing.hpp"
#include "Glazing_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** ThermochromicGlazing_Impl is a Glazing_Impl that is the implementation class for ThermochromicGlazing.*/
    class MODEL_API ThermochromicGlazing_Impl : public Glazing_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ThermochromicGlazing_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ThermochromicGlazing_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ThermochromicGlazing_Impl(const ThermochromicGlazing_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ThermochromicGlazing_Impl() override = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      virtual double thickness() const override;

      virtual double thermalConductivity() const;

      virtual double thermalConductance() const;

      virtual double thermalResistivity() const;

      virtual double thermalResistance() const;

      virtual double thermalTransmittance() const;

      virtual double thermalAbsorptance() const;

      virtual double thermalReflectance() const;

      virtual double solarTransmittance() const;

      virtual double solarAbsorptance() const;

      virtual double solarReflectance() const;

      virtual boost::optional<double> getVisibleTransmittance() const override;

      virtual double visibleAbsorptance() const;

      virtual double visibleReflectance() const;

      double opticalDataTemperature() const;

      // TODO: Handle this object's extensible fields.

      //@}
      /** @name Setters */
      //@{

      virtual bool setThickness(double value) override;

      virtual bool setThermalConductivity(double value);

      virtual bool setThermalConductance(double value);

      virtual bool setThermalResistivity(double value);

      virtual bool setThermalResistance(double value);

      virtual bool setThermalTransmittance(double value);

      virtual bool setThermalAbsorptance(double value);

      virtual bool setThermalReflectance(double value);

      virtual bool setSolarTransmittance(double value);

      virtual bool setSolarAbsorptance(double value);

      virtual bool setSolarReflectance(double value);

      virtual bool setVisibleTransmittance(double value);

      virtual bool setVisibleAbsorptance(double value);

      virtual bool setVisibleReflectance(double value);

      bool setOpticalDataTemperature(double value);

      // TODO: Handle this object's extensible fields.

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ThermochromicGlazing");

      std::vector<Glazing> mf_glazings() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_THERMOCHROMICGLAZING_IMPL_HPP
