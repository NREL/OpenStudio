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

#ifndef MODEL_SITEGROUNDTEMPERATURESHALLOW_HPP
#define MODEL_SITEGROUNDTEMPERATURESHALLOW_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "../utilities/time/Date.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class SiteGroundTemperatureShallow_Impl;

} // detail

/** SiteGroundTemperatureShallow is a ModelObject that wraps the OpenStudio IDD object 'OS_Site_GroundTemperature_Shallow'. */
class MODEL_API SiteGroundTemperatureShallow : public ModelObject {
 public:

  /** @name Constructors and Destructors */
  //@{

  explicit SiteGroundTemperatureShallow(Model& model);

  virtual ~SiteGroundTemperatureShallow() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  double januarySurfaceGroundTemperature() const;

  bool isJanuarySurfaceGroundTemperatureDefaulted() const;

  double februarySurfaceGroundTemperature() const;

  bool isFebruarySurfaceGroundTemperatureDefaulted() const;

  double marchSurfaceGroundTemperature() const;

  bool isMarchSurfaceGroundTemperatureDefaulted() const;

  double aprilSurfaceGroundTemperature() const;

  bool isAprilSurfaceGroundTemperatureDefaulted() const;

  double maySurfaceGroundTemperature() const;

  bool isMaySurfaceGroundTemperatureDefaulted() const;

  double juneSurfaceGroundTemperature() const;

  bool isJuneSurfaceGroundTemperatureDefaulted() const;

  double julySurfaceGroundTemperature() const;

  bool isJulySurfaceGroundTemperatureDefaulted() const;

  double augustSurfaceGroundTemperature() const;

  bool isAugustSurfaceGroundTemperatureDefaulted() const;

  double septemberSurfaceGroundTemperature() const;

  bool isSeptemberSurfaceGroundTemperatureDefaulted() const;

  double octoberSurfaceGroundTemperature() const;

  bool isOctoberSurfaceGroundTemperatureDefaulted() const;

  double novemberSurfaceGroundTemperature() const;

  bool isNovemberSurfaceGroundTemperatureDefaulted() const;

  double decemberSurfaceGroundTemperature() const;

  bool isDecemberSurfaceGroundTemperatureDefaulted() const;

  double getTemperatureByMonth(int month) const;

  double getTemperatureByMonth(const openstudio::MonthOfYear & month) const;

  bool isMonthDefaulted(int month) const;

  bool isMonthDefaulted(const openstudio::MonthOfYear & month) const;

  std::vector<double> getAllMonthlyTemperatures() const;

  //@}
  /** @name Setters */
  //@{

  void setJanuarySurfaceGroundTemperature(double januarySurfaceGroundTemperature);

  void resetJanuarySurfaceGroundTemperature();

  void setFebruarySurfaceGroundTemperature(double februarySurfaceGroundTemperature);

  void resetFebruarySurfaceGroundTemperature();

  void setMarchSurfaceGroundTemperature(double marchSurfaceGroundTemperature);

  void resetMarchSurfaceGroundTemperature();

  void setAprilSurfaceGroundTemperature(double aprilSurfaceGroundTemperature);

  void resetAprilSurfaceGroundTemperature();

  void setMaySurfaceGroundTemperature(double maySurfaceGroundTemperature);

  void resetMaySurfaceGroundTemperature();

  void setJuneSurfaceGroundTemperature(double juneSurfaceGroundTemperature);

  void resetJuneSurfaceGroundTemperature();

  void setJulySurfaceGroundTemperature(double julySurfaceGroundTemperature);

  void resetJulySurfaceGroundTemperature();

  void setAugustSurfaceGroundTemperature(double augustSurfaceGroundTemperature);

  void resetAugustSurfaceGroundTemperature();

  void setSeptemberSurfaceGroundTemperature(double septemberSurfaceGroundTemperature);

  void resetSeptemberSurfaceGroundTemperature();

  void setOctoberSurfaceGroundTemperature(double octoberSurfaceGroundTemperature);

  void resetOctoberSurfaceGroundTemperature();

  void setNovemberSurfaceGroundTemperature(double novemberSurfaceGroundTemperature);

  void resetNovemberSurfaceGroundTemperature();

  void setDecemberSurfaceGroundTemperature(double decemberSurfaceGroundTemperature);

  void resetDecemberSurfaceGroundTemperature();

  void setTemperatureByMonth(int month, double temperature);

  void setTemperatureByMonth(const openstudio::MonthOfYear & month, double temperature);

  void resetTemperatureByMonth(int month);

  void resetTemperatureByMonth(const openstudio::MonthOfYear & month);

  void resetAllMonths();

  bool setAllMonthlyTemperatures(const std::vector<double> &monthly_temperatures);

  //@}
 protected:
  /// @cond
  typedef detail::SiteGroundTemperatureShallow_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::SiteGroundTemperatureShallow_Impl;

  explicit SiteGroundTemperatureShallow(std::shared_ptr<detail::SiteGroundTemperatureShallow_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.SiteGroundTemperatureShallow");
};

/** \relates SiteGroundTemperatureShallow*/
typedef boost::optional<SiteGroundTemperatureShallow> OptionalSiteGroundTemperatureShallow;

/** \relates SiteGroundTemperatureShallow*/
typedef std::vector<SiteGroundTemperatureShallow> SiteGroundTemperatureShallowVector;

} // model
} // openstudio

#endif // MODEL_SITEGROUNDTEMPERATURESHALLOW_HPP
