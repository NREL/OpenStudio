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

#ifndef OPENSTUDIO_PLANARSURFACEWIDGET_HPP
#define OPENSTUDIO_PLANARSURFACEWIDGET_HPP

#include "../model/PlanarSurface.hpp"

#include <QWidget>

namespace openstudio {

class PlanarSurfaceWidget : public QWidget
{
  Q_OBJECT

  public:

    PlanarSurfaceWidget(bool isIP, QWidget * parent = 0 );

    virtual ~PlanarSurfaceWidget() {}

    virtual void attach(const openstudio::model::PlanarSurface& planarSurface);

    virtual void detach();

  private slots:

    void clear();

    void refresh();

  private:

    void setUnits(bool displayIP);
  
    boost::optional<openstudio::model::PlanarSurface> m_planarSurface;

    QString m_units;

  public slots:

    void toggleUnits(bool displayIP);
};


} // openstudio

#endif // OPENSTUDIO_PLANARSURFACEWIDGET_HPP

