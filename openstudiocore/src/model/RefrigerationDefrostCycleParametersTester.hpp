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

#ifndef MODEL_REFRIGERATIONDEFROSTCYCLEPARAMETERSTESTER_HPP
#define MODEL_REFRIGERATIONDEFROSTCYCLEPARAMETERSTESTER_HPP

#include <model/ModelAPI.hpp>
#include <model/ModelObject.hpp>

namespace openstudio {
namespace model {

namespace detail {

  class RefrigerationDefrostCycleParametersTester_Impl;

} // detail

/** RefrigerationDefrostCycleParametersTester is a ModelObject that wraps the OpenStudio IDD object 'OS:Refrigeration:DefrostCycleParameters'. */
class MODEL_API RefrigerationDefrostCycleParametersTester : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit RefrigerationDefrostCycleParametersTester(const Model& model);

  virtual ~RefrigerationDefrostCycleParametersTester() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<int> durationofDefrostCycle() const;

  boost::optional<int> dripDownTime() const;

  boost::optional<int> defrost1HourStartTime() const;

  boost::optional<int> defrost1MinuteStartTime() const;

  boost::optional<int> defrost2HourStartTime() const;

  boost::optional<int> defrost2MinuteStartTime() const;

  boost::optional<int> defrost3HourStartTime() const;

  boost::optional<int> defrost3MinuteStartTime() const;

  boost::optional<int> defrost4HourStartTime() const;

  boost::optional<int> defrost4MinuteStartTime() const;

  boost::optional<int> defrost5HourStartTime() const;

  boost::optional<int> defrost5MinuteStartTime() const;

  boost::optional<int> defrost6HourStartTime() const;

  boost::optional<int> defrost6MinuteStartTime() const;

  boost::optional<int> defrost7HourStartTime() const;

  boost::optional<int> defrost7MinuteStartTime() const;

  boost::optional<int> defrost8HourStartTime() const;

  boost::optional<int> defrost8MinuteStartTime() const;

  //@}
  /** @name Setters */
  //@{

  bool setDurationofDefrostCycle(int durationofDefrostCycle);

  void resetDurationofDefrostCycle();

  bool setDripDownTime(int dripDownTime);

  void resetDripDownTime();

  bool setDefrost1HourStartTime(int defrost1HourStartTime);

  void resetDefrost1HourStartTime();

  bool setDefrost1MinuteStartTime(int defrost1MinuteStartTime);

  void resetDefrost1MinuteStartTime();

  bool setDefrost2HourStartTime(int defrost2HourStartTime);

  void resetDefrost2HourStartTime();

  bool setDefrost2MinuteStartTime(int defrost2MinuteStartTime);

  void resetDefrost2MinuteStartTime();

  bool setDefrost3HourStartTime(int defrost3HourStartTime);

  void resetDefrost3HourStartTime();

  bool setDefrost3MinuteStartTime(int defrost3MinuteStartTime);

  void resetDefrost3MinuteStartTime();

  bool setDefrost4HourStartTime(int defrost4HourStartTime);

  void resetDefrost4HourStartTime();

  bool setDefrost4MinuteStartTime(int defrost4MinuteStartTime);

  void resetDefrost4MinuteStartTime();

  bool setDefrost5HourStartTime(int defrost5HourStartTime);

  void resetDefrost5HourStartTime();

  bool setDefrost5MinuteStartTime(int defrost5MinuteStartTime);

  void resetDefrost5MinuteStartTime();

  bool setDefrost6HourStartTime(int defrost6HourStartTime);

  void resetDefrost6HourStartTime();

  bool setDefrost6MinuteStartTime(int defrost6MinuteStartTime);

  void resetDefrost6MinuteStartTime();

  bool setDefrost7HourStartTime(int defrost7HourStartTime);

  void resetDefrost7HourStartTime();

  bool setDefrost7MinuteStartTime(int defrost7MinuteStartTime);

  void resetDefrost7MinuteStartTime();

  bool setDefrost8HourStartTime(int defrost8HourStartTime);

  void resetDefrost8HourStartTime();

  bool setDefrost8MinuteStartTime(int defrost8MinuteStartTime);

  void resetDefrost8MinuteStartTime();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::RefrigerationDefrostCycleParametersTester_Impl ImplType;

  explicit RefrigerationDefrostCycleParametersTester(std::shared_ptr<detail::RefrigerationDefrostCycleParametersTester_Impl> impl);

  friend class detail::RefrigerationDefrostCycleParametersTester_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.RefrigerationDefrostCycleParametersTester");
};

/** \relates RefrigerationDefrostCycleParametersTester*/
typedef boost::optional<RefrigerationDefrostCycleParametersTester> OptionalRefrigerationDefrostCycleParametersTester;

/** \relates RefrigerationDefrostCycleParametersTester*/
typedef std::vector<RefrigerationDefrostCycleParametersTester> RefrigerationDefrostCycleParametersTesterVector;

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONDEFROSTCYCLEPARAMETERSTESTER_HPP

