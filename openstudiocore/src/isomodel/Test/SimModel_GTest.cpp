/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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
#include <isomodel/Test/ISOModelFixture.hpp>
#include <isomodel/SimModel.hpp>
#include <isomodel/UserModel.hpp>
#include <resources.hxx>
#include <sstream>

using namespace openstudio::isomodel;
using namespace openstudio;

void testGenericFunctions() {
  double scalar = 2;
  double test[] = {0,1,2,3,4,5,6,7,8,9,10,11};
  Vector vTest = Vector(12), vTest2 = Vector(12);
  for(unsigned int i = 0;i<vTest.size();i++){
    vTest[i] = i;
    vTest2[i] = 11-i;
  }
  for(unsigned int i = 0;i<12;i++){
    EXPECT_EQ(i, test[i]);
    EXPECT_EQ(i, vTest[i]);
    EXPECT_EQ(11 - i, vTest2[i]);
  }


  Vector results = mult(test,scalar,12);
  for(unsigned int i = 0;i<12;i++){
    EXPECT_EQ(i*scalar, results[i]);
  }

  results = mult(vTest, scalar);
  for(unsigned int i = 0;i<vTest.size();i++){
    EXPECT_EQ(i * scalar, results[i]);
  }

  results = mult(vTest, test);
  for(unsigned int i = 0;i<vTest.size();i++){
    EXPECT_EQ(i * i, results[i]);
  }

  results = mult(vTest, vTest);
  for(unsigned int i = 0;i<vTest.size();i++){
    EXPECT_EQ(i * i, results[i]);
  }

  results = div(vTest, scalar);
  for(unsigned int i = 0;i<vTest.size();i++){
    EXPECT_EQ(i / scalar, results[i]);
  }

  results = div(scalar, vTest);
  EXPECT_EQ(std::numeric_limits<double>::max(), results[0]);
  for(unsigned int i = 1;i<vTest.size();i++){
    EXPECT_EQ(scalar / i, results[i]);
  }

  results = div(vTest, vTest);
  EXPECT_EQ(std::numeric_limits<double>::max(), results[0]);  //0 / 0
  for(unsigned int i = 1;i<vTest.size();i++){
    EXPECT_EQ(1, results[i]);
  }

  results = sum(vTest, vTest);
  for(unsigned int i = 0;i<vTest.size();i++){
    EXPECT_EQ(i + i, results[i]);
  }

  results = sum(vTest, scalar);
  for(unsigned int i = 0;i<vTest.size();i++){
    EXPECT_EQ(i + scalar, results[i]);
  }

  results = dif(vTest, vTest);
  for(unsigned int i = 0;i<vTest.size();i++){
    EXPECT_EQ(0, results[i]);
  }

  results = dif(vTest, scalar);
  for(unsigned int i = 0;i<vTest.size();i++){
    EXPECT_EQ(i - scalar, results[i]);
  }

  results = dif(scalar, vTest);
  for(unsigned int i = 0;i<vTest.size();i++){
    EXPECT_EQ(scalar - i, results[i]);
  }

  EXPECT_EQ(11, openstudio::isomodel::maximum(vTest));

  results = maximum(vTest,vTest2);
  EXPECT_EQ(11, results[0]);
  EXPECT_EQ(10, results[1]);
  EXPECT_EQ(9, results[2]);
  EXPECT_EQ(8, results[3]);
  EXPECT_EQ(7, results[4]);
  EXPECT_EQ(6, results[5]);
  EXPECT_EQ(6, results[6]);
  EXPECT_EQ(7, results[7]);
  EXPECT_EQ(8, results[8]);
  EXPECT_EQ(9, results[9]);
  EXPECT_EQ(10, results[10]);
  EXPECT_EQ(11, results[11]);

  for(unsigned int i = 0;i<12;i++){
    EXPECT_EQ(11-i, vTest2[i]);
  }

  results = maximum(vTest2,10);
  EXPECT_EQ(11, results[0]);  
  for(unsigned int i = 1;i<vTest2.size();i++){
    EXPECT_EQ(10, results[i]);
  }
  
  EXPECT_EQ(0, openstudio::isomodel::minimum(vTest));

  results = minimum(vTest,1);
  EXPECT_EQ(0, results[0]);  
  for(unsigned int i = 1;i<vTest.size();i++){
    EXPECT_EQ(1, results[i]);
  }
  
  results = pow(vTest, 3);
  for(unsigned int i = 0;i<vTest.size();i++){
    EXPECT_EQ(i*i*i, results[i]);
  }
  for(unsigned int i = 0;i<vTest.size();i++){
    if(i%2==0){
      vTest[i] = -1 * vTest[i];
    }
  }
  results = abs(vTest);
  for(unsigned int i = 0;i<vTest.size();i++){
    EXPECT_EQ(i, results[i]);
  }
  
}
TEST_F(ISOModelFixture, SimModel)
{
  //testGenericFunctions();
  UserModel userModel;
  userModel.load(openstudio::toString(resourcesPath() / openstudio::toPath("isomodel/exampleModel.ISO")));
  ASSERT_TRUE(userModel.valid());
  SimModel simModel = userModel.toSimModel();    
  ISOResults results = simModel.simulate();

  
double testVal = 0;
  
EXPECT_EQ(0, testVal = results.monthlyResults[0].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Heating) );
EXPECT_EQ(0, testVal = results.monthlyResults[1].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Heating) );
EXPECT_EQ(0, testVal = results.monthlyResults[2].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Heating) );
EXPECT_EQ(0, testVal = results.monthlyResults[3].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Heating) );
EXPECT_EQ(0, testVal = results.monthlyResults[4].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Heating) );
EXPECT_EQ(0, testVal = results.monthlyResults[5].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Heating) );
EXPECT_EQ(0, testVal = results.monthlyResults[6].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Heating) );
EXPECT_EQ(0, testVal = results.monthlyResults[7].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Heating) );
EXPECT_EQ(0, testVal = results.monthlyResults[8].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Heating) );
EXPECT_EQ(0, testVal = results.monthlyResults[9].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Heating) );
EXPECT_EQ(0, testVal = results.monthlyResults[10].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Heating) );
EXPECT_EQ(0, testVal = results.monthlyResults[11].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Heating) );

