exports.index =function(_Database){
  return function(req,res,next){
    //here we check if a user has access to this page
    if( (req.session.auth == 1 && req.session.name == 'admin') || (req.session.auth == 0 && req.session.name=='user')){
      next()
    }else{
      res.redirect('/login')
    }
  }
}
