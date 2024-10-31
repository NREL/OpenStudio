/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRGAP_IMPL_HPP
#define MODEL_AIRGAP_IMPL_HPP

#include "ModelAPI.hpp"
#include "OpaqueMaterial_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** AirGap_Impl is a OpaqueMaterial_Impl that is the implementation class for AirGap.*/
    class MODEL_API AirGap_Impl : public OpaqueMaterial_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      AirGap_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirGap_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirGap_Impl(const AirGap_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirGap_Impl() override = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      double thermalResistance() const override;

      virtual double thickness() const override;

      /** The conductivity of the material in W/m*K. Always throws because no real thickness, which
     *  is needed for conversion. */
      virtual double thermalConductivity() const override;

      /** The conductance of the material in W/m^2*K. Always throws because no real thickness, which
     *  is needed for conversion. */
      virtual double thermalConductance() const override;

      /** The resistivity of the material in m*K/W. Always throws because no real thickness, which
     *  is needed for conversion. */
      virtual double thermalResistivity() const override;

      virtual double thermalAbsorptance() const override;

      virtual boost::optional<double> thermalReflectance() const override;

      virtual double solarAbsorptance() const override;

      virtual boost::optional<double> solarReflectance() const override;

      virtual double visibleTransmittance() const;

      virtual double visibleAbsorptance() const override;

      virtual boost::optional<double> visibleReflectance() const override;

      //@}
      /** @name Setters */
      //@{

      bool setThermalResistance(boost::optional<double> thermalResistance);

      void resetThermalResistance();

      virtual bool setThickness(double value) override;

      /** Sets the conductivity of the material in W/m*K, if possible. Always returns false. */
      virtual bool setThermalConductivity(double value) override;

      /** Sets the conductance of the material in W/m^2*K, if possible. */
      virtual bool setThermalConductance(double value) override;

      /** Sets the resistivity of the material in m*K/W, if possible. */
      virtual bool setThermalResistivity(double value) override;

      /** Sets the resistance of the material in m^2*K/W, if possible. */
      virtual bool setThermalResistance(double value) override;

      virtual bool setThermalAbsorptance(double value) override;

      virtual bool setThermalReflectance(boost::optional<double> value) override;

      virtual bool setSolarAbsorptance(boost::optional<double> value) override;

      virtual bool setSolarReflectance(boost::optional<double> value) override;

      virtual bool setVisibleAbsorptance(boost::optional<double> value) override;

      virtual bool setVisibleReflectance(boost::optional<double> value) override;

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirGap");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRGAP_IMPL_HPP
