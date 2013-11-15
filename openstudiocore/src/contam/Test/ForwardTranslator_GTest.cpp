/**********************************************************************
*  Copyright (c) 2008-2010, Alliance for Sustainable Energy.
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
#include <contam/Test/ContamFixture.hpp>

#include <contam/ForwardTranslator.hpp>

#include <model/Model.hpp>

#include <resources.hxx>

#include <sstream>

using namespace openstudio;

TEST_F(ContamFixture, ForwardTranslator_exampleModel)
{
  model::Model model = model::exampleModel();

  path p = toPath("exampleModel.prj");
  path m = toPath("exampleModel.map");

  bool test = contam::ForwardTranslator::modelToPrj(model, p);

  //bool test = contam::ForwardTranslator::modelToContam(model, p, m);

  EXPECT_TRUE(test);
}
