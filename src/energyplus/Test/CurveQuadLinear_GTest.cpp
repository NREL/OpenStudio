/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/CurveQuadLinear.hpp"
#include "../../model/CurveQuadLinear_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/Curve_QuadLinear_FieldEnums.hxx>
#include <utilities/idd/GlobalGeometryRules_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CurveQuadLinear) {
  Model m;
  CurveQuadLinear c(m);

  EXPECT_TRUE(c.setCoefficient1Constant(1.0));
  EXPECT_TRUE(c.setCoefficient2w(2.0));
  EXPECT_TRUE(c.setCoefficient3x(3.0));
  EXPECT_TRUE(c.setCoefficient4y(4.0));
  EXPECT_TRUE(c.setCoefficient5z(5.0));

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

  EXPECT_TRUE(c.setInputUnitTypeforw("Temperature"));
  EXPECT_TRUE(c.setInputUnitTypeforx("VolumetricFlow"));
  EXPECT_TRUE(c.setInputUnitTypefory("MassFlow"));
  EXPECT_TRUE(c.setInputUnitTypeforz("Power"));

  ForwardTranslator ft;

  {
    Workspace w = ft.translateModel(m);
    WorkspaceObjectVector idfCurves(w.getObjectsByType(IddObjectType::Curve_QuadLinear));
    ASSERT_EQ(1u, idfCurves.size());
    WorkspaceObject idfCurve(idfCurves[0]);

    EXPECT_EQ("Curve Quad Linear 1", idfCurve.getString(Curve_QuadLinearFields::Name).get());
    EXPECT_EQ(1.0, idfCurve.getDouble(Curve_QuadLinearFields::Coefficient1Constant).get());
    EXPECT_EQ(2.0, idfCurve.getDouble(Curve_QuadLinearFields::Coefficient2w).get());
    EXPECT_EQ(3.0, idfCurve.getDouble(Curve_QuadLinearFields::Coefficient3x).get());
    EXPECT_EQ(4.0, idfCurve.getDouble(Curve_QuadLinearFields::Coefficient4y).get());
    EXPECT_EQ(5.0, idfCurve.getDouble(Curve_QuadLinearFields::Coefficient5z).get());
    EXPECT_EQ(-100.2, idfCurve.getDouble(Curve_QuadLinearFields::MinimumValueofw).get());
    EXPECT_EQ(100.2, idfCurve.getDouble(Curve_QuadLinearFields::MaximumValueofw).get());
    EXPECT_EQ(-100.3, idfCurve.getDouble(Curve_QuadLinearFields::MinimumValueofx).get());
    EXPECT_EQ(100.3, idfCurve.getDouble(Curve_QuadLinearFields::MaximumValueofx).get());
    EXPECT_EQ(0.5, idfCurve.getDouble(Curve_QuadLinearFields::MinimumValueofy).get());
    EXPECT_EQ(100.5, idfCurve.getDouble(Curve_QuadLinearFields::MaximumValueofy).get());
    EXPECT_EQ(0.0, idfCurve.getDouble(Curve_QuadLinearFields::MinimumValueofz).get());
    EXPECT_EQ(100.0, idfCurve.getDouble(Curve_QuadLinearFields::MaximumValueofz).get());
    EXPECT_EQ(0.05, idfCurve.getDouble(Curve_QuadLinearFields::MinimumCurveOutput).get());
    EXPECT_EQ(38.5, idfCurve.getDouble(Curve_QuadLinearFields::MaximumCurveOutput).get());
    EXPECT_EQ("Temperature", idfCurve.getString(Curve_QuadLinearFields::InputUnitTypeforw).get());
    EXPECT_EQ("VolumetricFlow", idfCurve.getString(Curve_QuadLinearFields::InputUnitTypeforx).get());
    EXPECT_EQ("MassFlow", idfCurve.getString(Curve_QuadLinearFields::InputUnitTypefory).get());
    EXPECT_EQ("Power", idfCurve.getString(Curve_QuadLinearFields::InputUnitTypeforz).get());
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_CurveQuadLinear) {

  ReverseTranslator reverseTranslator;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);
  OptionalWorkspaceObject _i_curve = w.addObject(IdfObject(IddObjectType::Curve_QuadLinear));
  ASSERT_TRUE(_i_curve);
  _i_curve->setName("Curve Quad Linear");

  EXPECT_TRUE(_i_curve->setString(Curve_QuadLinearFields::Name, "Curve Quad Linear 1"));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuadLinearFields::Coefficient1Constant, 1.0));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuadLinearFields::Coefficient2w, 2.0));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuadLinearFields::Coefficient3x, 3.0));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuadLinearFields::Coefficient4y, 4.0));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuadLinearFields::Coefficient5z, 5.0));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuadLinearFields::MinimumValueofw, -100.2));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuadLinearFields::MaximumValueofw, 100.2));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuadLinearFields::MinimumValueofx, -100.3));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuadLinearFields::MaximumValueofx, 100.3));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuadLinearFields::MinimumValueofy, 0.5));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuadLinearFields::MaximumValueofy, 100.5));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuadLinearFields::MinimumValueofz, 0.0));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuadLinearFields::MaximumValueofz, 100.0));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuadLinearFields::MinimumCurveOutput, 0.05));
  EXPECT_TRUE(_i_curve->setDouble(Curve_QuadLinearFields::MaximumCurveOutput, 38.5));
  EXPECT_TRUE(_i_curve->setString(Curve_QuadLinearFields::InputUnitTypeforw, "Temperature"));
  EXPECT_TRUE(_i_curve->setString(Curve_QuadLinearFields::InputUnitTypeforx, "VolumetricFlow"));
  EXPECT_TRUE(_i_curve->setString(Curve_QuadLinearFields::InputUnitTypefory, "MassFlow"));
  EXPECT_TRUE(_i_curve->setString(Curve_QuadLinearFields::InputUnitTypeforz, "Power"));

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

    auto curves = model.getConcreteModelObjects<openstudio::model::CurveQuadLinear>();
    ASSERT_EQ(static_cast<unsigned>(1), curves.size());
    auto curve = curves[0];

    EXPECT_EQ("Curve Quad Linear 1", curve.nameString());
    EXPECT_EQ(1.0, curve.coefficient1Constant());
    EXPECT_EQ(2.0, curve.coefficient2w());
    EXPECT_EQ(3.0, curve.coefficient3x());
    EXPECT_EQ(4.0, curve.coefficient4y());
    EXPECT_EQ(5.0, curve.coefficient5z());
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

    EXPECT_EQ("Temperature", curve.inputUnitTypeforw());
    EXPECT_EQ("VolumetricFlow", curve.inputUnitTypeforx());
    EXPECT_EQ("MassFlow", curve.inputUnitTypefory());
    EXPECT_EQ("Power", curve.inputUnitTypeforz());
  }
}
