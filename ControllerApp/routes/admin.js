exports.login = function(_Database){
  return function(req,res){
    //check the un and pw on the db
    var post = req.body;

    //if it matches create a user session
    //otherwise don't log them in


  }
}

exports.logout = function(_Database){
  return function(req,res){
    //delete the session
  }
}
