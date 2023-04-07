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

#ifndef MODEL_COILHEATINGGAS_IMPL_HPP
#define MODEL_COILHEATINGGAS_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;

  namespace detail {

    class MODEL_API CoilHeatingGas_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //2{

      // constructor
      CoilHeatingGas_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      CoilHeatingGas_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // copy constructor
      CoilHeatingGas_Impl(const CoilHeatingGas_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~CoilHeatingGas_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;
      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters and Setters */
      //@{

      Schedule availabilitySchedule() const;

      bool setAvailabilitySchedule(Schedule& schedule);

      std::string fuelType() const;

      bool setFuelType(const std::string& fuelType);

      void resetFuelType();

      // Get GasBurnerEfficiency
      double gasBurnerEfficiency() const;

      // Set GasBurnerEfficiency
      bool setGasBurnerEfficiency(double val);

      // Get ParasiticElectricLoad
      double parasiticElectricLoad() const;

      // Set parasiticElectricLoad
      bool setParasiticElectricLoad(double val);

      // Get ParasiticGasLoad
      double parasiticGasLoad() const;

      // Set parasiticGasLoad
      bool setParasiticGasLoad(double val);

      boost::optional<double> nominalCapacity() const;

      bool isNominalCapacityAutosized() const;

      bool setNominalCapacity(boost::optional<double> nominalCapacity);

      void resetNominalCapacity();

      void autosizeNominalCapacity();

      boost::optional<Curve> partLoadFractionCorrelationCurve() const;

      bool setPartLoadFractionCorrelationCurve(const Curve& curve);

      void resetPartLoadFractionCorrelationCurve();

      std::vector<ModelObject> children() const override;

      ModelObject clone(Model model) const override;

      bool addToNode(Node& node) override;

      AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);
      boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

      boost::optional<double> autosizedNominalCapacity() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingGas");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Schedule> optionalAvailabilitySchedule() const;

      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGGAS_IMPL_HPP
