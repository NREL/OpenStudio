/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef CONTAM_REVERSETRANSLATOR_HPP
#define CONTAM_REVERSETRANSLATOR_HPP

#include "ContamAPI.hpp"

//#include <model/Model.hpp>
#include <utilities/idf/Handle.hpp>
//#include <utilities/core/Path.hpp>
//#include <utilities/core/Optional.hpp>
#include <utilities/core/Logger.hpp>
#include <QMap>

#include "PrjData.hpp"

namespace openstudio {

  class ProgressBar;

namespace model {
  class Model;
  class ModelObject;
  class Surface;
}

namespace contam {

  class CONTAM_API ReverseTranslator
  {
  public:

    ReverseTranslator();

    virtual ~ReverseTranslator();
    
    boost::optional<openstudio::model::Model> translate(const prj::Data &data, ProgressBar* progressBar = NULL);

  private:
    void translateLevels(openstudio::model::Model &model, const prj::Data &data);
    bool translateZones(openstudio::model::Model &model, const prj::Data &data);
    bool translateSimpleAHS(openstudio::model::Model &model, const prj::Data &data);

    ProgressBar* m_progressBar;
    QMap<int,Handle> m_levelMap;
    QMap<int,Handle> m_zoneMap;
    QMap<int,Handle> m_ahsMap;
    QMap<int,int> m_connections;

    REGISTER_LOGGER("openstudio.contam.ReverseTranslator");
  };

} // contam
} // openstudio

#endif // CONTAM_REVERSETRANSLATOR_HPP
