/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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

    virtual boost::optional<double> ratedThermalToElectricalPowerRatio() const override;

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

    void setRatedElectricPowerOutput(double ratedElectricPowerOutput);

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

