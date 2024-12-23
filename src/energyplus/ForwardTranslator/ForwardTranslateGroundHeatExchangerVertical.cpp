/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/GroundHeatExchangerVertical.hpp"
#include "../../model/GroundHeatExchangerVertical_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/GroundHeatExchanger_System_FieldEnums.hxx>
#include <utilities/idd/GroundHeatExchanger_Vertical_Properties_FieldEnums.hxx>
#include <utilities/idd/GroundHeatExchanger_ResponseFactors_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateGroundHeatExchangerVertical(GroundHeatExchangerVertical& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;
    boost::optional<ModelObject> temp;

    // 1   { GroundHeatExchanger_VerticalFields::Name, "Name", "Name"},
    // 2   { GroundHeatExchanger_VerticalFields::InletNodeName, "InletNodeName", "Inlet Node Name"},
    // 3   { GroundHeatExchanger_VerticalFields::OutletNodeName, "OutletNodeName", "Outlet Node Name"},
    // 4   { GroundHeatExchanger_VerticalFields::DesignFlowRate, "DesignFlowRate", "Design Flow Rate"},
    // 5   { GroundHeatExchanger_VerticalFields::NumberofBoreHoles, "NumberofBoreHoles", "Number of Bore Holes"},
    // 6   { GroundHeatExchanger_VerticalFields::BoreHoleLength, "BoreHoleLength", "Bore Hole Length"},
    // 7   { GroundHeatExchanger_VerticalFields::BoreHoleRadius, "BoreHoleRadius", "Bore Hole Radius"},
    // 8   { GroundHeatExchanger_VerticalFields::GroundThermalConductivity, "GroundThermalConductivity", "Ground Thermal Conductivity"},
    // 9   { GroundHeatExchanger_VerticalFields::GroundThermalHeatCapacity, "GroundThermalHeatCapacity", "Ground Thermal Heat Capacity"},
    // 10  { GroundHeatExchanger_VerticalFields::GroundTemperature, "GroundTemperature", "Ground Temperature"},
    // 11  { GroundHeatExchanger_VerticalFields::GroutThermalConductivity, "GroutThermalConductivity", "Grout Thermal Conductivity"},
    // 12  { GroundHeatExchanger_VerticalFields::PipeThermalConductivity, "PipeThermalConductivity", "Pipe Thermal Conductivity"},
    // 13  { GroundHeatExchanger_VerticalFields::PipeOutDiameter, "PipeOutDiameter", "Pipe Out Diameter"},
    // 14  { GroundHeatExchanger_VerticalFields::UTubeDistance, "UTubeDistance", "U-Tube Distance"},
    // 15  { GroundHeatExchanger_VerticalFields::PipeThickness, "PipeThickness", "Pipe Thickness"},
    // 16  { GroundHeatExchanger_VerticalFields::MaximumLengthofSimulation, "MaximumLengthofSimulation", "Maximum Length of Simulation"},
    // 17  { GroundHeatExchanger_VerticalFields::GFunctionReferenceRatio, "GFunctionReferenceRatio", "G-Function Reference Ratio"},
    // 18  { GroundHeatExchanger_VerticalFields::NumberofDataPairsoftheGFunction, "NumberofDataPairsoftheGFunction", "Number of Data Pairs of the G Function"},

    //  1   { GroundHeatExchanger_SystemFields::Name, "Name", "Name"},
    //  2   { GroundHeatExchanger_SystemFields::InletNodeName, "InletNodeName", "Inlet Node Name"},
    //  3   { GroundHeatExchanger_SystemFields::OutletNodeName, "OutletNodeName", "Outlet Node Name"},
    //  4   { GroundHeatExchanger_SystemFields::DesignFlowRate, "DesignFlowRate", "Design Flow Rate"},
    //  5   { GroundHeatExchanger_SystemFields::UndisturbedGroundTemperatureModelType, "UndisturbedGroundTemperatureModelType", "Undisturbed Ground Temperature Model Type"},
    //  6   { GroundHeatExchanger_SystemFields::UndisturbedGroundTemperatureModelName, "UndisturbedGroundTemperatureModelName", "Undisturbed Ground Temperature Model Name"},
    //  7   { GroundHeatExchanger_SystemFields::GroundThermalConductivity, "GroundThermalConductivity", "Ground Thermal Conductivity"},
    //  8   { GroundHeatExchanger_SystemFields::GroundThermalHeatCapacity, "GroundThermalHeatCapacity", "Ground Thermal Heat Capacity"},
    //  9   { GroundHeatExchanger_SystemFields::GHE_Vertical_ResponseFactorsObjectName, "GHE_Vertical_ResponseFactorsObjectName", "GHE:Vertical:ResponseFactors Object Name"},
    //  10  { GroundHeatExchanger_SystemFields::GHE_Vertical_ArrayObjectName, "GHE_Vertical_ArrayObjectName", "GHE:Vertical:Array Object Name"},

    // 1  { GroundHeatExchanger_Vertical_PropertiesFields::Name, "Name", "Name"},
    // 2  { GroundHeatExchanger_Vertical_PropertiesFields::DepthofTopofBorehole, "DepthofTopofBorehole", "Depth of Top of Borehole"},
    // 3  { GroundHeatExchanger_Vertical_PropertiesFields::BoreholeLength, "BoreholeLength", "Borehole Length"},
    // 4  { GroundHeatExchanger_Vertical_PropertiesFields::BoreholeDiameter, "BoreholeDiameter", "Borehole Diameter"},
    // 5  { GroundHeatExchanger_Vertical_PropertiesFields::GroutThermalConductivity, "GroutThermalConductivity", "Grout Thermal Conductivity"},
    // 6  { GroundHeatExchanger_Vertical_PropertiesFields::GroutThermalHeatCapacity, "GroutThermalHeatCapacity", "Grout Thermal Heat Capacity"},
    // 7  { GroundHeatExchanger_Vertical_PropertiesFields::PipeThermalConductivity, "PipeThermalConductivity", "Pipe Thermal Conductivity"},
    // 8  { GroundHeatExchanger_Vertical_PropertiesFields::PipeThermalHeatCapacity, "PipeThermalHeatCapacity", "Pipe Thermal Heat Capacity"},
    // 9  { GroundHeatExchanger_Vertical_PropertiesFields::PipeOuterDiameter, "PipeOuterDiameter", "Pipe Outer Diameter"},
    // 10 { GroundHeatExchanger_Vertical_PropertiesFields::PipeThickness, "PipeThickness", "Pipe Thickness"},
    // 11 { GroundHeatExchanger_Vertical_PropertiesFields::UTubeDistance, "UTubeDistance", "U-Tube Distance"},

    // 1  { Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::Name, "Name", "Name"},
    // 2  { Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::SoilThermalConductivity, "SoilThermalConductivity", "Soil Thermal Conductivity"},
    // 3  { Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::SoilDensity, "SoilDensity", "Soil Density"},
    // 4  { Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::SoilSpecificHeat, "SoilSpecificHeat", "Soil Specific Heat"},
    // 5  { Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::AverageSoilSurfaceTemperature, "AverageSoilSurfaceTemperature", "Average Soil Surface Temperature"},
    // 6  { Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::AverageAmplitudeofSurfaceTemperature, "AverageAmplitudeofSurfaceTemperature", "Average Amplitude of Surface Temperature"},
    // 7  { Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::PhaseShiftofMinimumSurfaceTemperature, "PhaseShiftofMinimumSurfaceTemperature", "Phase Shift of Minimum Surface Temperature"},

    // 1  { GroundHeatExchanger_ResponseFactorsFields::Name, "Name", "Name"},
    // 2  { GroundHeatExchanger_ResponseFactorsFields::GHE_Vertical_PropertiesObjectName, "GHE_Vertical_PropertiesObjectName", "GHE:Vertical:Properties Object Name"},
    // 3  { GroundHeatExchanger_ResponseFactorsFields::NumberofBoreholes, "NumberofBoreholes", "Number of Boreholes"},
    // 4  { GroundHeatExchanger_ResponseFactorsFields::GFunctionReferenceRatio, "GFunctionReferenceRatio", "G-Function Reference Ratio"},

    // 1  { GroundHeatExchanger_ResponseFactorsExtensibleFields::gFunctionLn_T_Ts_Value, "gFunctionLn_T_Ts_Value", "g-Function Ln(T/Ts) Value"},
    // 2  { GroundHeatExchanger_ResponseFactorsExtensibleFields::gFunctiongValue, "gFunctiongValue", "g-Function g Value"},

    // UndisturbedGroundTemperatureModelName, is required, so start by that
    ModelObject undisturbedGroundTemperatureModel = modelObject.undisturbedGroundTemperatureModel();
    boost::optional<IdfObject> _undisturbedGroundTemperatureModel = translateAndMapModelObject(undisturbedGroundTemperatureModel);
    if (_undisturbedGroundTemperatureModel) {
      s = _undisturbedGroundTemperatureModel->name().get();
    } else {
      LOG(Warn, modelObject.briefDescription() << " cannot be translated as its undisturbed ground temperature model object cannot be translated: "
                                               << undisturbedGroundTemperatureModel.briefDescription() << ".");
      return boost::none;
    }

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::GroundHeatExchanger_System, modelObject);

    // Inlet Node Name
    if ((temp = modelObject.inletModelObject())) {
      idfObject.setString(GroundHeatExchanger_SystemFields::InletNodeName, temp->name().get());
    }

    // Outlet Node Name
    if ((temp = modelObject.outletModelObject())) {
      idfObject.setString(GroundHeatExchanger_SystemFields::OutletNodeName, temp->name().get());
    }

    // Design Flow Rate
    if ((value = modelObject.designFlowRate())) {
      idfObject.setDouble(GroundHeatExchanger_SystemFields::DesignFlowRate, value.get());
    }

    // Undisturbed Ground Temperature Model Type
    idfObject.setString(GroundHeatExchanger_SystemFields::UndisturbedGroundTemperatureModelType,
                        _undisturbedGroundTemperatureModel->iddObject().name());

    // Undisturbed Ground Temperature Model Name
    idfObject.setString(GroundHeatExchanger_SystemFields::UndisturbedGroundTemperatureModelName, s.get());

    // Ground Thermal Conductivity
    if ((value = modelObject.groundThermalConductivity())) {
      idfObject.setDouble(GroundHeatExchanger_SystemFields::GroundThermalConductivity, value.get());
    }

    // Ground Thermal Heat Capacity
    if ((value = modelObject.groundThermalHeatCapacity())) {
      idfObject.setDouble(GroundHeatExchanger_SystemFields::GroundThermalHeatCapacity, value.get());
    }

    // GHE:Vertical:ResponseFactors Object Name
    auto responseFactorsObjectName = modelObject.nameString() + " Response Factors";
    idfObject.setString(GroundHeatExchanger_SystemFields::GHE_Vertical_ResponseFactorsObjectName, responseFactorsObjectName);

    IdfObject propertiesIdfObject(IddObjectType::GroundHeatExchanger_Vertical_Properties);
    m_idfObjects.push_back(propertiesIdfObject);

    auto propertiesObjectName = modelObject.nameString() + " Properties";
    propertiesIdfObject.setName(propertiesObjectName);

    propertiesIdfObject.setDouble(GroundHeatExchanger_Vertical_PropertiesFields::DepthofTopofBorehole, modelObject.boreHoleTopDepth());

    if ((value = modelObject.boreHoleLength())) {
      propertiesIdfObject.setDouble(GroundHeatExchanger_Vertical_PropertiesFields::BoreholeLength, value.get());
    }

    if ((value = modelObject.boreHoleRadius())) {
      propertiesIdfObject.setDouble(GroundHeatExchanger_Vertical_PropertiesFields::BoreholeDiameter, value.get() * 2.0);
    }

    if ((value = modelObject.groutThermalConductivity())) {
      propertiesIdfObject.setDouble(GroundHeatExchanger_Vertical_PropertiesFields::GroutThermalConductivity, value.get());
    }

    propertiesIdfObject.setDouble(GroundHeatExchanger_Vertical_PropertiesFields::GroutThermalHeatCapacity, 3.90E+06);

    if ((value = modelObject.pipeThermalConductivity())) {
      propertiesIdfObject.setDouble(GroundHeatExchanger_Vertical_PropertiesFields::PipeThermalConductivity, value.get());
    }

    propertiesIdfObject.setDouble(GroundHeatExchanger_Vertical_PropertiesFields::PipeThermalHeatCapacity, 1.77E+06);

    if ((value = modelObject.pipeOutDiameter())) {
      propertiesIdfObject.setDouble(GroundHeatExchanger_Vertical_PropertiesFields::PipeOuterDiameter, value.get());
    }

    if ((value = modelObject.pipeThickness())) {
      propertiesIdfObject.setDouble(GroundHeatExchanger_Vertical_PropertiesFields::PipeThickness, value.get());
    }

    if ((value = modelObject.uTubeDistance())) {
      propertiesIdfObject.setDouble(GroundHeatExchanger_Vertical_PropertiesFields::UTubeDistance, value.get());
    }

    IdfObject rfIdfObject(IddObjectType::GroundHeatExchanger_ResponseFactors);
    m_idfObjects.push_back(rfIdfObject);

    rfIdfObject.setName(responseFactorsObjectName);

    rfIdfObject.setString(GroundHeatExchanger_ResponseFactorsFields::GHE_Vertical_PropertiesObjectName, propertiesObjectName);

    if ((value = modelObject.numberofBoreHoles())) {
      rfIdfObject.setDouble(GroundHeatExchanger_ResponseFactorsFields::NumberofBoreholes, value.get());
    }

    if ((value = modelObject.gFunctionReferenceRatio())) {
      rfIdfObject.setDouble(GroundHeatExchanger_ResponseFactorsFields::GFunctionReferenceRatio, value.get());
    }

    auto gFunctions = modelObject.gFunctions();
    for (const auto& gFunction : gFunctions) {
      auto eg = rfIdfObject.pushExtensibleGroup();

      eg.setDouble(GroundHeatExchanger_ResponseFactorsExtensibleFields::gFunctionLn_T_Ts_Value, gFunction.lnValue());
      eg.setDouble(GroundHeatExchanger_ResponseFactorsExtensibleFields::gFunctiongValue, gFunction.gValue());
    }

    //// Number of Bore Holes
    //if( (value = modelObject.numberofBoreHoles()) )
    //{
    //  idfObject.setDouble(GroundHeatExchanger_VerticalFields::NumberofBoreHoles,value.get());
    //}

    //// Bore Hole Length
    //if( (value = modelObject.boreHoleLength()) )
    //{
    //  idfObject.setDouble(GroundHeatExchanger_VerticalFields::BoreHoleLength,value.get());
    //}

    //// Bore Hole Radius
    //if( (value = modelObject.boreHoleRadius()) )
    //{
    //  idfObject.setDouble(GroundHeatExchanger_VerticalFields::BoreHoleRadius,value.get());
    //}

    //// Ground Thermal Conductivity
    //if( (value = modelObject.groundThermalConductivity()) )
    //{
    //  idfObject.setDouble(GroundHeatExchanger_VerticalFields::GroundThermalConductivity,value.get());
    //}

    //// Ground Thermal Heat Capacity
    //if( (value = modelObject.groundThermalHeatCapacity()) )
    //{
    //  idfObject.setDouble(GroundHeatExchanger_VerticalFields::GroundThermalHeatCapacity,value.get());
    //}

    //// Ground Temperature
    //if( (value = modelObject.groundTemperature()) )
    //{
    //  idfObject.setDouble(GroundHeatExchanger_VerticalFields::GroundTemperature,value.get());
    //}

    //// Design Flow Rate
    //if( (value = modelObject.designFlowRate()) )
    //{
    //  idfObject.setDouble(GroundHeatExchanger_VerticalFields::DesignFlowRate,value.get());
    //}

    //// Grout Thermal Conductivity
    //if( (value = modelObject.groutThermalConductivity()) )
    //{
    //  idfObject.setDouble(GroundHeatExchanger_VerticalFields::GroutThermalConductivity,value.get());
    //}

    //// Pipe Thermal Conductivity
    //if( (value = modelObject.pipeThermalConductivity()) )
    //{
    //  idfObject.setDouble(GroundHeatExchanger_VerticalFields::PipeThermalConductivity,value.get());
    //}

    //// Pipe Out Diameter
    //if( (value = modelObject.pipeOutDiameter()) )
    //{
    //  idfObject.setDouble(GroundHeatExchanger_VerticalFields::PipeOutDiameter,value.get());
    //}

    //// U-Tube Distance
    //if( (value = modelObject.uTubeDistance()) )
    //{
    //  idfObject.setDouble(GroundHeatExchanger_VerticalFields::UTubeDistance,value.get());
    //}

    //// Pipe Thickness
    //if( (value = modelObject.pipeThickness()) )
    //{
    //  idfObject.setDouble(GroundHeatExchanger_VerticalFields::PipeThickness,value.get());
    //}

    //// Maximum Length of Simulation
    //if( (value = modelObject.maximumLengthofSimulation()) )
    //{
    //  idfObject.setDouble(GroundHeatExchanger_VerticalFields::MaximumLengthofSimulation,value.get());
    //}

    //// G-Function Reference Ratio
    //if( (value = modelObject.gFunctionReferenceRatio()) )
    //{
    //  idfObject.setDouble(GroundHeatExchanger_VerticalFields::GFunctionReferenceRatio,value.get());
    //}

    //std::vector< GFunction > gFunctions = modelObject.gFunctions();

    //// Number of Data Pairs of the G Function
    //idfObject.setInt(GroundHeatExchanger_VerticalFields::NumberofDataPairsoftheGFunction,gFunctions.size());

    //if( !gFunctions.empty() )
    //{
    //  for( const auto & gFunction : gFunctions )
    //  {
    //    IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();

    //    eg.setDouble(GroundHeatExchanger_VerticalExtensibleFields::GFunctionLn_T_Ts_Value,gFunction.lnValue());
    //    eg.setDouble(GroundHeatExchanger_VerticalExtensibleFields::GFunctionGValue,gFunction.gValue());
    //  }
    //}
    //else {
    //  LOG(Error,modelObject.briefDescription() << ": At least one pair of GFunction data required");
    //  return boost::none;
    //}

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
