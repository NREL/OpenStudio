/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef MODEL_LIGHTINGSIMULATIONCONTROL_IMPL_HPP
#define MODEL_LIGHTINGSIMULATIONCONTROL_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** LightingSimulationControl_Impl is a ModelObject_Impl that is the implementation class for LightingSimulationControl.*/
  class MODEL_API LightingSimulationControl_Impl : public ModelObject_Impl {
    Q_OBJECT;
    Q_PROPERTY(bool runSimulationforDesignDays READ runSimulationforDesignDays WRITE setRunSimulationforDesignDays RESET resetRunSimulationforDesignDays);
    Q_PROPERTY(bool isRunSimulationforDesignDaysDefaulted READ isRunSimulationforDesignDaysDefaulted);
    Q_PROPERTY(bool runSimulationforWeatherFileRunPeriods READ runSimulationforWeatherFileRunPeriods WRITE setRunSimulationforWeatherFileRunPeriods RESET resetRunSimulationforWeatherFileRunPeriods);
    Q_PROPERTY(bool isRunSimulationforWeatherFileRunPeriodsDefaulted READ isRunSimulationforWeatherFileRunPeriodsDefaulted);
   public:

    /** @name Constructors and Destructors */
    //@{

    LightingSimulationControl_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    LightingSimulationControl_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                   Model_Impl* model,
                                   bool keepHandle);

    LightingSimulationControl_Impl(const LightingSimulationControl_Impl& other,
                                   Model_Impl* model,
                                   bool keepHandle);

    virtual ~LightingSimulationControl_Impl() {}

    //@}
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    /** @name Getters */
    //@{

    bool runSimulationforDesignDays() const;

    bool isRunSimulationforDesignDaysDefaulted() const;

    bool runSimulationforWeatherFileRunPeriods() const;

    bool isRunSimulationforWeatherFileRunPeriodsDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    void setRunSimulationforDesignDays(bool runSimulationforDesignDays);

    void resetRunSimulationforDesignDays();

    void setRunSimulationforWeatherFileRunPeriods(bool runSimulationforWeatherFileRunPeriods);

    void resetRunSimulationforWeatherFileRunPeriods();

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.LightingSimulationControl");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_LIGHTINGSIMULATIONCONTROL_IMPL_HPP

