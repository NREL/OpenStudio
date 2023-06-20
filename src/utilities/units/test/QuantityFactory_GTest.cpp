/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "UnitsFixture.hpp"

#include "../QuantityFactory.hpp"
#include "../ScaleFactory.hpp"

#include "../../core/Exception.hpp"

#include <boost/pointer_cast.hpp>

using openstudio::Unit;
using openstudio::Quantity;
using openstudio::QuantityFactory;
using openstudio::UnitFactory;
using openstudio::createQuantity;

using openstudio::Exception;

using std::shared_ptr;
using std::dynamic_pointer_cast;

TEST_F(UnitsFixture, QuantityFactory_ReturnQuantityObjects) {
  LOG(Debug, "QuantityFactory_ReturnQuantityObjects");

  Quantity l1 = createQuantity(32.0, "m").get();
  //ASSERT_THROW(createIPQuantity(32.0,"m"),Exception);
  testStreamOutput("32 m", l1);

  Quantity f1 = createQuantity(-70.1, "lb_f").get();
  //ASSERT_THROW(createSIQuantity(-70.1,"lb_f"),Exception);
  testStreamOutput("-70.1 lb_f", f1);

  Quantity P1 = createQuantity(300.0, "kBtu/h").get();
  //ASSERT_THROW(createIPQuantity(300,"Btu/h"),Exception);
  EXPECT_EQ(3, P1.scale().exponent);
  EXPECT_EQ(1, P1.baseUnitExponent("Btu"));

  Quantity m1 = createQuantity(23.522, "ft*m").get();
  EXPECT_EQ(1, m1.baseUnitExponent("ft"));
  EXPECT_EQ(1, m1.baseUnitExponent("m"));
  testStreamOutput("23.522 ft*m", m1);
}