EXPECT_EQ(0.49273338999072092, testVal = results.monthlyResults[0].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Cooling) );
EXPECT_EQ(0.69411004629154915, testVal = results.monthlyResults[1].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Cooling) );
EXPECT_EQ(1.7178541024054295, testVal = results.monthlyResults[2].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Cooling) );
EXPECT_EQ(2.8946145920389230, testVal = results.monthlyResults[3].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Cooling) );
EXPECT_EQ(4.3324984760862995, testVal = results.monthlyResults[4].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Cooling) );
EXPECT_EQ(5.1826700865996314, testVal = results.monthlyResults[5].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Cooling) );
EXPECT_EQ(5.8838601192395403, testVal = results.monthlyResults[6].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Cooling) );
EXPECT_EQ(5.3164859346724986, testVal = results.monthlyResults[7].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Cooling) );
EXPECT_EQ(4.4307064007522179, testVal = results.monthlyResults[8].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Cooling) );
EXPECT_EQ(2.9998841092154276, testVal = results.monthlyResults[9].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Cooling) );
EXPECT_EQ(1.4859095746027824, testVal = results.monthlyResults[10].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Cooling) );
EXPECT_EQ(0.54943277773709054, testVal = results.monthlyResults[11].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Cooling) );

EXPECT_EQ(3.0435906070795506, testVal = results.monthlyResults[0].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::InteriorLights) );
EXPECT_EQ(2.7490495805879811, testVal = results.monthlyResults[1].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::InteriorLights) );
EXPECT_EQ(3.0435906070795506, testVal = results.monthlyResults[2].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::InteriorLights) );
EXPECT_EQ(2.9454102649156932, testVal = results.monthlyResults[3].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::InteriorLights) );
EXPECT_EQ(3.0435906070795506, testVal = results.monthlyResults[4].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::InteriorLights) );
EXPECT_EQ(2.9454102649156932, testVal = results.monthlyResults[5].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::InteriorLights) );
EXPECT_EQ(3.0435906070795506, testVal = results.monthlyResults[6].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::InteriorLights) );
EXPECT_EQ(3.0435906070795506, testVal = results.monthlyResults[7].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::InteriorLights) );
EXPECT_EQ(2.9454102649156932, testVal = results.monthlyResults[8].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::InteriorLights) );
EXPECT_EQ(3.0435906070795506, testVal = results.monthlyResults[9].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::InteriorLights) );
EXPECT_EQ(2.9454102649156932, testVal = results.monthlyResults[10].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::InteriorLights) );
EXPECT_EQ(3.0435906070795506, testVal = results.monthlyResults[11].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::InteriorLights) );

