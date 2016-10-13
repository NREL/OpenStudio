/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
