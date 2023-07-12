/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../PlanarSurface.hpp"
#include "../PlanarSurface_Impl.hpp"

#include "../../utilities/units/QuantityFactory.hpp"
#include "../../utilities/units/QuantityConverter.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, PlanarSurface_FilmResistanceConversions) {
  Quantity q;
  OptionalQuantity qc;

  q = createQuantity(0.61, "ft^2*R*h/Btu").get();
  qc = QuantityConverter::instance().convert(q, UnitSystem(UnitSystem::SI));
  ASSERT_TRUE(qc);
  EXPECT_EQ("s^3*K/kg", qc->standardUnitsString());
  EXPECT_NEAR(qc->value(), PlanarSurface::filmResistance(FilmResistanceType::StillAir_HorizontalSurface_HeatFlowsUpward), 1.0E-8);

  q = createQuantity(0.62, "ft^2*R*h/Btu").get();
  qc = QuantityConverter::instance().convert(q, UnitSystem(UnitSystem::SI));
  ASSERT_TRUE(qc);
  EXPECT_EQ("s^3*K/kg", qc->standardUnitsString());
  EXPECT_NEAR(qc->value(), PlanarSurface::filmResistance(FilmResistanceType::StillAir_45DegreeSurface_HeatFlowsUpward), 1.0E-8);

  q = createQuantity(0.68, "ft^2*R*h/Btu").get();
  qc = QuantityConverter::instance().convert(q, UnitSystem(UnitSystem::SI));
  ASSERT_TRUE(qc);
  EXPECT_EQ("s^3*K/kg", qc->standardUnitsString());
  EXPECT_NEAR(qc->value(), PlanarSurface::filmResistance(FilmResistanceType::StillAir_VerticalSurface), 1.0E-8);

  q = createQuantity(0.76, "ft^2*R*h/Btu").get();
  qc = QuantityConverter::instance().convert(q, UnitSystem(UnitSystem::SI));
  ASSERT_TRUE(qc);
  EXPECT_EQ("s^3*K/kg", qc->standardUnitsString());
  EXPECT_NEAR(qc->value(), PlanarSurface::filmResistance(FilmResistanceType::StillAir_45DegreeSurface_HeatFlowsDownward), 1.0E-8);

  q = createQuantity(0.92, "ft^2*R*h/Btu").get();
  qc = QuantityConverter::instance().convert(q, UnitSystem(UnitSystem::SI));
  ASSERT_TRUE(qc);
  EXPECT_EQ("s^3*K/kg", qc->standardUnitsString());
  EXPECT_NEAR(qc->value(), PlanarSurface::filmResistance(FilmResistanceType::StillAir_HorizontalSurface_HeatFlowsDownward), 1.0E-8);

  q = createQuantity(0.17, "ft^2*R*h/Btu").get();
  qc = QuantityConverter::instance().convert(q, UnitSystem(UnitSystem::SI));
  ASSERT_TRUE(qc);
  EXPECT_EQ("s^3*K/kg", qc->standardUnitsString());
  EXPECT_NEAR(qc->value(), PlanarSurface::filmResistance(FilmResistanceType::MovingAir_15mph), 1.0E-8);

  q = createQuantity(0.25, "ft^2*R*h/Btu").get();
  qc = QuantityConverter::instance().convert(q, UnitSystem(UnitSystem::SI));
  ASSERT_TRUE(qc);
  EXPECT_EQ("s^3*K/kg", qc->standardUnitsString());
  EXPECT_NEAR(qc->value(), PlanarSurface::filmResistance(FilmResistanceType::MovingAir_7p5mph), 1.0E-8);
}
