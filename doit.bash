doit () 
{
  (
  set -- *;
  shopt -s nullglob
  while (($#)); do
    c="$1";
    shift;
    if [ -d "$c" ]; then
      set -- "$@" $c/*;
    else
      echo "$c";
    fi;
  done
)
}
return 0 2>/dev/null
doit "$@"
