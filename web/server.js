// set up ======================================================================
// get all the tools we need
var express  = require('express');
var app      = express();
var port     = process.env.PORT || 80;
var mongoose = require('mongoose');
var passport = require('passport');
var flash 	 = require('connect-flash');
var configDB = require('./config/database.js');
var path = require('path');
var server = require("http").createServer(app);
var arDrone = require('ar-drone');
var arClient = arDrone.createClient();
var io = require('./lib/sockets').listen(8080, arClient);

// configuration ===============================================================
mongoose.connect(configDB.url); // connect to our database

require('./config/passport')(passport); // pass passport for configuration

app.configure(function() {

	// set up our express application
	app.use(express.logger('dev')); // log every request to the console
	app.use(express.cookieParser()); // read cookies (needed for auth)
	app.use(express.bodyParser()); // get information from html forms

	app.set('view engine', 'ejs'); // set up ejs for templating

	// required for passport
	app.use(express.session({secret: 'War Eagle'})); // session secret
	app.use(passport.initialize());
	app.use(passport.session()); // persistent login sessions
	app.use(flash()); // use connect-flash for flash messages stored in session

	app.use(express.static(path.join(__dirname, 'public')));
	
});

// routes ======================================================================
require('./app/routes.js')(app, passport); // load our routes and pass in our app and fully configured passport

// launch ======================================================================
// server.listen(port);
// console.log('The magic happens on port ' + port);

/*
 * Important:
 *
 * pass in the server object to listen, not the express app
 * call 'listen' on the server, not the express app
 */
// To test without drone, comment out next line
require("dronestream").listen(server);
//require('ar-drone-png-stream')(arClient, { port: 8000 });
server.listen(port);