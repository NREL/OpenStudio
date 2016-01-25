######################################################################
#  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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
######################################################################
#
# == OpenStudio
#
# OpenStudio is a free plugin for the SketchUp 3D drawing program built
# around the OpenStudio building model. OpenStudio allows users to create and edit 
# building geometry for the energy simulation engines EnergyPlus and, eventually, 
# Radiance. Over time the OpenStudio plugin will become more and more integrated
# with other applications in the OpenStudio toolkit.
#
# The primary function of the OpenStudio plugin is to tie the functionality
# of the OpenStudio SDK to the Ruby API exposed by SketchUp.  The SketchUp Ruby API
# is well documented at http://www.sketchup.com/intl/en/developer/docs/.
#
# == Design Intent
#
# OpenStudio does not provide access to all objects in the OpenStudio building model, access
# to more objects is planned for future releases. Each OpenStudio::ModelObject exposed in OpenStudio 
# is mapped to a particular type of SketchUp::Entity. For example, the OpenStudio::Building object is 
# mapped to the SketchUp::ShadowInfo object, the OpenStudio::Space object is mapped to the SketchUp::Group
# object, and the OpenStudio::Surface object is mapped to the SketchUp::Face object.  For each of these
# mappings, an OpenStudio::DrawingInterface object is used to keep the OpenStudio::ModelObject in sync
# with the associated SketchUp::Entity.  The OpenStudio::DrawingInterface maintains a pointer to the
# SketchUp::Entity as well as the OpenStudio::ModelObject. The OpenStudio::DrawingInterface is the class which
# handles most updates to either the SketchUp::Entity or OpenStudio::ModelObject.  Updates to SketchUp::Entities
# are handled in OpenStudio::Observer classes.

# The SketchUp::Entity stores string ids for 
# both the OpenStudio::DrawingInterface and OpenStudio::ModelObject associated with it in its 'OpenStudio'
# attribute dictionary.  The associations stored in SketchUp::Entity are primarily used to restore the 
# model on reload.  The OpenStudio::Model is persisted on disk and a file path is stored within the SketchUp
# model so the two can be reassociated on reload.
#
# The OpenStudio::Plugin is a singleton object which is started when the plugin is loaded.
# This object allows access to all of the current OpenStudio::DrawingInterface and OpenStudio::Model
# objects.  Because each OpenStudio::Model corresponds to a OpenStudio::Model, access to these models
# should come through the OpenStudio::Plugin.model_manager.model_interface function.


