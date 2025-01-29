ps -ax -o pid,stat,comm,args | grep 'build/ProcessCon build/ProcessController' | grep -v grep | awk '{print $1}' | xargs -In kill -9 n


ps | grep lengthy_process | grep -v grep | awk '{print $1}' | xargs -I x kill x
