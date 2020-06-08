# Do not define "name" var !
AUTHOR="$USER"
DATE="$(date +'%d/%m/%Y')"
SOCIETY="NULL"
GIT_BRANCH="$(git branch 2>/dev/null | grep '^*' | colrm 1 2)"
