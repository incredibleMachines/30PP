
var keys = require('../modules/keys').index
  var crypto = require('crypto')

exports.index =function(_Database){
  return function(req,res,next){
    var auth = false;
    if(req.session.auth){
      console.log(keys)
      //here we check if a user has access to this page
      var counter = 0
      for(var i = 0; i< keys.length; i++){
        for(var key in keys[i]){

          console.log(req.session)
          var decipher = crypto.createDecipher('aes256','30_PP')
          var deciphertext = decipher.update(req.session.auth)
              deciphertext += decipher.final()
          //console.log(deciphertext)
          var string = keys[i][key]+" "+req.session.timestamp
          //console.log(string)
          console.log(key +" "+ req.session.name)
          console.log(deciphertext +" "+string)
          if( string === deciphertext && key == req.session.name) {
            console.log('here')
            auth = true
            //next()
          }

        }

        counter++
        if(counter == keys.length){
          if(auth == true) next()
          else res.redirect('/login')
        }
      }
    }else{
      res.redirect('/login')
    }
  }
}
