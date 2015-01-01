/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "SurfaceNetworkBuilder.hpp"

#include "../model/Model.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/Surface.hpp"
#include "../model/Surface_Impl.hpp"
#include "../model/SubSurface.hpp"
#include "../model/SubSurface_Impl.hpp"

#include "../utilities/plot/ProgressBar.hpp"

#include <QThread>

namespace openstudio {
namespace airflow {

SurfaceNetworkBuilder::SurfaceNetworkBuilder(ProgressBar *progressBar) : m_progressBar(progressBar)
{
  m_logSink.setLogLevel(Info);
  m_logSink.setChannelRegex(boost::regex("openstudio\\.model\\.SurfaceNetworkBuilder"));
  m_logSink.setThreadId(QThread::currentThread());
  m_progressBar = 0;
}

void SurfaceNetworkBuilder::progress()
{
  if (m_progressBar) {
    m_progressBar->setValue(m_progressBar->value() + 1);
  }
}

void SurfaceNetworkBuilder::initProgress(int max, std::string label)
{
  if (m_progressBar) {
    m_progressBar->setWindowTitle(label);
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(max);
    m_progressBar->setValue(0);
  }
}

bool SurfaceNetworkBuilder::linkExteriorSurface(model::ThermalZone zone, model::Space space, model::Surface surface)
{
  LOG(Info, "Surface '" << surface.name().get() << "' connects zone '" << zone.name().get() << "' to the ambient");
  return true;
}

bool SurfaceNetworkBuilder::linkInteriorSurface(model::ThermalZone zone, model::Space space, model::Surface surface,
  model::Surface adjacentSurface, model::Space adjacentSpace, model::ThermalZone adjacentZone)
{
  LOG(Info, "Surfaces '" << surface.name().get() << "' and '" << adjacentSurface.name().get() << "' connect zone '"
    << zone.name().get() << "' to zone '" << adjacentZone.name().get() << "'");
  return true;
}

bool SurfaceNetworkBuilder::linkExteriorSubSurface(model::ThermalZone zone, model::Space space, model::Surface surface, model::SubSurface subSurface)
{
  LOG(Info, "Subsurface '" << subSurface.name().get() << "' connects zone '" << zone.name().get() << "' to the ambient");
  return true;
}

bool SurfaceNetworkBuilder::linkInteriorSubSurface(model::ThermalZone zone, model::Space space, model::Surface surface, model::SubSurface subSurface,
  model::SubSurface adjacentSubSurface, model::Surface adjacentSurface, model::Space adjacentSpace, model::ThermalZone adjacentZone)
{
  LOG(Info, "Subsurfaces '" << subSurface.name().get() << "' and '" << adjacentSubSurface.name().get() << "' connect zone '"
    << zone.name().get() << "' to zone '" << adjacentZone.name().get() << "'");
  return true;
}

bool SurfaceNetworkBuilder::build(model::Model &model)
{
  bool first = true;
  bool nowarnings = true;
  QVector<openstudio::Handle> used;

  m_logSink.setThreadId(QThread::currentThread());
  m_logSink.resetStringStream();

  std::vector<model::Surface> surfaces = model.getConcreteModelObjects<model::Surface>();

  initProgress(surfaces.size(),"Processing surfaces for network creation");

  for (model::Surface surface : surfaces) {
    if(!first) {
      progress();
    }
    first=false;
    std::string bc = surface.outsideBoundaryCondition();
    if(bc == "Outdoors") {
      // Get the associated thermal zone
      boost::optional<openstudio::model::Space> space = surface.space();
      if(!space) {
        LOG(Warn, "Unattached surface '" << openstudio::toString(surface.handle()) << "'");
        nowarnings = false;
        continue;
      }
      boost::optional<openstudio::model::ThermalZone> thermalZone = space->thermalZone();
      if(!thermalZone) {
        LOG(Warn, "Unzoned space '" << openstudio::toString(space->handle()) << "'");
        nowarnings = false;
        continue;
      }
      // If we made it to here, then the exterior surface is good.
      linkExteriorSurface(thermalZone.get(),space.get(),surface);
      for(model::SubSurface subSurface : surface.subSurfaces()) {
        linkExteriorSubSurface(thermalZone.get(),space.get(),surface,subSurface);
      }
    } else if(!used.contains(surface.handle()) && bc == "Surface") {
      // Get the associated thermal zone
      boost::optional<openstudio::model::Space> space = surface.space();
      if(!space) {
        LOG(Warn, "Unattached surface '" << openstudio::toString(surface.handle()) << "'");
        nowarnings = false;
        continue;
      }
      boost::optional<openstudio::model::ThermalZone> thermalZone = space->thermalZone();
      if(!thermalZone) {
        LOG(Warn, "Unzoned space '" << openstudio::toString(space->handle()) << "'");
        nowarnings = false;
        continue;
      }
      boost::optional<openstudio::model::Surface> adjacentSurface = surface.adjacentSurface();
      if(!adjacentSurface) {
        LOG(Error, "Unable to find adjacent surface for surface '" << openstudio::toString(surface.handle()) << "'");
        nowarnings = false;
        continue;
      }
      boost::optional<openstudio::model::Space> adjacentSpace = adjacentSurface->space();
      if(!adjacentSpace) {
        LOG(Error, "Unattached adjacent surface '" << openstudio::toString(adjacentSurface->handle()) << "'");
        nowarnings = false;
        continue;
      }
      boost::optional<openstudio::model::ThermalZone> adjacentZone = adjacentSpace->thermalZone();
      if(!thermalZone) {
        LOG(Warn, "Unzoned adjacent space '" << openstudio::toString(adjacentSpace->handle()) << "'");
        nowarnings = false;
        continue;
      }
      // We could punt the checking of subsurfaces until later, but it is best to get this out of the way now
      for(model::SubSurface subSurface : surface.subSurfaces()) {
        boost::optional<model::SubSurface> adjacentSubSurface = subSurface.adjacentSubSurface();
        if(!adjacentSubSurface) {
          LOG(Warn, "Unable to find adjacent subsurface for subsurface of '" << openstudio::toString(surface.handle()) << "'");
          nowarnings = false;
          continue;
        }
        if(adjacentSubSurface->surface() != adjacentSurface) {
          LOG(Warn, "Adjacent subsurface for subsurface of '" << openstudio::toString(surface.handle()) << "' is not attached to the expected surface");
          nowarnings = false;
          continue;
        }
      }
      // Ok, now we a surface, a space, a zone, an adjacent surface, an adjacent space, and an adjacent zone. Finally.
      used.push_back(adjacentSurface->handle());
      if(thermalZone == adjacentZone) {
        continue;
      }
      // Now have a surface that is fully connected and separates two zones so it can be linked
      linkInteriorSurface(thermalZone.get(),space.get(),surface,adjacentSurface.get(),adjacentSpace.get(),adjacentZone.get());
      // Link subsurfaces
      for(model::SubSurface subSurface : surface.subSurfaces()) {
        // Now we need to check the connections as we did with the surface
        boost::optional<model::SubSurface> adjacentSubSurface = subSurface.adjacentSubSurface();
        if(!adjacentSubSurface) {
          LOG(Warn, "Unable to find adjacent subsurface for subsurface of '" << openstudio::toString(surface.handle()) << "'");
          nowarnings = false;
          continue;
        }
        if(adjacentSubSurface->surface() != adjacentSurface) {
          LOG(Warn, "Adjacent subsurface for subsurface of '" << openstudio::toString(surface.handle()) << "' is not attached to the expected surface");
          nowarnings = false;
          continue;
        }
        // If we made it here, then the subsurface is fully connected and can be linked
        linkInteriorSubSurface(thermalZone.get(),space.get(),surface,subSurface,adjacentSubSurface.get(),adjacentSurface.get(),
          adjacentSpace.get(),adjacentZone.get());
      }
    }
  }
  if(surfaces.size()>0) {
    progress();
  }
  return nowarnings;
}

std::vector<LogMessage> SurfaceNetworkBuilder::warnings() const
{
  std::vector<LogMessage> result;

  for(LogMessage logMessage : m_logSink.logMessages()) {
    if (logMessage.logLevel() == Warn) {
      result.push_back(logMessage);
    }
  }

  return result;
}

std::vector<LogMessage> SurfaceNetworkBuilder::errors() const
{
  std::vector<LogMessage> result;

  for(LogMessage logMessage : m_logSink.logMessages()) {
    if (logMessage.logLevel() > Warn) {
      result.push_back(logMessage);
    }
  }

  return result;
}

std::vector<LogMessage> SurfaceNetworkBuilder::logMessages() const
{
  return m_logSink.logMessages();
}

ProgressBar * SurfaceNetworkBuilder::progressBar() const
{
  return m_progressBar;
}

void SurfaceNetworkBuilder::setProgressBar(ProgressBar *progressBar)
{
  m_progressBar = progressBar;
}

} // airflow
} // openstudio
