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

#ifndef MODEL_SOLARCOLLECTORPERFORMANCEPHOTOVOLTAICTHERMALSIMPLE_HPP
#define MODEL_SOLARCOLLECTORPERFORMANCEPHOTOVOLTAICTHERMALSIMPLE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class SolarCollectorFlatPlatePhotovoltaicThermal;
  class Schedule;

  namespace detail {

    class SolarCollectorFlatPlatePhotovoltaicThermal_Impl;
    class SolarCollectorPerformancePhotovoltaicThermalSimple_Impl;

  }  // namespace detail

  /** SolarCollectorPerformancePhotovoltaicThermalSimple is a ModelObject that wraps the OpenStudio IDD object 'OS:SolarCollectorPerformance:PhotovoltaicThermal:Simple'. */
  class MODEL_API SolarCollectorPerformancePhotovoltaicThermalSimple : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SolarCollectorPerformancePhotovoltaicThermalSimple(const Model& model);

    virtual ~SolarCollectorPerformancePhotovoltaicThermalSimple() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SolarCollectorPerformancePhotovoltaicThermalSimple(const SolarCollectorPerformancePhotovoltaicThermalSimple& other) = default;
    SolarCollectorPerformancePhotovoltaicThermalSimple(SolarCollectorPerformancePhotovoltaicThermalSimple&& other) = default;
    SolarCollectorPerformancePhotovoltaicThermalSimple& operator=(const SolarCollectorPerformancePhotovoltaicThermalSimple&) = default;
    SolarCollectorPerformancePhotovoltaicThermalSimple& operator=(SolarCollectorPerformancePhotovoltaicThermalSimple&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> thermalConversionEfficiencyInputModeTypeValues();

    /** @name Getters */
    //@{

    double fractionOfSurfaceAreaWithActiveThermalCollector() const;

    std::string thermalConversionEfficiencyInputModeType() const;

    boost::optional<double> thermalConversionEfficiency() const;

    boost::optional<Schedule> thermalConversionEfficiencySchedule() const;

    double frontSurfaceEmittance() const;

    bool isFrontSurfaceEmittanceDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setFractionOfSurfaceAreaWithActiveThermalCollector(double fractionofSurfaceAreawithActiveThermalCollector);

    bool setThermalConversionEfficiency(double valueforThermalConversionEfficiencyifFixed);

    void resetThermalConversionEfficiency();

    bool setThermalConversionEfficiencySchedule(Schedule& schedule);

    void resetThermalConversionEfficiencySchedule();

    bool setFrontSurfaceEmittance(double frontSurfaceEmittance);

    void resetFrontSurfaceEmittance();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl;

    explicit SolarCollectorPerformancePhotovoltaicThermalSimple(
      std::shared_ptr<detail::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl> impl);

    friend class detail::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl;
    friend class detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl;
    friend class SolarCollectorFlatPlatePhotovoltaicThermal;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SolarCollectorPerformancePhotovoltaicThermalSimple");
  };

  /** \relates SolarCollectorPerformancePhotovoltaicThermalSimple*/
  using OptionalSolarCollectorPerformancePhotovoltaicThermalSimple = boost::optional<SolarCollectorPerformancePhotovoltaicThermalSimple>;

  /** \relates SolarCollectorPerformancePhotovoltaicThermalSimple*/
  using SolarCollectorPerformancePhotovoltaicThermalSimpleVector = std::vector<SolarCollectorPerformancePhotovoltaicThermalSimple>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SOLARCOLLECTORPERFORMANCEPHOTOVOLTAICTHERMALSIMPLE_HPP
