var utils = require('../modules/Utils');


exports.index = function(_Database){
 	//get all events and list them in the db
 	//retrieve all events

	return function(req,res){
	 	//retrieve all events

	 	//get request
	 	var type = req.query.type;

	 	if(typeof type === 'undefined' || type === "") type = 'default';
	 	//console.log(utils.toTitleCase(type))
	 	var title = utils.reverseSlug(type)
	 	//query scenes by event type

	 	_Database.queryCollectionWithOptions('scenes', {type: type} , {sort: 'order'} ,function(err, _scenes){
	 		//error handling needs to happen

		 	res.render('events/index', { current: req.url, title: utils.toTitleCase(title), page_slug: 'events-index', scenes: _scenes } );


	 	});
	}
}
