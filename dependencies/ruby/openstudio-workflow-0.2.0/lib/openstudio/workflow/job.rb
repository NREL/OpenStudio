module OpenStudio
  module Workflow
    class Job
      # @todo (rhorsey) adapter is for output? - DLM
      # @todo (rhorsey) registry should be a member of WorkflowRunner, pass WorkflowRunner in here instead - DLM
      def initialize(adapter, registry, options = {}, defaults = nil)
        defaults ||= {debug: false}
        @options = defaults.merge(options)
        @adapter = adapter
        @registry = registry
        @logger = @registry[:logger]
        @results = {}

        @logger.info "#{self.class} passed the following options #{@options}"
        @logger.info "#{self.class} passed the following registry #{@registry.to_hash}" if @options[:debug]
      end
    end

    def self.new_class(current_job, adapter, registry, options = {})
      new_job = Object.const_get(current_job).new(adapter, registry, options)
      return new_job
    end
  end
end
