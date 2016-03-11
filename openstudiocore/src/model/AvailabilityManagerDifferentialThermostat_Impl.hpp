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

#ifndef MODEL_AVAILABILITYMANAGERDIFFERENTIALTHERMOSTAT_IMPL_HPP
#define MODEL_AVAILABILITYMANAGERDIFFERENTIALTHERMOSTAT_IMPL_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager_Impl.hpp"

namespace openstudio {
namespace model {

class Node;

namespace detail {

  /** AvailabilityManagerDifferentialThermostat_Impl is a AvailabilityManager_Impl that is the implementation class for AvailabilityManagerDifferentialThermostat.*/
  class MODEL_API AvailabilityManagerDifferentialThermostat_Impl : public AvailabilityManager_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    AvailabilityManagerDifferentialThermostat_Impl(const IdfObject& idfObject,
                                                   Model_Impl* model,
                                                   bool keepHandle);

    AvailabilityManagerDifferentialThermostat_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                   Model_Impl* model,
                                                   bool keepHandle);

    AvailabilityManagerDifferentialThermostat_Impl(const AvailabilityManagerDifferentialThermostat_Impl& other,
                                                   Model_Impl* model,
                                                   bool keepHandle);

    virtual ~AvailabilityManagerDifferentialThermostat_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    //@}
    /** @name Getters */
    //@{

    boost::optional<Node> hotNode() const;

    boost::optional<Node> coldNode() const;

    double temperatureDifferenceOnLimit() const;

    double temperatureDifferenceOffLimit() const;

    //@}
    /** @name Setters */
    //@{

    bool setHotNode(const boost::optional<Node>& node);

    void resetHotNode();

    bool setColdNode(const boost::optional<Node>& node);

    void resetColdNode();

    void setTemperatureDifferenceOnLimit(double temperatureDifferenceOnLimit);

    void setTemperatureDifferenceOffLimit(double temperatureDifferenceOffLimit);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.AvailabilityManagerDifferentialThermostat");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_AVAILABILITYMANAGERDIFFERENTIALTHERMOSTAT_IMPL_HPP

