/**********************************************************************
*  Copyright (c) 2008-2010, Alliance for Sustainable Energy.
*  Copyright (c) 2013, The Pennsylvania State University.
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

#ifndef CONTAM_FORWARDTRANSLATOR_HPP
#define CONTAM_FORWARDTRANSLATOR_HPP

#include "ContamAPI.hpp"

#include <contam/PrjData.hpp>

#include <utilities/idf/Handle.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/core/Optional.hpp>
#include <utilities/core/Logger.hpp>

namespace openstudio {
namespace model {
class Model;
}

namespace contam
{

  class CONTAM_API ForwardTranslator
  {
  public:
    ForwardTranslator(){m_valid=false;}
    boost::optional<std::string> translateToPrj(const openstudio::model::Model& model,
      bool translateHVAC=true);
    boost::optional<std::string> toString();

    bool writeMaps(const openstudio::path& path);
    static bool modelToContam(const openstudio::model::Model& model, const openstudio::path& path,
      const openstudio::path& mapPath);
    static bool modelToContam(const openstudio::model::Model& model, const openstudio::path& path);

    bool valid() const {return m_valid && data.valid;}
    std::map <Handle, int> surfaceMap() const {return m_surfaceMap;}

    bool setSteadyWeather(double windSpeed, double windDirection); // This is all we need now, expand later

  private:
    int tableLookup(QMap<std::string,int> map, std::string str, const char *name);
    int tableLookup(QMap<Handle,int> map, Handle handle, const char *name);
    std::string reverseLookup(QMap<std::string,int> map, int nr, const char *name);
    Handle ForwardTranslator::reverseLookup(QMap<Handle,int> map, int nr, const char *name);

    prj::Data data;
    // Maps - will be populated after a call of translateToPrj
    // I'm not clear on how this information will be propagated for
    // postprocessing purposes - write a file?
    // These map element names to the CONTAM index (1,2,...,nElement)
    QMap<QString,int> afeMap;
    QMap <Handle, int> levelMap;      // Building story to level map by handle
    QMap <Handle, int> zoneMap;       // Thermal zone to airflow zone map by handle
    //QMap <std::string, int> volumeMap; // Map of AHS volumes - may not be needed
    QMap <std::string, int> pathMap;  // AHS paths stored by name
    std::map <Handle, int> m_surfaceMap;    // Surface paths stored by handle
    QMap <Handle, int> ahsMap;        // Airloop to AHS map by handle

    bool m_valid;

    REGISTER_LOGGER("openstudio.contam.ForwardTranslator");
  };

} // contam
} // openstudio

#endif // CONTAM_FORWARDTRANSLATOR_HPP
