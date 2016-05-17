/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_SITEGROUNDTEMPERATURESHALLOW_IMPL_HPP
#define MODEL_SITEGROUNDTEMPERATURESHALLOW_IMPL_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"
#include <utilities/time/Date.hpp>

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
   private:
    REGISTER_LOGGER("openstudio.model.SiteGroundTemperatureShallow");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SITEGROUNDTEMPERATURESHALLOW_IMPL_HPP
