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

#ifndef RUNMANAGER_LIB_TEST_RUNMANAGERTESTFIXTURE_HPP
#define RUNMANAGER_LIB_TEST_RUNMANAGERTESTFIXTURE_HPP

#include "../../../utilities/core/Logger.hpp"
#include "../../../utilities/core/FileLogSink.hpp"
#include <gtest/gtest.h>

class RunManagerTestFixture : public ::testing::Test {
  protected:

    // initialize for each test
    virtual void SetUp() {}

    // tear down after for each test
    virtual void TearDown() {}

    // initialize before all tests
    static void SetUpTestCase();

    // tear down after all tests
    static void TearDownTestCase();

    REGISTER_LOGGER("RunManagerTestFixture");

    static boost::optional<openstudio::FileLogSink> logFile;

    void run_stress_test(bool t_show_ui, bool t_listener, bool t_nullJob, 
        int t_runcount, bool t_kill, int t_min_time, int t_max_time);
};

#endif //RUNMANAGER_LIB_TEST_RUNMANAGERTESTFIXTURE_HPP


