
# test Dir glob
no_block_glob = Dir["*.txt", "*.rb"]

block_glob = []
Dir["*.txt", "*.rb"].each do |p|
  block_glob << p
end

no_block_glob = Dir.glob(["*.txt", "*.rb"])

no_block_glob = Dir.glob("*.txt")