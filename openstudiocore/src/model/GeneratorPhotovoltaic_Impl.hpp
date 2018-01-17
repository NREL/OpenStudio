/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_GENERATORPHOTOVOLTAIC_IMPL_HPP
#define MODEL_GENERATORPHOTOVOLTAIC_IMPL_HPP

#include "ModelAPI.hpp"
#include "Generator_Impl.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class AllShadingAndHTSurfName;
class PVModules;
class Schedule;

namespace detail {

  /** GeneratorPhotovoltaic_Impl is a Generator_Impl that is the implementation class for GeneratorPhotovoltaic.*/
  class MODEL_API GeneratorPhotovoltaic_Impl : public Generator_Impl {

   public:
    /** @name Constructors and Destructors */
    //@{

    GeneratorPhotovoltaic_Impl(const IdfObject& idfObject,
                               Model_Impl* model,
                               bool keepHandle);

    GeneratorPhotovoltaic_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                               Model_Impl* model,
                               bool keepHandle);

    GeneratorPhotovoltaic_Impl(const GeneratorPhotovoltaic_Impl& other,
                               Model_Impl* model,
                               bool keepHandle);

    virtual ~GeneratorPhotovoltaic_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    /// returns PhotovoltaicPerformance
    virtual std::vector<ModelObject> children() const override;

    /// remove the object from the model's workspace
    virtual std::vector<openstudio::IdfObject> remove() override;

    /// get a vector of allowable children types
    virtual std::vector<IddObjectType> allowableChildTypes() const override;

    /// returns deletes reference to Surface from clone
    virtual ModelObject clone(Model model) const override;

    virtual std::string generatorObjectType() const override;

    virtual boost::optional<double> ratedElectricPowerOutput() const override;

    virtual boost::optional<Schedule> availabilitySchedule() const override;

    virtual boost::optional<double> ratedThermaltoElectricalPowerRatio() const override;

    //@}
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

    //@}
    /** @name Setters */
    //@{

    bool setSurface(const PlanarSurface& surface);

    void resetSurface();

    bool setHeatTransferIntegrationMode(const std::string& heatTransferIntegrationMode);

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
   private:
    REGISTER_LOGGER("openstudio.model.GeneratorPhotovoltaic");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_GENERATORPHOTOVOLTAIC_IMPL_HPP
