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

#ifndef OPENSTUDIO_OSCOLLAPSIBLEITEMHEADER_HPP
#define OPENSTUDIO_OSCOLLAPSIBLEITEMHEADER_HPP

#include "OSItem.hpp"

class QLabel;
class QVBoxLayout;
class QPaintEvent;
class QMouseEvent;

namespace openstudio {

class OSCollapsibleItemHeader : public QWidget
{
  Q_OBJECT

  public:

    OSCollapsibleItemHeader(const std::string& text,
                            const OSItemId& itemId,
                            OSItemType type = OSItemType::CollapsibleListHeader,
                            QWidget * parent = 0);

    virtual ~OSCollapsibleItemHeader() {}

    bool expanded() const;
    void setExpanded(bool isExpanded);

    bool selected() const;
    void setSelected(bool isSelected);

  signals:

    void clicked(OSCollapsibleItemHeader * header);

  protected:

    void paintEvent(QPaintEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void leaveEvent(QEvent * event);
    QSize sizeHint() const;

  private:
    void setImage(bool expanded);

    QLabel * m_arrowLabel;

    bool m_expanded;
    bool m_selected;
    bool m_mouseDown;

};

} // openstudio

#endif // OPENSTUDIO_OSCOLLAPSIBLEITEMHEADER_HPP

