/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/FanComponentModel.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include <utilities/idd/Fan_ComponentModel_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateFanComponentModel(model::FanComponentModel& modelObject) {
    boost::optional<IdfObject> result;
    boost::optional<WorkspaceObject> _wo;
    boost::optional<ModelObject> _mo;

    // Instantiate an IdfObject of the class to store the values,
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Fan_ComponentModel, modelObject);
    // If it doesn't have a name, or if you aren't sure you are going to want to return it
    // IdfObject idfObject( openstudio::IddObjectType::Fan_ComponentModel );
    // m_idfObjects.push_back(idfObject);

    // TODO: Note JM 2018-10-17
    // You are responsible for implementing any additional logic based on choice fields, etc.
    // The ForwardTranslator generator script is meant to facilitate your work, not get you 100% of the way

    // TODO: If you keep createRegisterAndNameIdfObject above, you don't need this.
    // But in some cases, you'll want to handle failure without pushing to the map
    // Name
    if (boost::optional<std::string> moName = modelObject.name()) {
      idfObject.setName(*moName);
    }

    // Air Inlet Node Name: Required Node
    if (boost::optional<ModelObject> inletModelObject = modelObject.inletModelObject()) {
      if (boost::optional<Node> inletNode = inletModelObject->optionalCast<Node>()) {
        idfObject.setString(Fan_ComponentModelFields::AirInletNodeName, inletNode->name().get());
      }
    }

    // Air Outlet Node Name: Required Node
    if (boost::optional<ModelObject> outletModelObject = modelObject.outletModelObject()) {
      if (boost::optional<Node> outletNode = outletModelObject->optionalCast<Node>()) {
        idfObject.setString(Fan_ComponentModelFields::AirOutletNodeName, outletNode->name().get());
      }
    }

    // Availability Schedule Name: Optional Object
    {
      Schedule availabilitySchedule = modelObject.availabilitySchedule();
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(availabilitySchedule)) {
        idfObject.setString(Fan_ComponentModelFields::AvailabilityScheduleName, _owo->nameString());
      }
    }

    if (modelObject.isMaximumFlowRateAutosized()) {
      idfObject.setString(Fan_ComponentModelFields::MaximumFlowRate, "Autosize");
    } else {
      // Maximum Flow Rate: boost::optional<double>
      if (boost::optional<double> _maximumFlowRate = modelObject.maximumFlowRate()) {
        idfObject.setDouble(Fan_ComponentModelFields::MaximumFlowRate, _maximumFlowRate.get());
      }
    }

    if (modelObject.isMinimumFlowRateAutosized()) {
      idfObject.setString(Fan_ComponentModelFields::MinimumFlowRate, "Autosize");
    } else {
      // Minimum Flow Rate: boost::optional<double>
      if (boost::optional<double> _minimumFlowRate = modelObject.minimumFlowRate()) {
        idfObject.setDouble(Fan_ComponentModelFields::MinimumFlowRate, _minimumFlowRate.get());
      }
    }

    // Fan Sizing Factor: Optional Double
    double fanSizingFactor = modelObject.fanSizingFactor();
    idfObject.setDouble(Fan_ComponentModelFields::FanSizingFactor, fanSizingFactor);

    // Fan Wheel Diameter: Required Double
    double fanWheelDiameter = modelObject.fanWheelDiameter();
    idfObject.setDouble(Fan_ComponentModelFields::FanWheelDiameter, fanWheelDiameter);

    // Fan Outlet Area: Required Double
    double fanOutletArea = modelObject.fanOutletArea();
    idfObject.setDouble(Fan_ComponentModelFields::FanOutletArea, fanOutletArea);

    // Maximum Fan Static Efficiency: Required Double
    double maximumFanStaticEfficiency = modelObject.maximumFanStaticEfficiency();
    idfObject.setDouble(Fan_ComponentModelFields::MaximumFanStaticEfficiency, maximumFanStaticEfficiency);

    // Euler Number at Maximum Fan Static Efficiency: Required Double
    double eulerNumberatMaximumFanStaticEfficiency = modelObject.eulerNumberatMaximumFanStaticEfficiency();
    idfObject.setDouble(Fan_ComponentModelFields::EulerNumberatMaximumFanStaticEfficiency, eulerNumberatMaximumFanStaticEfficiency);

    // Maximum Dimensionless Fan Airflow: Required Double
    double maximumDimensionlessFanAirflow = modelObject.maximumDimensionlessFanAirflow();
    idfObject.setDouble(Fan_ComponentModelFields::MaximumDimensionlessFanAirflow, maximumDimensionlessFanAirflow);

    if (modelObject.isMotorFanPulleyRatioAutosized()) {
      idfObject.setString(Fan_ComponentModelFields::MotorFanPulleyRatio, "Autosize");
    } else {
      // Motor Fan Pulley Ratio: boost::optional<double>
      if (boost::optional<double> _motorFanPulleyRatio = modelObject.motorFanPulleyRatio()) {
        idfObject.setDouble(Fan_ComponentModelFields::MotorFanPulleyRatio, _motorFanPulleyRatio.get());
      }
    }

    if (modelObject.isBeltMaximumTorqueAutosized()) {
      idfObject.setString(Fan_ComponentModelFields::BeltMaximumTorque, "Autosize");
    } else {
      // Belt Maximum Torque: boost::optional<double>
      if (boost::optional<double> _beltMaximumTorque = modelObject.beltMaximumTorque()) {
        idfObject.setDouble(Fan_ComponentModelFields::BeltMaximumTorque, _beltMaximumTorque.get());
      }
    }

    // Belt Sizing Factor: Optional Double
    double beltSizingFactor = modelObject.beltSizingFactor();
    idfObject.setDouble(Fan_ComponentModelFields::BeltSizingFactor, beltSizingFactor);

    // Belt Fractional Torque Transition: Optional Double
    double beltFractionalTorqueTransition = modelObject.beltFractionalTorqueTransition();
    idfObject.setDouble(Fan_ComponentModelFields::BeltFractionalTorqueTransition, beltFractionalTorqueTransition);

    // Motor Maximum Speed: Required Double
    double motorMaximumSpeed = modelObject.motorMaximumSpeed();
    idfObject.setDouble(Fan_ComponentModelFields::MotorMaximumSpeed, motorMaximumSpeed);

    if (modelObject.isMaximumMotorOutputPowerAutosized()) {
      idfObject.setString(Fan_ComponentModelFields::MaximumMotorOutputPower, "Autosize");
    } else {
      // Maximum Motor Output Power: boost::optional<double>
      if (boost::optional<double> _maximumMotorOutputPower = modelObject.maximumMotorOutputPower()) {
        idfObject.setDouble(Fan_ComponentModelFields::MaximumMotorOutputPower, _maximumMotorOutputPower.get());
      }
    }

    // Motor Sizing Factor: Optional Double
    double motorSizingFactor = modelObject.motorSizingFactor();
    idfObject.setDouble(Fan_ComponentModelFields::MotorSizingFactor, motorSizingFactor);

    // Motor In Airstream Fraction: Optional Double
    double motorInAirstreamFraction = modelObject.motorInAirstreamFraction();
    idfObject.setDouble(Fan_ComponentModelFields::MotorInAirstreamFraction, motorInAirstreamFraction);

    // VFD Efficiency Type: boost::optional<std::string>
    if (boost::optional<std::string> _vFDEfficiencyType = modelObject.vFDEfficiencyType()) {
      idfObject.setString(Fan_ComponentModelFields::VFDEfficiencyType, _vFDEfficiencyType.get());
    }

    if (modelObject.isMaximumVFDOutputPowerAutosized()) {
      idfObject.setString(Fan_ComponentModelFields::MaximumVFDOutputPower, "Autosize");
    } else {
      // Maximum VFD Output Power: boost::optional<double>
      if (boost::optional<double> _maximumVFDOutputPower = modelObject.maximumVFDOutputPower()) {
        idfObject.setDouble(Fan_ComponentModelFields::MaximumVFDOutputPower, _maximumVFDOutputPower.get());
      }
    }

    // VFD Sizing Factor: Optional Double
    double vFDSizingFactor = modelObject.vFDSizingFactor();
    idfObject.setDouble(Fan_ComponentModelFields::VFDSizingFactor, vFDSizingFactor);

    // Fan Pressure Rise Curve Name: Required Object
    Curve fanPressureRiseCurve = modelObject.fanPressureRiseCurve();
    if (boost::optional<IdfObject> _owo = translateAndMapModelObject(fanPressureRiseCurve)) {
      idfObject.setString(Fan_ComponentModelFields::FanPressureRiseCurveName, _owo->nameString());
    }

    // Duct Static Pressure Reset Curve Name: Required Object
    Curve ductStaticPressureResetCurve = modelObject.ductStaticPressureResetCurve();
    if (boost::optional<IdfObject> _owo = translateAndMapModelObject(ductStaticPressureResetCurve)) {
      idfObject.setString(Fan_ComponentModelFields::DuctStaticPressureResetCurveName, _owo->nameString());
    }

    // Normalized Fan Static Efficiency Curve Name-Non-Stall Region: Required Object
    Curve normalizedFanStaticEfficiencyCurveNonStallRegion = modelObject.normalizedFanStaticEfficiencyCurveNonStallRegion();
    if (boost::optional<IdfObject> _owo = translateAndMapModelObject(normalizedFanStaticEfficiencyCurveNonStallRegion)) {
      idfObject.setString(Fan_ComponentModelFields::NormalizedFanStaticEfficiencyCurveNameNonStallRegion, _owo->nameString());
    }

    // Normalized Fan Static Efficiency Curve Name-Stall Region: Required Object
    Curve normalizedFanStaticEfficiencyCurveStallRegion = modelObject.normalizedFanStaticEfficiencyCurveStallRegion();
    if (boost::optional<IdfObject> _owo = translateAndMapModelObject(normalizedFanStaticEfficiencyCurveStallRegion)) {
      idfObject.setString(Fan_ComponentModelFields::NormalizedFanStaticEfficiencyCurveNameStallRegion, _owo->nameString());
    }

    // Normalized Dimensionless Airflow Curve Name-Non-Stall Region: Required Object
    Curve normalizedDimensionlessAirflowCurveNonStallRegion = modelObject.normalizedDimensionlessAirflowCurveNonStallRegion();
    if (boost::optional<IdfObject> _owo = translateAndMapModelObject(normalizedDimensionlessAirflowCurveNonStallRegion)) {
      idfObject.setString(Fan_ComponentModelFields::NormalizedDimensionlessAirflowCurveNameNonStallRegion, _owo->nameString());
    }

    // Normalized Dimensionless Airflow Curve Name-Stall Region: Required Object
    Curve normalizedDimensionlessAirflowCurveStallRegion = modelObject.normalizedDimensionlessAirflowCurveStallRegion();
    if (boost::optional<IdfObject> _owo = translateAndMapModelObject(normalizedDimensionlessAirflowCurveStallRegion)) {
      idfObject.setString(Fan_ComponentModelFields::NormalizedDimensionlessAirflowCurveNameStallRegion, _owo->nameString());
    }

    // Maximum Belt Efficiency Curve Name: Optional Object
    if (boost::optional<Curve> _maximumBeltEfficiencyCurve = modelObject.maximumBeltEfficiencyCurve()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_maximumBeltEfficiencyCurve.get())) {
        idfObject.setString(Fan_ComponentModelFields::MaximumBeltEfficiencyCurveName, _owo->nameString());
      }
    }

    // Normalized Belt Efficiency Curve Name - Region 1: Optional Object
    if (boost::optional<Curve> _normalizedBeltEfficiencyCurveRegion1 = modelObject.normalizedBeltEfficiencyCurveRegion1()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_normalizedBeltEfficiencyCurveRegion1.get())) {
        idfObject.setString(Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion1, _owo->nameString());
      }
    }

    // Normalized Belt Efficiency Curve Name - Region 2: Optional Object
    if (boost::optional<Curve> _normalizedBeltEfficiencyCurveRegion2 = modelObject.normalizedBeltEfficiencyCurveRegion2()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_normalizedBeltEfficiencyCurveRegion2.get())) {
        idfObject.setString(Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion2, _owo->nameString());
      }
    }

    // Normalized Belt Efficiency Curve Name - Region 3: Optional Object
    if (boost::optional<Curve> _normalizedBeltEfficiencyCurveRegion3 = modelObject.normalizedBeltEfficiencyCurveRegion3()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_normalizedBeltEfficiencyCurveRegion3.get())) {
        idfObject.setString(Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion3, _owo->nameString());
      }
    }

    // Maximum Motor Efficiency Curve Name: Optional Object
    if (boost::optional<Curve> _maximumMotorEfficiencyCurve = modelObject.maximumMotorEfficiencyCurve()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_maximumMotorEfficiencyCurve.get())) {
        idfObject.setString(Fan_ComponentModelFields::MaximumMotorEfficiencyCurveName, _owo->nameString());
      }
    }

    // Normalized Motor Efficiency Curve Name: Optional Object
    if (boost::optional<Curve> _normalizedMotorEfficiencyCurve = modelObject.normalizedMotorEfficiencyCurve()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_normalizedMotorEfficiencyCurve.get())) {
        idfObject.setString(Fan_ComponentModelFields::NormalizedMotorEfficiencyCurveName, _owo->nameString());
      }
    }

    // VFD Efficiency Curve Name: Optional Object
    if (boost::optional<Curve> _vFDEfficiencyCurve = modelObject.vFDEfficiencyCurve()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_vFDEfficiencyCurve.get())) {
        idfObject.setString(Fan_ComponentModelFields::VFDEfficiencyCurveName, _owo->nameString());
      }
    }

    // End-Use Subcategory: Optional String
    std::string endUseSubcategory = modelObject.endUseSubcategory();
    idfObject.setString(Fan_ComponentModelFields::EndUseSubcategory, endUseSubcategory);

    result = idfObject;
    return result;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
