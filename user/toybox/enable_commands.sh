#!/bin/bash
# Enable minimal set of commands
sed -i 's/^# CONFIG_CAT is not set$/CONFIG_CAT=y/' .config
sed -i 's/^# CONFIG_ECHO is not set$/CONFIG_ECHO=y/' .config
sed -i 's/^# CONFIG_LS is not set$/CONFIG_LS=y/' .config
sed -i 's/^# CONFIG_CP is not set$/CONFIG_CP=y/' .config
sed -i 's/^# CONFIG_MV is not set$/CONFIG_MV=y/' .config
sed -i 's/^# CONFIG_RM is not set$/CONFIG_RM=y/' .config
sed -i 's/^# CONFIG_MKDIR is not set$/CONFIG_MKDIR=y/' .config
sed -i 's/^# CONFIG_PWD is not set$/CONFIG_PWD=y/' .config
sed -i 's/^# CONFIG_DMESG is not set$/CONFIG_DMESG=y/' .config
sed -i 's/^# CONFIG_SYNC is not set$/CONFIG_SYNC=y/' .config
