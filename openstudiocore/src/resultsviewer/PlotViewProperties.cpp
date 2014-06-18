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

#include "PlotViewProperties.hpp"

namespace resultsviewer{

PlotViewProperties::PlotViewProperties(QWidget *parent, Qt::WindowFlags flags)
  : QDialog(parent, flags)
{
  ui.setupUi(this);

  ui.cboCurveStyle->addItem(tr("Smooth"));
  ui.cboCurveStyle->addItem(tr("Stair"));
  ui.cboCurveStyle->addItem(tr("Bar"));

  connect(ui.btnApply, SIGNAL(clicked()), this, SLOT(btnApply_onClick()));
  connect(ui.btnClose, SIGNAL(clicked()), this, SLOT(btnClose_onClick()));
}

PlotViewProperties::~PlotViewProperties()
{}

void PlotViewProperties::setPlotView(resultsviewer::PlotView *PlotViewItem)
{
  QString s;
  QFont font;
  unsigned fontSize;

  m_PlotView = PlotViewItem;

  s = m_PlotView->plot()->title().text();
  ui.titleText->setText(s);
  font = m_PlotView->plot()->title().font();
  ui.titleFont->setCurrentFont(font);
  fontSize = m_PlotView->plot()->title().font().pointSize();
  ui.titleFontSize->setRange(6,20);
  ui.titleFontSize->setValue(fontSize);

  s = m_PlotView->plot()->axisTitle(QwtPlot::xBottom).text();
  ui.bottomAxisText->setText(s);
  font = m_PlotView->plot()->axisTitle(QwtPlot::xBottom).font();
  ui.bottomAxisFont->setCurrentFont(font);
  fontSize = m_PlotView->plot()->axisTitle(QwtPlot::xBottom).font().pointSize();
  ui.bottomAxisFontSize->setRange(6,20);
  ui.bottomAxisFontSize->setValue(fontSize);

  s = m_PlotView->plot()->axisTitle(QwtPlot::xTop).text();
  ui.topAxisText->setText(s);
  font = m_PlotView->plot()->axisTitle(QwtPlot::xTop).font();
  ui.topAxisFont->setCurrentFont(font);
  fontSize = m_PlotView->plot()->axisTitle(QwtPlot::xTop).font().pointSize();
  ui.topAxisFontSize->setRange(6,20);
  ui.topAxisFontSize->setValue(fontSize);

  s = m_PlotView->plot()->axisTitle(QwtPlot::yLeft).text();
  ui.leftAxisText->setText(s);
  font = m_PlotView->plot()->axisTitle(QwtPlot::yLeft).font();
  ui.leftAxisFont->setCurrentFont(font);
  fontSize = m_PlotView->plot()->axisTitle(QwtPlot::yLeft).font().pointSize();
  ui.leftAxisFontSize->setRange(6,20);
  ui.leftAxisFontSize->setValue(fontSize);

  s = m_PlotView->plot()->axisTitle(QwtPlot::yRight).text();
  ui.rightAxisText->setText(s);
  font = m_PlotView->plot()->axisTitle(QwtPlot::yRight).font();
  ui.rightAxisFont->setCurrentFont(font);
  fontSize = m_PlotView->plot()->axisTitle(QwtPlot::yRight).font().pointSize();
  ui.rightAxisFontSize->setRange(6,20);
  ui.rightAxisFontSize->setValue(fontSize);

  bool showLegend;
  switch (m_PlotView->plotType())
  {
    case resultsviewer::RVPV_LINEPLOT:
      showLegend = m_PlotView->legend()->isVisible();
      if (showLegend)
        ui.chkShowLegend->setCheckState(Qt::Checked);
      else
        ui.chkShowLegend->setCheckState(Qt::Unchecked);
      ui.listWidget->setRowHidden(7,true); // hide "Flood Plot" properties
      ui.listWidget->setRowHidden(8,true); // hide "Illuminance Map" properties
      updateCurves();
      connect(ui.cboCurves, SIGNAL(currentIndexChanged(int)), this, SLOT(slotCurveChanged(int)));
      connect(ui.btnRemoveCurve, SIGNAL(clicked()), this, SLOT(slotRemoveCurve()));
      break;
    case resultsviewer::RVPV_FLOODPLOT:
      ui.listWidget->setRowHidden(5,true); //  hide "Legend" properties
      ui.listWidget->setRowHidden(6,true);  //  hide "Curves" properties
      ui.listWidget->setRowHidden(8,true); // hide "Illuminance Map" properties
      updateFloodPlot();
      break;
    case resultsviewer::RVPV_ILLUMINANCEPLOT:
      ui.listWidget->setRowHidden(5,true); //  hide "Legend" properties
      ui.listWidget->setRowHidden(6,true);  //  hide "Curves" properties
      ui.listWidget->setRowHidden(7,true); // hide "Flood Plot" properties
      updateIlluminancePlot();
      break;
  }

  // autoscaling
  if (m_PlotView->yLeftAutoScale())
    ui.leftAxisAutoScale->setCheckState(Qt::Checked);
  else
    ui.leftAxisAutoScale->setCheckState(Qt::Unchecked);
  if (m_PlotView->yRightAutoScale())
    ui.rightAxisAutoScale->setCheckState(Qt::Checked);
  else

ui.rightAxisAutoScale->setCheckState(Qt::Unchecked);

}

void PlotViewProperties::slotCurveChanged(int index)
{
  if ((index > -1) && (index < (int)m_curveVec.size()))
  {
    ui.cboCurveStyle->setCurrentIndex(m_curveVec[index]->linePlotStyle());
    QPen pen = m_curveVec[index]->pen();
    ui.cboCurveLineStyle->setCurrentIndex(pen.style());
    ui.spbCurveThickness->setValue(pen.width());
  }
}

void PlotViewProperties::slotRemoveCurve()
{
  int index = ui.cboCurves->currentIndex();
  if ((index > -1) && (index < (int)m_curveVec.size()))
  {
    m_curveVec[index]->detach();
    updateCurves();
    m_PlotView->plot()->replot();
    m_PlotView->legend()->refreshLegend();
  }
}

void PlotViewProperties::updateCurveProperties(int index)
{
  if ((index > -1) && (index < (int)m_curveVec.size()))
  {
    QPen pen(m_curveVec[index]->pen()); // need color
    pen.setWidth(ui.spbCurveThickness->value());
    pen.setStyle((Qt::PenStyle)ui.cboCurveLineStyle->currentIndex());
    m_curveVec[index]->setPen(pen);
    m_curveVec[index]->setLinePlotStyle((resultsviewer::LinePlotStyleType)ui.cboCurveStyle->currentIndex());
    m_PlotView->plot()->replot();
  }
}

void PlotViewProperties::updateFloodPlotProperties()
{
  m_PlotView->setFloodPlotAutoScale(ui.rbnFPAutoScale->isChecked());
  m_PlotView->setFloodPlotMin(ui.txtFPMin->text().toDouble());
  m_PlotView->setFloodPlotMax(ui.txtFPMax->text().toDouble());

  m_PlotView->showContour(ui.chkShowContour->checkState() == Qt::Checked);
  m_PlotView->showSpectrogram(ui.chkShowColorMap->checkState() == Qt::Checked);

  QString s(ui.cboColorMaps->itemText(ui.cboColorMaps->currentIndex()));
  if (ui.cboColorMaps->currentIndex() > -1) m_PlotView->setColorMap(s);

  m_PlotView->setDataRange();
  m_PlotView->plot()->replot();
}


void PlotViewProperties::updateFloodPlot()
{
  ui.chkShowColorMap->setChecked(m_PlotView->spectrogramOn());
  ui.chkShowContour->setChecked(m_PlotView->contourOn());
  // update color map list
  ui.cboColorMaps->clear();
  ui.cboColorMaps->addItems(m_PlotView->colorMapList());
  ui.cboColorMaps->setCurrentIndex(m_PlotView->colorMap());

  ui.txtFPMin->setText(QString::number(m_PlotView->floodPlotMin()));
  ui.txtFPMax->setText(QString::number(m_PlotView->floodPlotMax()));
  if (m_PlotView->floodPlotAutoScale())
    ui.rbnFPAutoScale->setChecked(true);
  else
    ui.rbnFPManualScale->setChecked(true);
}

void PlotViewProperties::updateIlluminancePlotProperties()
{
  m_PlotView->setFloodPlotAutoScale(ui.rbnIMAutoScale->isChecked());
  if (ui.rbnIMScaleToYear->isChecked())
  {
    m_PlotView->setFloodPlotMin(m_PlotView->floodPlotYearlyMin());
    m_PlotView->setFloodPlotMax(m_PlotView->floodPlotYearlyMax());
  }
  else
  {
    m_PlotView->setFloodPlotMin(ui.txtIMMin->text().toDouble());
    m_PlotView->setFloodPlotMax(ui.txtIMMax->text().toDouble());
  }

  if (ui.chkIMRefPt1->checkState() == Qt::Checked)
    m_PlotView->illuminanceMapRefPt1()->show();
  else
    m_PlotView->illuminanceMapRefPt1()->hide();

  if (ui.chkIMRefPt2->checkState() == Qt::Checked)
    m_PlotView->illuminanceMapRefPt2()->show();
  else
    m_PlotView->illuminanceMapRefPt2()->hide();

  m_PlotView->showContour(ui.chkIMShowContour->checkState() == Qt::Checked);
  m_PlotView->showSpectrogram(ui.chkIMShowColorMap->checkState() == Qt::Checked);

  QString s(ui.cboIMColorMaps->itemText(ui.cboIMColorMaps->currentIndex()));
  if (ui.cboIMColorMaps->currentIndex() > -1) m_PlotView->setColorMap(s);

  m_PlotView->setDataRange();
  m_PlotView->plot()->replot();
}


void PlotViewProperties::updateIlluminancePlot()
{
  ui.chkIMShowColorMap->setChecked(m_PlotView->spectrogramOn());
  ui.chkIMShowContour->setChecked(m_PlotView->contourOn());
  ui.chkIMRefPt1->setChecked(m_PlotView->illuminanceMapRefPt1()->isVisible());
  ui.chkIMRefPt2->setChecked(m_PlotView->illuminanceMapRefPt2()->isVisible());
  // update color map list
  ui.cboIMColorMaps->clear();
  ui.cboIMColorMaps->addItems(m_PlotView->colorMapList());
  ui.cboIMColorMaps->setCurrentIndex(m_PlotView->colorMap());

  ui.txtIMMin->setText(QString::number(m_PlotView->floodPlotMin()));
  ui.txtIMMax->setText(QString::number(m_PlotView->floodPlotMax()));
  if (m_PlotView->floodPlotAutoScale())
    ui.rbnIMAutoScale->setChecked(true);
  else
  {
    if ( (m_PlotView->floodPlotMin() == m_PlotView->floodPlotYearlyMin()) && (m_PlotView->floodPlotMax() == m_PlotView->floodPlotYearlyMax()) )
      ui.rbnIMScaleToYear->setChecked(true);
    else
      ui.rbnIMManualScale->setChecked(true);
  }
}

void PlotViewProperties::updateCurves()
{
  /// multiple curves based on units
  const QwtPlotItemList &listPlotItem = m_PlotView->plot()->itemList();
  QwtPlotItemIterator itPlotItem;
  m_curveVec.clear();
  ui.cboCurves->clear();
  for (itPlotItem = listPlotItem.begin();itPlotItem!=listPlotItem.end();++itPlotItem)
  {
    QwtPlotItem *plotItem = *itPlotItem;
    if ( plotItem->rtti() == QwtPlotItem::Rtti_PlotCurve)
    {
      ui.cboCurves->addItem(plotItem->title().text());
      m_curveVec.push_back((LinePlotCurve*)plotItem);
    }
  }

  if (m_curveVec.size() > 0) slotCurveChanged(0);
}


void PlotViewProperties::btnApply_onClick()
{
  QString s;
  QFont font;
  unsigned fontSize;
  QwtText title;

  s = ui.titleText->toPlainText();
  font = ui.titleFont->currentFont();
  fontSize = ui.titleFontSize->value();
  font.setPointSize(fontSize);
  title.setText(s);
  title.setFont(font);
  m_PlotView->plot()->setTitle(title);

  s = ui.bottomAxisText->toPlainText();
  font = ui.bottomAxisFont->currentFont();
  fontSize = ui.bottomAxisFontSize->value();
  font.setPointSize(fontSize);
  title.setText(s);
  title.setFont(font);
  m_PlotView->plot()->setAxisTitle(QwtPlot::xBottom,title);

  s = ui.topAxisText->toPlainText();
  font = ui.topAxisFont->currentFont();
  fontSize = ui.topAxisFontSize->value();
  font.setPointSize(fontSize);
  title.setText(s);
  title.setFont(font);
  m_PlotView->plot()->setAxisTitle(QwtPlot::xTop,title);

  s = ui.leftAxisText->toPlainText();
  font = ui.leftAxisFont->currentFont();
  fontSize = ui.leftAxisFontSize->value();
  font.setPointSize(fontSize);
  title.setText(s);
  title.setFont(font);
  m_PlotView->plot()->setAxisTitle(QwtPlot::yLeft,title);

  s = ui.rightAxisText->toPlainText();
  font = ui.rightAxisFont->currentFont();
  fontSize = ui.rightAxisFontSize->value();
  font.setPointSize(fontSize);
  title.setText(s);
  title.setFont(font);
  m_PlotView->plot()->setAxisTitle(QwtPlot::yRight,title);


  switch (m_PlotView->plotType())
  {
    case resultsviewer::RVPV_LINEPLOT:
      updateCurveProperties(ui.cboCurves->currentIndex());
      if (ui.chkShowLegend->checkState() == Qt::Checked)
      {
        m_PlotView->legend()->show();
        m_PlotView->legend()->raise();
        m_PlotView->legend()->refreshLegend();
      }
      else
      {
        m_PlotView->legend()->hide();
      }
      break;
    case resultsviewer::RVPV_FLOODPLOT:
      updateFloodPlotProperties();
      break;
    case resultsviewer::RVPV_ILLUMINANCEPLOT:
      updateIlluminancePlotProperties();
      break;
  }

  // autoscaling - affects line plots only
  if (ui.leftAxisAutoScale->checkState() == Qt::Checked)
    m_PlotView->setYLeftAutoScale(true);
  else 
    m_PlotView->setYLeftAutoScale(false);
  if (ui.rightAxisAutoScale->checkState() == Qt::Checked)
    m_PlotView->setYRightAutoScale(true);
  else 
    m_PlotView->setYRightAutoScale(false);



}

void PlotViewProperties::btnClose_onClick()
{
  this->close();
}

} // resultsviewer

