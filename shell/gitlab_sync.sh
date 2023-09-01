#!/bin/bash
##########################################################
# Copyright (C) 2023 wqshao All rights reserved.
#  File Name    : gitlab_sync.sh
#  Author       : wqshao
#  Created Time : 2023-06-26 16:29:05
#  Description  :
#	 sudo apt install jq
##########################################################


TOP=$(pwd)

echo "TOP: $TOP"

gitlab_ip="172.16.200.191"
gitlab_token="yCWrjkgBRM7RYtzfLscp"

groub_name="rk3399"
GB_ID=208

create_project()
{
	local project=$1
	local groub_id=$2
	local info="name=$project&path=$project&wiki_enabled=no&public_jobs=true&public=true&namespace_id=$groub_id&default_branch=master&private_token=$gitlab_token"

	echo "$0: project=$project, groub_id=$groub_id"

	set -x
	curl -d $info "http://$gitlab_ip/api/v4/projects"
	set +x
}

push_project()
{
	local repo_path=$1

	echo "$0: repo_path=$repo_path"

	set -x
	cd $TOP$repo_path
	git push --mirror git@$gitlab_ip:$groub_name$repo_path
	cd -
	set +x
}

create_subgroup()
{
	local subgroub=$1
	local groub_id=$2
	echo "$0: subgroub=$subgroub, groub_id=$groub_id"
	set -x
	GB_ID=$(curl --request POST --header "PRIVATE-TOKEN: $gitlab_token" --header "Content-Type: application/json" --data '{"path": "'$subgroub'", "name": "'$subgroub'", "parent_id": '$groub_id' }' "http://$gitlab_ip/api/v4/groups/" | jq -r '.id')
	set +x
}


create_sync()
{
	local cur_dir=$1
	local dir_path=$1
	local groub_id=$2

	echo "--- cur_dir=$cur_dir"

	for dir in `ls $cur_dir`
	do
		dir_path="$cur_dir/$dir"
		echo "Files: $dir, dir_path: $dir_path"
		if [ ! -d $dir_path ]; then
			continue
		fi
		echo "Dir: $dir_path"

		if [ "${dir_path##*.}"x = "git"x ]; then
			pro_name=$(echo ${dir%%.git})
			repo_path=$(echo ${dir_path#$TOP})
			echo "### Create project. $dir, pro_name=$pro_name, repo_path=$repo_path"
			create_project $pro_name
			push_project $repo_path
		else
			echo "### Create subgroub. $dir, groub_id=$groub_id"
			create_subgroup $dir $groub_id
			create_sync $dir_path $GB_ID
		fi

	done
}

create_sync $TOP $GB_ID
