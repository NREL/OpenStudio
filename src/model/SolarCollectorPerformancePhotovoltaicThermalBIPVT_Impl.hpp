/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SOLARCOLLECTORPERFORMANCEPHOTOVOLTAICTHERMALBIPVT_IMPL_HPP
#define MODEL_SOLARCOLLECTORPERFORMANCEPHOTOVOLTAICTHERMALBIPVT_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class SurfacePropertyOtherSideConditionsModel;
  class Schedule;

  namespace detail {

    /** SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl is a ModelObject_Impl that is the implementation class for SolarCollectorPerformancePhotovoltaicThermalBIPVT.*/
    class MODEL_API SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                             bool keepHandle);

      SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl(const SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl& other, Model_Impl* model,
                                                             bool keepHandle);

      virtual ~SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
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
     private:
      REGISTER_LOGGER("openstudio.model.SolarCollectorPerformancePhotovoltaicThermalBIPVT");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      boost::optional<SurfacePropertyOtherSideConditionsModel> optionalBoundaryConditionsModel() const;
      boost::optional<Schedule> optionalAvailabilitySchedule() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SOLARCOLLECTORPERFORMANCEPHOTOVOLTAICTHERMALBIPVT_IMPL_HPP
