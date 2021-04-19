/**
 * ----------------------------------------------------------------------------
 * ESP32 Remote Control with WebSocket
 * ----------------------------------------------------------------------------
 * © 2020 Stéphane Calderoni
 * ----------------------------------------------------------------------------
 */

var gateway = `ws://${window.location.hostname}/ws`;
var websocket;

// ----------------------------------------------------------------------------
// Initialization
// ----------------------------------------------------------------------------

window.addEventListener('load', onLoad);

function onLoad(event) {
    initWebSocket();
    initButton1();
    initButton2();
    initButton3();
    initButton4();
    initButton5();
    initButton6();
}

// ----------------------------------------------------------------------------
// WebSocket handling
// ----------------------------------------------------------------------------

function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}

function onOpen(event) {
    console.log('Connection opened');
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}

function onMessage(event) {
    let data = JSON.parse(event.data);
    //document.getElementById('led').className = data.status;
    // document.getElementById('led' + data.rank).className = data.status;
    // console.log(data.status);
    if (data.rank == "1") {
        if (data.status == "on") {
            document.getElementById('toggle1').style.backgroundColor = "#F9E79F";
            document.getElementById('toggle1').style.color = "#1B2E3C";
        } else {
            document.getElementById('toggle1').style.backgroundColor = "#1B2E3C";
            document.getElementById('toggle1').style.color = "#Fff";
        }
    }

    if (data.rank == "2") {
        if (data.status == "on") {
            document.getElementById('toggle2').style.backgroundColor = "#F9E79F";
            document.getElementById('toggle2').style.color = "#1B2E3C";
        } else {
            document.getElementById('toggle2').style.backgroundColor = "#1B2E3C";
            document.getElementById('toggle2').style.color = "#Fff";
        }
    }
    if (data.rank == "3") {
        if (data.status == "on") {
            document.getElementById('toggle3').style.backgroundColor = "#F9E79F";
            document.getElementById('toggle3').style.color = "#1B2E3C";
        } else {
            document.getElementById('toggle3').style.backgroundColor = "#1B2E3C";
            document.getElementById('toggle3').style.color = "#Fff";
        }
    }
    if (data.rank == "4") {
        if (data.status == "on") {
            document.getElementById('toggle4').style.backgroundColor = "#F9E79F";
            document.getElementById('toggle4').style.color = "#1B2E3C";
        } else {
            document.getElementById('toggle4').style.backgroundColor = "#1B2E3C";
            document.getElementById('toggle4').style.color = "#Fff";
        }
    }
    if (data.rank == "5") {
        if (data.status == "on") {
            document.getElementById('toggle5').style.backgroundColor = "#F9E79F";
            document.getElementById('toggle5').style.color = "#1B2E3C";
        } else {
            document.getElementById('toggle5').style.backgroundColor = "#1B2E3C";
            document.getElementById('toggle5').style.color = "#Fff";
        }
    }
    if (data.rank == "6") {
        if (data.status == "on") {
            document.getElementById('toggle6').style.backgroundColor = "#F9E79F";
            document.getElementById('toggle6').style.color = "#1B2E3C";
        } else {
            document.getElementById('toggle6').style.backgroundColor = "#1B2E3C";
            document.getElementById('toggle6').style.color = "#Fff";
        }
    }

}



// ----------------------------------------------------------------------------
// Button handling
// ----------------------------------------------------------------------------

function initButton1() {
    document.getElementById('toggle1').addEventListener('click', onToggle1);
}

function initButton2() {
    document.getElementById('toggle2').addEventListener('click', onToggle2);
}

function initButton3() {
    document.getElementById('toggle3').addEventListener('click', onToggle3);
}

function initButton4() {
    document.getElementById('toggle4').addEventListener('click', onToggle4);
}

function initButton5() {
    document.getElementById('toggle5').addEventListener('click', onToggle5);
}

function initButton6() {
    document.getElementById('toggle6').addEventListener('click', onToggle6);
}


function onToggle1(event) {
    websocket.send(JSON.stringify({ 'action': 'toggle1' }));
}

function onToggle2(event) {
    websocket.send(JSON.stringify({ 'action': 'toggle2' }));
}

function onToggle3(event) {
    websocket.send(JSON.stringify({ 'action': 'toggle3' }));
}

function onToggle4(event) {
    websocket.send(JSON.stringify({ 'action': 'toggle4' }));
}

function onToggle5(event) {
    websocket.send(JSON.stringify({ 'action': 'toggle5' }));
}

function onToggle6(event) {
    websocket.send(JSON.stringify({ 'action': 'toggle6' }));
}