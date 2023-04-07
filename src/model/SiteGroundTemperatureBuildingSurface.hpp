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

#ifndef MODEL_SITEGROUNDTEMPERATUREBUILDINGSURFACE_HPP
#define MODEL_SITEGROUNDTEMPERATUREBUILDINGSURFACE_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "../utilities/time/Date.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class SiteGroundTemperatureBuildingSurface_Impl;

  }  // namespace detail

  /** SiteGroundTemperatureBuildingSurface is a ModelObject that wraps the OpenStudio IDD object 'OS_Site_GroundTemperature_BuildingSurface'. */
  class MODEL_API SiteGroundTemperatureBuildingSurface : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SiteGroundTemperatureBuildingSurface(Model& model);

    virtual ~SiteGroundTemperatureBuildingSurface() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SiteGroundTemperatureBuildingSurface(const SiteGroundTemperatureBuildingSurface& other) = default;
    SiteGroundTemperatureBuildingSurface(SiteGroundTemperatureBuildingSurface&& other) = default;
    SiteGroundTemperatureBuildingSurface& operator=(const SiteGroundTemperatureBuildingSurface&) = default;
    SiteGroundTemperatureBuildingSurface& operator=(SiteGroundTemperatureBuildingSurface&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    double januaryGroundTemperature() const;

    bool isJanuaryGroundTemperatureDefaulted() const;

    double februaryGroundTemperature() const;

    bool isFebruaryGroundTemperatureDefaulted() const;

    double marchGroundTemperature() const;

    bool isMarchGroundTemperatureDefaulted() const;

    double aprilGroundTemperature() const;

    bool isAprilGroundTemperatureDefaulted() const;

    double mayGroundTemperature() const;

    bool isMayGroundTemperatureDefaulted() const;

    double juneGroundTemperature() const;

    bool isJuneGroundTemperatureDefaulted() const;

    double julyGroundTemperature() const;

    bool isJulyGroundTemperatureDefaulted() const;

    double augustGroundTemperature() const;

    bool isAugustGroundTemperatureDefaulted() const;

    double septemberGroundTemperature() const;

    bool isSeptemberGroundTemperatureDefaulted() const;

    double octoberGroundTemperature() const;

    bool isOctoberGroundTemperatureDefaulted() const;

    double novemberGroundTemperature() const;

    bool isNovemberGroundTemperatureDefaulted() const;

    double decemberGroundTemperature() const;

    bool isDecemberGroundTemperatureDefaulted() const;

    double getTemperatureByMonth(int month) const;

    double getTemperatureByMonth(const openstudio::MonthOfYear& month) const;

    bool isMonthDefaulted(int month) const;

    bool isMonthDefaulted(const openstudio::MonthOfYear& month) const;

    std::vector<double> getAllMonthlyTemperatures() const;

    //@}
    /** @name Setters */
    //@{

    bool setJanuaryGroundTemperature(double januaryGroundTemperature);

    void resetJanuaryGroundTemperature();

    bool setFebruaryGroundTemperature(double februaryGroundTemperature);

    void resetFebruaryGroundTemperature();

    bool setMarchGroundTemperature(double marchGroundTemperature);

    void resetMarchGroundTemperature();

    bool setAprilGroundTemperature(double aprilGroundTemperature);

    void resetAprilGroundTemperature();

    bool setMayGroundTemperature(double mayGroundTemperature);

    void resetMayGroundTemperature();

    bool setJuneGroundTemperature(double juneGroundTemperature);

    void resetJuneGroundTemperature();

    bool setJulyGroundTemperature(double julyGroundTemperature);

    void resetJulyGroundTemperature();

    bool setAugustGroundTemperature(double augustGroundTemperature);

    void resetAugustGroundTemperature();

    bool setSeptemberGroundTemperature(double septemberGroundTemperature);

    void resetSeptemberGroundTemperature();

    bool setOctoberGroundTemperature(double octoberGroundTemperature);

    void resetOctoberGroundTemperature();

    bool setNovemberGroundTemperature(double novemberGroundTemperature);

    void resetNovemberGroundTemperature();

    bool setDecemberGroundTemperature(double decemberGroundTemperature);

    void resetDecemberGroundTemperature();

    bool setTemperatureByMonth(int month, double temperature);

    bool setTemperatureByMonth(const openstudio::MonthOfYear& month, double temperature);

    void resetTemperatureByMonth(int month);

    void resetTemperatureByMonth(const openstudio::MonthOfYear& month);

    void resetAllMonths();

    bool setAllMonthlyTemperatures(const std::vector<double>& monthly_temperatures);

    //@}
   protected:
    /// @cond
    using ImplType = detail::SiteGroundTemperatureBuildingSurface_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::SiteGroundTemperatureBuildingSurface_Impl;

    explicit SiteGroundTemperatureBuildingSurface(std::shared_ptr<detail::SiteGroundTemperatureBuildingSurface_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SiteGroundTemperatureBuildingSurface");
  };

  /** \relates SiteGroundTemperatureBuildingSurface*/
  using OptionalSiteGroundTemperatureBuildingSurface = boost::optional<SiteGroundTemperatureBuildingSurface>;

  /** \relates SiteGroundTemperatureBuildingSurface*/
  using SiteGroundTemperatureBuildingSurfaceVector = std::vector<SiteGroundTemperatureBuildingSurface>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SITEGROUNDTEMPERATUREBUILDINGSURFACE_HPP
