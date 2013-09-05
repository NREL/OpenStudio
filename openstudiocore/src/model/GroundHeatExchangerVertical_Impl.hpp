/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef MODEL_GROUNDHEATEXCHANGERVERTICAL_IMPL_HPP
#define MODEL_GROUNDHEATEXCHANGERVERTICAL_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/StraightComponent_Impl.hpp>

namespace openstudio {
namespace model {
namespace detail {

  /** GroundHeatExchangerVertical_Impl is a StraightComponent_Impl that is the implementation class for GroundHeatExchangerVertical.*/
  class MODEL_API GroundHeatExchangerVertical_Impl : public StraightComponent_Impl {
    Q_OBJECT;

   public:
    /** @name Constructors and Destructors */
    //@{

    GroundHeatExchangerVertical_Impl(const IdfObject& idfObject,
                                     Model_Impl* model,
                                     bool keepHandle);

    GroundHeatExchangerVertical_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle);

    GroundHeatExchangerVertical_Impl(const GroundHeatExchangerVertical_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle);

    virtual ~GroundHeatExchangerVertical_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual ModelObject clone(Model model) const;
    
    virtual unsigned inletPort();

    virtual unsigned outletPort();

    bool addToNode(Node & node);

    bool addGFunction(double gFunctionLN, double gFunctionGValue);

    void removeGFunction(unsigned groupIndex);

    void removeAllGFunctions();

    std::vector< std::pair<double,double> > gFunctions();

    //@}
    /** @name Getters */
    //@{

    boost::optional<double> maximumFlowRate() const;

    boost::optional<int> numberofBoreHoles() const;

    boost::optional<double> boreHoleLength() const;

    boost::optional<double> boreHoleRadius() const;

    boost::optional<double> groundThermalConductivity() const;

    boost::optional<double> groundThermalHeatCapacity() const;

    boost::optional<double> groundTemperature() const;

    boost::optional<double> designFlowRate() const;

    boost::optional<double> groutThermalConductivity() const;

    boost::optional<double> pipeThermalConductivity() const;

    boost::optional<double> pipeOutDiameter() const;

    boost::optional<double> uTubeDistance() const;

    boost::optional<double> pipeThickness() const;

    boost::optional<double> maximumLengthofSimulation() const;

    double gFunctionReferenceRatio() const;

    bool isGFunctionReferenceRatioDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setMaximumFlowRate(boost::optional<double> maximumFlowRate);

    void resetMaximumFlowRate();

    bool setNumberofBoreHoles(boost::optional<int> numberofBoreHoles);

    void resetNumberofBoreHoles();

    bool setBoreHoleLength(boost::optional<double> boreHoleLength);

    void resetBoreHoleLength();

    bool setBoreHoleRadius(boost::optional<double> boreHoleRadius);

    void resetBoreHoleRadius();

    bool setGroundThermalConductivity(boost::optional<double> groundThermalConductivity);

    void resetGroundThermalConductivity();

    bool setGroundThermalHeatCapacity(boost::optional<double> groundThermalHeatCapacity);

    void resetGroundThermalHeatCapacity();

    bool setGroundTemperature(boost::optional<double> groundTemperature);

    void resetGroundTemperature();

    bool setDesignFlowRate(boost::optional<double> designFlowRate);

    void resetDesignFlowRate();

    bool setGroutThermalConductivity(boost::optional<double> groutThermalConductivity);

    void resetGroutThermalConductivity();

    bool setPipeThermalConductivity(boost::optional<double> pipeThermalConductivity);

    void resetPipeThermalConductivity();

    bool setPipeOutDiameter(boost::optional<double> pipeOutDiameter);

    void resetPipeOutDiameter();

    bool setUTubeDistance(boost::optional<double> uTubeDistance);

    void resetUTubeDistance();

    bool setPipeThickness(boost::optional<double> pipeThickness);

    void resetPipeThickness();

    bool setMaximumLengthofSimulation(boost::optional<double> maximumLengthofSimulation);

    void resetMaximumLengthofSimulation();

    bool setGFunctionReferenceRatio(double gFunctionReferenceRatio);

    void resetGFunctionReferenceRatio();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.GroundHeatExchangerVertical");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals

  };

} // detail

} // model
} // openstudio

#endif // MODEL_GROUNDHEATEXCHANGERVERTICAL_IMPL_HPP

