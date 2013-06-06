/**********************************************************************
*  Copyright (c) 2008-2010, Alliance for Sustainable Energy.
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

#ifndef CONTAM_FORWARDTRANSLATOR_HPP
#define CONTAM_FORWARDTRANSLATOR_HPP

#include "ContamAPI.hpp"

#include <utilities/core/Path.hpp>
#include <utilities/core/Optional.hpp>
#include <utilities/core/Logger.hpp>

namespace openstudio {
	namespace model {
		class Model;
	}

	namespace contam
	{
		class CONTAM_API ForwardTranslator 
		{
		public:
			static bool modelToContam(const openstudio::model::Model& model,
				const openstudio::path& path, const openstudio::path& mapPath);

			// Maps - will be populated after a call of translateToPrj
			// I'm not clear on how this information will be propagated for
			// postprocessing purpose - write a file?
			// These map element names to the CONTAM index (1,2,...,nElement)
			QMap<QString,int> afeMap;
			QMap <std::string, int> levelMap;
			QMap <std::string, int> zoneMap;
			QMap <std::string, int> pathMap;
			QMap <std::string, int> ahsMap;

		private:

			ForwardTranslator();

			int tableLookup(QMap<std::string,int> map, std::string str, const char *name);
			std::string reverseLookup(QMap<std::string,int> map, int nr, const char *name);

			boost::optional<QString> translateToPrj(const openstudio::model::Model& model);
			bool writeMaps(const openstudio::path& path);

			REGISTER_LOGGER("openstudio.contam.ForwardTranslator");
		};

	} // contam
} // openstudio

#endif // CONTAM_FORWARDTRANSLATOR_HPP
