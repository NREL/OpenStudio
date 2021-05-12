/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_FUELFACTORS_HPP
#define MODEL_FUELFACTORS_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  // TODO: Check the following class names against object getters and setters.
  class Schedule;
  class Schedule;
  class Schedule;
  class Schedule;
  class Schedule;
  class Schedule;
  class Schedule;
  class Schedule;
  class Schedule;
  class Schedule;
  class Schedule;
  class Schedule;
  class Schedule;
  class Schedule;
  class Schedule;
  class Schedule;
  class Schedule;

  namespace detail {

    class FuelFactors_Impl;

  }  // namespace detail

  /** FuelFactors is a ModelObject that wraps the OpenStudio IDD object 'OS:FuelFactors'. */
  class MODEL_API FuelFactors : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit FuelFactors(const Model& model);

    virtual ~FuelFactors() {}

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> existingFuelResourceNameValues();

    /** @name Getters */
    //@{

    std::string existingFuelResourceName() const;

    double sourceEnergyFactor() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> sourceEnergySchedule() const;

    double cO2EmissionFactor() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> cO2EmissionFactorSchedule() const;

    double cOEmissionFactor() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> cOEmissionFactorSchedule() const;

    double cH4EmissionFactor() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> cH4EmissionFactorSchedule() const;

    double nOxEmissionFactor() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> nOxEmissionFactorSchedule() const;

    double n1OEmissionFactor() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> n1OEmissionFactorSchedule() const;

    double sO2EmissionFactor() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> sO2EmissionFactorSchedule() const;

    double pMEmissionFactor() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> pMEmissionFactorSchedule() const;

    double pM10EmissionFactor() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> pM10EmissionFactorSchedule() const;

    double pM25EmissionFactor() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> pM25EmissionFactorSchedule() const;

    double nH3EmissionFactor() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> nH3EmissionFactorSchedule() const;

    double nMVOCEmissionFactor() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> nMVOCEmissionFactorSchedule() const;

    double hgEmissionFactor() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> hgEmissionFactorSchedule() const;

    double pbEmissionFactor() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> pbEmissionFactorSchedule() const;

    double waterEmissionFactor() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> waterEmissionFactorSchedule() const;

    double nuclearHighLevelEmissionFactor() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> nuclearHighLevelEmissionFactorSchedule() const;

    double nuclearLowLevelEmissionFactor() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> nuclearLowLevelEmissionFactorSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setExistingFuelResourceName(const std::string& existingFuelResourceName);

    bool setSourceEnergyFactor(double sourceEnergyFactor);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    // Note Schedules are passed by reference, not const reference.
    bool setSourceEnergySchedule(Schedule& schedule);

    void resetSourceEnergySchedule();

    bool setCO2EmissionFactor(double cO2EmissionFactor);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    // Note Schedules are passed by reference, not const reference.
    bool setCO2EmissionFactorSchedule(Schedule& schedule);

    void resetCO2EmissionFactorSchedule();

    bool setCOEmissionFactor(double cOEmissionFactor);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    // Note Schedules are passed by reference, not const reference.
    bool setCOEmissionFactorSchedule(Schedule& schedule);

    void resetCOEmissionFactorSchedule();

    bool setCH4EmissionFactor(double cH4EmissionFactor);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    // Note Schedules are passed by reference, not const reference.
    bool setCH4EmissionFactorSchedule(Schedule& schedule);

    void resetCH4EmissionFactorSchedule();

    bool setNOxEmissionFactor(double nOxEmissionFactor);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    // Note Schedules are passed by reference, not const reference.
    bool setNOxEmissionFactorSchedule(Schedule& schedule);

    void resetNOxEmissionFactorSchedule();

    bool setN1OEmissionFactor(double n1OEmissionFactor);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    // Note Schedules are passed by reference, not const reference.
    bool setN1OEmissionFactorSchedule(Schedule& schedule);

    void resetN1OEmissionFactorSchedule();

    bool setSO2EmissionFactor(double sO2EmissionFactor);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    // Note Schedules are passed by reference, not const reference.
    bool setSO2EmissionFactorSchedule(Schedule& schedule);

    void resetSO2EmissionFactorSchedule();

    bool setPMEmissionFactor(double pMEmissionFactor);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    // Note Schedules are passed by reference, not const reference.
    bool setPMEmissionFactorSchedule(Schedule& schedule);

    void resetPMEmissionFactorSchedule();

    bool setPM10EmissionFactor(double pM10EmissionFactor);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    // Note Schedules are passed by reference, not const reference.
    bool setPM10EmissionFactorSchedule(Schedule& schedule);

    void resetPM10EmissionFactorSchedule();

    bool setPM25EmissionFactor(double pM25EmissionFactor);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    // Note Schedules are passed by reference, not const reference.
    bool setPM25EmissionFactorSchedule(Schedule& schedule);

    void resetPM25EmissionFactorSchedule();

    bool setNH3EmissionFactor(double nH3EmissionFactor);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    // Note Schedules are passed by reference, not const reference.
    bool setNH3EmissionFactorSchedule(Schedule& schedule);

    void resetNH3EmissionFactorSchedule();

    bool setNMVOCEmissionFactor(double nMVOCEmissionFactor);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    // Note Schedules are passed by reference, not const reference.
    bool setNMVOCEmissionFactorSchedule(Schedule& schedule);

    void resetNMVOCEmissionFactorSchedule();

    bool setHgEmissionFactor(double hgEmissionFactor);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    // Note Schedules are passed by reference, not const reference.
    bool setHgEmissionFactorSchedule(Schedule& schedule);

    void resetHgEmissionFactorSchedule();

    bool setPbEmissionFactor(double pbEmissionFactor);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    // Note Schedules are passed by reference, not const reference.
    bool setPbEmissionFactorSchedule(Schedule& schedule);

    void resetPbEmissionFactorSchedule();

    bool setWaterEmissionFactor(double waterEmissionFactor);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    // Note Schedules are passed by reference, not const reference.
    bool setWaterEmissionFactorSchedule(Schedule& schedule);

    void resetWaterEmissionFactorSchedule();

    bool setNuclearHighLevelEmissionFactor(double nuclearHighLevelEmissionFactor);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    // Note Schedules are passed by reference, not const reference.
    bool setNuclearHighLevelEmissionFactorSchedule(Schedule& schedule);

    void resetNuclearHighLevelEmissionFactorSchedule();

    bool setNuclearLowLevelEmissionFactor(double nuclearLowLevelEmissionFactor);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    // Note Schedules are passed by reference, not const reference.
    bool setNuclearLowLevelEmissionFactorSchedule(Schedule& schedule);

    void resetNuclearLowLevelEmissionFactorSchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    typedef detail::FuelFactors_Impl ImplType;

    explicit FuelFactors(std::shared_ptr<detail::FuelFactors_Impl> impl);

    friend class detail::FuelFactors_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.FuelFactors");
  };

  /** \relates FuelFactors*/
  typedef boost::optional<FuelFactors> OptionalFuelFactors;

  /** \relates FuelFactors*/
  typedef std::vector<FuelFactors> FuelFactorsVector;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FUELFACTORS_HPP
