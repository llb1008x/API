var acListTotal   =  0;
var acListCurrent = -1;
var acDelay		  = 0;
var acURL		  = null;
var acSearchId	  = null;
var acResultsId	  = null;
var acSearchField = null;
var acResultsDiv  = null;

function setAutoComplete(field_id, results_id, get_url){

	// initialize vars
	acSearchId  = "#" + field_id;
	acResultsId = "#" + results_id;
	acURL 		= get_url;

	// create the results div
	$("body").append('<div id="' + results_id + '"></div>');

	// register mostly used vars
	acSearchField	= $(acSearchId);
	acResultsDiv	= $(acResultsId);

	// reposition div
	repositionResultsDiv();
	
	// on blur listener
	acSearchField.blur(function(){ setTimeout("clearAutoComplete()", 200) });

	// on key up listener
	acSearchField.keyup(function (e) {

		// get keyCode (window.event is for IE)
		var keyCode = e.keyCode || window.event.keyCode;
		var lastVal = acSearchField.val();

		// check an treat up and down arrows
		if(updownArrow(keyCode)){
			return;
		}

		// check for an ENTER or ESC
		if(keyCode == 13 || keyCode == 27){
			clearAutoComplete();
			return;
		}

		// if is text, call with delay
		setTimeout(function () {autoComplete(lastVal)}, acDelay);
	});
}
// treat the auto-complete action (delayed function)
function autoComplete(lastValue)
{
	// get the field value
	var part = acSearchField.val();
	
	// if it's empty clear the resuts box and return
	if(part == ''){
		clearAutoComplete();
		return;
	}
	// if it's equal the value from the time of the call, allow
	if(lastValue != part){
		return;
	}

	$.ajax({
		url:acURL+part,
		type:'GET',
		dataType: 'xml',
		success: function (xml){

			// get the total of results
			var ansLength = acListTotal = $(xml).find("MESSAGE").children("RECORDS").attr('NUM');
			clearAutoComplete();
			// if there are results populate the results div
			if(ansLength > 0){

				// create a div for each result
				$(xml).find("MESSAGE").children("RECORDS").children("RECORD").each(function(i){

						var tmpdiv= document.createElement("div");
						$(tmpdiv).html($(this).children("NAME").text());
						$(tmpdiv).addClass('unselected');
						acResultsDiv.append($(tmpdiv));
				});

				acResultsDiv.css("display","block");

				// for all divs in results
				var divs = $(acResultsId + " > div");

//var test = document.getElementById("results");
//alert(test.innerHTML);
			
				// on mouse over clean previous selected and set a new one
				divs.mouseover( function() {
					divs.each(function(){ this.className = "unselected"; });
					this.className = "selected";
				})
			
				// on click copy the result text to the search field and hide
				divs.click( function() {
					acSearchField.val(this.childNodes[0].nodeValue);
					clearAutoComplete();
				});
	
			} 
			else {
				clearAutoComplete();
			}
			
		}
	});
}

// clear auto complete box
function clearAutoComplete()
{
	acResultsDiv.html('');
	acResultsDiv.css("display","none");
}

// reposition the results div accordingly to the search field
function repositionResultsDiv()
{
	// get the field position
	var sf_pos    = acSearchField.offset();
	var sf_top    = sf_pos.top;
	var sf_left   = sf_pos.left;

	// get the field size
	var sf_height = acSearchField.height();
	var sf_width  = acSearchField.width();

	// apply the css styles - optimized for Firefox
	acResultsDiv.css("position","absolute");
	acResultsDiv.css("left", sf_left);
	acResultsDiv.css("top", sf_top + sf_height + 5);
	acResultsDiv.css("width", sf_width);
}


// treat up and down key strokes defining the next selected element
function updownArrow(keyCode) {
	if(keyCode == 40 || keyCode == 38){

		if(keyCode == 38){ // keyUp
			if(acListCurrent == 0 || acListCurrent == -1){
				acListCurrent = acListTotal-1;
			}else{
				acListCurrent--;
			}
		} else { // keyDown
			if(acListCurrent == acListTotal-1){
				acListCurrent = 0;
			}else {
				acListCurrent++;
			}
		}

		// loop through each result div applying the correct style
		acResultsDiv.children().each(function(i){
			if(i == acListCurrent){
				acSearchField.val(this.childNodes[0].nodeValue);
				this.className = "selected";
			} else {
				this.className = "unselected";
			}
		});

		return true;
	} else {
		// reset
		acListCurrent = -1;
		return false;
	}
}
function add_word(word)
{
}
