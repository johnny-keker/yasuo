 #!/bin/bash

declare -r TRUE=0
declare -r FALSE=1


function print_owner_name
{
  getent passwd `ls -n | grep " $1$" | awk '{print $3}'` | awk -F ':' '{print $1}'
}

function print_group_members
{
  for name in $(getent group `ls -n | grep " $1$" | awk '{print $4}'` | awk -F ':' '{print $4}' | tr "," "\n")
  do
    echo $name
  done
}

function print_others
{
  getent passwd | awk -F ':' '{print $1}'
}

function can_owner_search
{
  user_permission=`stat -c "%a" $1 | cut -c1`
  [ $user_permission -eq 5 -o $user_permission -eq 7 ] && return $TRUE || return $FALSE
}

function can_group_search
{
  group_permission=`stat -c "%a" $1 | cut -c2`
  [ $group_permission -eq 5 -o $group_permission -eq 7 ] && return $TRUE || return $FALSE
}

function can_others_search
{
  others_permission=`stat -c "%a" $1 | cut -c3`
  [ $others_permission -eq 5 -o $others_permission -eq 7 ] && return $TRUE || return $FALSE
}

can_owner_search $1 && print_owner_name $1 |
can_group_search $1 && print_group_members $1 |
can_others_search $1 && print_others $1 |
sort | uniq
