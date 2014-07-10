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

#include "CostLineItem.hpp"

namespace openstudio{

CostLineItem::CostLineItem():
  m_expectedLife(0),
  m_fixedOM(0),
  m_matCost(0),
  m_installCost(0),
  m_salvageCost(0),
  m_variableOM(0)
{
}

CostLineItem::~CostLineItem()
{
}

int CostLineItem::expectedLife()
{
  return m_expectedLife;
}

double CostLineItem::fixedOM()
{
  return m_fixedOM;
}

double CostLineItem::matCost()
{
  return m_matCost;
}

double CostLineItem::installCost()
{
  return m_installCost;
}

double CostLineItem::salvageCost()
{
  return m_salvageCost;
}

double CostLineItem::variableOM()
{
  return m_variableOM;
}

std::string CostLineItem::name()
{
  return m_name;
}

void CostLineItem::setExpectedLife(int expectedLife)
{
  m_expectedLife = expectedLife;
}

void CostLineItem::setFixedOM(double fixedOM)
{
  m_fixedOM = fixedOM;
}

void CostLineItem::setMatCost(double matCost)
{
  m_matCost = matCost;
}

void CostLineItem::setInstallCost(double installCost)
{
  m_installCost = installCost;
}

void CostLineItem::setSalvageCost(double salvageCost)
{
  m_salvageCost = salvageCost;
}

void CostLineItem::setVariableOM(double variableOM)
{
  m_variableOM = variableOM;
}

void CostLineItem::setName(std::string name)
{
  m_name = name;
}

}
