########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
#  following disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
#  products derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
#  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
#  specific prior written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
#  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
#  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

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


