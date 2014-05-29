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

TEST_F(UnitsFixture,QuantityFactory_ReturnQuantityObjects) {
  LOG(Debug,"QuantityFactory_ReturnQuantityObjects");

  Quantity l1 = createQuantity(32.0,"m").get();
  //ASSERT_THROW(createIPQuantity(32.0,"m"),Exception);
  testStreamOutput("32 m",l1);

  Quantity f1 = createQuantity(-70.1,"lb_f").get();
  //ASSERT_THROW(createSIQuantity(-70.1,"lb_f"),Exception);
  testStreamOutput("-70.1 lb_f",f1);

  Quantity P1 = createQuantity(300.0,"kBtu/h").get();
  //ASSERT_THROW(createIPQuantity(300,"Btu/h"),Exception);
  EXPECT_EQ(3,P1.scale().exponent);
  EXPECT_EQ(1,P1.baseUnitExponent("Btu"));

  Quantity m1 = createQuantity(23.522,"ft*m").get();
  EXPECT_EQ(1,m1.baseUnitExponent("ft"));
  EXPECT_EQ(1,m1.baseUnitExponent("m"));
  testStreamOutput("23.522 ft*m",m1);
}
