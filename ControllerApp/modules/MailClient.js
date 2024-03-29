var nodemailer = require("nodemailer");

/**
 *
 * Node Mailer Setup
 *
 */

var transport = nodemailer.createTransport("SMTP", {
    host: "smtp.gmail.com", // hostname
    secureConnection: true, // use SSL
    port: 465, // port for secure SMTP
    auth: {
        user: "30pp.installation@incrediblemachines.net",
        pass: "r0b0tz4eva"
    }
});

var mailOptions = {
    from: "30PP Installation <30pp.installation@incrediblemachines.net>", // sender address
    to: "30PP Team <30pp@incrediblemachines.net>", // list of receivers
    replyTo: "30PP Team <30pp@incrediblemachines.net>",
    //subject: "ControllerApp Starting", // Subject line
    //text: "This is an automated message to inform you that the ControllerApp is starting at "+ new Date() // plaintext body
}

var DEVELOPMENT = true;


exports.send = function(subject,text,cb){

  mailOptions.subject = subject
  mailOptions.text = text
  if(DEVELOPMENT == true){
    cb()
  }else{
    transport.sendMail(mailOptions,function(e,resp){
      if(e) cb(e)
      else cb(null,resp)
    })
  }
}
