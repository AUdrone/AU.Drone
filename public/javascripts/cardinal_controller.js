var keyBlock 			= 0; 	//Used to prevent repetitive keydown events
var isAirborne 			= false;
var isDisabled 			= false;

//ASCII Key Codes
var leftUpperCase 		= 97;
var leftLowerCase 		= 65;
var forwardUpperCase	= 119;
var forwardLowerCase	= 87;
var rightUpperCase 		= 100;
var rightLowerCase 		= 68;
var backwardUpperCase	= 115;
var backwardLowerCase	= 83;
var takeoffUpperCase	= 116;
var takeoffLowerCase	= 84;
var landUpperCase		= 108;
var landLowerCase		= 76;
var disableEmergencyLowerCase = 114;
var disableEmergencyUpperCase = 82;
var switchCameraLowerCase = 99;
var switchCameraUpperCase = 67;

var commands = {
	FORWARD: 0,
	BACKWARD: 1,
	LEFT: 2,
	RIGHT: 3,
	UP: 4,
	DOWN: 5,
	ROTLEFT: 6,
	ROTRIGHT: 7,
	TAKEOFF: 8,
	LAND: 9,
	STOP: 10,
	FLIPAHEAD: 11,
	FLIPLEFT: 12,
	FLIPBEHIND: 13,
	FLIPRIGHT: 14,
	DISABLEEMERGENCY: 15,
	SWITCHCAMERA: 16
};

$(document).ready(function() {
	setIsAirborne(false);
}); // end ready

/************************************************
*		Button click-event functionality		*
************************************************/
$('#takeoff').click(function() {
	sendMovement(commands.TAKEOFF);
}); // end takeoff button click

$('#land').click(function() {
	sendMovement(commands.LAND);
}); // end land button click

$('#btnUp').click(function() {
	sendMovement(commands.FORWARD);
}); // end btnUp click

$('#btnLeft').click(function() {
	sendMovement(commands.LEFT);
}); // end btnLeft click

$('#btnHover').click(function() {
	sendMovement(commands.STOP);
}); // end btnHover click

$('#btnRight').click(function() {
	sendMovement(commands.RIGHT);
}); // end btnRight click

$('#btnDown').click(function() {
	sendMovement(commands.BACKWARD);
}); // end btnDown click

/************************************************
*		End of click-event functionality		*
************************************************/

/********************************************
*		Keypress-event functionality		*
********************************************/
$(document).keyup(function(e) {
	switch( e.which ) {
		case leftUpperCase:
		case leftLowerCase:
		case forwardUpperCase:
		case forwardLowerCase:
		case rightUpperCase:
		case rightLowerCase:
		case backwardUpperCase:
		case backwardLowerCase:
			if( e.which == keyBlock ) {
				keyBlock = 0;
				sendMovement(commands.STOP);
			}
			
			e.preventDefault();
			break;

		case disableEmergencyUpperCase:
		case disableEmergencyLowerCase:
			sendMovement(commands.DISABLEEMERGENCY);
			
			e.preventDefault();
			break;
		
		case switchCameraUpperCase:
		case switchCameraLowerCase:
			sendMovement(commands.SWITCHCAMERA);

			e.preventDefault();
			break;

		default:
			break;
	}
	/*if( e.which == leftUpperCase || e.which == leftLowerCase 
		|| e.which == forwardUpperCase || e.which == forwardLowerCase
		|| e.which == rightUpperCase || e.which == rightLowerCase
		|| e.which == backwardUpperCase || e.which == backwardLowerCase ) {
		
		if( e.which == keyBlock ) {
			keyBlock = 0;
			sendMovement(commands.STOP);
		}

		e.preventDefault();
	} else if( ((e.which == disableEmergencyUpperCase || e.which == disableEmergencyLowerCase) && !isAirborne)
			|| (e.which == switchCameraUpperCase || e.which == switchCameraLowerCase) ) {
		sendMovement(commands.DISABLEEMERGENCY);
	}*/
});

$(document).keypress(function(e) { 
	if((e.which == takeoffLowerCase || e.which == takeoffUpperCase) && !isAirborne)
		sendMovement(commands.TAKEOFF);

	if((e.which == landLowerCase || e.which == landUpperCase) && isAirborne)
		sendMovement(commands.LAND);

	e.preventDefault();
});

