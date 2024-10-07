/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKSIMULATIONCONTROL_IMPL_HPP
#define MODEL_AIRFLOWNETWORKSIMULATIONCONTROL_IMPL_HPP

#include "ParentObject_Impl.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class MODEL_API AirflowNetworkSimulationControl_Impl : public ParentObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      // constructor
      AirflowNetworkSimulationControl_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      AirflowNetworkSimulationControl_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // copy constructor
      AirflowNetworkSimulationControl_Impl(const AirflowNetworkSimulationControl_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~AirflowNetworkSimulationControl_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // return any children objects in the hierarchy
      virtual std::vector<ModelObject> children() const override;

      /// get a vector of allowable children types
      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<std::string> airflowNetworkControl() const;
      bool isAirflowNetworkControlDefaulted() const;
      boost::optional<std::string> windPressureCoefficientType() const;
      bool isWindPressureCoefficientTypeDefaulted() const;
      boost::optional<std::string> heightSelectionforLocalWindPressureCalculation() const;
      bool isHeightSelectionforLocalWindPressureCalculationDefaulted() const;
      boost::optional<std::string> buildingType() const;
      bool isBuildingTypeDefaulted() const;
      boost::optional<int> maximumNumberofIterations() const;
      bool isMaximumNumberofIterationsDefaulted() const;
      boost::optional<std::string> initializationType() const;
      bool isInitializationTypeDefaulted() const;
      boost::optional<double> relativeAirflowConvergenceTolerance() const;
      bool isRelativeAirflowConvergenceToleranceDefaulted() const;
      boost::optional<double> absoluteAirflowConvergenceTolerance() const;
      bool isAbsoluteAirflowConvergenceToleranceDefaulted() const;
      boost::optional<double> convergenceAccelerationLimit() const;
      bool isConvergenceAccelerationLimitDefaulted() const;
      boost::optional<double> azimuthAngleofLongAxisofBuilding() const;
      bool isAzimuthAngleofLongAxisofBuildingDefaulted() const;
      boost::optional<double> buildingAspectRatio() const;
      bool isBuildingAspectRatioDefaulted() const;
      bool heightDependenceofExternalNodeTemperature() const;
      bool isHeightDependenceofExternalNodeTemperatureDefaulted() const;
      std::string solver() const;
      bool isSolverDefaulted() const;
      bool allowUnsupportedZoneEquipment() const;
      bool isAllowUnsupportedZoneEquipmentDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setAirflowNetworkControl(const std::string& airflowNetworkControl);
      void resetAirflowNetworkControl();
      bool setWindPressureCoefficientType(const std::string& type);
      void resetWindPressureCoefficientType();
      bool setHeightSelectionforLocalWindPressureCalculation(const std::string& height);
      void resetHeightSelectionforLocalWindPressureCalculation();
      bool setBuildingType(const std::string& type);
      void resetBuildingType();
      bool setMaximumNumberofIterations(int number);
      void resetMaximumNumberofIterations();
      bool setInitializationType(const std::string& type);
      void resetInitializationType();
      bool setRelativeAirflowConvergenceTolerance(double tolerance);
      void resetRelativeAirflowConvergenceTolerance();
      bool setAbsoluteAirflowConvergenceTolerance(double tolerance);
      void resetAbsoluteAirflowConvergenceTolerance();
      bool setConvergenceAccelerationLimit(double limit);
      void resetConvergenceAccelerationLimit();
      bool setAzimuthAngleofLongAxisofBuilding(double angle);
      void resetAzimuthAngleofLongAxisofBuilding();
      bool setBuildingAspectRatio(double ratio);
      void resetBuildingAspectRatio();
      bool setHeightDependenceofExternalNodeTemperature(bool tf);
      void resetHeightDependenceofExternalNodeTemperature();
      bool setSolver(const std::string& solver);
      void resetSolver();
      bool setAllowUnsupportedZoneEquipment(bool allowUnsupportedZoneEquipment);
      void resetAllowUnsupportedZoneEquipment();

      //@}

     private:
      REGISTER_LOGGER("openstudio.model.AirflowNetworkSimulationControl");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKSIMULATIONCONTROL_IMPL_HPP
