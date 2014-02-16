var rotKeyBlock 	= 0; 	//Used to prevent repetitive keydown events

//ASCII Key Codes
var leftKey 		= 37;
var upKey			= 38;
var downKey			= 40;
var rightKey 		= 39;

var flipAhead		= 117; // u
var flipBehind		= 106; // j
var flipLeft		= 104; // h
var flipRight 		= 107; // k


$(document).ready(function() {
	enableRotateButtons();
});

/************************************************
*		Button click-event functionality		*
************************************************/
$('#btnRotLeft').click(function() {
	sendRotation(commands.ROTLEFT);
}); // end btnUp click

$('#btnRotForward').click(function() {
	sendRotation(commands.UP);
}); // end btnUp click

$('#btnRotRight').click(function() {
	sendRotation(commands.ROTRIGHT);
}); // end btnUp click

$('#btnStable').click(function() {
	sendRotation(commands.STOP);
}); // end btnUp click

$('#btnRotBack').click(function() {
	sendRotation(commands.DOWN);
}); // end btnUp click

/************************************************
*		End of click-event functionality		*
************************************************/

/********************************************
*		Keypress-event functionality		*
********************************************/
$(document).keyup(function(e) {
	if( e.which == leftKey || e.which == upKey ||  e.which == rightKey || e.which == downKey ) {
		if( e.which == rotKeyBlock ) {
			rotKeyBlock = 0;
			sendRotation(commands.STOP);
		}
	}
});

$(document).keypress(function(e) {
	switch(e.which) {
		case flipAhead:
			sendRotation(commands.FLIPAHEAD);
			break;
		
		case flipLeft:
			sendRotation(commands.FLIPLEFT);
			break;

		case flipBehind:
			sendRotation(commands.FLIPBEHIND);
			break;

		case flipRight:
			sendRotation(commands.FLIPRIGHT);
			break;

		default:
			break;
	}

	console.log(e.which);
	e.preventDefault();
});

$(document).keydown(function(e) {
	switch(e.which) {
		case leftKey: // left
			if( rotKeyBlock != leftKey) {
				sendRotation(commands.ROTLEFT);
				rotKeyBlock = e.which;
			}
			break;

		case upKey: // up 
			if( rotKeyBlock != e.which) {
				sendRotation(commands.UP);
				rotKeyBlock = e.which;
			}
			break;

		case rightKey: // right
			if( rotKeyBlock != rightKey) {
				sendRotation(commands.ROTRIGHT);
				rotKeyBlock = e.which;
			}
			break;

		case downKey: // down 
			if( rotKeyBlock != e.which) {
				sendRotation(commands.DOWN);
				rotKeyBlock = e.which;
			}
			break;

		default: 
			return;
	}
	e.preventDefault();
});

/************************************************
*		End keypress-event functionality		*
************************************************/

/* enableRotateButtons( disabledButton )
 * 		Enables all buttons of class controller-rotate-btn
 *		except the button passed in.
 *
 *	Parameters:
 *		disabledButton: button to remain disabled.
 */
function enableRotateButtons(disabledButton) {
	$('.controller-rotate-btn').removeAttr("disabled", "disabled");

	disabledButton.attr("disabled", "disabled");
}

/* sendRotation( movement )
 * 		Performs validation and posts the rotation specified to
 *		the PHP controller. Also displays any errors produced
 *		by the PHP controller.
 *
 * Parameters:
 *		movement: the movement to pass to the PHP controller.
 */
function sendRotation(movement) {
	if( isAirborne && !isDisabled )
		socket.emit("command", movement);
}

/* processSendRotationResponse( data, status )
 *		Processes the response of the PHP controller, and
 *		displays the necessary feedback. This is the callback
 *		method for our sendRotation method.
 *
 * Parameters:
 	data: the data returned from the PHP controller.
 	status: the status of the PHP request.
 */
function processSendRotationResponse(data, status) {
	if( status == 'success' ) {
		var button;
		console.log(data);
		switch(data) {
			case "rotLeft":
				button = $('#btnRotLeft');
				$('#drone-rotational-status').html('Rotating Left');
				break;

			case "rotForward":
				button = $('#btnRotForward');
				$('#drone-rotational-status').html('Rotating Forward');
				break;

			case "stable":
				button = $('#btnStable');
				$('#drone-rotational-status').html('Stable');
				break;

			case "rotRight":
				button = $('#btnRotRight');
				$('#drone-rotational-status').html('Rotating Right');
				break;

			case "rotBack":
				button = $('#btnRotBack');
				$('#drone-rotational-status').html('Rotating Back');
				break;
		}

		enableRotateButtons(button);

		$('#errorMessage').html("");
	}
}

/* sendRotationError
 * 		Displays an error message if something goes wrong posting
 *		a rotation to the PHP controller. This is the error method
 *		for our sendRotation function.
 */
function sendRotationError() {
	$('#errorMessage').html("The server is not responding.");
}

