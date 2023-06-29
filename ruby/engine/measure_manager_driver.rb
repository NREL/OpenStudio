########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require_relative 'measure_manager'
require_relative 'measure_manager_server'

port = 1234

server = WEBrick::HTTPServer.new(:Port => port)

server.mount "/", MeasureManagerServlet

trap("INT") {
    server.shutdown
}

server.start
