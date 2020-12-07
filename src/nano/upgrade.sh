commit_sha=eafbceee9d8b4c56996e9e221bbf6bbae3550a73

declare -a files=(
"LICENSE"
"README.md"
"nano_function.hpp"
"nano_mutex.hpp"
"nano_observer.hpp"
"nano_signal_slot.hpp"
)

for filename in "${files[@]}"; do
  wget -O $filename https://raw.githubusercontent.com/NoAvailableAlias/nano-signal-slot/$commit_sha/$filename
done
