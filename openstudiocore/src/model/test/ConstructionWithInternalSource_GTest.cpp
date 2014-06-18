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
#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../ConstructionWithInternalSource.hpp"
#include "../ConstructionWithInternalSource_Impl.hpp"

#include "../StandardOpaqueMaterial.hpp"
#include "../StandardOpaqueMaterial_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST(ConstructionWithInternalSource, DefaultConstructed)
{
  Model model;

  ConstructionWithInternalSource construction(model);
  EXPECT_EQ(0, construction.numLayers());
  EXPECT_EQ(0, construction.layers().size());
  EXPECT_EQ(1, construction.sourcePresentAfterLayerNumber());
  EXPECT_EQ(1, construction.temperatureCalculationRequestedAfterLayerNumber());
  EXPECT_EQ(1, construction.dimensionsForTheCTFCalculation());
  EXPECT_EQ(0.154, construction.tubeSpacing());
}

TEST(ConstructionWithInternalSource, FromLayers)
{
  Model model;

  // Create some materials
  StandardOpaqueMaterial exterior(model);
  StandardOpaqueMaterial interior(model);

  OpaqueMaterialVector layers;

  EXPECT_EQ(0, model.getModelObjects<ConstructionWithInternalSource>().size());

  EXPECT_EQ(0, layers.size());
  EXPECT_THROW((ConstructionWithInternalSource(layers)), std::exception);

  EXPECT_EQ(0, model.getModelObjects<ConstructionWithInternalSource>().size());
  
  layers.push_back(exterior);

  EXPECT_EQ(1u, layers.size());
  EXPECT_THROW((ConstructionWithInternalSource(layers)), std::exception);

  EXPECT_EQ(0, model.getModelObjects<ConstructionWithInternalSource>().size());

  layers.push_back(interior);

  EXPECT_EQ(2u, layers.size());
  ConstructionWithInternalSource construction(layers);
  EXPECT_EQ(2u, construction.numLayers());
  EXPECT_EQ(2u, construction.layers().size());
  EXPECT_EQ(1, construction.sourcePresentAfterLayerNumber());
  EXPECT_EQ(1, construction.temperatureCalculationRequestedAfterLayerNumber());
  EXPECT_EQ(1, construction.dimensionsForTheCTFCalculation());
  EXPECT_EQ(0.154, construction.tubeSpacing());

  // check that we can't mess up the construction
  EXPECT_FALSE(construction.setSourcePresentAfterLayerNumber(3));
  EXPECT_EQ(1, construction.sourcePresentAfterLayerNumber());
  EXPECT_FALSE(construction.setTemperatureCalculationRequestedAfterLayerNumber(3));
  EXPECT_EQ(1, construction.temperatureCalculationRequestedAfterLayerNumber());

  EXPECT_FALSE(construction.eraseLayer(1));
  EXPECT_EQ(2u, construction.numLayers());

  EXPECT_FALSE(construction.setLayers(MaterialVector()));
  EXPECT_EQ(2u, construction.numLayers());

  MaterialVector testLayers = construction.layers();
  ASSERT_EQ(static_cast<unsigned>(2),testLayers.size());
  EXPECT_TRUE(testLayers[0] == exterior);
  EXPECT_TRUE(testLayers[1] == interior);

  testLayers.push_back(interior);
  testLayers.push_back(exterior);

  EXPECT_TRUE(construction.setLayers(testLayers));
  EXPECT_EQ(4u, construction.numLayers());

  testLayers = construction.layers();
  ASSERT_EQ(static_cast<unsigned>(4),testLayers.size());
  EXPECT_TRUE(testLayers[0] == exterior);
  EXPECT_TRUE(testLayers[1] == interior);
  EXPECT_TRUE(testLayers[2] == interior);
  EXPECT_TRUE(testLayers[3] == exterior);

  EXPECT_TRUE(construction.setSourcePresentAfterLayerNumber(3));
  EXPECT_EQ(3, construction.sourcePresentAfterLayerNumber());
  EXPECT_TRUE(construction.setTemperatureCalculationRequestedAfterLayerNumber(3));
  EXPECT_EQ(3, construction.temperatureCalculationRequestedAfterLayerNumber());

  testLayers.clear();
  testLayers.push_back(interior);
  testLayers.push_back(exterior);

  EXPECT_TRUE(construction.setLayers(testLayers));
  EXPECT_EQ(2u, construction.numLayers());

  testLayers = construction.layers();
  ASSERT_EQ(static_cast<unsigned>(2),testLayers.size());
  EXPECT_TRUE(testLayers[0] == interior);
  EXPECT_TRUE(testLayers[1] == exterior);

  EXPECT_EQ(1, construction.sourcePresentAfterLayerNumber());
  EXPECT_EQ(1, construction.temperatureCalculationRequestedAfterLayerNumber());

  EXPECT_FALSE(construction.setSourcePresentAfterLayerNumber(3));
  EXPECT_EQ(1, construction.sourcePresentAfterLayerNumber());
  EXPECT_FALSE(construction.setTemperatureCalculationRequestedAfterLayerNumber(3));
  EXPECT_EQ(1, construction.temperatureCalculationRequestedAfterLayerNumber());
}

TEST(ConstructionWithInternalSource, ReverseConstructionWithInternalSource_1)
{
  Model model;

  // Create some materials
  StandardOpaqueMaterial exterior(model);

  OpaqueMaterialVector layers;
  layers.push_back(exterior);
  layers.push_back(exterior);

  ConstructionWithInternalSource construction(layers);

  EXPECT_EQ(1u, model.getModelObjects<ConstructionWithInternalSource>().size());

  ConstructionWithInternalSource construction2 = construction.reverseConstructionWithInternalSource();

  EXPECT_EQ(1u, model.getModelObjects<ConstructionWithInternalSource>().size());
  EXPECT_EQ(construction.handle(), construction2.handle());
}

TEST(ConstructionWithInternalSource, ReverseConstructionWithInternalSource_2)
{
  Model model;

  // Create some materials
  StandardOpaqueMaterial exterior(model);
  StandardOpaqueMaterial interior(model);

  OpaqueMaterialVector layers;
  layers.push_back(exterior);
  layers.push_back(interior);

  ConstructionWithInternalSource construction(layers);

  EXPECT_EQ(1u, model.getModelObjects<ConstructionWithInternalSource>().size());

  ConstructionWithInternalSource construction2 = construction.reverseConstructionWithInternalSource();

  EXPECT_EQ(2u, model.getModelObjects<ConstructionWithInternalSource>().size());
  EXPECT_NE(construction.handle(), construction2.handle());

  MaterialVector testLayers = construction2.layers();
  ASSERT_EQ(static_cast<unsigned>(2),testLayers.size());
  EXPECT_TRUE(testLayers[0] == interior);
  EXPECT_TRUE(testLayers[1] == exterior);

  ConstructionWithInternalSource construction3 = construction.reverseConstructionWithInternalSource();
  EXPECT_EQ(2u, model.getModelObjects<ConstructionWithInternalSource>().size());
  EXPECT_NE(construction.handle(), construction3.handle());
  EXPECT_EQ(construction2.handle(), construction3.handle());
}

