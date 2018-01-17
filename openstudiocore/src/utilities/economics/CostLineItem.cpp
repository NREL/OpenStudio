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
