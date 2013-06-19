#!/bin/bash

killtree() {
  local _pid=$1
  local _sig=${2-TERM}
#  echo "Stopping  ${_pid}"
  kill -stop ${_pid} # needed to stop quickly forking parent from producing child between child killing and parent killing
  local _regex="[ ]*([0-9]+)[ ]+${_pid}"

  for _child in $(ps ax -o "pid= ppid=" | grep -E "${_regex}" | sed -r "s/${_regex}/\1/g"); do
    if ["${_pid}" -ne "${_child}"]
    then
      killtree ${_child} ${_sig}
    fi
  done
#  echo "Killing -${_sig} ${_pid}"
  kill -${_sig} ${_pid}
}

if [ $# -eq 0 -o $# -gt 2 ]; then
  echo "Usage: $(basename $0) <pid> [signal]"
  exit 1
fi

killtree $@


