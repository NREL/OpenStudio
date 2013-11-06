/**********************************************************************
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
  SimFile(){}
  explicit SimFile(openstudio::path path);

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

  openstudio::TimeSeries deltaP(int nr) const;
  openstudio::TimeSeries flow0(int nr) const;
  openstudio::TimeSeries flow1(int nr) const;

  std::vector<DateTime> dateTimes() const
  {
    return m_dateTimes;
  }

private:
  void clearLfr();
  bool readLfr(QString fileName);
  bool computeDateTimes(QVector<QString> day, QVector<QString> time);

  std::vector<int> m_pathNr;  // the CONTAM path index
  std::vector<std::vector<double> > m_dP;
  std::vector<std::vector<double> > m_F0;
  std::vector<std::vector<double> > m_F1;
  std::vector<DateTime> m_dateTimes;

  bool m_hasLfr;
  bool m_hasNfr;
  bool m_hasNcr;

  REGISTER_LOGGER("openstudio.contam.SimFile");
};

} // contam
} // openstudio

#endif // SIMFILE_H
