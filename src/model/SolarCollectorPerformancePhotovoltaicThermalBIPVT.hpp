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

    virtual ~SolarCollectorPerformancePhotovoltaicThermalBIPVT() = default;
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
