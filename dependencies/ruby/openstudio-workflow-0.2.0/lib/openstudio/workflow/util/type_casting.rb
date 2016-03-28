module OpenStudio
  module Workflow
    module Util

      # Contains the method {#os_typecasting} to translate arguments into pure-ruby objects
      #
      module TypeCasting

        # Take the OS Argument type and map it correctly to the argument value.
        #     OPENSTUDIO_ENUM( OSArgumentType,
        #     ((Boolean)(Bool)(0))
        #     ((Double)(Double)(1))
        #     ((Quantity)(Quantity)(2))
        #     ((Integer)(Int)(3))
        #     ((String)(String)(4))
        #     ((Choice)(Choice)(5))
        #     ((Path)(Path)(6))
        #     );
        #
        # @param [String] os_argument_name The string of the argument to check
        # @param [OSArgumentMap] user_arguments Map of the arguments to check
        # @return [String, Bool, Double, Integer] Returns a value cast into pure-ruby
        #
        def cast_os_type(os_argument_name, user_arguments)
          out = nil
          user_arguments.each do |arg_name, arg|
            # get the type cast value
            next unless os_argument_name == arg_name

            case arg.type.valueName
              when 'Boolean'
                out = arg.valueAsBool if arg.hasValue
              when 'Double'
                out = arg.valueAsDouble if arg.hasValue
              when 'Quantity'
                Workflow.logger.warn 'This OpenStudio Argument Type is deprecated'
              when 'Integer'
                out = arg.valueAsInteger if arg.hasValue
              when 'String'
                out = arg.valueAsString if arg.hasValue
              when 'Choice'
                out = arg.valueAsString if arg.hasValue
              when 'Path'
                out = arg.valueAsPath.to_s if arg.hasValue
            end
          end

          out
        end
      end
    end
  end
end