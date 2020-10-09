#!/bin/bash
##########################################################
# File Name		: ssh_run_cmd.sh
# Author		: winddoing
# Created Time	: 2020年10月09日 星期五 14时39分53秒
# Description	:
##########################################################

run_cmd_shell=$(tempfile)
cat > $run_cmd_shell << EOF
#!/bin/bash

ssh root@192.168.101.55 'pwd; ls; sleep 10'
EOF
echo "Run cmd shell: $run_cmd_shell"

for win in {1..5}
do
    gnome-terminal -t "win-$win" --window -e \
        "bash ${run_cmd_shell}"
done

