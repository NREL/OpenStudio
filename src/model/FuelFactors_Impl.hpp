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

#ifndef MODEL_FUELFACTORS_IMPL_HPP
#define MODEL_FUELFACTORS_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    /** FuelFactors_Impl is a ModelObject_Impl that is the implementation class for FuelFactors.*/
    class MODEL_API FuelFactors_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      FuelFactors_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      FuelFactors_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      FuelFactors_Impl(const FuelFactors_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~FuelFactors_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      std::string existingFuelResourceName() const;

      double sourceEnergyFactor() const;

      boost::optional<Schedule> sourceEnergySchedule() const;

      double cO2EmissionFactor() const;

      boost::optional<Schedule> cO2EmissionFactorSchedule() const;

      double cOEmissionFactor() const;

      boost::optional<Schedule> cOEmissionFactorSchedule() const;

      double cH4EmissionFactor() const;

      boost::optional<Schedule> cH4EmissionFactorSchedule() const;

      double nOxEmissionFactor() const;

      boost::optional<Schedule> nOxEmissionFactorSchedule() const;

      double n2OEmissionFactor() const;

      boost::optional<Schedule> n2OEmissionFactorSchedule() const;

      double sO2EmissionFactor() const;

      boost::optional<Schedule> sO2EmissionFactorSchedule() const;

      double pMEmissionFactor() const;

      boost::optional<Schedule> pMEmissionFactorSchedule() const;

      double pM10EmissionFactor() const;

      boost::optional<Schedule> pM10EmissionFactorSchedule() const;

      double pM25EmissionFactor() const;

      boost::optional<Schedule> pM25EmissionFactorSchedule() const;

      double nH3EmissionFactor() const;

      boost::optional<Schedule> nH3EmissionFactorSchedule() const;

      double nMVOCEmissionFactor() const;

      boost::optional<Schedule> nMVOCEmissionFactorSchedule() const;

      double hgEmissionFactor() const;

      boost::optional<Schedule> hgEmissionFactorSchedule() const;

      double pbEmissionFactor() const;

      boost::optional<Schedule> pbEmissionFactorSchedule() const;

      double waterEmissionFactor() const;

      boost::optional<Schedule> waterEmissionFactorSchedule() const;

      double nuclearHighLevelEmissionFactor() const;

      boost::optional<Schedule> nuclearHighLevelEmissionFactorSchedule() const;

      double nuclearLowLevelEmissionFactor() const;

      boost::optional<Schedule> nuclearLowLevelEmissionFactorSchedule() const;

      //@}
      /** @name Setters */
      //@{

      bool setExistingFuelResourceName(const std::string& existingFuelResourceName);

      bool setSourceEnergyFactor(double sourceEnergyFactor);

      bool setSourceEnergySchedule(Schedule& schedule);
      void resetSourceEnergySchedule();

      bool setCO2EmissionFactor(double cO2EmissionFactor);

      bool setCO2EmissionFactorSchedule(Schedule& schedule);
      void resetCO2EmissionFactorSchedule();

      bool setCOEmissionFactor(double cOEmissionFactor);

      bool setCOEmissionFactorSchedule(Schedule& schedule);
      void resetCOEmissionFactorSchedule();

      bool setCH4EmissionFactor(double cH4EmissionFactor);

      bool setCH4EmissionFactorSchedule(Schedule& schedule);
      void resetCH4EmissionFactorSchedule();

      bool setNOxEmissionFactor(double nOxEmissionFactor);

      bool setNOxEmissionFactorSchedule(Schedule& schedule);
      void resetNOxEmissionFactorSchedule();

      bool setN2OEmissionFactor(double n2OEmissionFactor);

      bool setN2OEmissionFactorSchedule(Schedule& schedule);
      void resetN2OEmissionFactorSchedule();

      bool setSO2EmissionFactor(double sO2EmissionFactor);

      bool setSO2EmissionFactorSchedule(Schedule& schedule);
      void resetSO2EmissionFactorSchedule();

      bool setPMEmissionFactor(double pMEmissionFactor);

      bool setPMEmissionFactorSchedule(Schedule& schedule);
      void resetPMEmissionFactorSchedule();

      bool setPM10EmissionFactor(double pM10EmissionFactor);

      bool setPM10EmissionFactorSchedule(Schedule& schedule);
      void resetPM10EmissionFactorSchedule();

      bool setPM25EmissionFactor(double pM25EmissionFactor);

      bool setPM25EmissionFactorSchedule(Schedule& schedule);
      void resetPM25EmissionFactorSchedule();

      bool setNH3EmissionFactor(double nH3EmissionFactor);

      bool setNH3EmissionFactorSchedule(Schedule& schedule);
      void resetNH3EmissionFactorSchedule();

      bool setNMVOCEmissionFactor(double nMVOCEmissionFactor);

      bool setNMVOCEmissionFactorSchedule(Schedule& schedule);
      void resetNMVOCEmissionFactorSchedule();

      bool setHgEmissionFactor(double hgEmissionFactor);

      bool setHgEmissionFactorSchedule(Schedule& schedule);
      void resetHgEmissionFactorSchedule();

      bool setPbEmissionFactor(double pbEmissionFactor);

      bool setPbEmissionFactorSchedule(Schedule& schedule);
      void resetPbEmissionFactorSchedule();

      bool setWaterEmissionFactor(double waterEmissionFactor);

      bool setWaterEmissionFactorSchedule(Schedule& schedule);
      void resetWaterEmissionFactorSchedule();

      bool setNuclearHighLevelEmissionFactor(double nuclearHighLevelEmissionFactor);

      bool setNuclearHighLevelEmissionFactorSchedule(Schedule& schedule);
      void resetNuclearHighLevelEmissionFactorSchedule();

      bool setNuclearLowLevelEmissionFactor(double nuclearLowLevelEmissionFactor);

      bool setNuclearLowLevelEmissionFactorSchedule(Schedule& schedule);
      void resetNuclearLowLevelEmissionFactorSchedule();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.FuelFactors");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FUELFACTORS_IMPL_HPP
