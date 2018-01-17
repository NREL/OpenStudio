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

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../Curve.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveBiquadratic.hpp"
#include "../EnergyManagementSystemCurveOrTableIndexVariable.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, EMS_CurveOrTableIndexVariable)
{
  Model model;

  // Create a curve
  CurveBiquadratic c1(model);

  EnergyManagementSystemCurveOrTableIndexVariable emsCurve(model, c1);
  //emsCurve.setCurveorTableObject(c1);
  EXPECT_EQ(c1.handle(), emsCurve.curveOrTableObject().handle());
  model.save(toPath("./EMS_curvetest.osm"), true);
}
