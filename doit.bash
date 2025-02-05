doit () 
{ 
  set -- *;
  while (($#)); do
    c="$1";
    shift;
    if [ -d "$c" ]; then
      set -- "$@" $_/*;
    else
      echo "$_";
    fi;
  done
}
