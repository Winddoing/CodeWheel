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
gitlab_token="ee2bcRexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"

groub_name="tst1"
GB_ID=309

create_project()
{
	local project=$1
	local groub_id=$GB_ID
	local info="name=$project&path=$project&wiki_enabled=no&public_jobs=true&public=true&namespace_id=$groub_id&default_branch=master&private_token=$gitlab_token"
	local ret=0

	echo "$FUNCNAME: project=$project, groub_id=$groub_id"

	if [ x"$groub_id" == x"null" ]; then
		echo "$FUNCNAME :Not found parent_id. project may exist."
		return
	fi

	set -x
	curl -d $info "http://$gitlab_ip/api/v4/projects"
	ret=$?
	set +x
	if [ $ret -ne 0 ]; then
		echo "Create [$project] project failed."
		exit
	fi
}

push_project()
{
	local repo_path=$1
	local ret=0

	echo "$FUNCNAME: repo_path=$repo_path"

	set -x
	cd $TOP$repo_path
	git push --mirror git@$gitlab_ip:$groub_name$repo_path
	ret=$?
	cd -
	set +x
	if [ $ret -ne 0 ]; then
		echo "Push [$repo_path] repo failed."
		exit
	fi
}

create_subgroup()
{
	local subgroub=$1
	local groub_id=$2
	echo "$FUNCNAME: subgroub=$subgroub, groub_id=$groub_id"

	if [ x"$groub_id" == x"null" ]; then
		echo "$FUNCNAME :Not found parent_id. subgroub may exist."
		return
	fi

	set -x
	GB_ID=$(curl --request POST --header "PRIVATE-TOKEN: $gitlab_token" --header "Content-Type: application/json" --data '{"path": "'$subgroub'", "name": "'$subgroub'", "parent_id": '$groub_id' }' "http://$gitlab_ip/api/v4/groups/" | jq -r '.id')
	set +x
	if [ -z $GB_ID ]; then
		echo "Create [$subgroub] subgroub failed."
		exit
	fi
	if [ x"$GB_ID" == x"null" ]; then
		echo "The current [$subgroub] subgroup may exist or be incorrectly configured (token, name). Please confirm..,"
		#exit
	fi
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

[ -x "$(command -v jq)" ] || echo -e "\nInstall SW: sudo apt install jq"
[ -x "$(command -v git)" ] || echo -e "\nInstall SW: sudo apt install git"
[ -x "$(command -v curl)" ] || echo -e "\nInstall SW: sudo apt install curl"

create_sync $TOP $GB_ID
