var spawn = require('child_process').spawn,
    exec = require('child_process').exec,
    folders = require('../modules/FolderStructure')

//globals

var player,
    status,
    start;

exports.index = {

  start : function(cb){
      var options = []
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
      start = new Date()
      status = true;
      player.stdout.on('data', function (data) {

        console.log('  PID: %s '.inverse+' %s '.grey, player.pid, data)
      })

      player.stderr.on('data', function (data) {
        console.log(' PID: %s '.inverse+' '+' stderr: '.inverse.red+' %s '.grey, player.pid, data)
        bError = true
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
        //callback once the process has finished

      })
  },
  getStatus: function(){
    return status
  },
  end: function(){

    player.kill('SIGHUP')
    status = false
  }

}