EXPECT_EQ(0, testVal = results.monthlyResults[0].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::ExteriorLights) );
EXPECT_EQ(0, testVal = results.monthlyResults[1].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::ExteriorLights) );
EXPECT_EQ(0, testVal = results.monthlyResults[2].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::ExteriorLights) );
EXPECT_EQ(0, testVal = results.monthlyResults[3].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::ExteriorLights) );
EXPECT_EQ(0, testVal = results.monthlyResults[4].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::ExteriorLights) );
EXPECT_EQ(0, testVal = results.monthlyResults[5].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::ExteriorLights) );
EXPECT_EQ(0, testVal = results.monthlyResults[6].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::ExteriorLights) );
EXPECT_EQ(0, testVal = results.monthlyResults[7].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::ExteriorLights) );
EXPECT_EQ(0, testVal = results.monthlyResults[8].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::ExteriorLights) );
EXPECT_EQ(0, testVal = results.monthlyResults[9].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::ExteriorLights) );
EXPECT_EQ(0, testVal = results.monthlyResults[10].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::ExteriorLights) );
EXPECT_EQ(0, testVal = results.monthlyResults[11].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::ExteriorLights) );

EXPECT_EQ(0.67548764212692503, testVal = results.monthlyResults[0].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Fans) );
EXPECT_EQ(0.71337130590097453, testVal = results.monthlyResults[1].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Fans) );
EXPECT_EQ(1.4963236211423268, testVal = results.monthlyResults[2].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Fans) );
EXPECT_EQ(2.5015482460471317, testVal = results.monthlyResults[3].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Fans) );
EXPECT_EQ(3.7437833897807606, testVal = results.monthlyResults[4].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Fans) );
EXPECT_EQ(4.4784293814470111, testVal = results.monthlyResults[5].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Fans) );
EXPECT_EQ(5.0843390750178070, testVal = results.monthlyResults[6].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Fans) );
EXPECT_EQ(4.5940618287401582, testVal = results.monthlyResults[7].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Fans) );
EXPECT_EQ(3.8286453756945233, testVal = results.monthlyResults[8].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Fans) );
EXPECT_EQ(2.5924146291517078, testVal = results.monthlyResults[9].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Fans) );
EXPECT_EQ(1.2960680791967272, testVal = results.monthlyResults[10].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Fans) );
EXPECT_EQ(0.66600981342347976, testVal = results.monthlyResults[11].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Fans) );

EXPECT_EQ(0.093419352275004267, testVal = results.monthlyResults[0].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Pumps) );
EXPECT_EQ(0.098658629962501562, testVal = results.monthlyResults[1].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Pumps) );
EXPECT_EQ(0.20694025288273024, testVal = results.monthlyResults[2].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Pumps) );
EXPECT_EQ(0.34596194253763241, testVal = results.monthlyResults[3].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Pumps) );
EXPECT_EQ(0.51776198041165877, testVal = results.monthlyResults[4].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Pumps) );
EXPECT_EQ(0.61936288087638347, testVal = results.monthlyResults[5].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Pumps) );
EXPECT_EQ(0.70315966349745485, testVal = results.monthlyResults[6].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Pumps) );
EXPECT_EQ(0.63535474757299071, testVal = results.monthlyResults[7].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Pumps) );
EXPECT_EQ(0.52949831911338807, testVal = results.monthlyResults[8].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Pumps) );
EXPECT_EQ(0.35852868413851996, testVal = results.monthlyResults[9].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Pumps) );
EXPECT_EQ(0.17924508593765875, testVal = results.monthlyResults[10].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Pumps) );
EXPECT_EQ(0.092108576824455141, testVal = results.monthlyResults[11].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Pumps) );

