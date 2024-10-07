/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SHADE_IMPL_HPP
#define MODEL_SHADE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ShadingMaterial_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** Shade_Impl is a ShadingMaterial_Impl that is the implementation class for Shade.*/
    class MODEL_API Shade_Impl : public ShadingMaterial_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      Shade_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      Shade_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      Shade_Impl(const Shade_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~Shade_Impl() override = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      /** Get the thickness of the material. Note that the construction may need to add additional
     *  thickness for an air gap, depending on context. */
      virtual double thickness() const override;

      /** The conductivity of the material in W/m*K. */
      double thermalConductivity() const;

      /** The conductance of the material in W/m^2*K. */
      double thermalConductance() const;

      /** The resistivity of the material in m*K/W. */
      double thermalResistivity() const;

      /** The resistance of the material in m^2*K/W. */
      double thermalResistance() const;

      double thermalTransmittance() const;

      double solarTransmittance() const;

      double solarAbsorptance() const;

      double solarReflectance() const;

      double visibleAbsorptance() const;

      virtual boost::optional<double> interiorVisibleAbsorptance() const override;

      virtual boost::optional<double> exteriorVisibleAbsorptance() const override;

      double visibleReflectance() const;

      double visibleTransmittance() const;

      virtual boost::optional<double> getVisibleTransmittance() const override;

      double thermalHemisphericalEmissivity() const;

      double conductivity() const;

      double shadetoGlassDistance() const;

      bool isShadetoGlassDistanceDefaulted() const;

      double topOpeningMultiplier() const;

      bool isTopOpeningMultiplierDefaulted() const;

      double bottomOpeningMultiplier() const;

      bool isBottomOpeningMultiplierDefaulted() const;

      double leftSideOpeningMultiplier() const;

      bool isLeftSideOpeningMultiplierDefaulted() const;

      double rightSideOpeningMultiplier() const;

      bool isRightSideOpeningMultiplierDefaulted() const;

      double airflowPermeability() const;

      bool isAirflowPermeabilityDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      /** Set thickness to value (m). */
      virtual bool setThickness(double thickness) override;

      /** Sets the conductivity of the material in W/m*K, if possible. */
      bool setThermalConductivity(double value);

      /** Sets the conductance of the material in W/m^2*K, if possible. */
      bool setThermalConductance(double value);

      /** Sets the resistivity of the material in m*K/W, if possible. */
      bool setThermalResistivity(double value);

      /** Sets the resistance of the material in m^2*K/W, if possible. */
      bool setThermalResistance(double value);

      bool setThermalTransmittance(double thermalTransmittance);

      bool setSolarTransmittance(double solarTransmittance);

      bool setSolarReflectance(double solarReflectance);

      bool setVisibleTransmittance(double visibleTransmittance);

      bool setVisibleReflectance(double visibleReflectance);

      bool setThermalHemisphericalEmissivity(double thermalHemisphericalEmissivity);

      bool setConductivity(double conductivity);

      bool setShadetoGlassDistance(double shadetoGlassDistance);

      void resetShadetoGlassDistance();

      bool setTopOpeningMultiplier(double topOpeningMultiplier);

      void resetTopOpeningMultiplier();

      bool setBottomOpeningMultiplier(double bottomOpeningMultiplier);

      void resetBottomOpeningMultiplier();

      bool setLeftSideOpeningMultiplier(double leftSideOpeningMultiplier);

      void resetLeftSideOpeningMultiplier();

      bool setRightSideOpeningMultiplier(double rightSideOpeningMultiplier);

      void resetRightSideOpeningMultiplier();

      bool setAirflowPermeability(double airflowPermeability);

      void resetAirflowPermeability();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.Shade");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SHADE_IMPL_HPP
