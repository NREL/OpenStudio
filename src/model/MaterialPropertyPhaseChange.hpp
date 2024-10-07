/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_MATERIALPROPERTYPHASECHANGE_HPP
#define MODEL_MATERIALPROPERTYPHASECHANGE_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class Material;
  class StandardOpaqueMaterial;
  class MasslessOpaqueMaterial;

  namespace detail {

    class MaterialPropertyPhaseChange_Impl;

  }  // namespace detail

  /** This class implements a temperature enthalpy */
  class MODEL_API TemperatureEnthalpy
  {
   public:
    TemperatureEnthalpy(double temperature, double enthalpy);

    double temperature() const;
    double enthalpy() const;

   private:
    double m_temperature;
    double m_enthalpy;
    REGISTER_LOGGER("openstudio.model.TemperatureEnthalpy");
  };

  // Overload operator<<
  MODEL_API std::ostream& operator<<(std::ostream& out, const openstudio::model::TemperatureEnthalpy& temperatureEnthalpy);

  /** MaterialPropertyPhaseChange is a ModelObject that wraps the OpenStudio IDD object 'OS:MaterialProperty:PhaseChange'. */
  class MODEL_API MaterialPropertyPhaseChange : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit MaterialPropertyPhaseChange(StandardOpaqueMaterial& material);

    explicit MaterialPropertyPhaseChange(MasslessOpaqueMaterial& material);

    explicit MaterialPropertyPhaseChange(StandardOpaqueMaterial& material, const std::vector<TemperatureEnthalpy>& temperatureEnthalpys);

    explicit MaterialPropertyPhaseChange(MasslessOpaqueMaterial& material, const std::vector<TemperatureEnthalpy>& temperatureEnthalpys);

    virtual ~MaterialPropertyPhaseChange() override = default;

    //@}

    static IddObjectType iddObjectType();

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

    void removeTemperatureEnthalpy(unsigned groupIndex);

    void removeAllTemperatureEnthalpys();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::MaterialPropertyPhaseChange_Impl;

    explicit MaterialPropertyPhaseChange(std::shared_ptr<detail::MaterialPropertyPhaseChange_Impl> impl);

    friend class detail::MaterialPropertyPhaseChange_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.MaterialPropertyPhaseChange");
  };

  /** \relates MaterialPropertyPhaseChange*/
  using OptionalMaterialPropertyPhaseChange = boost::optional<MaterialPropertyPhaseChange>;

  /** \relates MaterialPropertyPhaseChange*/
  using MaterialPropertyPhaseChangeVector = std::vector<MaterialPropertyPhaseChange>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_MATERIALPROPERTYPHASECHANGE_HPP
