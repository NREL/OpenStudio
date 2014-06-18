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

#ifndef MODEL_REFRIGERATIONDEFROSTCYCLEPARAMETERS_IMPL_HPP
#define MODEL_REFRIGERATIONDEFROSTCYCLEPARAMETERS_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {

class Time;

namespace model {

namespace detail {

  /** RefrigerationDefrostCycleParameters_Impl is a ModelObject_Impl that is the implementation class for RefrigerationDefrostCycleParameters.*/
  class MODEL_API RefrigerationDefrostCycleParameters_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    RefrigerationDefrostCycleParameters_Impl(const IdfObject& idfObject,
                                             Model_Impl* model,
                                             bool keepHandle);

    RefrigerationDefrostCycleParameters_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle);

    RefrigerationDefrostCycleParameters_Impl(const RefrigerationDefrostCycleParameters_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle);

    virtual ~RefrigerationDefrostCycleParameters_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    boost::optional<int> durationofDefrostCycle() const;

    boost::optional<int> dripDownTime() const;

    boost::optional<openstudio::Time> defrost1StartTime() const;

    boost::optional<openstudio::Time> defrost2StartTime() const;

    boost::optional<openstudio::Time> defrost3StartTime() const;

    boost::optional<openstudio::Time> defrost4StartTime() const;

    boost::optional<openstudio::Time> defrost5StartTime() const;

    boost::optional<openstudio::Time> defrost6StartTime() const;

    boost::optional<openstudio::Time> defrost7StartTime() const;

    boost::optional<openstudio::Time> defrost8StartTime() const;

    //@}
    /** @name Setters */
    //@{

    bool setDurationofDefrostCycle(boost::optional<int> durationofDefrostCycle);

    void resetDurationofDefrostCycle();

    bool setDripDownTime(boost::optional<int> dripDownTime);

    void resetDripDownTime();

    bool setDefrost1StartTime(const openstudio::Time& defrost1StartTime);

    void resetDefrost1StartTime();

    bool setDefrost2StartTime(const openstudio::Time& defrost2StartTime);

    void resetDefrost2StartTime();

    bool setDefrost3StartTime(const openstudio::Time& defrost3StartTime);

    void resetDefrost3StartTime();

    bool setDefrost4StartTime(const openstudio::Time& defrost4StartTime);

    void resetDefrost4StartTime();

    bool setDefrost5StartTime(const openstudio::Time& defrost5StartTime);

    void resetDefrost5StartTime();

    bool setDefrost6StartTime(const openstudio::Time& defrost6StartTime);

    void resetDefrost6StartTime();

    bool setDefrost7StartTime(const openstudio::Time& defrost7StartTime);

    void resetDefrost7StartTime();

    bool setDefrost8StartTime(const openstudio::Time& defrost8StartTime);

    void resetDefrost8StartTime();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationDefrostCycleParameters");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONDEFROSTCYCLEPARAMETERS_IMPL_HPP

