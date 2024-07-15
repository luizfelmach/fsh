#!/bin/bash

shell=zsh

# Obtém os PIDs dos processos zsh
pids=($(pgrep $shell))

# Verifica se há processos $shell em execução
if [ ${#pids[@]} -eq 0 ]; then
    echo "Nenhum processo $shell encontrado."
    exit 1
fi

echo "Escolha um PID dos processos $shell em execução:"

# Cria um menu com os PIDs
select pid in "${pids[@]}" "Sair"; do
    case $pid in
        "Sair")
            echo "Saindo..."
            break
            ;;
        *)
            if [[ " ${pids[@]} " =~ " $pid " ]]; then
                watch -d -t -n 0.1 ps --forest -o pid,ppid,pgid,pgrp,sid,wchan,tty,stat,time,cmd -g $pid
            else
                echo "PID inválido. Tente novamente."
            fi
            ;;
    esac
done





