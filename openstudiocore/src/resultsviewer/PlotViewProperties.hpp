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

#ifndef RESULTSVIEWER_PLOTVIEWPROPERTIES_HPP
#define RESULTSVIEWER_PLOTVIEWPROPERTIES_HPP
#define RESULTSVIEWER_API

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QList>
#include <QAction>
#include <QMenu>
#include <QMdiSubWindow>
#include <string>
#include "PlotView.hpp"
#include <resultsviewer/ui_PlotViewProperties.h>
#include <qwt/qwt_plot_curve.h>

namespace resultsviewer{

/**
PlotViewProperties is a ui widget to change PlotView properties.
*/
class PlotViewProperties : public QDialog
{

  Q_OBJECT

public:
  PlotViewProperties(QWidget *parent = nullptr, Qt::WindowFlags flags = nullptr);
  virtual ~PlotViewProperties();

  void setPlotView(PlotView *PlotViewItem);


private:
  Ui::PlotViewPropertiesClass ui;
  resultsviewer::PlotView *m_PlotView;
  std::vector<LinePlotCurve*> m_curveVec;
  void updateCurveProperties(int index);
  void updateCurves();
  void updateFloodPlot();
  void updateFloodPlotProperties();
  void updateIlluminancePlot();
  void updateIlluminancePlotProperties();

private slots:
  void btnApply_onClick();
  void btnClose_onClick();
  void slotCurveChanged(int index);
  void slotRemoveCurve();
};

} // resultsviewer

#endif
