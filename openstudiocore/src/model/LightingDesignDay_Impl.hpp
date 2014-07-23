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

#ifndef MODEL_LIGHTINGDESIGNDAY_IMPL_HPP
#define MODEL_LIGHTINGDESIGNDAY_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {

class DateTime;
class Date;
class Time;

namespace model {

namespace detail {

  /** LightingDesignDay_Impl is a ModelObject_Impl that is the implementation class for LightingDesignDay.*/
  class MODEL_API LightingDesignDay_Impl : public ModelObject_Impl {
    Q_OBJECT;
    Q_PROPERTY(std::string cieSkyModel READ cieSkyModel WRITE setCIESkyModel);
    Q_PROPERTY(int snowIndicator READ snowIndicator WRITE setSnowIndicator RESET resetSnowIndicator);
    Q_PROPERTY(bool isSnowIndicatorDefaulted READ isSnowIndicatorDefaulted);
   public:

    /** @name Constructors and Destructors */
    //@{

    LightingDesignDay_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    LightingDesignDay_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    LightingDesignDay_Impl(const LightingDesignDay_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    virtual ~LightingDesignDay_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual boost::optional<ParentObject> parent() const;

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //}
    /** @name Getters */
    //@{

    std::string cieSkyModel() const;

    int snowIndicator() const;

    bool isSnowIndicatorDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setCIESkyModel(std::string cIESkyModel);

    bool setSnowIndicator(int snowIndicator);

    void resetSnowIndicator();

    //@}

    openstudio::Date date() const;

    bool setDate(const openstudio::Date& date);

    std::vector<openstudio::Time> simulationTimes() const;
  
    std::vector<openstudio::DateTime> simulationDateTimes() const;

    bool addSimulationTime(const openstudio::Time& time);

    void clearSimulationTimes();

    // ensure that this object does not contain the date 2/29
    void ensureNoLeapDays();

   protected:
   private:
    REGISTER_LOGGER("openstudio.model.LightingDesignDay");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_LIGHTINGDESIGNDAY_IMPL_HPP

