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

#include "OutputEnvironmentalImpactFactors.hpp"
#include "OutputEnvironmentalImpactFactors_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Output_EnvironmentalImpactFactors_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    OutputEnvironmentalImpactFactors_Impl::OutputEnvironmentalImpactFactors_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == OutputEnvironmentalImpactFactors::iddObjectType());
    }

    OutputEnvironmentalImpactFactors_Impl::OutputEnvironmentalImpactFactors_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == OutputEnvironmentalImpactFactors::iddObjectType());
    }

    OutputEnvironmentalImpactFactors_Impl::OutputEnvironmentalImpactFactors_Impl(const OutputEnvironmentalImpactFactors_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& OutputEnvironmentalImpactFactors_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        "Environmental Impact NaturalGas Source Energy",
        "Environmental Impact NaturalGas CO2 Emissions Mass",
        "Environmental Impact NaturalGas CO Emissions Mass",
        "Environmental Impact NaturalGas CH4 Emissions Mass",
        "Environmental Impact NaturalGas NOx Emissions Mass",
        "Environmental Impact NaturalGas N2O Emissions Mass",
        "Environmental Impact NaturalGas SO2 Emissions Mass",
        "Environmental Impact NaturalGas PM Emissions Mass",
        "Environmental Impact NaturalGas PM10 Emissions Mass",
        "Environmental Impact NaturalGas PM2.5 Emissions Mass",
        "Environmental Impact NaturalGas NH3 Emissions Mass",
        "Environmental Impact NaturalGas NMVOC Emissions Mass",
        "Environmental Impact NaturalGas Hg Emissions Mass",
        "Environmental Impact NaturalGas Pb Emissions Mass",
        "Environmental Impact NaturalGas Water Consumption Volume",
        "Environmental Impact NaturalGas Nuclear High Level Waste Mass",
        "Environmental Impact NaturalGas Nuclear Low Level Waste Volume",
        "Environmental Impact FuelOilNo2 Source Energy",
        "Environmental Impact FuelOilNo2 CO2 Emissions Mass",
        "Environmental Impact FuelOilNo2 CO Emissions Mass",
        "Environmental Impact FuelOilNo2 CH4 Emissions Mass",
        "Environmental Impact FuelOilNo2 NOx Emissions Mass",
        "Environmental Impact FuelOilNo2 N2O Emissions Mass",
        "Environmental Impact FuelOilNo2 SO2 Emissions Mass",
        "Environmental Impact FuelOilNo2 PM Emissions Mass",
        "Environmental Impact FuelOilNo2 PM10 Emissions Mass",
        "Environmental Impact FuelOilNo2 PM2.5 Emissions Mass",
        "Environmental Impact FuelOilNo2 NH3 Emissions Mass",
        "Environmental Impact FuelOilNo2 NMVOC Emissions Mass",
        "Environmental Impact FuelOilNo2 Hg Emissions Mass",
        "Environmental Impact FuelOilNo2 Pb Emissions Mass",
        "Environmental Impact FuelOilNo2 Water Consumption Volume",
        "Environmental Impact FuelOilNo2 Nuclear High Level Waste Mass",
        "Environmental Impact FuelOilNo2 Nuclear Low Level Waste Volume",
        "Environmental Impact FuelOilNo1 Source Energy",
        "Environmental Impact FuelOilNo1 CO2 Emissions Mass",
        "Environmental Impact FuelOilNo1 CO Emissions Mass",
        "Environmental Impact FuelOilNo1 CH4 Emissions Mass",
        "Environmental Impact FuelOilNo1 NOx Emissions Mass",
        "Environmental Impact FuelOilNo1 N2O Emissions Mass",
        "Environmental Impact FuelOilNo1 SO2 Emissions Mass",
        "Environmental Impact FuelOilNo1 PM Emissions Mass",
        "Environmental Impact FuelOilNo1 PM10 Emissions Mass",
        "Environmental Impact FuelOilNo1 PM2.5 Emissions Mass",
        "Environmental Impact FuelOilNo1 NH3 Emissions Mass",
        "Environmental Impact FuelOilNo1 NMVOC Emissions Mass",
        "Environmental Impact FuelOilNo1 Hg Emissions Mass",
        "Environmental Impact FuelOilNo1 Pb Emissions Mass",
        "Environmental Impact FuelOilNo1 Water Consumption Volume",
        "Environmental Impact FuelOilNo1 Nuclear High Level Waste Mass",
        "Environmental Impact FuelOilNo1 Nuclear Low Level Waste Volume",
        "Environmental Impact Coal Source Energy",
        "Environmental Impact Coal CO2 Emissions Mass",
        "Environmental Impact Coal CO Emissions Mass",
        "Environmental Impact Coal CH4 Emissions Mass",
        "Environmental Impact Coal NOx Emissions Mass",
        "Environmental Impact Coal N2O Emissions Mass",
        "Environmental Impact Coal SO2 Emissions Mass",
        "Environmental Impact Coal PM Emissions Mass",
        "Environmental Impact Coal PM10 Emissions Mass",
        "Environmental Impact Coal PM2.5 Emissions Mass",
        "Environmental Impact Coal NH3 Emissions Mass",
        "Environmental Impact Coal NMVOC Emissions Mass",
        "Environmental Impact Coal Hg Emissions Mass",
        "Environmental Impact Coal Pb Emissions Mass",
        "Environmental Impact Coal Water Consumption Volume",
        "Environmental Impact Coal Nuclear High Level Waste Mass",
        "Environmental Impact Coal Nuclear Low Level Waste Volume",
        "Environmental Impact Electricity Source Energy",
        "Environmental Impact Electricity CO2 Emissions Mass",
        "Environmental Impact Electricity CO Emissions Mass",
        "Environmental Impact Electricity CH4 Emissions Mass",
        "Environmental Impact Electricity NOx Emissions Mass",
        "Environmental Impact Electricity N2O Emissions Mass",
        "Environmental Impact Electricity SO2 Emissions Mass",
        "Environmental Impact Electricity PM Emissions Mass",
        "Environmental Impact Electricity PM10 Emissions Mass",
        "Environmental Impact Electricity PM2.5 Emissions Mass",
        "Environmental Impact Electricity NH3 Emissions Mass",
        "Environmental Impact Electricity NMVOC Emissions Mass",
        "Environmental Impact Electricity Hg Emissions Mass",
        "Environmental Impact Electricity Pb Emissions Mass",
        "Environmental Impact Electricity Water Consumption Volume",
        "Environmental Impact Electricity Nuclear High Level Waste Mass",
        "Environmental Impact Electricity Nuclear Low Level Waste Volume",
        "Environmental Impact Purchased Electricity Source Energy",
        "Environmental Impact Surplus Sold Electricity Source",
        "Environmental Impact Gasoline Source Energy",
        "Environmental Impact Gasoline CO2 Emissions Mass",
        "Environmental Impact Gasoline CO Emissions Mass",
        "Environmental Impact Gasoline CH4 Emissions Mass",
        "Environmental Impact Gasoline NOx Emissions Mass",
        "Environmental Impact Gasoline N2O Emissions Mass",
        "Environmental Impact Gasoline SO2 Emissions Mass",
        "Environmental Impact Gasoline PM Emissions Mass",
        "Environmental Impact Gasoline PM10 Emissions Mass",
        "Environmental Impact Gasoline PM2.5 Emissions Mass",
        "Environmental Impact Gasoline NH3 Emissions Mass",
        "Environmental Impact Gasoline NMVOC Emissions Mass",
        "Environmental Impact Gasoline Hg Emissions Mass",
        "Environmental Impact Gasoline Pb Emissions Mass",
        "Environmental Impact Gasoline Water Consumption Volume",
        "Environmental Impact Gasoline Nuclear High Level Waste Mass",
        "Environmental Impact Gasoline Nuclear Low Level Waste Volume",
        "Environmental Impact Propane Source Energy",
        "Environmental Impact Propane CO2 Emissions Mass",
        "Environmental Impact Propane CO Emissions Mass",
        "Environmental Impact Propane CH4 Emissions Mass",
        "Environmental Impact Propane NOx Emissions Mass",
        "Environmental Impact Propane N2O Emissions Mass",
        "Environmental Impact Propane SO2 Emissions Mass",
        "Environmental Impact Propane PM Emissions Mass",
        "Environmental Impact Propane PM10 Emissions Mass",
        "Environmental Impact Propane PM2.5 Emissions Mass",
        "Environmental Impact Propane NH3 Emissions Mass",
        "Environmental Impact Propane NMVOC Emissions Mass",
        "Environmental Impact Propane Hg Emissions Mass",
        "Environmental Impact Propane Pb Emissions Mass",
        "Environmental Impact Propane Water Consumption Volume",
        "Environmental Impact Propane Nuclear High Level Waste Mass",
        "Environmental Impact Propane Nuclear Low Level Waste Volume",
        "Environmental Impact Diesel Source Energy",
        "Environmental Impact Diesel CO2 Emissions Mass",
        "Environmental Impact Diesel CO Emissions Mass",
        "Environmental Impact Diesel CH4 Emissions Mass",
        "Environmental Impact Diesel NOx Emissions Mass",
        "Environmental Impact Diesel N2O Emissions Mass",
        "Environmental Impact Diesel SO2 Emissions Mass",
        "Environmental Impact Diesel PM Emissions Mass",
        "Environmental Impact Diesel PM10 Emissions Mass",
        "Environmental Impact Diesel PM2.5 Emissions Mass",
        "Environmental Impact Diesel NH3 Emissions Mass",
        "Environmental Impact Diesel NMVOC Emissions Mass",
        "Environmental Impact Diesel Hg Emissions Mass",
        "Environmental Impact Diesel Pb Emissions Mass",
        "Environmental Impact Diesel Water Consumption Volume",
        "Environmental Impact Diesel Nuclear High Level Waste Mass",
        "Environmental Impact Diesel Nuclear Low Level Waste Volume",
        "Environmental Impact OtherFuel1 Source Energy",
        "Environmental Impact OtherFuel1 CO2 Emissions Mass",
        "Environmental Impact OtherFuel1 CO Emissions Mass",
        "Environmental Impact OtherFuel1 CH4 Emissions Mass",
        "Environmental Impact OtherFuel1 NOx Emissions Mass",
        "Environmental Impact OtherFuel1 N2O Emissions Mass",
        "Environmental Impact OtherFuel1 SO2 Emissions Mass",
        "Environmental Impact OtherFuel1 PM Emissions Mass",
        "Environmental Impact OtherFuel1 PM10 Emissions Mass",
        "Environmental Impact OtherFuel1 PM2.5 Emissions Mass",
        "Environmental Impact OtherFuel1 NH3 Emissions Mass",
        "Environmental Impact OtherFuel1 NMVOC Emissions Mass",
        "Environmental Impact OtherFuel1 Hg Emissions Mass",
        "Environmental Impact OtherFuel1 Pb Emissions Mass",
        "Environmental Impact OtherFuel1 CO2 Water Consumption Volume",
        "Environmental Impact OtherFuel1 Nuclear High Level Waste Mass",
        "Environmental Impact OtherFuel1 Nuclear Low Level Waste Volume",
        "Environmental Impact OtherFuel2 Source Energy",
        "Environmental Impact OtherFuel2 CO2 Emissions Mass",
        "Environmental Impact OtherFuel2 CO Emissions Mass",
        "Environmental Impact OtherFuel2 CH4 Emissions Mass",
        "Environmental Impact OtherFuel2 NOx Emissions Mass",
        "Environmental Impact OtherFuel2 N2O Emissions Mass",
        "Environmental Impact OtherFuel2 SO2 Emissions Mass",
        "Environmental Impact OtherFuel2 PM Emissions Mass",
        "Environmental Impact OtherFuel2 PM10 Emissions Mass",
        "Environmental Impact OtherFuel2 PM2.5 Emissions Mass",
        "Environmental Impact OtherFuel2 NH3 Emissions Mass",
        "Environmental Impact OtherFuel2 NMVOC Emissions Mass",
        "Environmental Impact OtherFuel2 Hg Emissions Mass",
        "Environmental Impact OtherFuel2 Pb Emissions Mass",
        "Environmental Impact OtherFuel2 CO2 Water Consumption Volume",
        "Environmental Impact OtherFuel2 Nuclear High Level Waste Mass",
        "Environmental Impact OtherFuel2 Nuclear Low Level Waste Volume",
        "Environmental Impact Total N2O Emissions Carbon Equivalent Mass",
        "Environmental Impact Total CH4 Emissions Carbon Equivalent Mass",
        "Environmental Impact Total CO2 Emissions Carbon Equivalent Mass",
      };
      return result;
    }

    IddObjectType OutputEnvironmentalImpactFactors_Impl::iddObjectType() const {
      return OutputEnvironmentalImpactFactors::iddObjectType();
    }

    std::string OutputEnvironmentalImpactFactors_Impl::reportingFrequency() const {
      boost::optional<std::string> value = getString(OS_Output_EnvironmentalImpactFactorsFields::ReportingFrequency, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool OutputEnvironmentalImpactFactors_Impl::setReportingFrequency(const std::string& reportingFrequency) {
      bool result = setString(OS_Output_EnvironmentalImpactFactorsFields::ReportingFrequency, reportingFrequency);
      return result;
    }

  }  // namespace detail

  OutputEnvironmentalImpactFactors::OutputEnvironmentalImpactFactors(const Model& model)
    : ModelObject(OutputEnvironmentalImpactFactors::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::OutputEnvironmentalImpactFactors_Impl>());

    setReportingFrequency("Monthly");
  }

  IddObjectType OutputEnvironmentalImpactFactors::iddObjectType() {
    return {IddObjectType::OS_Output_EnvironmentalImpactFactors};
  }

  std::vector<std::string> OutputEnvironmentalImpactFactors::reportingFrequencyValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Output_EnvironmentalImpactFactorsFields::ReportingFrequency);
  }

  std::string OutputEnvironmentalImpactFactors::reportingFrequency() const {
    return getImpl<detail::OutputEnvironmentalImpactFactors_Impl>()->reportingFrequency();
  }

  bool OutputEnvironmentalImpactFactors::setReportingFrequency(const std::string& reportingFrequency) {
    return getImpl<detail::OutputEnvironmentalImpactFactors_Impl>()->setReportingFrequency(reportingFrequency);
  }

  /// @cond
  OutputEnvironmentalImpactFactors::OutputEnvironmentalImpactFactors(std::shared_ptr<detail::OutputEnvironmentalImpactFactors_Impl> impl)
    : ModelObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
