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

#include <thread>

namespace openstudio {
namespace airflow {

SurfaceNetworkBuilder::SurfaceNetworkBuilder(ProgressBar *progressBar) : m_progressBar(progressBar)
{
  m_logSink.setLogLevel(Info);
  m_logSink.setChannelRegex(boost::regex("openstudio\\.model\\.SurfaceNetworkBuilder"));
  m_logSink.setThreadId(std::this_thread::get_id());
  m_progressBar = nullptr;
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
  std::vector<openstudio::Handle> used;

  m_logSink.setThreadId(std::this_thread::get_id());
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
    } else if((std::find(used.begin(), used.end(), surface.handle())==used.end()) && (bc == "Surface")) {
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