EXPECT_EQ(2.7583969507693009, testVal = results.monthlyResults[0].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::InteriorEquipment) );
EXPECT_EQ(2.4914553103722721, testVal = results.monthlyResults[1].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::InteriorEquipment) );
EXPECT_EQ(2.7583969507693009, testVal = results.monthlyResults[2].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::InteriorEquipment) );
EXPECT_EQ(2.6694164039702915, testVal = results.monthlyResults[3].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::InteriorEquipment) );
EXPECT_EQ(2.7583969507693009, testVal = results.monthlyResults[4].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::InteriorEquipment) );
EXPECT_EQ(2.6694164039702915, testVal = results.monthlyResults[5].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::InteriorEquipment) );
EXPECT_EQ(2.7583969507693009, testVal = results.monthlyResults[6].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::InteriorEquipment) );
EXPECT_EQ(2.7583969507693009, testVal = results.monthlyResults[7].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::InteriorEquipment) );
EXPECT_EQ(2.6694164039702915, testVal = results.monthlyResults[8].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::InteriorEquipment) );
EXPECT_EQ(2.7583969507693009, testVal = results.monthlyResults[9].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::InteriorEquipment) );
EXPECT_EQ(2.6694164039702915, testVal = results.monthlyResults[10].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::InteriorEquipment) );
EXPECT_EQ(2.7583969507693009, testVal = results.monthlyResults[11].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::InteriorEquipment) );

EXPECT_EQ(1.3545742640190246, testVal = results.monthlyResults[0].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating) );
EXPECT_EQ(0.61612557000190027, testVal = results.monthlyResults[1].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating) );
EXPECT_EQ(0.064542074240392697, testVal = results.monthlyResults[2].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating) );
EXPECT_EQ(0.0014362890514717094, testVal = results.monthlyResults[3].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating) );
EXPECT_EQ(5.9814022171029652e-006, testVal = results.monthlyResults[4].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating) );
EXPECT_EQ(9.4272286303841226e-011, testVal = results.monthlyResults[5].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating) );
EXPECT_EQ(0, testVal = results.monthlyResults[6].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating) );
EXPECT_EQ(6.3970953687061980e-012, testVal = results.monthlyResults[7].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating) );
EXPECT_EQ(1.4150427251524848e-007, testVal = results.monthlyResults[8].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating) );
EXPECT_EQ(0.00090079185025864915, testVal = results.monthlyResults[9].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating) );
EXPECT_EQ(0.065472347285332952, testVal = results.monthlyResults[10].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating) );
EXPECT_EQ(1.0373822892817233, testVal = results.monthlyResults[11].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating) );

EXPECT_EQ(0, testVal = results.monthlyResults[0].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::WaterSystems) );
EXPECT_EQ(0, testVal = results.monthlyResults[1].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::WaterSystems) );
EXPECT_EQ(0, testVal = results.monthlyResults[2].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::WaterSystems) );
EXPECT_EQ(0, testVal = results.monthlyResults[3].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::WaterSystems) );
EXPECT_EQ(0, testVal = results.monthlyResults[4].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::WaterSystems) );
EXPECT_EQ(0, testVal = results.monthlyResults[5].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::WaterSystems) );
EXPECT_EQ(0, testVal = results.monthlyResults[6].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::WaterSystems) );
EXPECT_EQ(0, testVal = results.monthlyResults[7].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::WaterSystems) );
EXPECT_EQ(0, testVal = results.monthlyResults[8].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::WaterSystems) );
EXPECT_EQ(0, testVal = results.monthlyResults[9].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::WaterSystems) );
EXPECT_EQ(0, testVal = results.monthlyResults[10].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::WaterSystems) );
EXPECT_EQ(0, testVal = results.monthlyResults[11].getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::WaterSystems) );


