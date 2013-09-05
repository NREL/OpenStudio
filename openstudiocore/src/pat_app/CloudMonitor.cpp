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
#include <utilities/cloud/CloudProvider.hpp>
#include <utilities/cloud/CloudProvider_Impl.hpp>
#include <utilities/core/Assert.hpp>

namespace openstudio {

namespace pat {

QSharedPointer<CloudProvider> CloudMonitor::cloudProvider() const
{
  return m_cloudProvider;
}

void CloudMonitor::setCloudProvider(QSharedPointer<CloudProvider> cloudProvider)
{
  m_cloudProvider = cloudProvider;

  bool bingo;

  bingo = connect(m_cloudProvider->getImpl<detail::CloudProvider_Impl>().get(),SIGNAL(serverStarting()),this,SIGNAL(serverStarting()));
  OS_ASSERT(bingo);

  bingo = connect(m_cloudProvider->getImpl<detail::CloudProvider_Impl>().get(),SIGNAL(serverStarted(const Url&)),
                  this,SIGNAL(serverStarted(const Url&)));
  OS_ASSERT(bingo);

  bingo = connect(m_cloudProvider->getImpl<detail::CloudProvider_Impl>().get(),SIGNAL(workerStarting()),
                  this,SIGNAL(workerStarting()));
  OS_ASSERT(bingo);

  bingo = connect(m_cloudProvider->getImpl<detail::CloudProvider_Impl>().get(),SIGNAL(workerStarted(const Url&)),
                  this,SIGNAL(workerStarted(const Url&)));
  OS_ASSERT(bingo);

  bingo = connect(m_cloudProvider->getImpl<detail::CloudProvider_Impl>().get(),SIGNAL(allWorkersStarted()),
                  this,SIGNAL(allWorkersStarted()));
  OS_ASSERT(bingo);

  bingo = connect(m_cloudProvider->getImpl<detail::CloudProvider_Impl>().get(),SIGNAL(terminating()),
                  this,SIGNAL(terminating()));
  OS_ASSERT(bingo);

  bingo = connect(m_cloudProvider->getImpl<detail::CloudProvider_Impl>().get(),SIGNAL(terminateComplete()),
                  this,SIGNAL(terminateComplete()));
  OS_ASSERT(bingo);
}

} // pat

} // openstudio
