var spawn = require('child_process').spawn,
    exec = require('child_process').exec,
    folders = require('../modules/FolderStructure')
    Mailer = require('../modules/MailClient')

//globals

var player,
    status = false,
    start,
    bError,
    bClose = false,
    bRestart = false,
    pID;

exports.index = {

  start : function(_Mailer,cb){
      if(status ==false){
        var options = [""]
        var _this = this
        //console.log(folders.root())
        // var child = exec(folders.root()+'/../PlayerApp/bin/30PP_MapperDebug.app/Contents/MacOS/30PP_MapperDebug',
        //     function (error, stdout, stderr) {
        //       console.log('stdout: ' + stdout);
        //       console.log('stderr: ' + stderr);
        //       if (error !== null) {
        //       console.log('exec error: ' + error);
        //     }
        //   })
        player = spawn(folders.root()+'/../PlayerApp/bin/30PP_MapperDebug.app/Contents/MacOS/30PP_MapperDebug', options)
        //var child = exec("osascript -e 'tell application \"30PP_MapperDebug\" to activate'")
        //player = spawn(folders.root()+'/../PlayerApp/bin/30PP_MapperDebug.app', options)

        //setTimeout(this.activate(player.pid),1000)
        pID = player.pid
        start = new Date()
        status = true;
        player.stdout.on('data', function (data) {

          console.log('  PID: %s '.inverse+' %s '.grey, player.pid, data)
        })

        player.stderr.on('data', function (data) {
          console.log(' PID: %s '.inverse+' '+' stderr: '.inverse.red+' %s '.grey, player.pid, data)
          //bError = true
        })

        player.on('error',function(error){
          console.error('error: '+error)
          bError = true;
        })
        player.on('close', function (code) {
          console.log('PlayerApp PID: %s '.inverse+' exited with code %s '.cyan, player.pid, code)
          if(code !=0) bError = true;
          var end = new Date()
          var duration = end-start
          duration = duration/1000
          duration = duration/60
          console.log('PlayerApp PID: %s '.inverse+' Completed in %s minutes '.green, player.pid, duration)
          status = false
          if(bClose == false){
            console.log(' PlayerApp Rebooting... '.inverse.green)
            _this.start()

            if(bRestart == true){
              var subject = "[30PP] PlayerApp has Restarted Itself"
              var message = "This is an automated message to inform you that the PlayerApp as signaled that it needs to be restarted."
            }else{
              var subject = "[30PP] PlayerApp has Crashed"
              var message = "This is an automated message to inform you that the PlayerApp has unexpectedly quit and been restarted by the ControllerApp."
            }
            Mailer.send(subject,message,function(e,resp){
              if(e) console.error(e)
            })

          }else{
            var subject = "[30PP] PlayerApp has been Closed"
            var message = "This is an automated message to inform you that the PlayerApp has been intentionally closed."
            Mailer.send(subject,message,function(e,resp){
              if(e) console.error(e)
            })
          }
          bClose = false //reset the closeState
          bRestart = false
        })
      }
  },
  activate: function(){
    //make the PlayerApp Process be the first thing that runs
    var child = exec("osascript -e 'tell application \"System Events\" ' -e 'set frontmost of the first process whose unix id is "+pID+" to true' -e 'end tell' ",
    function (error, stdout, stderr) {
      if(stdout) console.log('stdout: ' + stdout);
      if(stderr) console.log('stderr: ' + stderr);
      if(error)  console.log('exec error: ' + error);
      if(!stdout && !stderr && !error) console.log(" PlayerApp pID: %s Activated ".inverse.green, pID)
    })
  },
  getStatus: function(){
    return status
  },
  closeState: function(state){
    bClose = state
  },
  restart: function(state){
    bRestart = state
  },
  end: function(){

    player.kill('SIGHUP')
    status = false
  }

}
