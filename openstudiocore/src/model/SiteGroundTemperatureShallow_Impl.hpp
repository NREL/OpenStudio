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

#ifndef MODEL_SITEGROUNDTEMPERATURESHALLOW_IMPL_HPP
#define MODEL_SITEGROUNDTEMPERATURESHALLOW_IMPL_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"
#include "../utilities/time/Date.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** SiteGroundTemperatureShallow_Impl is a ModelObject_Impl that is the implementation class for SiteGroundTemperatureShallow.*/
  class MODEL_API SiteGroundTemperatureShallow_Impl : public ModelObject_Impl {

   public:

    /** @name Constructors and Destructors */
    //@{

    SiteGroundTemperatureShallow_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    SiteGroundTemperatureShallow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                              Model_Impl* model,
                                              bool keepHandle);

    SiteGroundTemperatureShallow_Impl(const SiteGroundTemperatureShallow_Impl& other,
                                              Model_Impl* model,
                                              bool keepHandle);

    virtual ~SiteGroundTemperatureShallow_Impl() {}

    //@}

    virtual boost::optional<ParentObject> parent() const override;

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

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

    bool setTemperatureByMonth(const openstudio::MonthOfYear & month, double temperature);

    void resetTemperatureByMonth(int month);

    void resetTemperatureByMonth(const openstudio::MonthOfYear & month);

    void resetAllMonths();

    bool setAllMonthlyTemperatures(const std::vector<double> &monthly_temperatures);

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.SiteGroundTemperatureShallow");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SITEGROUNDTEMPERATURESHALLOW_IMPL_HPP
