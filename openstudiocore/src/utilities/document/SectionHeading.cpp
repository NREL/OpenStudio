/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#include <utilities/document/SectionHeading.hpp>
#include <utilities/document/SectionHeading_Impl.hpp>

#include <algorithm>
#include <functional>

namespace openstudio {

namespace detail {

  SectionHeading_Impl::SectionHeading_Impl(unsigned level, const std::string& text) 
    : m_level(level), m_text(text) {}

  DocumentElement SectionHeading_Impl::clone() const {
    boost::shared_ptr<SectionHeading_Impl> impl(new SectionHeading_Impl(*this));
    return SectionHeading(impl).cast<DocumentElement>();
  }

  unsigned SectionHeading_Impl::level() const {
    return m_level;
  }

  std::string SectionHeading_Impl::text() const { 
    return m_text;
  }

  void SectionHeading_Impl::setLevel(unsigned level) {
    m_level = level;
  }

  void SectionHeading_Impl::setText(const std::string& text) {
    m_text = text;
  }

  std::string SectionHeading_Impl::print(DocumentFormat fmt) const {
    if (fmt == DocumentFormat::COUT) { return mf_printCOUT(); }
    if (fmt == DocumentFormat::LaTeX) { return mf_printLaTeX(); }
    if (fmt == DocumentFormat::XHTML) { return mf_printXHTML(); }
    return std::string();
  }

  std::ostream& SectionHeading_Impl::printToStream(std::ostream& os, DocumentFormat fmt) const {
    os << print(fmt);
    return os;
  }

  // PRIVATE

  std::string SectionHeading_Impl::mf_printCOUT() const {
    std::string TEXT(m_text);
    std::transform(TEXT.begin(),TEXT.end(),TEXT.begin(),std::ptr_fun <int,int> (std::toupper));
    std::stringstream ss;
    ss << std::endl << TEXT << std::endl;
    return ss.str();
  }

  std::string SectionHeading_Impl::mf_printLaTeX() const {
    std::stringstream ss;
    ss << std::endl;
    if (m_level == 0) { ss << std::endl << "\\chapter{"; }
    if (m_level == 1) { ss << std::endl<< "\\section{"; }
    if (m_level == 2) { ss << std::endl<< "\\subsection{"; }
    if (m_level == 3) { ss << std::endl<< "\\paragraph{"; }
    if (m_level >= 4) { ss << std::endl<< "\\subparagraph{"; }
    ss << m_text << "}" << std::endl;
    return ss.str();
  }

  std::string SectionHeading_Impl::mf_printXHTML() const {
    std::stringstream ss;
    ss << std::endl;
    if (m_level == 0) { ss << std::endl<< "<h1>" << m_text << "</h1>"; }
    if (m_level == 1) { ss << std::endl<< "<h2>" << m_text << "</h2>"; }
    if (m_level == 2) { ss << std::endl<< "<h3>" << m_text << "</h3>"; }
    if (m_level == 3) { ss << std::endl<< "<h4>" << m_text << "</h4>"; }
    if (m_level == 4) { ss << std::endl<< "<h5>" << m_text << "</h5>"; }
    if (m_level >= 5) { ss << std::endl<< "<h6>" << m_text << "</h6>"; }
    ss << std::endl;
    return ss.str();
  }

} // detail

SectionHeading::SectionHeading(unsigned level, const std::string& text)
  : DocumentElement(boost::shared_ptr<detail::SectionHeading_Impl>(new detail::SectionHeading_Impl(level,text)))
{}

unsigned SectionHeading::level() const {
  return getImpl<detail::SectionHeading_Impl>()->level();
}

std::string SectionHeading::text() const {
  return getImpl<detail::SectionHeading_Impl>()->text();
}

void SectionHeading::setLevel(unsigned level) {
  getImpl<detail::SectionHeading_Impl>()->setLevel(level);
}

void SectionHeading::setText(const std::string& text) {
  getImpl<detail::SectionHeading_Impl>()->setText(text);
}

SectionHeading::SectionHeading(boost::shared_ptr<detail::SectionHeading_Impl> impl) 
  : DocumentElement(impl)
{}

} // openstudio
