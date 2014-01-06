var utils = require('../modules/Utils');


//Routes for our event functionality

exports.index = function(_Database){
 	//get all events and list them in the db
 	//retrieve all events

	return function(req,res){
	 	//retrieve all events
	 	_Database.getAll('events',function(err, _events){
		 	res.render('events/index', { title: 'Events', page_slug: 'event-index', events: _events } );
	 	});
	}
}
//post for adding an event.
exports.add = function(_Database){
	//add a new event into the database
	return function(req,res){
		//where does the user get sent?
		//AJAX res serves jsonp
		
		console.log('Event Add Request')
		//console.log(req.body);
		var post = req.body;
		/*******************
		Sample Post Body: 
		
			{ 	title: '',
				anim_type: 'anim_0',
				duration: '35',
				duration_view: '35 seconds' 
				}
		*
		*/
		delete post.duration_view;
		post.slug = utils.makeSlug(post.title);
		if(!post.hasOwnProperty('assets')) post.assets =[];
		post.created_at = new Date();
		console.log(post)
		
		_Database.add(post,'events',function(_a, e){
			
			if(!e) res.jsonp(_a)
			else res.jsonp(500,e);
		})
		//res.render('events/index', { title: 'Event Added', page_slug: 'event-index event-added',events:[] });
	}
}

exports.single = function(_Database){

	return function(req,res){
		res.render('events/index', { title: 'single event', page_slug: 'event-single', events: [] });
	}
}

exports.delete = function(_Database){

	return function(req,res){
		//res.render('events/index', { title: 'add event' });
	}
}

exports.update = function(_Database){

	return function(req,res){
		res.render('events/index', { title: 'add event' });
	}
}

exports.emitOne = function(_Database){

	return function(req,res){
		res.render('events/index', { title: 'add event' });
	}
}