#!/bin/bash

wget http://git.ingenic.com.cn:8082/bj/repo

./repo init -u http://git1.ingenic.com.cn:8082/gerrit/mipsia/platform/manifest.git -b ingenic-android-lollipop_mr1-kernel3.10.14-newton2_plus-v3.0-20160908
./repo sync
./repo forall -c "git reset --hard ingenic-android-lollipop_mr1-kernel3.10.14-newton2_plus-v3.0-20160908"


./repo sync
while [ $? -ne 0 ]
do
    ./repo sync
done
