#!/bin/bash
#
# Start Forever On Unix with Launchctl
#

/usr/local/bin/node /usr/local/bin/forever start -a -o ~/.forever/logs/30PPControllerAppOutput.log -l ~/.forever/logs/30PPControllerApp.log -e ~/.forever/logs/30PPControllerAppErrors.log ~/oF/081/apps/30PP/ControllerApp/app.js
