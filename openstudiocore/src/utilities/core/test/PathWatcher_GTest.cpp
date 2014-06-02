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

#include <resources.hxx>

#include "CoreFixture.hpp"
#include "../Path.hpp"
#include "../PathWatcher.hpp"
#include "../Application.hpp"
#include "../System.hpp"

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>

#include <QThread>

#include <iostream>

using std::ios_base;
using openstudio::toPath;
using openstudio::Application;
using openstudio::System;

struct TestPathWatcher : public openstudio::PathWatcher{

  // set periodic timer to 1 ms
  TestPathWatcher(const openstudio::path& path)
    : PathWatcher(path, 1), added(false), changed(false), removed(false)
  {}
  
  virtual void onPathAdded() { added = true; }
  virtual void onPathChanged() { changed = true; }
  virtual void onPathRemoved() { removed = true; }
  
  bool added;
  bool changed;
  bool removed;
};

// writes seem to have to occur in another thread for watcher to detect them
struct TestFileWriter : public QThread{

  TestFileWriter(const openstudio::path& path, const std::string& contents)
    : m_path(path), m_contents(contents)
  {}

  void run(){
    boost::filesystem::ofstream outFile(m_path, ios_base::out | ios_base::trunc);
    ASSERT_TRUE(outFile?true:false);
    outFile << m_contents;
    outFile.close();
  }

  openstudio::path m_path;
  std::string m_contents;
};

// removes seem to have to occur in another thread for watcher to detect them
struct TestFileRemover : public QThread{

  TestFileRemover(const openstudio::path& path)
    : m_path(path)
  {}

  void run(){
    boost::filesystem::remove(m_path);
  }

  openstudio::path m_path;
};

TEST_F(CoreFixture, PathWatcher_File)
{
  Application::instance().application();

  openstudio::path path = toPath("./PathWatcher_File");
  TestFileWriter w1(path, "test 1"); w1.start(); 
  while (!w1.isFinished()){  
    // do not call process events
    QThread::yieldCurrentThread();
  }
  ASSERT_TRUE(boost::filesystem::exists(path));

  TestPathWatcher watcher(path);
  EXPECT_FALSE(watcher.added);
  EXPECT_FALSE(watcher.changed);
  EXPECT_FALSE(watcher.removed);

  EXPECT_EQ(path.string(), watcher.path().string());

  TestFileWriter w2(path, "test 2"); w2.start(); 
  while (!w2.isFinished()){  
    // do not call process events
    QThread::yieldCurrentThread();
  }
  EXPECT_TRUE(boost::filesystem::exists(path));

  // calls processEvents
  System::msleep(10);

  EXPECT_FALSE(watcher.added);
  EXPECT_TRUE(watcher.changed);
  EXPECT_FALSE(watcher.removed);
  watcher.changed = false;
  EXPECT_FALSE(watcher.changed);

  TestFileRemover r1(path); r1.start();
  while (!r1.isFinished()){  
    // do not call process events
    QThread::yieldCurrentThread();
  }
  EXPECT_FALSE(boost::filesystem::exists(path));

  // calls processEvents
  System::msleep(10);

  EXPECT_FALSE(watcher.added);
  EXPECT_FALSE(watcher.changed);
  EXPECT_TRUE(watcher.removed);
}

TEST_F(CoreFixture, PathWatcher_Dir)
{
  Application::instance().application();

  openstudio::path path = toPath("./");
  ASSERT_TRUE(boost::filesystem::exists(path));

  openstudio::path filePath = toPath("./PathWatcher_Dir");
  if (boost::filesystem::exists(filePath)){
    boost::filesystem::remove(filePath);
  }
  ASSERT_FALSE(boost::filesystem::exists(filePath));

  TestPathWatcher watcher(path);
  EXPECT_FALSE(watcher.changed);

  EXPECT_EQ(path.string(), watcher.path().string());

  // catches the file addition
  TestFileWriter w1(filePath, "test 1"); w1.start(); 
  while (!w1.isFinished()){  
    // do not call process events
    QThread::yieldCurrentThread();
  }
  EXPECT_TRUE(boost::filesystem::exists(filePath));

  // calls processEvents
  System::msleep(10);

  EXPECT_TRUE(watcher.changed);
  watcher.changed = false;
  EXPECT_FALSE(watcher.changed);

  // does not catch changes to the file
  TestFileWriter w2(filePath, "test 2"); w2.start(); 
  while (!w2.isFinished()){  
    // do not call process events
    QThread::yieldCurrentThread();
  }
  EXPECT_TRUE(boost::filesystem::exists(filePath));

  // calls processEvents
  System::msleep(10);

  EXPECT_FALSE(watcher.changed);
  
  // catches file removal
  TestFileRemover r1(filePath); r1.start();
  while (!r1.isFinished()){  
    // do not call process events
    QThread::yieldCurrentThread();
  }
  EXPECT_FALSE(boost::filesystem::exists(filePath));

  // calls processEvents
  System::msleep(10);

  EXPECT_TRUE(watcher.changed);
}
