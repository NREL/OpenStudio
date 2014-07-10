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

#ifndef OPENSTUDIO_OSINSPECTORVIEW_HPP
#define OPENSTUDIO_OSINSPECTORVIEW_HPP

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"

#include <boost/smart_ptr.hpp>
#include <boost/optional.hpp>
#include <QWidget>

class QLabel;
class QStackedWidget;

namespace openstudio {

class OSItem;

class OSInspectorView : public QWidget
{
  Q_OBJECT

  public:

    OSInspectorView( bool addScrollArea,
                     QWidget * parent = 0);

    virtual ~OSInspectorView() {}

  signals:
    
    void dropZoneItemClicked(OSItem* item);

  public slots:

    void clearSelection();

    void selectItem(OSItem* item);

  protected:

    virtual void onSelectItem(OSItem *item) = 0;

    virtual void onClearSelection() = 0;

    virtual void onUpdate() = 0;

    QStackedWidget* stackedWidget() const;

  protected slots:

    void update();

  private:

    QStackedWidget* m_stackedWidget;
};


} // openstudio

#endif // OPENSTUDIO_OSINSPECTORVIEW_HPP

