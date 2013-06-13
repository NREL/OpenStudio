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

//#include <utilities/units/Quantity.hpp>
//#include <utilities/units/OSOptionalQuantity.hpp>

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
//class Connection;
//class Connection;

namespace detail {

  /** GroundHeatExchangerVertical_Impl is a StraightComponent_Impl that is the implementation class for GroundHeatExchangerVertical.*/
  class MODEL_API GroundHeatExchangerVertical_Impl : public StraightComponent_Impl {
    Q_OBJECT;

    Q_PROPERTY(boost::optional<double> maximumFlowRate READ maximumFlowRate WRITE setMaximumFlowRate RESET resetMaximumFlowRate);
    //Q_PROPERTY(openstudio::OSOptionalQuantity maximumFlowRate_SI READ maximumFlowRate_SI WRITE setMaximumFlowRate RESET resetMaximumFlowRate);
    //Q_PROPERTY(openstudio::OSOptionalQuantity maximumFlowRate_IP READ maximumFlowRate_IP WRITE setMaximumFlowRate RESET resetMaximumFlowRate);

    Q_PROPERTY(boost::optional<int> numberofBoreHoles READ numberofBoreHoles WRITE setNumberofBoreHoles RESET resetNumberofBoreHoles);

    Q_PROPERTY(boost::optional<double> boreHoleLength READ boreHoleLength WRITE setBoreHoleLength RESET resetBoreHoleLength);
    //Q_PROPERTY(openstudio::OSOptionalQuantity boreHoleLength_SI READ boreHoleLength_SI WRITE setBoreHoleLength RESET resetBoreHoleLength);
    //Q_PROPERTY(openstudio::OSOptionalQuantity boreHoleLength_IP READ boreHoleLength_IP WRITE setBoreHoleLength RESET resetBoreHoleLength);

    Q_PROPERTY(boost::optional<double> boreHoleRadius READ boreHoleRadius WRITE setBoreHoleRadius RESET resetBoreHoleRadius);
    //Q_PROPERTY(openstudio::OSOptionalQuantity boreHoleRadius_SI READ boreHoleRadius_SI WRITE setBoreHoleRadius RESET resetBoreHoleRadius);
    //Q_PROPERTY(openstudio::OSOptionalQuantity boreHoleRadius_IP READ boreHoleRadius_IP WRITE setBoreHoleRadius RESET resetBoreHoleRadius);

    Q_PROPERTY(boost::optional<double> groundThermalConductivity READ groundThermalConductivity WRITE setGroundThermalConductivity RESET resetGroundThermalConductivity);
    //Q_PROPERTY(openstudio::OSOptionalQuantity groundThermalConductivity_SI READ groundThermalConductivity_SI WRITE setGroundThermalConductivity RESET resetGroundThermalConductivity);
    //Q_PROPERTY(openstudio::OSOptionalQuantity groundThermalConductivity_IP READ groundThermalConductivity_IP WRITE setGroundThermalConductivity RESET resetGroundThermalConductivity);

    Q_PROPERTY(boost::optional<double> groundThermalHeatCapacity READ groundThermalHeatCapacity WRITE setGroundThermalHeatCapacity RESET resetGroundThermalHeatCapacity);
    //Q_PROPERTY(openstudio::OSOptionalQuantity groundThermalHeatCapacity_SI READ groundThermalHeatCapacity_SI WRITE setGroundThermalHeatCapacity RESET resetGroundThermalHeatCapacity);
    //Q_PROPERTY(openstudio::OSOptionalQuantity groundThermalHeatCapacity_IP READ groundThermalHeatCapacity_IP WRITE setGroundThermalHeatCapacity RESET resetGroundThermalHeatCapacity);

    Q_PROPERTY(boost::optional<double> groundTemperature READ groundTemperature WRITE setGroundTemperature RESET resetGroundTemperature);
    //Q_PROPERTY(openstudio::OSOptionalQuantity groundTemperature_SI READ groundTemperature_SI WRITE setGroundTemperature RESET resetGroundTemperature);
    //Q_PROPERTY(openstudio::OSOptionalQuantity groundTemperature_IP READ groundTemperature_IP WRITE setGroundTemperature RESET resetGroundTemperature);

    Q_PROPERTY(boost::optional<double> designFlowRate READ designFlowRate WRITE setDesignFlowRate RESET resetDesignFlowRate);
    //Q_PROPERTY(openstudio::OSOptionalQuantity designFlowRate_SI READ designFlowRate_SI WRITE setDesignFlowRate RESET resetDesignFlowRate);
    //Q_PROPERTY(openstudio::OSOptionalQuantity designFlowRate_IP READ designFlowRate_IP WRITE setDesignFlowRate RESET resetDesignFlowRate);

