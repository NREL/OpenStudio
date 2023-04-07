/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
      virtual ~AirflowNetworkSimulationControl_Impl() = default;

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
