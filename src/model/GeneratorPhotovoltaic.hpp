/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_GENERATORPHOTOVOLTAIC_HPP
#define MODEL_GENERATORPHOTOVOLTAIC_HPP

#include "ModelAPI.hpp"
#include "Generator.hpp"

namespace openstudio {

namespace model {

  class PlanarSurface;
  class PhotovoltaicPerformance;
  class Schedule;

  namespace detail {

    class GeneratorPhotovoltaic_Impl;

  }  // namespace detail

  /** GeneratorPhotovoltaic is a Generator that wraps the OpenStudio IDD object 'OS:Generator:Photovoltaic'. */
  class MODEL_API GeneratorPhotovoltaic : public Generator
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /// Creates a GeneratorPhotovoltaic object with PhotovoltaicPerformanceSimple performance
    static GeneratorPhotovoltaic simple(const Model& model);

    /// Creates a GeneratorPhotovoltaic object with PhotovoltaicPerformanceEquivalentOneDiode performance
    static GeneratorPhotovoltaic equivalentOneDiode(const Model& model);

    virtual ~GeneratorPhotovoltaic() {}

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> heatTransferIntegrationModeValues();

    /** @name Getters */
    //@{

    boost::optional<PlanarSurface> surface() const;

    PhotovoltaicPerformance photovoltaicPerformance() const;

    std::string heatTransferIntegrationMode() const;

    bool isHeatTransferIntegrationModeDefaulted() const;

    double numberOfModulesInParallel() const;

    bool isNumberOfModulesInParallelDefaulted() const;

    double numberOfModulesInSeries() const;

    bool isNumberOfModulesInSeriesDefaulted() const;

    boost::optional<double> ratedElectricPowerOutput() const;

    boost::optional<Schedule> availabilitySchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setSurface(const PlanarSurface& surface);

    void resetSurface();

    bool setHeatTransferIntegrationMode(std::string heatTransferIntegrationMode);

    void resetHeatTransferIntegrationMode();

    bool setNumberOfModulesInParallel(double numberOfModulesInParallel);

    void resetNumberOfModulesInParallel();

    bool setNumberOfModulesInSeries(double numberOfModulesInSeries);

    void resetNumberOfModulesInSeries();

    bool setRatedElectricPowerOutput(double ratedElectricPowerOutput);

    void resetRatedElectricPowerOutput();

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    typedef detail::GeneratorPhotovoltaic_Impl ImplType;

    GeneratorPhotovoltaic(const Model& model, const PhotovoltaicPerformance& performance);

    explicit GeneratorPhotovoltaic(std::shared_ptr<detail::GeneratorPhotovoltaic_Impl> impl);

    friend class detail::GeneratorPhotovoltaic_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.GeneratorPhotovoltaic");
  };

  /** \relates GeneratorPhotovoltaic*/
  typedef boost::optional<GeneratorPhotovoltaic> OptionalGeneratorPhotovoltaic;

  /** \relates GeneratorPhotovoltaic*/
  typedef std::vector<GeneratorPhotovoltaic> GeneratorPhotovoltaicVector;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORPHOTOVOLTAIC_HPP
