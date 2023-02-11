#/usr/bin/env bash
_BetterMain_completions()
{
    local args=("-h --help -e --execName")
    local file_args=("-o --output")
    local user_args=("-u --user")
    local group_args=("-g --group")
    local curr_arg;
    if [[ ${#COMP_WORDS[@]} -ge 1 ]]; then
        local curr_arg="${COMP_WORDS[COMP_CWORD]}"

        if [[ ${#COMP_WORDS[@]} -ge 2 ]]; then
            local prev_arg="${COMP_WORDS[COMP_CWORD-1]}"
            if [[ " ${file_args[*]} " =~ " ${prev_arg} " ]]; then
                COMPREPLY=($(compgen -f -- "${curr_arg}"))
                return 0
            fi
            if [[ " ${user_args[*]} " =~ " ${prev_arg} " ]]; then
                COMPREPLY=($(compgen -u -- "${curr_arg}"))
                return 0
            fi
            if [[ " ${group_args[*]} " =~ " ${prev_arg} " ]]; then
                COMPREPLY=($(compgen -g -- "${curr_arg}"))
                return 0
            fi
        fi

        if [[ ${curr_arg:0:1} == "-" ]]; then
            COMPREPLY=($(compgen -W "${args[*]} ${file_args[*]} ${user_args[*]} " -- "${curr_arg}"))
            return 0
        fi

        if [[ -z "${curr_arg}" ]]; then
            COMPREPLY=($(compgen -f))
            return 0
        fi

        COMPREPLY=($(compgen -f -- "${curr_arg}"))
    fi
    return 0
}
complete -o filenames -F _BetterMain_completions BetterMain
