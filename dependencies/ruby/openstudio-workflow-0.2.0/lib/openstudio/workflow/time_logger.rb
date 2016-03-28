# Class to store run times in a useful structure. Data are stored in a hash based on a the channel name
# There is no concept of multi-levels. The onus is on the user to make sure that they don't add a value to the
# logger that may be a level.
class TimeLogger
  attr_reader :channels

  def initialize
    @logger = []
    @channels = {}
  end

  # name of the moniker that you are tracking. If the name is already in use, then it restarts the timer.
  def start(channel)
    # warning -- "will reset timer for #{moniker}" if @monikers.key? moniker
    s = ::Time.now
    @channels[channel] = { start_time_str: "#{s}", start_time: s.to_f }
  end

  def stop(channel)
    end_time = ::Time.now.to_f
    @logger << {
      channel: channel,
      start_time: @channels[channel][:start_time],
      start_time_str: @channels[channel][:start_time_str],
      end_time: end_time,
      delta: end_time - @channels[channel][:start_time]
    }

    # remove the channel
    @channels.delete(channel) if @channels.key? channel
  end

  def stop_all
    @channels.each_key do |channel|
      stop(channel)
    end
  end

  # return the entire report
  def report
    @logger
  end

  # this will report all the values for all the channels with this name.
  def delta(channel)
    @logger.map { |k| { channel.to_s => k[:delta] } if k[:channel] == channel }.compact
  end

  # save the data to a file. This will overwrite the file if it already exists
  def save(filename)
    File.open(filename, 'w') { |f| f << JSON.pretty_generate(@logger) }
  end
end
