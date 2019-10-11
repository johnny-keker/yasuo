 #!/bin/bash

declare -r TRUE=0
declare -r FALSE=1


function get_owner_name
{
  getent passwd `ls -n | grep " $1$" | awk '{print $3}'` | awk -F ':' '{print $1}'
}

function can_owner_search
{
  user_permission=`stat -c "%a" $1 | cut -c1`
  [ $user_permission -eq 5 -o $user_permission -eq 7 ] && return $TRUE || return $FALSE
}

can_owner_search $1 && echo yes || echo no
