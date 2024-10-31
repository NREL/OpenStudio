/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OPAQUEMATERIAL_IMPL_HPP
#define MODEL_OPAQUEMATERIAL_IMPL_HPP

#include "Material_Impl.hpp"

namespace openstudio {
namespace model {

  // forward declaration
  class OpaqueMaterial;

  namespace detail {

    class MODEL_API OpaqueMaterial_Impl : public Material_Impl
    {

     public:
      // Construct completely new object.
      OpaqueMaterial_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // Construct from existing workspace object (typically when Model is being constructed
      // from Workspace).
      OpaqueMaterial_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // Clone copy constructor.
      OpaqueMaterial_Impl(const OpaqueMaterial_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~OpaqueMaterial_Impl() override = default;

      /** @name Getters */
      //@{

      /** The conductivity of the material in W/m*K. */
      virtual double thermalConductivity() const = 0;

      /** The conductance of the material in W/m^2*K. */
      virtual double thermalConductance() const = 0;

      /** The resistivity of the material in m*K/W. */
      virtual double thermalResistivity() const = 0;

      /** The resistance of the material in m^2*K/W. */
      virtual double thermalResistance() const = 0;

      virtual double thermalAbsorptance() const = 0;

      virtual boost::optional<double> thermalReflectance() const = 0;

      virtual double solarAbsorptance() const = 0;

      virtual boost::optional<double> solarReflectance() const = 0;

      virtual boost::optional<double> getVisibleTransmittance() const override;

      virtual double visibleAbsorptance() const = 0;

      virtual boost::optional<double> interiorVisibleAbsorptance() const override;

      virtual boost::optional<double> exteriorVisibleAbsorptance() const override;

      virtual boost::optional<double> visibleReflectance() const = 0;

      //@}
      /** @name Setters */
      //@{

      /** Sets the conductivity of the material in W/m*K, if possible. */
      virtual bool setThermalConductivity(double value) = 0;

      /** Sets the conductance of the material in W/m^2*K, if possible. */
      virtual bool setThermalConductance(double value) = 0;

      /** Sets the resistivity of the material in m*K/W, if possible. */
      virtual bool setThermalResistivity(double value) = 0;

      /** Sets the resistance of the material in m^2*K/W, if possible. */
      virtual bool setThermalResistance(double value) = 0;

      virtual bool setThermalAbsorptance(double value) = 0;

      virtual bool setThermalReflectance(boost::optional<double> value) = 0;

      virtual bool setSolarAbsorptance(boost::optional<double> value) = 0;

      virtual bool setSolarReflectance(boost::optional<double> value) = 0;

      virtual bool setVisibleAbsorptance(boost::optional<double> value) = 0;

      virtual bool setVisibleReflectance(boost::optional<double> value) = 0;

      //@}
      /** @name Queries */
      //@{

      /** Energy it takes to raise the temperature of this material 1 K (J/m^2*K). Equal to
     *  specificHeat() * density() * thickness(). */
      virtual boost::optional<double> heatCapacity() const;

      //@}

     private:
      REGISTER_LOGGER("openstudio.model.OpaqueMaterial");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OPAQUEMATERIAL_IMPL_HPP
