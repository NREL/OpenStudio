/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

      virtual ~SolarCollectorPerformancePhotovoltaicThermalBIPVT_Impl() = default;

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
