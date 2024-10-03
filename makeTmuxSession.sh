#!/bin/bash

SESSION_NAME="$1"

if tmux has-session -t $SESSION_NAME 2>/dev/null; then
    echo "Session $SESSION_NAME already exists. Attaching to it."
    tmux attach-session -t $SESSION_NAME
else
    tmux new-session -d -s $SESSION_NAME
    tmux rename-window -t 0 nvim
    tmux send-keys -t 0 'cd src' C-m
    tmux send-keys -t 0 'nvim main.c' C-m
    tmux new-window
    tmux rename-window -t 1 build
    tmux send-keys -t 1 'cd build' C-m
    tmux attach-session -t $SESSION_NAME
fi

