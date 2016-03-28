######################################################################
#  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
#  All rights reserved.
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
######################################################################

require_relative '../adapter'

module OpenStudio
  module Workflow
    module Adapters
      # @todo (rhorsey) seems like Mongo adapters should be WebAdapters that use an API instead of talking to database directly - DLM
      class MongoLog
        def initialize(datapoint_model)
          fail 'Mongo is not supported as an adapter in the branch of the WFG. Please use develop or master.'
        end
      end

      class Mongo < Adapter
        def initialize(options = {})
          fail 'Mongo is not supported as an adapter in the branch of the WFG. Please use develop or master.'
        end
      end
    end
  end
end
