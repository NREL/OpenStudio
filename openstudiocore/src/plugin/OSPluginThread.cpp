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

#include <plugin/OSPluginThread.hpp>
#include <plugin/OSPlugin.hpp>

namespace openstudio {
namespace plugin {

OSPluginThread::OSPluginThread(QObject* parent)
  : QThread(parent)
{
}

void OSPluginThread::run()
{
  LOG(Trace, "run");
  QThread* ct = QThread::currentThread();
  LOG(Trace, "run, currentThread = " << ct);
  QThread* thread = this->thread();
  LOG(Trace, "run, thread = " << thread);

  OSPlugin::instance()->exec();

  LOG(Trace, "run, complete");
}

} // plugin
} // openstudio

