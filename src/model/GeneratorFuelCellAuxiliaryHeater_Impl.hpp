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

#ifndef MODEL_GENERATORFUELCELLAUXILIARYHEATER_IMPL_HPP
#define MODEL_GENERATORFUELCELLAUXILIARYHEATER_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;
  class GeneratorFuelCell;

  namespace detail {

    /** GeneratorFuelCellAuxiliaryHeater_Impl is a ModelObject_Impl that is the implementation class for GeneratorFuelCellAuxiliaryHeater.*/
    class MODEL_API GeneratorFuelCellAuxiliaryHeater_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      GeneratorFuelCellAuxiliaryHeater_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      GeneratorFuelCellAuxiliaryHeater_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      GeneratorFuelCellAuxiliaryHeater_Impl(const GeneratorFuelCellAuxiliaryHeater_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~GeneratorFuelCellAuxiliaryHeater_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<IddObjectType> allowableChildTypes() const;

      virtual std::vector<ModelObject> children() const;

      //@}
      /** @name Getters */
      //@{

      double excessAirRatio() const;

      double ancillaryPowerConstantTerm() const;

      double ancillaryPowerLinearTerm() const;

      double skinLossUFactorTimesAreaValue() const;

      std::string skinLossDestination() const;

      boost::optional<ThermalZone> zonetoReceiveSkinLosses() const;

      std::string heatingCapacityUnits() const;

      double maximumHeatingCapacityinWatts() const;

      double minimumHeatingCapacityinWatts() const;

      double maximumHeatingCapacityinKmolperSecond() const;

      double minimumHeatingCapacityinKmolperSecond() const;

      // Return optional parent generator
      boost::optional<GeneratorFuelCell> fuelCell() const;

      //@}
      /** @name Setters */
      //@{

      bool setExcessAirRatio(double excessAirRatio);

      void resetExcessAirRatio();

      bool setAncillaryPowerConstantTerm(double ancillaryPowerConstantTerm);

      void resetAncillaryPowerConstantTerm();

      bool setAncillaryPowerLinearTerm(double ancillaryPowerLinearTerm);

      void resetAncillaryPowerLinearTerm();

      bool setSkinLossUFactorTimesAreaValue(double skinLossUFactorTimesAreaValue);

      void resetSkinLossUFactorTimesAreaValue();

      bool setSkinLossDestination(const std::string& skinLossDestination);

      void resetSkinLossDestination();

      bool setZonetoReceiveSkinLosses(const ThermalZone& zone);

      void resetZonetoReceiveSkinLosses();

      bool setHeatingCapacityUnits(const std::string& heatingCapacityUnits);

      void resetHeatingCapacityUnits();

      bool setMaximumHeatingCapacityinWatts(double maximumHeatingCapacityinWatts);

      void resetMaximumHeatingCapacityinWatts();

      bool setMinimumHeatingCapacityinWatts(double minimumHeatingCapacityinWatts);

      void resetMinimumHeatingCapacityinWatts();

      bool setMaximumHeatingCapacityinKmolperSecond(double maximumHeatingCapacityinKmolperSecond);

      void resetMaximumHeatingCapacityinKmolperSecond();

      bool setMinimumHeatingCapacityinKmolperSecond(double minimumHeatingCapacityinKmolperSecond);

      void resetMinimumHeatingCapacityinKmolperSecond();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.GeneratorFuelCellAuxiliaryHeater");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORFUELCELLAUXILIARYHEATER_IMPL_HPP
