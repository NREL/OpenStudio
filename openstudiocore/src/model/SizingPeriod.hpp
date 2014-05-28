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

#ifndef MODEL_SIZINGPERIOD_HPP
#define MODEL_SIZINGPERIOD_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {
namespace model {

namespace detail {
  class SizingPeriod_Impl;
} // detail

class MODEL_API SizingPeriod : public ParentObject {
 public:

  virtual ~SizingPeriod() {}

  // ensure that this object does not contain the date 2/29
  void ensureNoLeapDays();

 protected:

  typedef detail::SizingPeriod_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  SizingPeriod(IddObjectType type,const Model& model);

  // constructor
  explicit SizingPeriod(std::shared_ptr<detail::SizingPeriod_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.SizingPeriod");

};

/** \relates SizingPeriod */
typedef boost::optional<SizingPeriod> OptionalSizingPeriod;

/** \relates SizingPeriod */
typedef std::vector<SizingPeriod> SizingPeriodVector;

} // model
} // openstudio

#endif // MODEL_SIZINGPERIOD_HPP
