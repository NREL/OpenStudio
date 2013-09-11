/**********************************************************************
 *  Copyright (c) 2008-2012, Alliance for Sustainable Energy.
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

#include <pat_app/CloudMonitor.hpp>
#include <pat_app/RunTabController.hpp>
#include <pat_app/RunView.hpp>
#include <pat_app/PatApp.hpp>
#include <utilities/cloud/CloudProvider.hpp>
#include <utilities/cloud/CloudProvider_Impl.hpp>
#include <utilities/cloud/VagrantProvider.hpp>
#include <utilities/cloud/VagrantProvider_Impl.hpp>
#include <utilities/core/Assert.hpp>
#include <pat_app/VagrantConfiguration.hxx>

namespace openstudio {

namespace pat {

CloudMonitor::CloudMonitor()
  : QObject()  
{    
  makeProvider();
}      

CloudMonitor::~CloudMonitor()
{
}

QSharedPointer<CloudProvider> CloudMonitor::cloudProvider() const
{
  return m_cloudProvider;
}

void CloudMonitor::setCloudProvider(QSharedPointer<CloudProvider> cloudProvider)
{
  m_cloudProvider = cloudProvider;

  bool bingo;

  if( m_cloudProvider )
  {
    bingo = connect(m_cloudProvider->getImpl<detail::CloudProvider_Impl>().get(),SIGNAL(serverStarted(const Url&)),
                    this,SLOT(onCloudStartupComplete()));
    OS_ASSERT(bingo);

    bingo = connect(m_cloudProvider->getImpl<detail::CloudProvider_Impl>().get(),SIGNAL(terminated()),
                   this,SLOT(onCloudTerminateComplete()));
    OS_ASSERT(bingo);
  }
}

void CloudMonitor::toggleCloud(bool on)
{
  if( cloudProvider() )
  {
    if( QSharedPointer<RunTabController> runTabController = PatApp::instance()->runTabController() )
    {
      RunView * runView = runTabController->runView; 

      OS_ASSERT(runView);

      runView->runStatusView->toggleCloudButton->setEnabled(false);
    }

    if( on )
    {
      cloudProvider()->requestStartServer();
    }
    else
    {
      cloudProvider()->requestTerminate();
    }
  }
}

void CloudMonitor::onCloudStartupComplete()
{
  if( QSharedPointer<RunTabController> runTabController = PatApp::instance()->runTabController() )
  {
    RunView * runView = runTabController->runView; 

    OS_ASSERT(runView);
    
    runView->runStatusView->toggleCloudButton->setStarting(false);

    runView->runStatusView->toggleCloudButton->setEnabled(true);
  }
}

void CloudMonitor::onCloudTerminateComplete()
{
  makeProvider();

  if( QSharedPointer<RunTabController> runTabController = PatApp::instance()->runTabController() )
  {
    RunView * runView = runTabController->runView; 

    OS_ASSERT(runView);

    runView->runStatusView->toggleCloudButton->setStopping(false);

    runView->runStatusView->toggleCloudButton->setEnabled(true);
  }
}

QSharedPointer<VagrantProvider> CloudMonitor::makeProvider()
{
  // create the vagrant provider
  path serverPath = vagrantServerPath();
  Url serverUrl("http://localhost:8080");
  path workerPath = vagrantWorkerPath();
  Url workerUrl("http://localhost:8081");

  QSharedPointer<VagrantProvider> provider;
  // TODO fix this to use new Settings api
  provider = QSharedPointer<VagrantProvider>(new VagrantProvider());

  //provider->signUserAgreement(true);

  setCloudProvider(provider);

  return provider;
}

} // pat

} // openstudio
