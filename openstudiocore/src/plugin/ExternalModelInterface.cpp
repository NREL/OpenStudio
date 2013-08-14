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

#include <plugin/ExternalModelInterface.hpp>
#include <plugin/ExternalPlugin.hpp>
#include <plugin/ExternalObjectInterface.hpp>

#include <model/Model_Impl.hpp>
#include <model/ModelObject_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <QThread>

namespace openstudio {
namespace plugin {

ModelHolder::ModelHolder(openstudio::model::detail::Model_Impl* ptr)
  : m_ptr(ptr)
{}

openstudio::model::detail::Model_Impl* ModelHolder::ptr() const
{
  return m_ptr;
}

ExternalModelInterface* ExternalModelInterface::create(const std::string& key, const ModelHolder& modelHolder)
{
  ExternalModelInterface* result = new ExternalModelInterface(key, modelHolder);
  return result;
}

ExternalModelInterface::ExternalModelInterface(const std::string& key, const ModelHolder& modelHolder)
  : QObject(), m_key(key)
{
  LOG(Trace, "ExternalModelInterface, key = " << key);
  //LOG(Trace, "ExternalModelInterface, modelHolder.ptr() = " << modelHolder.ptr());
  //QThread* ct = QThread::currentThread();
  //LOG(Trace, "ExternalModelInterface, currentThread = " << ct);
  //QThread* thread = this->thread();
  //LOG(Trace, "ExternalModelInterface, this->thread() = " << thread);

  // connect signals
  bool test;
  test = connect(this, SIGNAL(requestInitialModelObjects()), modelHolder.ptr(), SLOT(reportInitialModelObjects()), Qt::QueuedConnection);
  OS_ASSERT(test);
  test = connect(modelHolder.ptr(), SIGNAL(initialModelObject(openstudio::model::detail::ModelObject_Impl*, const openstudio::IddObjectType&, const openstudio::UUID&)), this, SLOT(initialModelObject(openstudio::model::detail::ModelObject_Impl*, const openstudio::IddObjectType&, const openstudio::UUID&)), Qt::QueuedConnection);
  OS_ASSERT(test);
  test = connect(modelHolder.ptr(), SIGNAL(initialReportComplete()), this, SLOT(initialize()), Qt::QueuedConnection);
  OS_ASSERT(test);
  test = connect(modelHolder.ptr(), SIGNAL(addWorkspaceObject(boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&)), this, SLOT(newWorkspaceObject(boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&)), Qt::QueuedConnection);
  OS_ASSERT(test);

  emit requestInitialModelObjects();
}

ExternalModelInterface::~ExternalModelInterface()
{
  LOG(Trace, "~ExternalModelInterface");
}
    
std::string ExternalModelInterface::key() const
{
  return m_key;
}

void ExternalModelInterface::onInitialModelObject(const ModelObjectHolder& modelObjectHolder, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  LOG(Trace, "onInitialModelObject");
}

void ExternalModelInterface::onInitialize()
{
  LOG(Trace, "onInitialize");
}

void ExternalModelInterface::onNewModelObject(const ModelObjectHolder& modelObjectHolder, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  LOG(Trace, "onNewModelObject");
}

void ExternalModelInterface::initialModelObject(openstudio::model::detail::ModelObject_Impl* modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  LOG(Trace, "initialModelObject");
  ModelObjectHolder modelObjectHolder(modelObject);
  onInitialModelObject(modelObjectHolder, iddObjectType, handle);
}

void ExternalModelInterface::initialize()
{
  LOG(Trace, "initialize");
  onInitialize();
}

void ExternalModelInterface::newWorkspaceObject(boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl> workspaceObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  LOG(Trace, "newWorkspaceObject");
  boost::shared_ptr<openstudio::model::detail::ModelObject_Impl> modelObject = boost::dynamic_pointer_cast<openstudio::model::detail::ModelObject_Impl>(workspaceObject);
  OS_ASSERT(modelObject);
  ModelObjectHolder modelObjectHolder(modelObject.get());
  onNewModelObject(modelObjectHolder, iddObjectType, handle);
} 

} // plugin
} // openstudio
