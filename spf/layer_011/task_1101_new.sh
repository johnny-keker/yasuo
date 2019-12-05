declare -r TRUE=0
declare -r FALSE=1

good_boys=()  # Users that match criteria

IFS=$'\n'
# iterate over files
for item in `ls -1`
do
  good_boys=()
  # skip all items except from files
  if [ ! -f "$item" ]; then
    continue
  fi

  owner_name=$(getent passwd `ls -n | grep " $item$" | nawk '{print $3}'` | nawk -F ':' '{print $1}')
  owner_gid=$(ls -n | grep " $item$" | nawk '{print $4}')

  group_str=$(getent group `ls -n | grep " $item$" | nawk '{print $4}'` | nawk -F ':' '{print $4}' | tr "," " ")
  IFS=' '; group_members=($group_str); unset IFS;
  others_str=$(getent passwd | nawk -F ':' '{print $1}')
  IFS=' '; others=($others_str); unset IFS;



  for acl in $(/bin/ls -V "$item" | (read; cat) | tr -d " \t")
  do
    if [[ "$acl" == *"owner@"* ]]; then
      user_acl=$acl
      user_per=$(nawk -F ':' '{print $(NF-2)}' <<< $acl)
    fi
    if [[ "$acl" == *"group@"* ]]; then
      g_acl=$acl
      g_per=$(nawk -F ':' '{print $(NF-2)}' <<< $acl)
    fi
    if [[ "$acl" == *"everyone@"* ]]; then
      o_acl=$acl
      o_per=$(nawk -F ':' '{print $(NF-2)}' <<< $acl)
    fi
  done

  # check owner
  if [ "$USER" == "$owner_name" ]; then
    if [[ $user_per =~ "p" ]] && [[ ! $user_per =~ "w" ]]; then
      good_boys=( "$owner_name" )
    fi
  fi

  # Check group
  if [[ `/usr/xpg4/bin/id -G "$USER"` == *"$owner_gid"* ]]; then
    if [[ ! $g_per =~ "w" ]] && [[ $g_per =~ "p" ]]; then
      good_boys=( "${good_boys[@]}" "${group_members[@]}" )
    fi
  fi

  if [[ ! $o_per =~ "w" ]] && [[ $o_per =~ "p" ]]; then
    good_boys=( "${good_boys[@]}" "${others[@]}" )
  fi
  if [[ "${good_boys[@]}" =~ "$USER" ]]; then
    echo $item
  fi
done
