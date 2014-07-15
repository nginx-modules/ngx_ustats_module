#!/bin/bash

current_dir=$(dirname $0)
current_dir=$(readlink -f $current_dir)

#$current_dir/monitoring.py --url='http://wwwnew1.lan/nginx-ustats?json=1' --action=upstream-list
$current_dir/monitoring.py --url='http://wwwnew1.lan/nginx-ustats?json=1' --action=upstream-500 --tick=20 --tick-num=30 --upstream='backend_10.5.2.6:8002' --path=/tmp -v
#$current_dir/monitoring.py --url='http://wwwnew1.lan/nginx-ustats?json=1' --action=upstream-restart --tick=20 --tick-num=30 --upstream='backend_10.5.2.6:8002' --path=/tmp -v

#$current_dir/monitoring.py --url='http://www26.lan/nginx-ustats?json=1' --action=upstream-list
#$current_dir/monitoring.py --url='http://www26.lan/nginx-ustats?json=1' --action=upstream-500 --tick=20 --tick-num=30 --upstream='127.0.0.1:8002_127.0.0.1:8002' --path=/tmp
#$current_dir/monitoring.py --url='http://www26.lan/nginx-ustats?json=1' --action=upstream-restart --tick=20 --tick-num=30 --upstream='127.0.0.1:8002_127.0.0.1:8002' --path=/tmp -v