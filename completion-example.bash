#/usr/bin/env bash
_BetterMain_completions()
{
    local file_arg=("-i -o -c --input --output --config");
    local args=("-v --verbose");
    local prev_arg;
    local curr_arg;

    if [[ ${#COMP_WORDS[@]} -ge 2 ]]; then
        prev_arg="${COMP_WORDS[COMP_CWORD-1]}"
        curr_arg="${COMP_WORDS[COMP_CWORD]}"

      if [[ " ${file_arg[*]} " =~ " ${prev_arg} " ]]; then
          COMPREPLY=($(compgen -f -- "${curr_arg}"))
      else
          COMPREPLY=($(compgen -W "${file_arg[*]} ${args[*]}" -- "${curr_arg}"))
      fi
    fi
}

complete -o filenames -F _BetterMain_completions BetterMain
