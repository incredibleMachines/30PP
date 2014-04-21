exports.index = function(req,res){

  res.render('admin/login',{})

}
exports.login = function(_Database){
  return function(req,res){
    //check the un and pw on the db
    var post = req.body;
    //console.log(post)

    if( (post.name == 'admin' && post.creds == 'adminpass') || (post.name == 'user' && post.creds == 'userpass') ){
      console.log('Login Success')
      console.log(post)
      req.session.name = post.name
      req.session.timestamp = new Date()
      req.session.auth = (post.name == 'admin')? 1:0
      res.redirect('/')
    }else{
      console.log('Login Fail')
      console.log(post)
      res.jsonp({login:'here'})

    }


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
