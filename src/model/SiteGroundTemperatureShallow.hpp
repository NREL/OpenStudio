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

  }  // namespace detail

  /** SiteGroundTemperatureShallow is a ModelObject that wraps the OpenStudio IDD object 'OS_Site_GroundTemperature_Shallow'. */
  class MODEL_API SiteGroundTemperatureShallow : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SiteGroundTemperatureShallow(Model& model);

    virtual ~SiteGroundTemperatureShallow() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SiteGroundTemperatureShallow(const SiteGroundTemperatureShallow& other) = default;
    SiteGroundTemperatureShallow(SiteGroundTemperatureShallow&& other) = default;
    SiteGroundTemperatureShallow& operator=(const SiteGroundTemperatureShallow&) = default;
    SiteGroundTemperatureShallow& operator=(SiteGroundTemperatureShallow&&) = default;

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

    double getTemperatureByMonth(const openstudio::MonthOfYear& month) const;

    bool isMonthDefaulted(int month) const;

    bool isMonthDefaulted(const openstudio::MonthOfYear& month) const;

    std::vector<double> getAllMonthlyTemperatures() const;

    //@}
    /** @name Setters */
    //@{

    bool setJanuarySurfaceGroundTemperature(double januarySurfaceGroundTemperature);

    void resetJanuarySurfaceGroundTemperature();

    bool setFebruarySurfaceGroundTemperature(double februarySurfaceGroundTemperature);

    void resetFebruarySurfaceGroundTemperature();

    bool setMarchSurfaceGroundTemperature(double marchSurfaceGroundTemperature);

    void resetMarchSurfaceGroundTemperature();

    bool setAprilSurfaceGroundTemperature(double aprilSurfaceGroundTemperature);

    void resetAprilSurfaceGroundTemperature();

    bool setMaySurfaceGroundTemperature(double maySurfaceGroundTemperature);

    void resetMaySurfaceGroundTemperature();

    bool setJuneSurfaceGroundTemperature(double juneSurfaceGroundTemperature);

    void resetJuneSurfaceGroundTemperature();

    bool setJulySurfaceGroundTemperature(double julySurfaceGroundTemperature);

    void resetJulySurfaceGroundTemperature();

    bool setAugustSurfaceGroundTemperature(double augustSurfaceGroundTemperature);

    void resetAugustSurfaceGroundTemperature();

    bool setSeptemberSurfaceGroundTemperature(double septemberSurfaceGroundTemperature);

    void resetSeptemberSurfaceGroundTemperature();

    bool setOctoberSurfaceGroundTemperature(double octoberSurfaceGroundTemperature);

    void resetOctoberSurfaceGroundTemperature();

    bool setNovemberSurfaceGroundTemperature(double novemberSurfaceGroundTemperature);

    void resetNovemberSurfaceGroundTemperature();

    bool setDecemberSurfaceGroundTemperature(double decemberSurfaceGroundTemperature);

    void resetDecemberSurfaceGroundTemperature();

    bool setTemperatureByMonth(int month, double temperature);

    bool setTemperatureByMonth(const openstudio::MonthOfYear& month, double temperature);

    void resetTemperatureByMonth(int month);

    void resetTemperatureByMonth(const openstudio::MonthOfYear& month);

    void resetAllMonths();

    bool setAllMonthlyTemperatures(const std::vector<double>& monthly_temperatures);

    //@}
   protected:
    /// @cond
    using ImplType = detail::SiteGroundTemperatureShallow_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::SiteGroundTemperatureShallow_Impl;

    explicit SiteGroundTemperatureShallow(std::shared_ptr<detail::SiteGroundTemperatureShallow_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SiteGroundTemperatureShallow");
  };

  /** \relates SiteGroundTemperatureShallow*/
  using OptionalSiteGroundTemperatureShallow = boost::optional<SiteGroundTemperatureShallow>;

  /** \relates SiteGroundTemperatureShallow*/
  using SiteGroundTemperatureShallowVector = std::vector<SiteGroundTemperatureShallow>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SITEGROUNDTEMPERATURESHALLOW_HPP
