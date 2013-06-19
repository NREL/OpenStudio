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


#include <plugin/ExternalPlugin.hpp>
#include <plugin/ExternalModelInterface.hpp>
#include <plugin/OSPlugin.hpp>
#include <plugin/OSPluginThread.hpp>

#include <model/Model_Impl.hpp>

#include <utilities/core/Application.hpp>
#include <utilities/core/Assert.hpp>

#include <iostream>

#include <QThread>
#include <QApplication>
#include <QEventLoop>
#include <QAbstractEventDispatcher>
#include <QVariant>

#if _WIN32 || _MSC_VER
  #include "qeventdispatcher_win_p.h"
#else
  #include "qeventdispatcher_unix_p.h"
#endif

namespace openstudio {
namespace plugin {

ExternalPlugin::ExternalPlugin()
  : m_started(false)
{
  //LOG(Trace, "ExternalPlugin");
  //QThread* ct = QThread::currentThread();
  //LOG(Trace, "ExternalPlugin, currentThread = " << ct);
  //QThread* thread = this->thread();
  //LOG(Trace, "ExternalPlugin, thread = " << thread);
  //BOOST_ASSERT(thread);
  //BOOST_ASSERT(ct == thread);

  //QAbstractEventDispatcher* eventDispatcher = NULL;
  
#if _WIN32 || _MSC_VER
  /*eventDispatcher =*/ new QEventDispatcherWin32(this);
#else
  /*eventDispatcher =*/ new QEventDispatcherUNIX(this);
#endif
  
  m_osPluginThread = new OSPluginThread(this);
  m_osPluginThread->start();
}

ExternalPlugin::~ExternalPlugin()
{
  LOG(Trace, "~ExternalPlugin, entering");

  emit quitting();

  while(m_osPluginThread->isRunning()){
  
    QCoreApplication* coreApplication = QApplication::instance();
    if(coreApplication){
      coreApplication->processEvents(QEventLoop::AllEvents);
    }

    QThread::yieldCurrentThread();

  }

  LOG(Trace, "~ExternalPlugin, leaving");
}

void ExternalPlugin::processEvents(int maxTime)
{
  //LOG(Trace, "processEvents");
  //QThread* ct = QThread::currentThread();
  //LOG(Trace, "processEvents, currentThread = " << ct);
  //QThread* thread = this->thread();
  //LOG(Trace, "processEvents, this->thread() = " << thread);
  //QAbstractEventDispatcher* eventDispatcher = QAbstractEventDispatcher::instance(ct);
  //LOG(Trace, "processEvents, eventDispatcher = " << eventDispatcher);

  QCoreApplication* coreApplication = QApplication::instance();
  if(coreApplication){
    //thread = coreApplication->thread();
    //LOG(Trace, "processEvents, coreApplication->thread() = " << thread);

    if (!m_started){
      start();
    }
  
    coreApplication->processEvents(QEventLoop::AllEvents, maxTime);
  }
}

void ExternalPlugin::requestStartup(const std::string& key)
{
  LOG(Trace, "requestStartup, key = " << key);

  emit startupRequested(key);
}

bool ExternalPlugin::isVisible(const std::string& key) const
{
  LOG(Trace, "isVisible, key = " << key);

  std::map<std::string, bool>::const_iterator it = m_isVisibleMap.find(key);
  if (it != m_isVisibleMap.end()){
    return it->second;
  }
  return false;
}

void ExternalPlugin::show(const std::string& key)
{
  LOG(Trace, "show, key = " << key);

  emit showRequested(key);
}

void ExternalPlugin::hide(const std::string& key)
{
  LOG(Trace, "hide, key = " << key);

  emit hideRequested(key);
}

void ExternalPlugin::start()
{
  LOG(Trace, "start");
  
  OSPlugin* osPlugin = OSPlugin::instance();

  bool test;
  test = QObject::connect(this, SIGNAL(startupRequested(const std::string&)), osPlugin, SLOT(startupPrompt(const std::string&)), Qt::QueuedConnection);
  BOOST_ASSERT(test);
  test = QObject::connect(this, SIGNAL(showRequested(const std::string&)), osPlugin, SLOT(showDocument(const std::string&)), Qt::QueuedConnection);
  BOOST_ASSERT(test);
  test = QObject::connect(this, SIGNAL(hideRequested(const std::string&)), osPlugin, SLOT(hideDocument(const std::string&)), Qt::QueuedConnection);
  BOOST_ASSERT(test);
  test = QObject::connect(this, SIGNAL(quitting()), osPlugin, SLOT(quit()), Qt::QueuedConnection);
  BOOST_ASSERT(test);

  test = QObject::connect(osPlugin, SIGNAL(newModel(const std::string&, openstudio::model::detail::Model_Impl*)), this, SLOT(newModel(const std::string&, openstudio::model::detail::Model_Impl*)), Qt::QueuedConnection);
  BOOST_ASSERT(test);
  test = QObject::connect(osPlugin, SIGNAL(modelClosed(const std::string&)), this, SLOT(modelClosed(const std::string&)), Qt::QueuedConnection);
  BOOST_ASSERT(test);
  test = QObject::connect(osPlugin, SIGNAL(documentShown(const std::string&)), this, SLOT(documentShown(const std::string&)), Qt::QueuedConnection);
  BOOST_ASSERT(test);
  test = QObject::connect(osPlugin, SIGNAL(documentHidden(const std::string&)), this, SLOT(documentHidden(const std::string&)), Qt::QueuedConnection);
  BOOST_ASSERT(test);

  m_started = true;
}

void ExternalPlugin::onNewModel(const std::string& key, const ModelHolder& modelHolder)
{
  LOG(Trace, "onNewModel, key = " << key);
}
 
void ExternalPlugin::onModelClosed(const std::string& key)
{
  LOG(Trace, "onModelClosed, key = " << key);
}

void ExternalPlugin::onDocumentShown(const std::string& key)
{
  LOG(Trace, "onDocumentShown, key = " << key);

  m_isVisibleMap[key] = true;
}
 
void ExternalPlugin::onDocumentHidden(const std::string& key)
{
  LOG(Trace, "onDocumentHidden, key = " << key);

  m_isVisibleMap[key] = false;
}

void ExternalPlugin::newModel(const std::string& key, openstudio::model::detail::Model_Impl* model)
{
  LOG(Trace, "newModel, key = " << key);

  ModelHolder modelHolder(model);
  onNewModel(key, modelHolder);
}

void ExternalPlugin::modelClosed(const std::string& key)
{
  LOG(Trace, "modelClosed, key = " << key);

  onModelClosed(key);
}

void ExternalPlugin::documentShown(const std::string& key)
{
  LOG(Trace, "documentShown, key = " << key);

  onDocumentShown(key);
}

void ExternalPlugin::documentHidden(const std::string& key)
{
  LOG(Trace, "documentHidden, key = " << key);

  onDocumentHidden(key);
}

} // plugin
} // openstudio


