declare -r TRUE=0
declare -r FALSE=1

good_boys=()  # Users that match criteria
bad_boys=()   # Users that don't

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

owner=$(getent passwd `ls -n | grep " $1$" | awk '{print $3}'` | awk -F ':' '{print $1}')
group_members=$(getent group `ls -n | grep " $1$" | awk '{print $4}'` | awk -F ':' '{print $4}' | tr "," " ")
others_str=$(getent passwd | awk -F ':' '{print $1}')
IFS=' '; others=($others_str); unset IFS;

can_owner_search $1 && good_boys=( "$owner" ) || bad_boys=( "$owner" )
can_group_search $1 && good_boys=( "${good_boys1[@]}" "${group_members[@]}" ) || bad_boys=( "${bad_boys[@]}" "${group_members[@]}" )
can_others_search $1 && good_boys=( "${good_boys2[@]}" "${others[@]}" ) || bad_boys=( "${bad_boys[@]}" "${others[@]}" )


for boy in ${good_boys[@]}
do
  if [[ "${bad_boys[@]}" =~ "${boy}" ]]; then
    continue
  fi
  echo $boy
done