    Q_PROPERTY(boost::optional<double> groutThermalConductivity READ groutThermalConductivity WRITE setGroutThermalConductivity RESET resetGroutThermalConductivity);
    //Q_PROPERTY(openstudio::OSOptionalQuantity groutThermalConductivity_SI READ groutThermalConductivity_SI WRITE setGroutThermalConductivity RESET resetGroutThermalConductivity);
    //Q_PROPERTY(openstudio::OSOptionalQuantity groutThermalConductivity_IP READ groutThermalConductivity_IP WRITE setGroutThermalConductivity RESET resetGroutThermalConductivity);

    Q_PROPERTY(boost::optional<double> pipeThermalConductivity READ pipeThermalConductivity WRITE setPipeThermalConductivity RESET resetPipeThermalConductivity);
    //Q_PROPERTY(openstudio::OSOptionalQuantity pipeThermalConductivity_SI READ pipeThermalConductivity_SI WRITE setPipeThermalConductivity RESET resetPipeThermalConductivity);
    //Q_PROPERTY(openstudio::OSOptionalQuantity pipeThermalConductivity_IP READ pipeThermalConductivity_IP WRITE setPipeThermalConductivity RESET resetPipeThermalConductivity);

    Q_PROPERTY(boost::optional<double> pipeOutDiameter READ pipeOutDiameter WRITE setPipeOutDiameter RESET resetPipeOutDiameter);
    //Q_PROPERTY(openstudio::OSOptionalQuantity pipeOutDiameter_SI READ pipeOutDiameter_SI WRITE setPipeOutDiameter RESET resetPipeOutDiameter);
    //Q_PROPERTY(openstudio::OSOptionalQuantity pipeOutDiameter_IP READ pipeOutDiameter_IP WRITE setPipeOutDiameter RESET resetPipeOutDiameter);

    Q_PROPERTY(boost::optional<double> uTubeDistance READ uTubeDistance WRITE setUTubeDistance RESET resetUTubeDistance);
    //Q_PROPERTY(openstudio::OSOptionalQuantity uTubeDistance_SI READ uTubeDistance_SI WRITE setUTubeDistance RESET resetUTubeDistance);
    //Q_PROPERTY(openstudio::OSOptionalQuantity uTubeDistance_IP READ uTubeDistance_IP WRITE setUTubeDistance RESET resetUTubeDistance);

    Q_PROPERTY(boost::optional<double> pipeThickness READ pipeThickness WRITE setPipeThickness RESET resetPipeThickness);
    //Q_PROPERTY(openstudio::OSOptionalQuantity pipeThickness_SI READ pipeThickness_SI WRITE setPipeThickness RESET resetPipeThickness);
    //Q_PROPERTY(openstudio::OSOptionalQuantity pipeThickness_IP READ pipeThickness_IP WRITE setPipeThickness RESET resetPipeThickness);

    Q_PROPERTY(boost::optional<double> maximumLengthofSimulation READ maximumLengthofSimulation WRITE setMaximumLengthofSimulation RESET resetMaximumLengthofSimulation);
    //Q_PROPERTY(openstudio::OSOptionalQuantity maximumLengthofSimulation_SI READ maximumLengthofSimulation_SI WRITE setMaximumLengthofSimulation RESET resetMaximumLengthofSimulation);
    //Q_PROPERTY(openstudio::OSOptionalQuantity maximumLengthofSimulation_IP READ maximumLengthofSimulation_IP WRITE setMaximumLengthofSimulation RESET resetMaximumLengthofSimulation);

    Q_PROPERTY(double gFunctionReferenceRatio READ gFunctionReferenceRatio WRITE setGFunctionReferenceRatio RESET resetGFunctionReferenceRatio);
    //Q_PROPERTY(openstudio::Quantity gFunctionReferenceRatio_SI READ gFunctionReferenceRatio_SI WRITE setGFunctionReferenceRatio RESET resetGFunctionReferenceRatio);
    //Q_PROPERTY(openstudio::Quantity gFunctionReferenceRatio_IP READ gFunctionReferenceRatio_IP WRITE setGFunctionReferenceRatio RESET resetGFunctionReferenceRatio);
    Q_PROPERTY(bool isGFunctionReferenceRatioDefaulted READ isGFunctionReferenceRatioDefaulted);

