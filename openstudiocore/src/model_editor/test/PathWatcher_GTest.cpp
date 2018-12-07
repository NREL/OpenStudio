/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include <resources.hxx>

#include "ModelEditorFixture.hpp"

#include "../PathWatcher.hpp"
#include "../Application.hpp"

#include <utilities/core/Path.hpp>
#include <utilities/core/System.hpp>

#include <iostream>
#include <thread>

using std::ios_base;
using openstudio::toPath;
using openstudio::Application;
using openstudio::System;

struct TestPathWatcher : public PathWatcher{

  // set periodic timer to 1 ms
  TestPathWatcher(const openstudio::path& path)
    : PathWatcher(path, 1), added(false), changed(false), removed(false)
  {}

  virtual void onPathAdded() override { added = true; }
  virtual void onPathChanged() override { changed = true; }
  virtual void onPathRemoved() override { removed = true; }

  bool added;
  bool changed;
  bool removed;
};

// writes seem to have to occur in another thread for watcher to detect them
void write_file(const openstudio::path& path, const std::string& contents)
{
  openstudio::filesystem::ofstream outFile(path, ios_base::out | ios_base::trunc);
  ASSERT_TRUE(outFile?true:false);
  outFile << contents;
  outFile.close();
};

// removes seem to have to occur in another thread for watcher to detect them
void remove_file(const openstudio::path& path)
{
  openstudio::filesystem::remove(path);
}


TEST_F(ModelEditorFixture, PathWatcher_File)
{
  Application::instance().application(false);

  openstudio::path path = toPath("./PathWatcher_File");
  auto w1 = std::thread(write_file, path, "test 1");
  w1.join();

  ASSERT_TRUE(openstudio::filesystem::exists(path));

  TestPathWatcher watcher(path);
  EXPECT_FALSE(watcher.added);
  EXPECT_FALSE(watcher.changed);
  EXPECT_FALSE(watcher.removed);

  EXPECT_EQ(path.string(), watcher.path().string());

  auto w2 = std::thread(write_file, path, "test 2");
  w2.join();

  EXPECT_TRUE(openstudio::filesystem::exists(path));

  // calls processEvents
  System::msleep(10);

  EXPECT_FALSE(watcher.added);
  EXPECT_TRUE(watcher.changed);
  EXPECT_FALSE(watcher.removed);
  watcher.changed = false;
  EXPECT_FALSE(watcher.changed);

  auto r1 = std::thread(remove_file, path);
  r1.join();

  EXPECT_FALSE(openstudio::filesystem::exists(path));

  // calls processEvents
  System::msleep(10);

  EXPECT_FALSE(watcher.added);
  EXPECT_FALSE(watcher.changed);
  EXPECT_TRUE(watcher.removed);
}

TEST_F(ModelEditorFixture, PathWatcher_Dir)
{
  Application::instance().application(false);

  openstudio::path path = toPath("./");
  ASSERT_TRUE(openstudio::filesystem::exists(path));

  openstudio::path filePath = toPath("./PathWatcher_Dir");
  if (openstudio::filesystem::exists(filePath)){
    openstudio::filesystem::remove(filePath);
  }
  ASSERT_FALSE(openstudio::filesystem::exists(filePath));

  TestPathWatcher watcher(path);
  EXPECT_FALSE(watcher.changed);

  EXPECT_EQ(path.string(), watcher.path().string());

  // catches the file addition
  auto w1 = std::thread(write_file, filePath, "test 1");
  w1.join();

  EXPECT_TRUE(openstudio::filesystem::exists(filePath));

  // calls processEvents
  System::msleep(10);

  EXPECT_TRUE(watcher.changed);
  watcher.changed = false;
  EXPECT_FALSE(watcher.changed);

  // does not catch changes to the file
  auto w2 = std::thread(write_file, filePath, "test 2");
  w2.join();

  EXPECT_TRUE(openstudio::filesystem::exists(filePath));

  // calls processEvents
  System::msleep(10);

  EXPECT_FALSE(watcher.changed);

  // catches file removal
  auto r1 = std::thread(remove_file, filePath);
  r1.join();

  EXPECT_FALSE(openstudio::filesystem::exists(filePath));

  // calls processEvents
  System::msleep(10);

  EXPECT_TRUE(watcher.changed);
}
