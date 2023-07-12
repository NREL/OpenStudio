/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/FanComponentModel.hpp"

#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"

#include <utilities/idd/Fan_ComponentModel_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateFanComponentModel(const WorkspaceObject& workspaceObject) {
    boost::optional<ModelObject> result;
    boost::optional<WorkspaceObject> _wo;
    boost::optional<ModelObject> _mo;

    // Instantiate an object of the class to store the values,
    // but we don't return it until we know it's ok
    // TODO: check constructor, it might need other objects
    openstudio::model::FanComponentModel modelObject(m_model);

    // TODO: Note JM 2018-10-17
    // You are responsible for implementing any additional logic based on choice fields, etc.
    // The ReverseTranslator generator script is meant to facilitate your work, not get you 100% of the way

    // Name
    if (boost::optional<std::string> _name = workspaceObject.name()) {
      modelObject.setName(_name.get());
    }

    // Air Inlet Node Name: Required Node

    // Air Outlet Node Name: Required Node

    // Availability Schedule Name: Optional Object
    if ((_wo = workspaceObject.getTarget(Fan_ComponentModelFields::AvailabilityScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Schedule> _availabilitySchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setAvailabilitySchedule(_availabilitySchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Availability Schedule Name'");
        }
      }
    }
    // Maximum Flow Rate: Optional Double
    if (boost::optional<double> _maximumFlowRate = workspaceObject.getDouble(Fan_ComponentModelFields::MaximumFlowRate)) {
      modelObject.setMaximumFlowRate(_maximumFlowRate.get());
    } else {
      modelObject.autosizeMaximumFlowRate();
    }

    // Minimum Flow Rate: Optional Double
    if (boost::optional<double> _minimumFlowRate = workspaceObject.getDouble(Fan_ComponentModelFields::MinimumFlowRate)) {
      modelObject.setMinimumFlowRate(_minimumFlowRate.get());
    } else {
      modelObject.autosizeMinimumFlowRate();
    }

    // Fan Sizing Factor: Optional Double
    if (boost::optional<double> _fanSizingFactor = workspaceObject.getDouble(Fan_ComponentModelFields::FanSizingFactor)) {
      modelObject.setFanSizingFactor(_fanSizingFactor.get());
    }

    // Fan Wheel Diameter: Required Double
    if (boost::optional<double> _fanWheelDiameter = workspaceObject.getDouble(Fan_ComponentModelFields::FanWheelDiameter)) {
      modelObject.setFanWheelDiameter(_fanWheelDiameter.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Fan Wheel Diameter'");
      return result;
    }

    // Fan Outlet Area: Required Double
    if (boost::optional<double> _fanOutletArea = workspaceObject.getDouble(Fan_ComponentModelFields::FanOutletArea)) {
      modelObject.setFanOutletArea(_fanOutletArea.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Fan Outlet Area'");
      return result;
    }

    // Maximum Fan Static Efficiency: Required Double
    if (boost::optional<double> _maximumFanStaticEfficiency = workspaceObject.getDouble(Fan_ComponentModelFields::MaximumFanStaticEfficiency)) {
      modelObject.setMaximumFanStaticEfficiency(_maximumFanStaticEfficiency.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Maximum Fan Static Efficiency'");
      return result;
    }

    // Euler Number at Maximum Fan Static Efficiency: Required Double
    if (boost::optional<double> _eulerNumberatMaximumFanStaticEfficiency =
          workspaceObject.getDouble(Fan_ComponentModelFields::EulerNumberatMaximumFanStaticEfficiency)) {
      modelObject.setEulerNumberatMaximumFanStaticEfficiency(_eulerNumberatMaximumFanStaticEfficiency.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Euler Number at Maximum Fan Static Efficiency'");
      return result;
    }

    // Maximum Dimensionless Fan Airflow: Required Double
    if (boost::optional<double> _maximumDimensionlessFanAirflow =
          workspaceObject.getDouble(Fan_ComponentModelFields::MaximumDimensionlessFanAirflow)) {
      modelObject.setMaximumDimensionlessFanAirflow(_maximumDimensionlessFanAirflow.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Maximum Dimensionless Fan Airflow'");
      return result;
    }

    // Motor Fan Pulley Ratio: Autosizable
    if (boost::optional<double> _motorFanPulleyRatio = workspaceObject.getDouble(Fan_ComponentModelFields::MotorFanPulleyRatio)) {
      modelObject.setMotorFanPulleyRatio(_motorFanPulleyRatio.get());
    } else {
      modelObject.autosizeMotorFanPulleyRatio();
    }

    // Belt Maximum Torque: Autosizable
    if (boost::optional<double> _beltMaximumTorque = workspaceObject.getDouble(Fan_ComponentModelFields::BeltMaximumTorque)) {
      modelObject.setBeltMaximumTorque(_beltMaximumTorque.get());
    } else {
      modelObject.autosizeBeltMaximumTorque();
    }

    // Belt Sizing Factor: Optional Double
    if (boost::optional<double> _beltSizingFactor = workspaceObject.getDouble(Fan_ComponentModelFields::BeltSizingFactor)) {
      modelObject.setBeltSizingFactor(_beltSizingFactor.get());
    }

    // Belt Fractional Torque Transition: Optional Double
    if (boost::optional<double> _beltFractionalTorqueTransition =
          workspaceObject.getDouble(Fan_ComponentModelFields::BeltFractionalTorqueTransition)) {
      modelObject.setBeltFractionalTorqueTransition(_beltFractionalTorqueTransition.get());
    }

    // Motor Maximum Speed: Required Double
    if (boost::optional<double> _motorMaximumSpeed = workspaceObject.getDouble(Fan_ComponentModelFields::MotorMaximumSpeed)) {
      modelObject.setMotorMaximumSpeed(_motorMaximumSpeed.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Motor Maximum Speed'");
      return result;
    }

    // Maximum Motor Output Power: Autosizable
    if (boost::optional<double> _maximumMotorOutputPower = workspaceObject.getDouble(Fan_ComponentModelFields::MaximumMotorOutputPower)) {
      modelObject.setMaximumMotorOutputPower(_maximumMotorOutputPower.get());
    } else {
      modelObject.autosizeMaximumMotorOutputPower();
    }

    // Motor Sizing Factor: Optional Double
    if (boost::optional<double> _motorSizingFactor = workspaceObject.getDouble(Fan_ComponentModelFields::MotorSizingFactor)) {
      modelObject.setMotorSizingFactor(_motorSizingFactor.get());
    }

    // Motor In Airstream Fraction: Optional Double
    if (boost::optional<double> _motorInAirstreamFraction = workspaceObject.getDouble(Fan_ComponentModelFields::MotorInAirstreamFraction)) {
      modelObject.setMotorInAirstreamFraction(_motorInAirstreamFraction.get());
    }

    // VFD Efficiency Type: Optional String
    if (boost::optional<std::string> _vFDEfficiencyType = workspaceObject.getString(Fan_ComponentModelFields::VFDEfficiencyType)) {
      modelObject.setVFDEfficiencyType(_vFDEfficiencyType.get());
    }

    // Maximum VFD Output Power: Autosizable
    if (boost::optional<double> _maximumVFDOutputPower = workspaceObject.getDouble(Fan_ComponentModelFields::MaximumVFDOutputPower)) {
      modelObject.setMaximumVFDOutputPower(_maximumVFDOutputPower.get());
    } else {
      modelObject.autosizeMaximumVFDOutputPower();
    }

    // VFD Sizing Factor: Optional Double
    if (boost::optional<double> _vFDSizingFactor = workspaceObject.getDouble(Fan_ComponentModelFields::VFDSizingFactor)) {
      modelObject.setVFDSizingFactor(_vFDSizingFactor.get());
    }

    // Fan Pressure Rise Curve Name: Required Object
    if ((_wo = workspaceObject.getTarget(Fan_ComponentModelFields::FanPressureRiseCurveName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Curve> _fanPressureRiseCurve = _mo->optionalCast<Curve>()) {
          // TODO: Should probably catch the case where the setter fails (wrong curve type...)
          modelObject.setFanPressureRiseCurve(_fanPressureRiseCurve.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Fan Pressure Rise Curve Name'");
        }
      } else {
        LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot reverse translate required object 'Fan Pressure Rise Curve Name'");
        return result;
      }
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required object 'Fan Pressure Rise Curve Name'");
      return result;
    }
    // Duct Static Pressure Reset Curve Name: Required Object
    if ((_wo = workspaceObject.getTarget(Fan_ComponentModelFields::DuctStaticPressureResetCurveName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Curve> _ductStaticPressureResetCurve = _mo->optionalCast<Curve>()) {
          modelObject.setDuctStaticPressureResetCurve(_ductStaticPressureResetCurve.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Duct Static Pressure Reset Curve Name'");
        }
      } else {
        LOG(Error,
            "For " << workspaceObject.briefDescription() << ", cannot reverse translate required object 'Duct Static Pressure Reset Curve Name'");
        return result;
      }
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required object 'Duct Static Pressure Reset Curve Name'");
      return result;
    }
    // Normalized Fan Static Efficiency Curve Name-Non-Stall Region: Required Object
    if ((_wo = workspaceObject.getTarget(Fan_ComponentModelFields::NormalizedFanStaticEfficiencyCurveNameNonStallRegion))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Curve> _normalizedFanStaticEfficiencyCurveNonStallRegion = _mo->optionalCast<Curve>()) {
          modelObject.setNormalizedFanStaticEfficiencyCurveNonStallRegion(_normalizedFanStaticEfficiencyCurveNonStallRegion.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Normalized Fan Static Efficiency Curve Name-Non-Stall Region'");
        }
      } else {
        LOG(Error, "For " << workspaceObject.briefDescription()
                          << ", cannot reverse translate required object 'Normalized Fan Static Efficiency Curve Name-Non-Stall Region'");
        return result;
      }
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription()
                        << ", cannot find required object 'Normalized Fan Static Efficiency Curve Name-Non-Stall Region'");
      return result;
    }
    // Normalized Fan Static Efficiency Curve Name-Stall Region: Required Object
    if ((_wo = workspaceObject.getTarget(Fan_ComponentModelFields::NormalizedFanStaticEfficiencyCurveNameStallRegion))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Curve> _normalizedFanStaticEfficiencyCurveStallRegion = _mo->optionalCast<Curve>()) {
          modelObject.setNormalizedFanStaticEfficiencyCurveStallRegion(_normalizedFanStaticEfficiencyCurveStallRegion.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Normalized Fan Static Efficiency Curve Name-Stall Region'");
        }
      } else {
        LOG(Error, "For " << workspaceObject.briefDescription()
                          << ", cannot reverse translate required object 'Normalized Fan Static Efficiency Curve Name-Stall Region'");
        return result;
      }
    } else {
      LOG(Error,
          "For " << workspaceObject.briefDescription() << ", cannot find required object 'Normalized Fan Static Efficiency Curve Name-Stall Region'");
      return result;
    }
    // Normalized Dimensionless Airflow Curve Name-Non-Stall Region: Required Object
    if ((_wo = workspaceObject.getTarget(Fan_ComponentModelFields::NormalizedDimensionlessAirflowCurveNameNonStallRegion))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Curve> _normalizedDimensionlessAirflowCurveNonStallRegion = _mo->optionalCast<Curve>()) {
          modelObject.setNormalizedDimensionlessAirflowCurveNonStallRegion(_normalizedDimensionlessAirflowCurveNonStallRegion.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Normalized Dimensionless Airflow Curve Name-Non-Stall Region'");
        }
      } else {
        LOG(Error, "For " << workspaceObject.briefDescription()
                          << ", cannot reverse translate required object 'Normalized Dimensionless Airflow Curve Name-Non-Stall Region'");
        return result;
      }
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription()
                        << ", cannot find required object 'Normalized Dimensionless Airflow Curve Name-Non-Stall Region'");
      return result;
    }
    // Normalized Dimensionless Airflow Curve Name-Stall Region: Required Object
    if ((_wo = workspaceObject.getTarget(Fan_ComponentModelFields::NormalizedDimensionlessAirflowCurveNameStallRegion))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Curve> _normalizedDimensionlessAirflowCurveStallRegion = _mo->optionalCast<Curve>()) {
          modelObject.setNormalizedDimensionlessAirflowCurveStallRegion(_normalizedDimensionlessAirflowCurveStallRegion.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Normalized Dimensionless Airflow Curve Name-Stall Region'");
        }
      } else {
        LOG(Error, "For " << workspaceObject.briefDescription()
                          << ", cannot reverse translate required object 'Normalized Dimensionless Airflow Curve Name-Stall Region'");
        return result;
      }
    } else {
      LOG(Error,
          "For " << workspaceObject.briefDescription() << ", cannot find required object 'Normalized Dimensionless Airflow Curve Name-Stall Region'");
      return result;
    }
    // Maximum Belt Efficiency Curve Name: Optional Object
    if ((_wo = workspaceObject.getTarget(Fan_ComponentModelFields::MaximumBeltEfficiencyCurveName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Curve> _maximumBeltEfficiencyCurve = _mo->optionalCast<Curve>()) {
          modelObject.setMaximumBeltEfficiencyCurve(_maximumBeltEfficiencyCurve.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Maximum Belt Efficiency Curve Name'");
        }
      }
    }
    // Normalized Belt Efficiency Curve Name - Region 1: Optional Object
    if ((_wo = workspaceObject.getTarget(Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion1))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Curve> _normalizedBeltEfficiencyCurveRegion1 = _mo->optionalCast<Curve>()) {
          modelObject.setNormalizedBeltEfficiencyCurveRegion1(_normalizedBeltEfficiencyCurveRegion1.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Normalized Belt Efficiency Curve Name - Region 1'");
        }
      }
    }
    // Normalized Belt Efficiency Curve Name - Region 2: Optional Object
    if ((_wo = workspaceObject.getTarget(Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion2))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Curve> _normalizedBeltEfficiencyCurveRegion2 = _mo->optionalCast<Curve>()) {
          modelObject.setNormalizedBeltEfficiencyCurveRegion2(_normalizedBeltEfficiencyCurveRegion2.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Normalized Belt Efficiency Curve Name - Region 2'");
        }
      }
    }
    // Normalized Belt Efficiency Curve Name - Region 3: Optional Object
    if ((_wo = workspaceObject.getTarget(Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion3))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Curve> _normalizedBeltEfficiencyCurveRegion3 = _mo->optionalCast<Curve>()) {
          modelObject.setNormalizedBeltEfficiencyCurveRegion3(_normalizedBeltEfficiencyCurveRegion3.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Normalized Belt Efficiency Curve Name - Region 3'");
        }
      }
    }
    // Maximum Motor Efficiency Curve Name: Optional Object
    if ((_wo = workspaceObject.getTarget(Fan_ComponentModelFields::MaximumMotorEfficiencyCurveName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Curve> _maximumMotorEfficiencyCurve = _mo->optionalCast<Curve>()) {
          modelObject.setMaximumMotorEfficiencyCurve(_maximumMotorEfficiencyCurve.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Maximum Motor Efficiency Curve Name'");
        }
      }
    }
    // Normalized Motor Efficiency Curve Name: Optional Object
    if ((_wo = workspaceObject.getTarget(Fan_ComponentModelFields::NormalizedMotorEfficiencyCurveName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Curve> _normalizedMotorEfficiencyCurve = _mo->optionalCast<Curve>()) {
          modelObject.setNormalizedMotorEfficiencyCurve(_normalizedMotorEfficiencyCurve.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Normalized Motor Efficiency Curve Name'");
        }
      }
    }
    // VFD Efficiency Curve Name: Optional Object
    if ((_wo = workspaceObject.getTarget(Fan_ComponentModelFields::VFDEfficiencyCurveName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Curve> _vFDEfficiencyCurve = _mo->optionalCast<Curve>()) {
          modelObject.setVFDEfficiencyCurve(_vFDEfficiencyCurve.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'VFD Efficiency Curve Name'");
        }
      }
    }
    // End-Use Subcategory: Optional String
    if (boost::optional<std::string> _endUseSubcategory = workspaceObject.getString(Fan_ComponentModelFields::EndUseSubcategory)) {
      modelObject.setEndUseSubcategory(_endUseSubcategory.get());
    }

    result = modelObject;
    return result;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
