/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SOLARCOLLECTORPERFORMANCEPHOTOVOLTAICTHERMALBIPVT_HPP
#define MODEL_SOLARCOLLECTORPERFORMANCEPHOTOVOLTAICTHERMALBIPVT_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class SurfacePropertyOtherSideConditionsModel;
  class Schedule;

  namespace detail {

    class SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl;

  }  // namespace detail

  /** SolarCollectorPerformancePhotovoltaicThermalBIPVT is a ModelObject that wraps the OpenStudio IDD object 'OS:SolarCollectorPerformance:PhotovoltaicThermal:BIPVT'. */
  class MODEL_API SolarCollectorPerformancePhotovoltaicThermalBIPVT : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SolarCollectorPerformancePhotovoltaicThermalBIPVT(const Model& model);
    explicit SolarCollectorPerformancePhotovoltaicThermalBIPVT(const SurfacePropertyOtherSideConditionsModel& oscm);

    virtual ~SolarCollectorPerformancePhotovoltaicThermalBIPVT() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SolarCollectorPerformancePhotovoltaicThermalBIPVT(const SolarCollectorPerformancePhotovoltaicThermalBIPVT& other) = default;
    SolarCollectorPerformancePhotovoltaicThermalBIPVT(SolarCollectorPerformancePhotovoltaicThermalBIPVT&& other) = default;
    SolarCollectorPerformancePhotovoltaicThermalBIPVT& operator=(const SolarCollectorPerformancePhotovoltaicThermalBIPVT&) = default;
    SolarCollectorPerformancePhotovoltaicThermalBIPVT& operator=(SolarCollectorPerformancePhotovoltaicThermalBIPVT&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    SurfacePropertyOtherSideConditionsModel boundaryConditionsModel() const;

    Schedule availabilitySchedule() const;

    double effectivePlenumGapThicknessBehindPVModules() const;

    double pVCellNormalTransmittanceAbsorptanceProduct() const;

    double backingMaterialNormalTransmittanceAbsorptanceProduct() const;

    double claddingNormalTransmittanceAbsorptanceProduct() const;

    double fractionofCollectorGrossAreaCoveredbyPVModule() const;

    double fractionofPVCellAreatoPVModuleArea() const;

    double pVModuleTopThermalResistance() const;

    double pVModuleBottomThermalResistance() const;

    double pVModuleFrontLongwaveEmissivity() const;

    double pVModuleBackLongwaveEmissivity() const;

    double glassThickness() const;

    double glassRefractionIndex() const;

    double glassExtinctionCoefficient() const;

    //@}
    /** @name Setters */
    //@{

    bool setBoundaryConditionsModel(const SurfacePropertyOtherSideConditionsModel& surfacePropertyOtherSideConditionsModel);

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setEffectivePlenumGapThicknessBehindPVModules(double effectivePlenumGapThicknessBehindPVModules);

    bool setPVCellNormalTransmittanceAbsorptanceProduct(double pVCellNormalTransmittanceAbsorptanceProduct);

    bool setBackingMaterialNormalTransmittanceAbsorptanceProduct(double backingMaterialNormalTransmittanceAbsorptanceProduct);

    bool setCladdingNormalTransmittanceAbsorptanceProduct(double claddingNormalTransmittanceAbsorptanceProduct);

    bool setFractionofCollectorGrossAreaCoveredbyPVModule(double fractionofCollectorGrossAreaCoveredbyPVModule);

    bool setFractionofPVCellAreatoPVModuleArea(double fractionofPVCellAreatoPVModuleArea);

    bool setPVModuleTopThermalResistance(double pVModuleTopThermalResistance);

    bool setPVModuleBottomThermalResistance(double pVModuleBottomThermalResistance);

    bool setPVModuleFrontLongwaveEmissivity(double pVModuleFrontLongwaveEmissivity);

    bool setPVModuleBackLongwaveEmissivity(double pVModuleBackLongwaveEmissivity);

    bool setGlassThickness(double glassThickness);

    bool setGlassRefractionIndex(double glassRefractionIndex);

    bool setGlassExtinctionCoefficient(double glassExtinctionCoefficient);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl;

    explicit SolarCollectorPerformancePhotovoltaicThermalBIPVT(std::shared_ptr<detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl> impl);

    friend class detail::SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SolarCollectorPerformancePhotovoltaicThermalBIPVT");
  };

  /** \relates SolarCollectorPerformancePhotovoltaicThermalBIPVT*/
  using OptionalSolarCollectorPerformancePhotovoltaicThermalBIPVT = boost::optional<SolarCollectorPerformancePhotovoltaicThermalBIPVT>;

  /** \relates SolarCollectorPerformancePhotovoltaicThermalBIPVT*/
  using SolarCollectorPerformancePhotovoltaicThermalBIPVTVector = std::vector<SolarCollectorPerformancePhotovoltaicThermalBIPVT>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SOLARCOLLECTORPERFORMANCEPHOTOVOLTAICTHERMALBIPVT_HPP
