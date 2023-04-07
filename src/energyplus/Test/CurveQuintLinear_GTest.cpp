/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/CurveQuintLinear.hpp"
#include "../../model/CurveQuintLinear_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/Curve_QuintLinear_FieldEnums.hxx>
#include <utilities/idd/GlobalGeometryRules_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CurveQuintLinear) {
  Model m;
  CurveQuintLinear c(m);

  EXPECT_TRUE(c.setCoefficient1Constant(1.0));
  EXPECT_TRUE(c.setCoefficient2v(2.0));
  EXPECT_TRUE(c.setCoefficient3w(3.0));
  EXPECT_TRUE(c.setCoefficient4x(4.0));
  EXPECT_TRUE(c.setCoefficient5y(5.0));
  EXPECT_TRUE(c.setCoefficient6z(6.0));

  EXPECT_TRUE(c.setMinimumValueofv(-100.1));
  EXPECT_TRUE(c.setMaximumValueofv(100.1));

  EXPECT_TRUE(c.setMinimumValueofw(-100.2));
  EXPECT_TRUE(c.setMaximumValueofw(100.2));

  EXPECT_TRUE(c.setMinimumValueofx(-100.3));
  EXPECT_TRUE(c.setMaximumValueofx(100.3));

  EXPECT_TRUE(c.setMinimumValueofy(0.4));
  EXPECT_TRUE(c.setMaximumValueofy(100.4));

  EXPECT_TRUE(c.setMinimumValueofy(0.5));
  EXPECT_TRUE(c.setMaximumValueofy(100.5));

  EXPECT_TRUE(c.setMinimumCurveOutput(0.05));
  EXPECT_TRUE(c.setMaximumCurveOutput(38.5));

  EXPECT_TRUE(c.setInputUnitTypeforv("Dimensionless"));
  EXPECT_TRUE(c.setInputUnitTypeforw("Temperature"));
  EXPECT_TRUE(c.setInputUnitTypeforx("VolumetricFlow"));
  EXPECT_TRUE(c.setInputUnitTypefory("MassFlow"));
  EXPECT_TRUE(c.setInputUnitTypeforz("Power"));

  ForwardTranslator ft;

  {
    Workspace w = ft.translateModel(m);
    WorkspaceObjectVector idfCurves(w.getObjectsByType(IddObjectType::Curve_QuintLinear));
    ASSERT_EQ(1u, idfCurves.size());
    WorkspaceObject idfCurve(idfCurves[0]);

    EXPECT_EQ("Curve Quint Linear 1", idfCurve.getString(Curve_QuintLinearFields::Name).get());
    EXPECT_EQ(1.0, idfCurve.getDouble(Curve_QuintLinearFields::Coefficient1Constant).get());
    EXPECT_EQ(2.0, idfCurve.getDouble(Curve_QuintLinearFields::Coefficient2v).get());
    EXPECT_EQ(3.0, idfCurve.getDouble(Curve_QuintLinearFields::Coefficient3w).get());
    EXPECT_EQ(4.0, idfCurve.getDouble(Curve_QuintLinearFields::Coefficient4x).get());
    EXPECT_EQ(5.0, idfCurve.getDouble(Curve_QuintLinearFields::Coefficient5y).get());
    EXPECT_EQ(6.0, idfCurve.getDouble(Curve_QuintLinearFields::Coefficient6z).get());
    EXPECT_EQ(-100.1, idfCurve.getDouble(Curve_QuintLinearFields::MinimumValueofv).get());
    EXPECT_EQ(100.1, idfCurve.getDouble(Curve_QuintLinearFields::MaximumValueofv).get());
    EXPECT_EQ(-100.2, idfCurve.getDouble(Curve_QuintLinearFields::MinimumValueofw).get());
    EXPECT_EQ(100.2, idfCurve.getDouble(Curve_QuintLinearFields::MaximumValueofw).get());
    EXPECT_EQ(-100.3, idfCurve.getDouble(Curve_QuintLinearFields::MinimumValueofx).get());
    EXPECT_EQ(100.3, idfCurve.getDouble(Curve_QuintLinearFields::MaximumValueofx).get());
    EXPECT_EQ(0.5, idfCurve.getDouble(Curve_QuintLinearFields::MinimumValueofy).get());
    EXPECT_EQ(100.5, idfCurve.getDouble(Curve_QuintLinearFields::MaximumValueofy).get());
    EXPECT_EQ(0.0, idfCurve.getDouble(Curve_QuintLinearFields::MinimumValueofz).get());
    EXPECT_EQ(100.0, idfCurve.getDouble(Curve_QuintLinearFields::MaximumValueofz).get());
    EXPECT_EQ(0.05, idfCurve.getDouble(Curve_QuintLinearFields::MinimumCurveOutput).get());
    EXPECT_EQ(38.5, idfCurve.getDouble(Curve_QuintLinearFields::MaximumCurveOutput).get());
    EXPECT_EQ("Dimensionless", idfCurve.getString(Curve_QuintLinearFields::InputUnitTypeforv).get());
    EXPECT_EQ("Temperature", idfCurve.getString(Curve_QuintLinearFields::InputUnitTypeforw).get());
    EXPECT_EQ("VolumetricFlow", idfCurve.getString(Curve_QuintLinearFields::InputUnitTypeforx).get());
    EXPECT_EQ("MassFlow", idfCurve.getString(Curve_QuintLinearFields::InputUnitTypefory).get());
    EXPECT_EQ("Power", idfCurve.getString(Curve_QuintLinearFields::InputUnitTypeforz).get());
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_CurveQuintLinear) {

  ReverseTranslator reverseTranslator;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);
  OptionalWorkspaceObject _i_curve = w.addObject(IdfObject(IddObjectType::Curve_QuintLinear));
  ASSERT_TRUE(_i_curve);
  _i_curve->setName("Curve Quint Linear");

  EXPECT_TRUE(_i_curve->setString(Curve_QuintLinearFields::Name, "Curve Quint Linear 1"));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuintLinearFields::Coefficient1Constant, 1.0));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuintLinearFields::Coefficient2v, 2.0));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuintLinearFields::Coefficient3w, 3.0));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuintLinearFields::Coefficient4x, 4.0));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuintLinearFields::Coefficient5y, 5.0));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuintLinearFields::Coefficient6z, 6.0));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuintLinearFields::MinimumValueofv, -100.1));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuintLinearFields::MaximumValueofv, 100.1));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuintLinearFields::MinimumValueofw, -100.2));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuintLinearFields::MaximumValueofw, 100.2));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuintLinearFields::MinimumValueofx, -100.3));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuintLinearFields::MaximumValueofx, 100.3));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuintLinearFields::MinimumValueofy, 0.5));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuintLinearFields::MaximumValueofy, 100.5));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuintLinearFields::MinimumValueofz, 0.0));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuintLinearFields::MaximumValueofz, 100.0));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuintLinearFields::MinimumCurveOutput, 0.05));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuintLinearFields::MaximumCurveOutput, 38.5));
  EXPECT_TRUE(_i_curve->setString(Curve_QuintLinearFields::InputUnitTypeforv, "Dimensionless"));
  EXPECT_TRUE(_i_curve->setString(Curve_QuintLinearFields::InputUnitTypeforw, "Temperature"));
  EXPECT_TRUE(_i_curve->setString(Curve_QuintLinearFields::InputUnitTypeforx, "VolumetricFlow"));
  EXPECT_TRUE(_i_curve->setString(Curve_QuintLinearFields::InputUnitTypefory, "MassFlow"));
  EXPECT_TRUE(_i_curve->setString(Curve_QuintLinearFields::InputUnitTypeforz, "Power"));

  // To avoid other warnings, we add required objects
  OptionalWorkspaceObject _i_globalGeometryRules = w.addObject(IdfObject(IddObjectType::GlobalGeometryRules));
  ASSERT_TRUE(_i_globalGeometryRules);

  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::StartingVertexPosition, "UpperLeftCorner");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::VertexEntryDirection, "Counterclockwise");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::CoordinateSystem, "Relative");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::DaylightingReferencePointCoordinateSystem, "Relative");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::RectangularSurfaceCoordinateSystem, "Relative");

  OptionalWorkspaceObject _i_building = w.addObject(IdfObject(IddObjectType::Building));
  ASSERT_TRUE(_i_building);

  {
    ASSERT_NO_THROW(reverseTranslator.translateWorkspace(w));
    Model model = reverseTranslator.translateWorkspace(w);
    EXPECT_TRUE(reverseTranslator.errors().empty());
    EXPECT_TRUE(reverseTranslator.warnings().empty());

    auto curves = model.getConcreteModelObjects<openstudio::model::CurveQuintLinear>();
    ASSERT_EQ(static_cast<unsigned>(1), curves.size());
    auto curve = curves[0];

    EXPECT_EQ("Curve Quint Linear 1", curve.nameString());
    EXPECT_EQ(1.0, curve.coefficient1Constant());
    EXPECT_EQ(2.0, curve.coefficient2v());
    EXPECT_EQ(3.0, curve.coefficient3w());
    EXPECT_EQ(4.0, curve.coefficient4x());
    EXPECT_EQ(5.0, curve.coefficient5y());
    EXPECT_EQ(6.0, curve.coefficient6z());
    EXPECT_EQ(-100.1, curve.minimumValueofv());
    EXPECT_EQ(100.1, curve.maximumValueofv());
    EXPECT_EQ(-100.2, curve.minimumValueofw());
    EXPECT_EQ(100.2, curve.maximumValueofw());
    EXPECT_EQ(-100.3, curve.minimumValueofx());
    EXPECT_EQ(100.3, curve.maximumValueofx());
    EXPECT_EQ(0.5, curve.minimumValueofy());
    EXPECT_EQ(100.5, curve.maximumValueofy());
    EXPECT_EQ(0.0, curve.minimumValueofz());
    EXPECT_EQ(100.0, curve.maximumValueofz());

    ASSERT_TRUE(curve.minimumCurveOutput());
    EXPECT_EQ(0.05, curve.minimumCurveOutput().get());
    ASSERT_TRUE(curve.maximumCurveOutput());
    EXPECT_EQ(38.5, curve.maximumCurveOutput().get());

    EXPECT_EQ("Dimensionless", curve.inputUnitTypeforv());
    EXPECT_EQ("Temperature", curve.inputUnitTypeforw());
    EXPECT_EQ("VolumetricFlow", curve.inputUnitTypeforx());
    EXPECT_EQ("MassFlow", curve.inputUnitTypefory());
    EXPECT_EQ("Power", curve.inputUnitTypeforz());
  }
}
