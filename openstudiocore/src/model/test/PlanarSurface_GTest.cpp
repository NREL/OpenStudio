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

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../PlanarSurface.hpp"
#include "../PlanarSurface_Impl.hpp"

#include "../../utilities/units/QuantityFactory.hpp"
#include "../../utilities/units/QuantityConverter.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, PlanarSurface_FilmResistanceConversions)
{
  Quantity q;
  OptionalQuantity qc;

  q = createQuantity(0.61,"ft^2*R*h/Btu").get();
  qc = QuantityConverter::instance().convert(q,UnitSystem(UnitSystem::SI));
  ASSERT_TRUE(qc);
  EXPECT_EQ("s^3*K/kg",qc->standardUnitsString());
  EXPECT_NEAR(qc->value(),PlanarSurface::filmResistance(FilmResistanceType::StillAir_HorizontalSurface_HeatFlowsUpward),1.0E-8);

  q = createQuantity(0.62,"ft^2*R*h/Btu").get();
  qc = QuantityConverter::instance().convert(q,UnitSystem(UnitSystem::SI));
  ASSERT_TRUE(qc);
  EXPECT_EQ("s^3*K/kg",qc->standardUnitsString());
  EXPECT_NEAR(qc->value(),PlanarSurface::filmResistance(FilmResistanceType::StillAir_45DegreeSurface_HeatFlowsUpward),1.0E-8);

  q = createQuantity(0.68,"ft^2*R*h/Btu").get();
  qc = QuantityConverter::instance().convert(q,UnitSystem(UnitSystem::SI));
  ASSERT_TRUE(qc);
  EXPECT_EQ("s^3*K/kg",qc->standardUnitsString());
  EXPECT_NEAR(qc->value(),PlanarSurface::filmResistance(FilmResistanceType::StillAir_VerticalSurface),1.0E-8);

  q = createQuantity(0.76,"ft^2*R*h/Btu").get();
  qc = QuantityConverter::instance().convert(q,UnitSystem(UnitSystem::SI));
  ASSERT_TRUE(qc);
  EXPECT_EQ("s^3*K/kg",qc->standardUnitsString());
  EXPECT_NEAR(qc->value(),PlanarSurface::filmResistance(FilmResistanceType::StillAir_45DegreeSurface_HeatFlowsDownward),1.0E-8);

  q = createQuantity(0.92,"ft^2*R*h/Btu").get();
  qc = QuantityConverter::instance().convert(q,UnitSystem(UnitSystem::SI));
  ASSERT_TRUE(qc);
  EXPECT_EQ("s^3*K/kg",qc->standardUnitsString());
  EXPECT_NEAR(qc->value(),PlanarSurface::filmResistance(FilmResistanceType::StillAir_HorizontalSurface_HeatFlowsDownward),1.0E-8);

  q = createQuantity(0.17,"ft^2*R*h/Btu").get();
  qc = QuantityConverter::instance().convert(q,UnitSystem(UnitSystem::SI));
  ASSERT_TRUE(qc);
  EXPECT_EQ("s^3*K/kg",qc->standardUnitsString());
  EXPECT_NEAR(qc->value(),PlanarSurface::filmResistance(FilmResistanceType::MovingAir_15mph),1.0E-8);

  q = createQuantity(0.25,"ft^2*R*h/Btu").get();
  qc = QuantityConverter::instance().convert(q,UnitSystem(UnitSystem::SI));
  ASSERT_TRUE(qc);
  EXPECT_EQ("s^3*K/kg",qc->standardUnitsString());
  EXPECT_NEAR(qc->value(),PlanarSurface::filmResistance(FilmResistanceType::MovingAir_7p5mph),1.0E-8);
}

