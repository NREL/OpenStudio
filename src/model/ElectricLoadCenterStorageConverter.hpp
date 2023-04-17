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

#ifndef MODEL_ELECTRICLOADCENTERSTORAGECONVERTER_HPP
#define MODEL_ELECTRICLOADCENTERSTORAGECONVERTER_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Curve;
  class ThermalZone;
  class ElectricLoadCenterDistribution;

  namespace detail {

    class ElectricLoadCenterStorageConverter_Impl;

  }  // namespace detail

  /** ElectricLoadCenterStorageConverter is a ParentObject that wraps the OpenStudio IDD object 'OS:ElectricLoadCenter:Storage:Converter'. */
  class MODEL_API ElectricLoadCenterStorageConverter : public ParentObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ElectricLoadCenterStorageConverter(const Model& model);

    virtual ~ElectricLoadCenterStorageConverter() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ElectricLoadCenterStorageConverter(const ElectricLoadCenterStorageConverter& other) = default;
    ElectricLoadCenterStorageConverter(ElectricLoadCenterStorageConverter&& other) = default;
    ElectricLoadCenterStorageConverter& operator=(const ElectricLoadCenterStorageConverter&) = default;
    ElectricLoadCenterStorageConverter& operator=(ElectricLoadCenterStorageConverter&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> powerConversionEfficiencyMethodValues();

    /** @name Getters */
    //@{

    boost::optional<ElectricLoadCenterDistribution> electricLoadCenterDistribution() const;

    Schedule availabilitySchedule() const;
    bool isAvailabilityScheduleDefaulted() const;

    std::string powerConversionEfficiencyMethod() const;

    // Required if powerConversionEfficiencyMethod == "SimpleFixed"
    boost::optional<double> simpleFixedEfficiency() const;

    // Required if powerConversionEfficiencyMethod == "FunctionOfPower"
    boost::optional<double> designMaximumContinuousInputPower() const;

    // Required if powerConversionEfficiencyMethod == "FunctionOfPower"
    // TODO: Later may need to add support for Tables. (UniVariateCurves, UniVariateTables.)
    boost::optional<Curve> efficiencyFunctionofPowerCurve() const;

    // Defaults to 0.0
    double ancillaryPowerConsumedInStandby() const;

    bool isAncillaryPowerConsumedInStandbyDefaulted() const;

    // TODO: Check return type. From object lists, some candidates are: ThermalZone.
    boost::optional<ThermalZone> thermalZone() const;

    double radiativeFraction() const;

    bool isRadiativeFractionDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    // Note Schedules are passed by reference, not const reference.
    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    // No Setter, will be done when calling one of the three following functions
    //bool setPowerConversionEfficiencyMethod(const std::string& powerConversionEfficiencyMethod);

    bool setSimpleFixedEfficiency(double simpleFixedEfficiency);

    bool setDesignMaximumContinuousInputPower(double designMaximumContinuousInputPower);

    bool setEfficiencyFunctionofPowerCurve(const Curve& efficiencyFunctionofPowerCurve);

    bool setAncillaryPowerConsumedInStandby(double ancillaryPowerConsumedInStandby);

    void resetAncillaryPowerConsumedInStandby();

    // TODO: Check argument type. From object lists, some candidates are: ThermalZone.
    bool setThermalZone(const ThermalZone& thermalZone);

    void resetThermalZone();

    bool setRadiativeFraction(double radiativeFraction);

    void resetRadiativeFraction();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ElectricLoadCenterStorageConverter_Impl;

    explicit ElectricLoadCenterStorageConverter(std::shared_ptr<detail::ElectricLoadCenterStorageConverter_Impl> impl);

    friend class detail::ElectricLoadCenterStorageConverter_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ElectricLoadCenterStorageConverter");
  };

  /** \relates ElectricLoadCenterStorageConverter */
  using OptionalElectricLoadCenterStorageConverter = boost::optional<ElectricLoadCenterStorageConverter>;

  /** \relates ElectricLoadCenterStorageConverter */
  using ElectricLoadCenterStorageConverterVector = std::vector<ElectricLoadCenterStorageConverter>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ELECTRICLOADCENTERSTORAGECONVERTER_HPP
