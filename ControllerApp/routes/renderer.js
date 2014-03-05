

exports.index = function(_Database){
	
	return function(req,res){
		_Database.queryCollection('scenes',{render: true},function(e,queue){
			//console.log(data)
			if(e){ res.jsonp({"Error": "Query Collection Error: Events"})
			}else{
				_Database.getAll('events',function(_e, events){
					if(_e) res.jsonp({"Error": "Query Collection Error: Events"})
				
					else res.render('renderqueue/index', { current: req.url, title: 'Render Queue', page_slug: 'renderqueue-index', renderqueue: queue, events: events } );	
				})
			}
		})
		
	}
}