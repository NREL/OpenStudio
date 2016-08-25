
function setElementValue(elementName, value) {
  // Name is used instead of Id because radio buttons are composed of a group of elements,
  // not just one element which could be referenced by Id.

  //  Warning:  The variable 'name' appears to be a reserved word on Mac/Safari

  if (is_mac()) {
    // Workarounds for Mac bugs
    value = value.substring(1);  // Mac adds an extra blank space at the front of the string!
    value = value.replace(/%comma%/g, ',');  // Must decode any commas (a Mac quirk)
  }

  elements = document.getElementsByName(elementName);
    
  if (elements.length > 0) {

    for (var i = 0; i < elements.length; i++) {
      element = elements[i];

      switch(element.tagName) {
        case 'INPUT':
          switch(element.type.toUpperCase()) {
            case 'TEXT':
              element.value = value;
              break;

            case 'BUTTON':
              element.value = value;
              break;

            case 'CHECKBOX':
              switch(value.toUpperCase()) {
                case 'TRUE':
                  element.checked = true;
                  break;
                case 'FALSE':
                  element.checked = false;
                  break;
                default:
                  // Bad value; leave unchanged
                  break;
              }
              break;

            case 'RADIO':  // may have broken this when allowing to loop through all elements
              for (var j = 0; j < elements.length; j++) {
                if (elements[j].value == value) {
                  elements[j].checked = true;  // Other radio buttons automatically get unchecked
                  break;
                }
              }
              break;
          }
          break;

        case 'SELECT':
          // SELECT is case sensitive (and probably important to leave it that way)

          if (is_mac()) {
            // Works for Mac
            for (var j = 0; j < element.options.length; j++) {
             if (element.options[j].value == value) {
                element.options[j].selected = true;
                break;
              }
            }
          }
          else {
            element.value = value;  // Only works on Windows
          }
          break;

        case 'TEXTAREA':
          element.value = value;
          break;

        case 'BUTTON':
          element.value = value;
          break;
          
        default: 
          element.value = value;
          break;
      }

      //onChangeElement(element);  // seems to cause an error
    }
  }else{
    value;
  }
}


function setElementSource(elementName, path) {
  //  Warning:  The variable 'name' appears to be a reserved word on Mac/Safari

  elements = document.getElementsByName(elementName);

  if (elements.length > 0) {
    element = elements[0];
    element.src = path;
  }
}


function setSelectOptions(elementName, value_string, text_string) {
  //value_array = value_string.replace(/%comma%/g, ',').split(',');
  //text_array = text_string.replace(/%comma%/g, ',').split(',');
  value_array = value_string.split(',');
  text_array = text_string.split(',');
  
  elements = document.getElementsByName(elementName);

  if (elements.length > 0) {
    for (var i = 0; i < elements.length; i++) {
      element = elements[i];

      if (element.tagName == 'SELECT') {
        element.options.length = 0;  // Clear the current options

        for (var i = 0; i < value_array.length; i++) {

          value_array[i] = value_array[i].replace(/%apos%/g, "'");  // Decode any single quotes
          text_array[i] = text_array[i].replace(/%apos%/g, "'");  // Decode any single quotes

          option = new Option(text_array[i], value_array[i], false, false);
          element.add(option);
        }
      }

      //onChangeElement(element);
    }
  }
}


function setBackgroundColor(color) {
  document.bgColor = color;
  setStyleByClass('static_text', 'backgroundColor', color);
  setStyleByClass('button', 'backgroundColor', color);
}


function disableElement(elementName) {

  elements = document.getElementsByName(elementName);

  for (var i = 0; i < elements.length; i++) {
    element = elements[i];
      
    if (element.tagName == 'BUTTON') {
      // Do not change the background color for buttons
      elements[i].disabled = true;
    }
    else if (element.tagName == 'INPUT') {
      if ((element.type.toUpperCase() == 'BUTTON') || (element.type.toUpperCase() == 'CHECKBOX')) {
        // Do not change the background color for buttons and checkboxes
        elements[i].disabled = true;
      }
      else {
        elements[i].disabled = true;
        elements[i].style.backgroundColor = document.bgColor;   
      }
    }
    else {
      elements[i].disabled = true;
      elements[i].style.backgroundColor = document.bgColor;    
    }
  }
}


function enableElement(elementName) {

  elements = document.getElementsByName(elementName);

  for (var i = 0; i < elements.length; i++) {
    element = elements[i];
      
    if (element.tagName == 'BUTTON') {
      // Do not change the background color for buttons
      elements[i].disabled = false;
    }
    else if (element.tagName == 'INPUT') {
      if ((element.type.toUpperCase() == 'BUTTON') || (element.type.toUpperCase() == 'CHECKBOX')) {
        // Do not change the background color for buttons and checkboxes
        elements[i].disabled = false;
      }
      else {
        elements[i].disabled = false;
        elements[i].style.backgroundColor = "#ffffff";   
      }
    }
    else {
      elements[i].disabled = false;
      elements[i].style.backgroundColor = "#ffffff";    
    }
  }
}


