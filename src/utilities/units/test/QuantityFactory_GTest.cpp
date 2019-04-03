/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
