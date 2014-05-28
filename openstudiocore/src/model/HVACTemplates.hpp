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

#ifndef MODEL_HVACTEMPLATES_HPP
#define MODEL_HVACTEMPLATES_HPP

#include "ModelAPI.hpp"
#include <vector>

namespace openstudio {

namespace model {

class Loop;

class Model;

class ThermalZone;

MODEL_API void addSystemType1(Model & model, std::vector<ThermalZone> zones);

MODEL_API void addSystemType2(Model & model, std::vector<ThermalZone> zones);

MODEL_API Loop addSystemType3(Model & model);

MODEL_API Loop addSystemType4(Model & model);

MODEL_API Loop addSystemType5(Model & model);

MODEL_API Loop addSystemType6(Model & model);

MODEL_API Loop addSystemType7(Model & model);

MODEL_API Loop addSystemType8(Model & model);

MODEL_API Loop addSystemType9(Model & model);

MODEL_API Loop addSystemType10(Model & model);

} // model

} // openstudio

#endif // MODEL_HVACTEMPLATES_HPP
