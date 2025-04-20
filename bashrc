alias ll='ls -lh'
export PATH="$PATH:./"

function cd { builtin cd $* && pwd && ll -CF; }