$(document).keydown(function(e) {
	if( e.which == leftLowerCase || e.which == leftUpperCase ) {
		if( keyBlock != e.which) {
			sendMovement(commands.LEFT);
			keyBlock = e.which;
			e.preventDefault();
		}
	} else if( e.which == forwardUpperCase || e.which == forwardLowerCase ) {
		if( keyBlock != e.which) {
			sendMovement(commands.FORWARD);
			keyBlock = e.which;
			e.preventDefault();
		}
	} else if( e.which == rightLowerCase || e.which == rightUpperCase ) {
		if( keyBlock != e.which) {
			sendMovement(commands.RIGHT);
			keyBlock = e.which;
			e.preventDefault();
		}
	} else if( e.which == backwardUpperCase || e.which == backwardLowerCase ) {
		if( keyBlock != e.which) {
			sendMovement(commands.BACKWARD);
			keyBlock = e.which;
			e.preventDefault();
		}
	}
});

/************************************************
*		End keypress-event functionality		*
************************************************/

/* enableButtons( disabledButton )
 * 		Enables all buttons of class controller-move-btn
 *		or controller-btn except the button passed in.
 *
 *	Parameters:
 *		disabledButton: button to remain disabled.
 */
function enableButtons(disabledButton) {
	$('.controller-move-btn').removeAttr("disabled", "disabled");
	$('.controller-btn').removeAttr("disabled", "disabled");

	disabledButton.attr("disabled", "disabled");
}

/* sendMovement( movement )
 * 		Performs validation and posts the movement specified to
 *		the PHP controller. Also displays any errors produced
 *		by the PHP controller.
 *
 * Parameters:
 *		movement: the movement to pass to the PHP controller.
 */
function sendMovement(movement) {
	if(( ((movement != commands.TAKEOFF) && isAirborne) 
		|| (( movement == commands.TAKEOFF) && !isAirborne) ) && !isDisabled) {
		socket.emit("command", movement);
	} else if( movement == commands.DISABLEEMERGENCY && !isAirborne && !isDisabled) {
		socket.emit("command", movement);
	} else if( movement == commands.SWITCHCAMERA && !isDisabled) {
		socket.emit("command", movement);
	}
}

socket.on('state-change', function(data){
	setIsAirborne(data.flying);
	console.log( "flying: " + data.flying );
});

socket.on('battery-change', function(data){
	$('#battery').html(data);
	console.log( data );
});

socket.on('disable-controls', function(data){
	disableControls(data.disable);
	console.log( "disable: " + data.disable );
});

/* processSendMovementResponse( data, status )
 *		Processes the response of the PHP controller, and
 *		displays the necessary feedback. This is the callback
 *		method for our sendMovement method.
 *
 * Parameters:
 	data: the data returned from the PHP controller.
 	status: the status of the PHP request.
 */
function processSendMovementResponse(data, status) {
	if( status == 'success' ) {
		var button;

		switch(data) {
			case "left":
				button = $('#btnLeft');
				$('#drone-cardinal-status').html('Flying Left');
				break;

			case "up":
				button = $('#btnUp');
				$('#drone-cardinal-status').html('Flying Up');
				break;

			case "hover":
				button = $('#btnHover');
				$('#drone-cardinal-status').html('Hovering in Place');
				break;

			case "right":
				button = $('#btnRight');
				$('#drone-cardinal-status').html('Flying Right');
				break;

			case "down":
				button = $('#btnDown');
				$('#drone-cardinal-status').html('Flying Down');
				break;

			case "takeoff":
				button = $('#takeoff')
				//$('#drone-cardinal-status').html('Airborne');
				break;

			case "land":
				button = $('#land');
				/*$('#drone-cardinal-status').html('Landed');
				isAirborne = false;
				$('#takeoff-controls').show();
				$('#flight-controls').hide();
				$('#landing-controls').hide();*/
				break;
		}

		enableButtons(button);

		$('#errorMessage').html("");
	}
}

function setIsAirborne(bIsAirborne) {
	isAirborne = bIsAirborne;

	if( isAirborne && !isDisabled ) {
		$('#flight-controls').show();
		$('#landing-controls').show();
		$('#takeoff-controls').hide();
	} else if( !isAirborne && !isDisabled ) {
		$('#flight-controls').hide();
		$('#landing-controls').hide();
		$('#takeoff-controls').show();
	}
}

// disableControls
// Hides the clickable controls when passed true,
// and displays them otherwise. 

function disableControls(bIsDisabled)
{
	isDisabled = bIsDisabled;

	if( isDisabled ) {
		$('#flight-controls').hide();
		$('#landing-controls').hide();
		$('#takeoff-controls').hide();
	} else {
		setIsAirborne(isAirborne);
	}
}

/* sendMovementError
 * 		Displays an error message if something goes wrong posting
 *		a movement to the PHP controller. This is the error method
 *		for our sendMovement function.
 */
function sendMovementError() {
	$('#errorMessage').html("The server is not responding.");
}

