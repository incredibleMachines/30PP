exports.index =function(_Database){
  return function(req,res,next){

    //here we check if a user has access to this page
    if(req.session.auth){

    }else{
      res.redirect('/login')
    }
  }
}