    Q_PROPERTY(boost::optional<int> numberofDataPairsoftheGFunction READ numberofDataPairsoftheGFunction WRITE setNumberofDataPairsoftheGFunction RESET resetNumberofDataPairsoftheGFunction);

    // TODO: Add relationships for objects related to this one, but not pointed to by the underlying data.
    //       Such relationships can be generated by the GenerateRelationships.rb script.
//    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> inletNode READ inletNodeAsModelObject WRITE setInletNodeAsModelObject);
//    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> outletNode READ outletNodeAsModelObject WRITE setOutletNodeAsModelObject);
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


    //@}
    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: Connection.
//    Connection inletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
//    Connection outletNode() const;

    boost::optional<double> maximumFlowRate() const;

//    OSOptionalQuantity getMaximumFlowRate(bool returnIP=false) const;

    boost::optional<int> numberofBoreHoles() const;

    boost::optional<double> boreHoleLength() const;

//    OSOptionalQuantity getBoreHoleLength(bool returnIP=false) const;

    boost::optional<double> boreHoleRadius() const;

//    OSOptionalQuantity getBoreHoleRadius(bool returnIP=false) const;

    boost::optional<double> groundThermalConductivity() const;

//    OSOptionalQuantity getGroundThermalConductivity(bool returnIP=false) const;

    boost::optional<double> groundThermalHeatCapacity() const;

//    OSOptionalQuantity getGroundThermalHeatCapacity(bool returnIP=false) const;

    boost::optional<double> groundTemperature() const;

//    OSOptionalQuantity getGroundTemperature(bool returnIP=false) const;

    boost::optional<double> designFlowRate() const;

//    OSOptionalQuantity getDesignFlowRate(bool returnIP=false) const;

    boost::optional<double> groutThermalConductivity() const;

//    OSOptionalQuantity getGroutThermalConductivity(bool returnIP=false) const;

    boost::optional<double> pipeThermalConductivity() const;

//    OSOptionalQuantity getPipeThermalConductivity(bool returnIP=false) const;

    boost::optional<double> pipeOutDiameter() const;

//    OSOptionalQuantity getPipeOutDiameter(bool returnIP=false) const;

    boost::optional<double> uTubeDistance() const;

//    OSOptionalQuantity getUTubeDistance(bool returnIP=false) const;

    boost::optional<double> pipeThickness() const;

//    OSOptionalQuantity getPipeThickness(bool returnIP=false) const;

    boost::optional<double> maximumLengthofSimulation() const;

//    OSOptionalQuantity getMaximumLengthofSimulation(bool returnIP=false) const;

    double gFunctionReferenceRatio() const;

//    Quantity getGFunctionReferenceRatio(bool returnIP=false) const;

    bool isGFunctionReferenceRatioDefaulted() const;

    boost::optional<int> numberofDataPairsoftheGFunction() const;

    // TODO: Handle this object's extensible fields.

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: Connection.
//    bool setInletNode(const Connection& connection);

    // TODO: Check argument type. From object lists, some candidates are: Connection.
//    bool setOutletNode(const Connection& connection);

    bool setMaximumFlowRate(boost::optional<double> maximumFlowRate);

//    bool setMaximumFlowRate(const OSOptionalQuantity& maximumFlowRate);

    void resetMaximumFlowRate();

    bool setNumberofBoreHoles(boost::optional<int> numberofBoreHoles);

    void resetNumberofBoreHoles();

    bool setBoreHoleLength(boost::optional<double> boreHoleLength);

//    bool setBoreHoleLength(const OSOptionalQuantity& boreHoleLength);

    void resetBoreHoleLength();

    bool setBoreHoleRadius(boost::optional<double> boreHoleRadius);

//    bool setBoreHoleRadius(const OSOptionalQuantity& boreHoleRadius);

    void resetBoreHoleRadius();

    bool setGroundThermalConductivity(boost::optional<double> groundThermalConductivity);

//    bool setGroundThermalConductivity(const OSOptionalQuantity& groundThermalConductivity);

    void resetGroundThermalConductivity();

    bool setGroundThermalHeatCapacity(boost::optional<double> groundThermalHeatCapacity);

//    bool setGroundThermalHeatCapacity(const OSOptionalQuantity& groundThermalHeatCapacity);

    void resetGroundThermalHeatCapacity();

    bool setGroundTemperature(boost::optional<double> groundTemperature);

//    bool setGroundTemperature(const OSOptionalQuantity& groundTemperature);

    void resetGroundTemperature();

    bool setDesignFlowRate(boost::optional<double> designFlowRate);

//    bool setDesignFlowRate(const OSOptionalQuantity& designFlowRate);

