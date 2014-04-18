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
#ifndef SIMFILE_H
#define SIMFILE_H

#include "PrjDefines.hpp"

#include <utilities/data/TimeSeries.hpp>
#include <utilities/core/Path.hpp>

#include <QVector>

#include "ContamAPI.hpp"

namespace openstudio {
namespace contam {

class CONTAM_API SimFile {
public:
  explicit SimFile(openstudio::path path);

  // These are provided for advanced use
  std::vector<std::vector<double> > dP() const
  {
    return m_dP;
  }
  std::vector<std::vector<double> > F0() const
  {
    return m_F0;
  }
  std::vector<std::vector<double> > F1() const
  {
    return m_F1;
  }
  std::vector<std::vector<double> > T() const
  {
    return m_T;
  }
  std::vector<std::vector<double> > P() const
  {
    return m_P;
  }
  std::vector<std::vector<double> > D() const
  {
    return m_D;
  }


  // Most use should be confined to these
  boost::optional<openstudio::TimeSeries> pathDeltaP(int nr) const;
  boost::optional<openstudio::TimeSeries> pathFlow0(int nr) const;
  boost::optional<openstudio::TimeSeries> pathFlow1(int nr) const;
  boost::optional<openstudio::TimeSeries> pathFlow(int nr) const;
  boost::optional<openstudio::TimeSeries> nodeTemperature(int nr) const;
  boost::optional<openstudio::TimeSeries> nodePressure(int nr) const;
  boost::optional<openstudio::TimeSeries> nodeDensity(int nr) const;
  /** Returns a vector of DateTime objects that give the EnergyPlus-style
   *  end of interval times. These are not the actual times in the SIM file */
  std::vector<openstudio::DateTime> dateTimes() const;

  /** Returns a vector of DateTime objects that the SIM file contains data for.
   *  CONTAM always includes a start time result, so a yearly simulation will
   *  result in 8761 times. */
  std::vector<openstudio::DateTime> fileDateTimes() const
  {
    return m_dateTimes;
  }

private:
  void clearLfr();
  bool readLfr(QString fileName);
  void clearNfr();
  bool readNfr(QString fileName);
  bool computeDateTimes(QVector<QString> day, QVector<QString> time);

  QVector<int> m_pathNr;  // the CONTAM path index
  std::vector<std::vector<double> > m_dP;
  std::vector<std::vector<double> > m_F0;
  std::vector<std::vector<double> > m_F1;
  QVector<int> m_nodeNr;  // the CONTAM node index
  std::vector<std::vector<double> > m_T;
  std::vector<std::vector<double> > m_P;
  std::vector<std::vector<double> > m_D;
  std::vector<openstudio::DateTime> m_dateTimes;

  bool m_hasLfr;
  bool m_hasNfr;
  bool m_hasNcr;

  REGISTER_LOGGER("openstudio.contam.SimFile");
};

} // contam
} // openstudio

#endif // SIMFILE_H
