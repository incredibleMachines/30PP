
var keys = require('../modules/keys').index
var crypto = require('crypto')
exports.index = function(req,res){

  res.render('admin/login',{})

}
exports.login = function(_Database){
  return function(req,res){
    //check the un and pw on the db
    var post = req.body;
    //console.log(post)
    //console.log(keys)
    var counter = 0;
    for(var i = 0; i< keys.length; i++){
      for(var key in keys[i]){
        //console.log(keys)
        if( keys[i][key] == post.creds && key == post.name){
          console.log('Login Success')
          req.session.name = post.name
          req.session.timestamp = new Date().getTime();
          var cipher = crypto.createCipher('aes256','30_PP')
          var ciphertext = cipher.update(keys[i][key]+" "+req.session.timestamp, 'utf-8','binary')
          req.session.auth = ciphertext+cipher.final('binary')
          console.log(req.session)
          res.redirect('/')
        }
        counter++
        if(counter == keys.length){
          console.log('Login Fail')
          //console.log(post)
          //res.jsonp({login:'here'})
          res.redirect('/')
        }
      }
    }
    // if( (post.name == 'admin' && post.creds == 'adminpass')
    //    || (post.name == 'user' && post.creds == 'userpass') ){
    //   console.log('Login Success')
    //   //console.log(post)
    //   req.session.name = post.name
    //   req.session.timestamp = new Date()
    //   req.session.auth = (post.name == 'admin')? 1:0
    //   res.redirect('/')
    // }else{
    //   console.log('Login Fail')
    //   //console.log(post)
    //   //res.jsonp({login:'here'})
    //   res.redirect('/')
    // }


    //if it matches create a user session
    //otherwise don't log them in


  }
}

exports.logout = function(_Database){
  return function(req,res){

    //delete the session
    req.session.destroy()
    //req.session.destroy //delete a database
    res.redirect('/')

  }
}