    void resetDesignFlowRate();

    bool setGroutThermalConductivity(boost::optional<double> groutThermalConductivity);

//    bool setGroutThermalConductivity(const OSOptionalQuantity& groutThermalConductivity);

    void resetGroutThermalConductivity();

    bool setPipeThermalConductivity(boost::optional<double> pipeThermalConductivity);

//    bool setPipeThermalConductivity(const OSOptionalQuantity& pipeThermalConductivity);

    void resetPipeThermalConductivity();

    bool setPipeOutDiameter(boost::optional<double> pipeOutDiameter);

//    bool setPipeOutDiameter(const OSOptionalQuantity& pipeOutDiameter);

    void resetPipeOutDiameter();

    bool setUTubeDistance(boost::optional<double> uTubeDistance);

//    bool setUTubeDistance(const OSOptionalQuantity& uTubeDistance);

    void resetUTubeDistance();

    bool setPipeThickness(boost::optional<double> pipeThickness);

//    bool setPipeThickness(const OSOptionalQuantity& pipeThickness);

    void resetPipeThickness();

    bool setMaximumLengthofSimulation(boost::optional<double> maximumLengthofSimulation);

//    bool setMaximumLengthofSimulation(const OSOptionalQuantity& maximumLengthofSimulation);

    void resetMaximumLengthofSimulation();

    bool setGFunctionReferenceRatio(double gFunctionReferenceRatio);

//    bool setGFunctionReferenceRatio(const Quantity& gFunctionReferenceRatio);

    void resetGFunctionReferenceRatio();

    bool setNumberofDataPairsoftheGFunction(boost::optional<int> numberofDataPairsoftheGFunction);

    void resetNumberofDataPairsoftheGFunction();

    bool addToNode(Node & node);

    // TODO: Handle this object's extensible fields.

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
//    boost::optional<Connection> optionalInletNode() const;
//    boost::optional<Connection> optionalOutletNode() const;

    //openstudio::OSOptionalQuantity maximumFlowRate_SI() const;
    //openstudio::OSOptionalQuantity maximumFlowRate_IP() const;
    //openstudio::OSOptionalQuantity boreHoleLength_SI() const;
    //openstudio::OSOptionalQuantity boreHoleLength_IP() const;
    //openstudio::OSOptionalQuantity boreHoleRadius_SI() const;
    //openstudio::OSOptionalQuantity boreHoleRadius_IP() const;
    //openstudio::OSOptionalQuantity groundThermalConductivity_SI() const;
    //openstudio::OSOptionalQuantity groundThermalConductivity_IP() const;
    //openstudio::OSOptionalQuantity groundThermalHeatCapacity_SI() const;
    //openstudio::OSOptionalQuantity groundThermalHeatCapacity_IP() const;
    //openstudio::OSOptionalQuantity groundTemperature_SI() const;
    //openstudio::OSOptionalQuantity groundTemperature_IP() const;
    //openstudio::OSOptionalQuantity designFlowRate_SI() const;
    //openstudio::OSOptionalQuantity designFlowRate_IP() const;
    //openstudio::OSOptionalQuantity groutThermalConductivity_SI() const;
    //openstudio::OSOptionalQuantity groutThermalConductivity_IP() const;
    //openstudio::OSOptionalQuantity pipeThermalConductivity_SI() const;
    //openstudio::OSOptionalQuantity pipeThermalConductivity_IP() const;
    //openstudio::OSOptionalQuantity pipeOutDiameter_SI() const;
    //openstudio::OSOptionalQuantity pipeOutDiameter_IP() const;
    //openstudio::OSOptionalQuantity uTubeDistance_SI() const;
    //openstudio::OSOptionalQuantity uTubeDistance_IP() const;
    //openstudio::OSOptionalQuantity pipeThickness_SI() const;
    //openstudio::OSOptionalQuantity pipeThickness_IP() const;
    //openstudio::OSOptionalQuantity maximumLengthofSimulation_SI() const;
    //openstudio::OSOptionalQuantity maximumLengthofSimulation_IP() const;
    //openstudio::Quantity gFunctionReferenceRatio_SI() const;
    //openstudio::Quantity gFunctionReferenceRatio_IP() const;

//    boost::optional<ModelObject> inletNodeAsModelObject() const;
//    boost::optional<ModelObject> outletNodeAsModelObject() const;

//    bool setInletNodeAsModelObject(const boost::optional<ModelObject>& modelObject);
//    bool setOutletNodeAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_GROUNDHEATEXCHANGERVERTICAL_IMPL_HPP