function hideElement(elementName) {

  elements = document.getElementsByTagName("*");  // Get all elements because document.getElementsByName was not including table elements

  for (var i = 0; i < elements.length; i++) {
    if (elements[i].name == elementName) {
      elements[i].style.display = "none";
    }
  }
}


function showElement(elementName) {

  elements = document.getElementsByTagName("*");  // Get all elements because document.getElementsByName was not including table elements

  for (var i = 0; i < elements.length; i++) {
    if (elements[i].name == elementName) {
      elements[i].style.display = "block";
    }
  }
}


function markElement(elementName) {
  // Only can mark pre-arranged elements that are surrounded by a <div> tag with a name that ends with ".border"
  // This is because border style properties do not work for SELECT elements.
  name = name + ".border"

  elements = document.getElementsByTagName("*");  // Get all elements because document.getElementsByName was not including table elements

  for (var i = 0; i < elements.length; i++) {
    if (elements[i].name == elementName) {
      elements[i].style.borderBottom = "3px dashed #FF0000";  // solid, dotted, or dashed
      elements[i].style.marginBottom = "-3px";
    }
  }
}


function setStyleByClass(class_name, property, value) {
  // Name is used instead of Id because radio buttons are composed of a group of elements,
  // not just one element which could be referenced by Id.

  elements = document.getElementsByTagName("*");

  if (elements.length > 0) {
    for (var i = 0; i < elements.length; i++) {
      if (elements[i].className == class_name) {
        eval('elements[i].style.' + property + " = '" + value + "'");
      }
    }
  }
}


function unmarkElement(elementName) {
  // Only can mark pre-arranged elements that are surrounded by a <div> tag with a name that ends with ".border"
  // This is because border style properties do not work for SELECT elements.
  elementName = elementName + ".border"

  elements = document.getElementsByTagName("*");  // Get all elements because document.getElementsByName was not including table elements

  for (var i = 0; i < elements.length; i++) {
    if (elements[i].name == elementName) {
      elements[i].style.borderBottom = "none";
      elements[i].style.marginBottom = "0px";
    }
  }
}


function onChangeElement(element) {

  switch(element.tagName) {
    case 'INPUT':
      switch(element.type.toUpperCase()) {
        case 'TEXT':
          value = element.value;
          value = value.replace(/'/g, "\\'");  // Must encode any single quotes
          break;

        case 'RADIO':
          value = element.value;
          value = value.replace(/'/g, "\\'");  // Must encode any single quotes
          break;

        case 'CHECKBOX':
          value = element.checked;
          break;
      }
      break;

    case 'SELECT':
      value = element.value;
      value = value.replace(/'/g, "\\'");  // Must encode any single quotes
      break;

    case 'TEXTAREA':
      value = element.value;
      value = value.replace(/'/g, "\\'");  // Must encode any single quotes
      break;
    
    default: 
      value = element.value;
      value = value.replace(/'/g, "\\'");  // Must encode any single quotes
      break;               
  }

  elementName = element.name;  //  Warning:  The variable 'name' appears to be a reserved word on Mac/Safari

  if (elementName == "") elementName = element.id;
  window.location='skp:on_change_element@' + elementName + ',' + value;

  return(value);
}


function daysInMonth(month) {

  switch(month) {
    case '1':
      days_in_month = "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31";
      break;

    case '2':
      days_in_month = "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28";
      break;

    case '3':
      days_in_month = "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31";
      break;

    case '4':
      days_in_month = "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30";
      break;

    case '5':
      days_in_month = "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31";
      break;

    case '6':
      days_in_month = "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30";
      break;

    case '7':
      days_in_month = "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31";
      break;

    case '8':
      days_in_month = "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31";
      break;

    case '9':
      days_in_month = "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30";
      break;

    case '10':
      days_in_month = "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31";
      break;

    case '11':
      days_in_month = "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30";
      break;

    case '12':
      days_in_month = "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31";
      break;
      
    default:
      days_in_month = "0";  // A bad month was passed in
  }
  
  return(days_in_month);
}


function is_mac() {
  // Check if this is running on the Mac/Safari
  if (navigator.platform.search(/mac/i) > -1) {
    return(true);
  }
  else {
    return(false);
  }
}


function setRows(rows) {
  // This is the Mac version of the function that is found in the header of ObjectInfo.html.
  // There are two apparent bugs that require this special Mac kludge.
  //   1.  Javascript in the header of a frameset html page is ignored by Safari, or SketchUp.
  //   2.  Strings with commas that are passed to a WebDialog with execute_script truncate
  //       them after the first comma.

  //rows = rows.replace(/%comma%/g, ',');  // Decode any commas
  parent.document.getElementById('frameset').rows = rows;
}


function invalidate() {
  // Mac kludge to trigger the WebDialog window to redraw.
  parent.blur();
  parent.focus();
}
