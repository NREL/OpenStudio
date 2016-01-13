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
#include "../CoilCoolingDXMultiSpeed.hpp"
#include "../CoilCoolingDXMultiSpeed_Impl.hpp"
#include "../CoilCoolingDXMultiSpeedStageData.hpp"
#include "../CoilCoolingDXMultiSpeedStageData_Impl.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveBiquadratic_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

CoilCoolingDXMultiSpeedStageData makeStage(Model & model) {
  CurveBiquadratic cooling_curve_1(model);
  cooling_curve_1.setCoefficient1Constant(0.766956);
  cooling_curve_1.setCoefficient2x(0.0107756);
  cooling_curve_1.setCoefficient3xPOW2(-0.0000414703);
  cooling_curve_1.setCoefficient4y(0.00134961);
  cooling_curve_1.setCoefficient5yPOW2(-0.000261144);
  cooling_curve_1.setCoefficient6xTIMESY(0.000457488);
  cooling_curve_1.setMinimumValueofx(17.0);
  cooling_curve_1.setMaximumValueofx(22.0);
  cooling_curve_1.setMinimumValueofy(13.0);
  cooling_curve_1.setMaximumValueofy(46.0);
  
  CurveQuadratic cooling_curve_2(model);
  cooling_curve_2.setCoefficient1Constant(0.8);
  cooling_curve_2.setCoefficient2x(0.2);
  cooling_curve_2.setCoefficient3xPOW2(0.0);
  cooling_curve_2.setMinimumValueofx(0.5);
  cooling_curve_2.setMaximumValueofx(1.5);
  
  CurveBiquadratic cooling_curve_3(model);
  cooling_curve_3.setCoefficient1Constant(0.297145);
  cooling_curve_3.setCoefficient2x(0.0430933);
  cooling_curve_3.setCoefficient3xPOW2(-0.000748766);
  cooling_curve_3.setCoefficient4y(0.00597727);
  cooling_curve_3.setCoefficient5yPOW2(0.000482112);
  cooling_curve_3.setCoefficient6xTIMESY(-0.000956448);
  cooling_curve_3.setMinimumValueofx(17.0);
  cooling_curve_3.setMaximumValueofx(22.0);
  cooling_curve_3.setMinimumValueofy(13.0);
  cooling_curve_3.setMaximumValueofy(46.0);
  
  CurveQuadratic cooling_curve_4(model);
  cooling_curve_4.setCoefficient1Constant(1.156);
  cooling_curve_4.setCoefficient2x(-0.1816);
  cooling_curve_4.setCoefficient3xPOW2(0.0256);
  cooling_curve_4.setMinimumValueofx(0.5);
  cooling_curve_4.setMaximumValueofx(1.5);
  
  CurveQuadratic cooling_curve_5(model);
  cooling_curve_5.setCoefficient1Constant(0.75);
  cooling_curve_5.setCoefficient2x(0.25);
  cooling_curve_5.setCoefficient3xPOW2(0.0);
  cooling_curve_5.setMinimumValueofx(0.0);
  cooling_curve_5.setMaximumValueofx(1.0);
  
  CurveBiquadratic cooling_curve_6(model);
  cooling_curve_6.setCoefficient1Constant(1.0);
  cooling_curve_6.setCoefficient2x(0.0);
  cooling_curve_6.setCoefficient3xPOW2(0.0);
  cooling_curve_6.setCoefficient4y(0.0);
  cooling_curve_6.setCoefficient5yPOW2(0.0);
  cooling_curve_6.setCoefficient6xTIMESY(0.0);
  cooling_curve_6.setMinimumValueofx(0.0);
  cooling_curve_6.setMaximumValueofx(0.0);
  cooling_curve_6.setMinimumValueofy(0.0);
  cooling_curve_6.setMaximumValueofy(0.0);

  CoilCoolingDXMultiSpeedStageData stage(model,
    cooling_curve_1,
    cooling_curve_2,
    cooling_curve_3,
    cooling_curve_4,
    cooling_curve_5,
    cooling_curve_6);

  return stage;
}

TEST_F(ModelFixture, CoilCoolingDXMultiSpeed_DefaultConstructors)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model m;
    CoilCoolingDXMultiSpeed coil(m);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, CoilCoolingDXMultiSpeed_Stages)
{
  Model m;
  CoilCoolingDXMultiSpeed coil(m);

  auto stage1 = makeStage(m);
  coil.addStage(stage1);

  auto stage2 = makeStage(m);
  coil.addStage(stage2);

  ASSERT_EQ(2u,coil.stages().size());
  
}


