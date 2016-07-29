#! /bin/sh
test –f /usr/bin/sshd || exit 0
case “$1” in
    start)
            echo –n “Starting sshd: sshd”
            /usr/sbin/sshd
            echo “.”
            ;;
   stop) 
          echo –n “Stopping sshd: sshd”
          kill `cat /var/run/sshd.pid`
          echo “.”
           ;;
   restart)
          echo –n “Stopping sshd: sshd”
          kill `cat /var/run/sshd.pid`
          echo “.”
          echo –n “Starting sshd: sshd”
            /usr/sbin/sshd
            echo “.”
            ;;
        *)
          echo “Usage: /fms-service start|stop|restart”
          exit 1
          ;;
    esac