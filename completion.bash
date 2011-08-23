_blam_complete() {
    local cur prev lists
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"

    lists=`blam | awk '{print $1}'`

    case "${prev}" in
      blam)
        COMPREPLY=( $(compgen -W "${lists}" -- ${cur}) )
        return 0
        ;;
      *)
        for ((i = 0; i < ${#lists}; i++)); do
          local items=`blam $prev | awk '{print $1}' | sed -e 's/://'`
          COMPREPLY=( $(compgen -W "${items}" -- ${cur}))
          return 0
        done
        ;;
    esac
}
complete -F _blam_complete blam
