/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../Construction.hpp"
#include "../Construction_Impl.hpp"
#include "../DefaultSubSurfaceConstructions.hpp"
#include "../DefaultSubSurfaceConstructions_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, DefaultSubSurfaceConstructions) {
  Model model;

  DefaultSubSurfaceConstructions defaultSubSurfaceConstructions(model);
  Construction construction(model);

  EXPECT_FALSE(defaultSubSurfaceConstructions.fixedWindowConstruction());
  EXPECT_TRUE(defaultSubSurfaceConstructions.setFixedWindowConstruction(construction));
  ASSERT_TRUE(defaultSubSurfaceConstructions.fixedWindowConstruction());
  EXPECT_EQ(construction.handle(), defaultSubSurfaceConstructions.fixedWindowConstruction()->handle());
  defaultSubSurfaceConstructions.resetFixedWindowConstruction();
  EXPECT_FALSE(defaultSubSurfaceConstructions.fixedWindowConstruction());

  EXPECT_FALSE(defaultSubSurfaceConstructions.operableWindowConstruction());
  EXPECT_TRUE(defaultSubSurfaceConstructions.setOperableWindowConstruction(construction));
  ASSERT_TRUE(defaultSubSurfaceConstructions.operableWindowConstruction());
  EXPECT_EQ(construction.handle(), defaultSubSurfaceConstructions.operableWindowConstruction()->handle());
  defaultSubSurfaceConstructions.resetOperableWindowConstruction();
  EXPECT_FALSE(defaultSubSurfaceConstructions.operableWindowConstruction());

  EXPECT_FALSE(defaultSubSurfaceConstructions.doorConstruction());
  EXPECT_TRUE(defaultSubSurfaceConstructions.setDoorConstruction(construction));
  ASSERT_TRUE(defaultSubSurfaceConstructions.doorConstruction());
  EXPECT_EQ(construction.handle(), defaultSubSurfaceConstructions.doorConstruction()->handle());
  defaultSubSurfaceConstructions.resetDoorConstruction();
  EXPECT_FALSE(defaultSubSurfaceConstructions.doorConstruction());

  EXPECT_FALSE(defaultSubSurfaceConstructions.glassDoorConstruction());
  EXPECT_TRUE(defaultSubSurfaceConstructions.setGlassDoorConstruction(construction));
  ASSERT_TRUE(defaultSubSurfaceConstructions.glassDoorConstruction());
  EXPECT_EQ(construction.handle(), defaultSubSurfaceConstructions.glassDoorConstruction()->handle());
  defaultSubSurfaceConstructions.resetGlassDoorConstruction();
  EXPECT_FALSE(defaultSubSurfaceConstructions.glassDoorConstruction());

  EXPECT_FALSE(defaultSubSurfaceConstructions.overheadDoorConstruction());
  EXPECT_TRUE(defaultSubSurfaceConstructions.setOverheadDoorConstruction(construction));
  ASSERT_TRUE(defaultSubSurfaceConstructions.overheadDoorConstruction());
  EXPECT_EQ(construction.handle(), defaultSubSurfaceConstructions.overheadDoorConstruction()->handle());
  defaultSubSurfaceConstructions.resetOverheadDoorConstruction();
  EXPECT_FALSE(defaultSubSurfaceConstructions.overheadDoorConstruction());

  EXPECT_FALSE(defaultSubSurfaceConstructions.skylightConstruction());
  EXPECT_TRUE(defaultSubSurfaceConstructions.setSkylightConstruction(construction));
  ASSERT_TRUE(defaultSubSurfaceConstructions.skylightConstruction());
  EXPECT_EQ(construction.handle(), defaultSubSurfaceConstructions.skylightConstruction()->handle());
  defaultSubSurfaceConstructions.resetSkylightConstruction();
  EXPECT_FALSE(defaultSubSurfaceConstructions.skylightConstruction());

  EXPECT_FALSE(defaultSubSurfaceConstructions.tubularDaylightDomeConstruction());
  EXPECT_TRUE(defaultSubSurfaceConstructions.setTubularDaylightDomeConstruction(construction));
  ASSERT_TRUE(defaultSubSurfaceConstructions.tubularDaylightDomeConstruction());
  EXPECT_EQ(construction.handle(), defaultSubSurfaceConstructions.tubularDaylightDomeConstruction()->handle());
  defaultSubSurfaceConstructions.resetTubularDaylightDomeConstruction();
  EXPECT_FALSE(defaultSubSurfaceConstructions.tubularDaylightDomeConstruction());

  EXPECT_FALSE(defaultSubSurfaceConstructions.tubularDaylightDiffuserConstruction());
  EXPECT_TRUE(defaultSubSurfaceConstructions.setTubularDaylightDiffuserConstruction(construction));
  ASSERT_TRUE(defaultSubSurfaceConstructions.tubularDaylightDiffuserConstruction());
  EXPECT_EQ(construction.handle(), defaultSubSurfaceConstructions.tubularDaylightDiffuserConstruction()->handle());
  defaultSubSurfaceConstructions.resetTubularDaylightDiffuserConstruction();
  EXPECT_FALSE(defaultSubSurfaceConstructions.tubularDaylightDiffuserConstruction());
}
