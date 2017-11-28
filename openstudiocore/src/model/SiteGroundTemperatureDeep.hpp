/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_SITEGROUNDTEMPERATUREDEEP_HPP
#define MODEL_SITEGROUNDTEMPERATUREDEEP_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "../utilities/time/Date.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class SiteGroundTemperatureDeep_Impl;

} // detail

/** SiteGroundTemperatureDeep is a ModelObject that wraps the OpenStudio IDD object 'OS_Site_GroundTemperature_Deep'. */
class MODEL_API SiteGroundTemperatureDeep : public ModelObject {
 public:

  /** @name Constructors and Destructors */
  //@{

  explicit SiteGroundTemperatureDeep(Model& model);

  virtual ~SiteGroundTemperatureDeep() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  double januaryDeepGroundTemperature() const;

  bool isJanuaryDeepGroundTemperatureDefaulted() const;

  double februaryDeepGroundTemperature() const;

  bool isFebruaryDeepGroundTemperatureDefaulted() const;

  double marchDeepGroundTemperature() const;

  bool isMarchDeepGroundTemperatureDefaulted() const;

  double aprilDeepGroundTemperature() const;

  bool isAprilDeepGroundTemperatureDefaulted() const;

  double mayDeepGroundTemperature() const;

  bool isMayDeepGroundTemperatureDefaulted() const;

  double juneDeepGroundTemperature() const;

  bool isJuneDeepGroundTemperatureDefaulted() const;

  double julyDeepGroundTemperature() const;

  bool isJulyDeepGroundTemperatureDefaulted() const;

  double augustDeepGroundTemperature() const;

  bool isAugustDeepGroundTemperatureDefaulted() const;

  double septemberDeepGroundTemperature() const;

  bool isSeptemberDeepGroundTemperatureDefaulted() const;

  double octoberDeepGroundTemperature() const;

  bool isOctoberDeepGroundTemperatureDefaulted() const;

  double novemberDeepGroundTemperature() const;

  bool isNovemberDeepGroundTemperatureDefaulted() const;

  double decemberDeepGroundTemperature() const;

  bool isDecemberDeepGroundTemperatureDefaulted() const;

  double getTemperatureByMonth(int month) const;

  double getTemperatureByMonth(const openstudio::MonthOfYear & month) const;

  bool isMonthDefaulted(int month) const;

  bool isMonthDefaulted(const openstudio::MonthOfYear & month) const;

  std::vector<double> getAllMonthlyTemperatures() const;

  //@}
  /** @name Setters */
  //@{

  void setJanuaryDeepGroundTemperature(double januaryDeepGroundTemperature);

  void resetJanuaryDeepGroundTemperature();

  void setFebruaryDeepGroundTemperature(double februaryDeepGroundTemperature);

  void resetFebruaryDeepGroundTemperature();

  void setMarchDeepGroundTemperature(double marchDeepGroundTemperature);

  void resetMarchDeepGroundTemperature();

  void setAprilDeepGroundTemperature(double aprilDeepGroundTemperature);

  void resetAprilDeepGroundTemperature();

  void setMayDeepGroundTemperature(double mayDeepGroundTemperature);

  void resetMayDeepGroundTemperature();

  void setJuneDeepGroundTemperature(double juneDeepGroundTemperature);

  void resetJuneDeepGroundTemperature();

  void setJulyDeepGroundTemperature(double julyDeepGroundTemperature);

  void resetJulyDeepGroundTemperature();

  void setAugustDeepGroundTemperature(double augustDeepGroundTemperature);

  void resetAugustDeepGroundTemperature();

  void setSeptemberDeepGroundTemperature(double septemberDeepGroundTemperature);

  void resetSeptemberDeepGroundTemperature();

  void setOctoberDeepGroundTemperature(double octoberDeepGroundTemperature);

  void resetOctoberDeepGroundTemperature();

  void setNovemberDeepGroundTemperature(double novemberDeepGroundTemperature);

  void resetNovemberDeepGroundTemperature();

  void setDecemberDeepGroundTemperature(double decemberDeepGroundTemperature);

  void resetDecemberDeepGroundTemperature();

  void setTemperatureByMonth(int month, double temperature);

  void setTemperatureByMonth(const openstudio::MonthOfYear & month, double temperature);

  void resetTemperatureByMonth(int month);

  void resetTemperatureByMonth(const openstudio::MonthOfYear & month);

  void resetAllMonths();

  bool setAllMonthlyTemperatures(const std::vector<double> &monthly_temperatures);

  //@}
 protected:
  /// @cond
  typedef detail::SiteGroundTemperatureDeep_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::SiteGroundTemperatureDeep_Impl;

  explicit SiteGroundTemperatureDeep(std::shared_ptr<detail::SiteGroundTemperatureDeep_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.SiteGroundTemperatureDeep");
};

/** \relates SiteGroundTemperatureDeep*/
typedef boost::optional<SiteGroundTemperatureDeep> OptionalSiteGroundTemperatureDeep;

/** \relates SiteGroundTemperatureDeep*/
typedef std::vector<SiteGroundTemperatureDeep> SiteGroundTemperatureDeepVector;

} // model
} // openstudio

#endif // MODEL_SITEGROUNDTEMPERATUREDeep_HPP
