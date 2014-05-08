var querystring = require('querystring')
var http = require('http')

var hostnames = ['10.1.30.22', '10.1.30.23','10.1.30.24']
var options = {

    port: 80,
    auth: 'NeighborhoodRoom:30P4rkPl4c3'

}

exports.on = function(req,res){

  var post_data = querystring.stringify({
      'lang' : 'e',
      'Submit.x': '38',
      'Submit.y': '6'
  });

  options.method = "POST"
  options.path = "/cgi-bin/power_on.cgi"

  var responses = ''
  var c =0;
  hostnames.forEach(function(host){

    options.hostname = host

    options.headers = {
            'Content-Type': 'application/x-www-form-urlencoded',
            'Content-Length': post_data.length,
            'Origin': 'http://'+host,
            'Referer': 'http://'+host+'/cgi-bin/power_ctl.cgi?key=pow_on&lang=e&from=BASIC_CTL'

        }

    console.log("Requesting "+host)
    var req = http.request(options,function(res){
      res.on('data',function(d){
        responses += d +'\n\n'
        console.log(d)
      })
    })
    req.on('error', function(e) {
      console.log('[REQ ERROR] '+host+' request: ' + e.message);
    });
    req.write(post_data)
    req.end();
    c++;
    if(c==hostnames.length) res.send(responses)
  })


}

exports.off = function(req,res){
  var post_data = querystring.stringify({
      'lang' : 'e',
      'Submit.x': '38',
      'Submit.y': '6'
  });
  //standby
  options.method = "POST"
  options.path = "/cgi-bin/power_off.cgi"

  var responses = ''
  var c =0;
  hostnames.forEach(function(host){

    options.hostname = host

    options.headers = {
            'Content-Type': 'application/x-www-form-urlencoded',
            'Content-Length': post_data.length,
            'Origin': 'http://'+host,
            'Referer': 'http://'+host+'/cgi-bin/power_ctl.cgi?key=pow_off&lang=e&from=BASIC_CTL'

        }

    console.log("Requesting "+host)
    var req = http.request(options,function(res){
      res.on('data',function(d){
        responses += d +'\n\n'
        console.log(d)
      })
    })
    req.on('error', function(e) {
      console.log('[REQ ERROR] '+host+' request: ' + e.message);
    });
    req.end();
    c++;
    if(c==hostnames.length) res.send(responses)
  })

}

exports.hdmi = function(req,res){
  options.method="GET"
  options.path = "/cgi-bin/proj_ctl.cgi?key=hdmi&lang=e&osd=off"
  delete options.headers;

  var responses = ''
  var c =0;
  hostnames.forEach(function(host){

    options.hostname = host
    console.log("Requesting "+host)
    var req = http.request(options,function(res){
      res.on('data',function(d){
        responses += d +'\n\n'
        console.log(d)
      })
    })
    req.on('error', function(e) {
      console.log('[REQ ERROR] '+host+' request: ' + e.message);
    });
    req.end();
    c++;
    if(c==hostnames.length) res.send(responses)
  })

}
