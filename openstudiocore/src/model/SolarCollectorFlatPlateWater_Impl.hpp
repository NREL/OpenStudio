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

#ifndef MODEL_SOLARCOLLECTORFLATPLATEWATER_IMPL_HPP
#define MODEL_SOLARCOLLECTORFLATPLATEWATER_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class SolarCollectorPerformanceFlatPlate;
class PlanarSurface;
//class Connection;

namespace detail {

  /** SolarCollectorFlatPlateWater_Impl is a StraightComponent_Impl that is the implementation class for SolarCollectorFlatPlateWater.*/
  class MODEL_API SolarCollectorFlatPlateWater_Impl : public StraightComponent_Impl {

   public:
    /** @name Constructors and Destructors */
    //@{

    SolarCollectorFlatPlateWater_Impl(const IdfObject& idfObject,
                                      Model_Impl* model,
                                      bool keepHandle);

    SolarCollectorFlatPlateWater_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                      Model_Impl* model,
                                      bool keepHandle);

    SolarCollectorFlatPlateWater_Impl(const SolarCollectorFlatPlateWater_Impl& other,
                                      Model_Impl* model,
                                      bool keepHandle);

    virtual ~SolarCollectorFlatPlateWater_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual ModelObject clone(Model model) const override;

    virtual std::vector<IdfObject> remove() override;

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ModelObject> children() const override;

    virtual unsigned inletPort() override;

    virtual unsigned outletPort() override;

    virtual bool addToNode(Node & node) override;

    //@}
    /** @name Getters */
    //@{

    SolarCollectorPerformanceFlatPlate solarCollectorPerformance() const;

    boost::optional<PlanarSurface> surface() const;

    //Connection inletNode() const;

    //Connection outletNode() const;

    boost::optional<double> maximumFlowRate() const;

    //@}
    /** @name Setters */
    //@{

    bool setSolarCollectorPerformance(const SolarCollectorPerformanceFlatPlate& solarCollectorPerformanceFlatPlate);

    void resetSolarCollectorPerformance();
    
    bool setSurface(const PlanarSurface& surface);
    
    void resetSurface();

    //bool setInletNode(const Connection& connection);

    //bool setOutletNode(const Connection& connection);

    bool setMaximumFlowRate(double maximumFlowRate);

    void resetMaximumFlowRate();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.SolarCollectorFlatPlateWater");
    
    bool setSolarCollectorPerformanceNoClone(const SolarCollectorPerformanceFlatPlate& parameters);
    
    friend class openstudio::model::SolarCollectorFlatPlateWater;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SOLARCOLLECTORFLATPLATEWATER_IMPL_HPP