EXPECT_EQ(0, testVal = results.monthlyResults[0].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Cooling) );
EXPECT_EQ(0, testVal = results.monthlyResults[1].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Cooling) );
EXPECT_EQ(0, testVal = results.monthlyResults[2].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Cooling) );
EXPECT_EQ(0, testVal = results.monthlyResults[3].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Cooling) );
EXPECT_EQ(0, testVal = results.monthlyResults[4].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Cooling) );
EXPECT_EQ(0, testVal = results.monthlyResults[5].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Cooling) );
EXPECT_EQ(0, testVal = results.monthlyResults[6].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Cooling) );
EXPECT_EQ(0, testVal = results.monthlyResults[7].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Cooling) );
EXPECT_EQ(0, testVal = results.monthlyResults[8].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Cooling) );
EXPECT_EQ(0, testVal = results.monthlyResults[9].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Cooling) );
EXPECT_EQ(0, testVal = results.monthlyResults[10].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Cooling) );
EXPECT_EQ(0, testVal = results.monthlyResults[11].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Cooling) );

EXPECT_EQ(8.0840634632175608, testVal = results.monthlyResults[0].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::InteriorEquipment) );
EXPECT_EQ(7.3017347409706996, testVal = results.monthlyResults[1].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::InteriorEquipment) );
EXPECT_EQ(8.0840634632175608, testVal = results.monthlyResults[2].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::InteriorEquipment) );
EXPECT_EQ(7.8232872224686067, testVal = results.monthlyResults[3].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::InteriorEquipment) );
EXPECT_EQ(8.0840634632175608, testVal = results.monthlyResults[4].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::InteriorEquipment) );
EXPECT_EQ(7.8232872224686067, testVal = results.monthlyResults[5].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::InteriorEquipment) );
EXPECT_EQ(8.0840634632175608, testVal = results.monthlyResults[6].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::InteriorEquipment) );
EXPECT_EQ(8.0840634632175608, testVal = results.monthlyResults[7].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::InteriorEquipment) );
EXPECT_EQ(7.8232872224686067, testVal = results.monthlyResults[8].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::InteriorEquipment) );
EXPECT_EQ(8.0840634632175608, testVal = results.monthlyResults[9].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::InteriorEquipment) );
EXPECT_EQ(7.8232872224686067, testVal = results.monthlyResults[10].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::InteriorEquipment) );
EXPECT_EQ(8.0840634632175608, testVal = results.monthlyResults[11].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::InteriorEquipment) );

EXPECT_EQ(0, testVal = results.monthlyResults[0].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::WaterSystems) );
EXPECT_EQ(0, testVal = results.monthlyResults[1].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::WaterSystems) );
EXPECT_EQ(0, testVal = results.monthlyResults[2].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::WaterSystems) );
EXPECT_EQ(0, testVal = results.monthlyResults[3].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::WaterSystems) );
EXPECT_EQ(0, testVal = results.monthlyResults[4].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::WaterSystems) );
EXPECT_EQ(0, testVal = results.monthlyResults[5].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::WaterSystems) );
EXPECT_EQ(0, testVal = results.monthlyResults[6].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::WaterSystems) );
EXPECT_EQ(0, testVal = results.monthlyResults[7].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::WaterSystems) );
EXPECT_EQ(0, testVal = results.monthlyResults[8].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::WaterSystems) );
EXPECT_EQ(0, testVal = results.monthlyResults[9].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::WaterSystems) );
EXPECT_EQ(0, testVal = results.monthlyResults[10].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::WaterSystems) );
EXPECT_EQ(0, testVal = results.monthlyResults[11].getEndUse(EndUseFuelType::Gas, EndUseCategoryType::WaterSystems) );
}
