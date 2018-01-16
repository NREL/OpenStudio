/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_SITEWATERMAINSTEMPERATURE_HPP
#define MODEL_SITEWATERMAINSTEMPERATURE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class Schedule;

namespace detail {

  class SiteWaterMainsTemperature_Impl;

} // detail

/** SiteWaterMainsTemperature is a ModelObject that wraps the OpenStudio IDD object
 *  'OS:Site:WaterMainsTemperature'. */
class MODEL_API SiteWaterMainsTemperature : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~SiteWaterMainsTemperature() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> calculationMethodValues();

  /** \deprecated */
  static std::vector<std::string> validCalculationMethodValues();

  /** @name Getters */
  //@{

  std::string calculationMethod() const;

  boost::optional<Schedule> temperatureSchedule() const;

  boost::optional<double> annualAverageOutdoorAirTemperature() const;

  OSOptionalQuantity getAnnualAverageOutdoorAirTemperature(bool returnIP=false) const;

  boost::optional<double> maximumDifferenceInMonthlyAverageOutdoorAirTemperatures() const;

  OSOptionalQuantity getMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(bool returnIP=false) const;

  //@}
  /** @name Setters */
  //@{

  bool setCalculationMethod(std::string calculationMethod);

  /** Also sets calculation method to 'Schedule'. */
  bool setTemperatureSchedule(Schedule& schedule);

  void resetTemperatureSchedule();

  /** Also sets calculation method to 'Correlation'. */
  bool setAnnualAverageOutdoorAirTemperature(double annualAverageOutdoorAirTemperature);

  /** Also sets calculation method to 'Correlation'. */
  bool setAnnualAverageOutdoorAirTemperature(const Quantity& annualAverageOutdoorAirTemperature);

  void resetAnnualAverageOutdoorAirTemperature();

  /** Also sets calculation method to 'Correlation'. */
  bool setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(double maximumDifferenceInMonthlyAverageOutdoorAirTemperatures);

  /** Also sets calculation method to 'Correlation'. */
  bool setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(const Quantity& maximumDifferenceInMonthlyAverageOutdoorAirTemperatures);

  void resetMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures();

  //@}
 protected:

  /// @cond
  typedef detail::SiteWaterMainsTemperature_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::SiteWaterMainsTemperature_Impl;

  explicit SiteWaterMainsTemperature(std::shared_ptr<detail::SiteWaterMainsTemperature_Impl> impl);

  explicit SiteWaterMainsTemperature(const Model& model);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.SiteWaterMainsTemperature");
};

/** \relates SiteWaterMainsTemperature*/
typedef boost::optional<SiteWaterMainsTemperature> OptionalSiteWaterMainsTemperature;

/** \relates SiteWaterMainsTemperature*/
typedef std::vector<SiteWaterMainsTemperature> SiteWaterMainsTemperatureVector;

} // model
} // openstudio

#endif // MODEL_SITEWATERMAINSTEMPERATURE_HPP
