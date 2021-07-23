// wait for DOM to load
document.addEventListener('DOMContentLoaded', (event) => {
  init_grid()
})

const init_grid = () => {
  // only initialize everythong once the ws connection has been established.
  if (!ws_ready) {
    window.setTimeout(init_grid, 100)
  } else {
    setupGridElements()  
  }
}

let ws_ready = false
let mode = "visual"

// establish websocket connection with Browser Grid Device
let socket = new WebSocket("ws://127.0.0.1:2718")

// setup ws msg handler
socket.onmessage = (event) => {
  msg = JSON.parse(event.data);

  switch (msg.type) {
  case "configuration":
    console.log(msg)
    mode = msg.mode
    break
  case "press_event":
    handle_press_event(msg)
    break
  case "led_event":
    handle_led_event(msg)
    break
  case "midi_event":
    handle_midi_event(msg)
    break
  case "description":
    handle_description(msg)
    break
  default:
  }
}

// once the socket is open, we may begin.
socket.onopen = (event) => {
  ws_ready = true

  // tell server we are connected
  const msg = {
    type: 'connected',
  }

  socket.send(JSON.stringify(msg))

  // display start button
  let details = document.getElementById("details-container")
  let startButton = document.createElement("DIV")
  startButton.id = "start-button"
  startButton.innerText = "start test"
  details.appendChild(startButton)
  startButton.addEventListener("click", () => {
    // send the ready message to the server
    socket.send(JSON.stringify({type: "ready"}))    

    // remove this element
    details.removeChild(startButton)

    // add details div
    let detailsBody = document.createElement("DIV")
    detailsBody.id = "details-body"
    detailsBody.innerText = ""
    details.appendChild(detailsBody)
  })
}

// global map of which buttons are pressed down
let pressed_down = {}
let led_level = {}

// setup grid html representation
const setupGridElements = (width=16, height=8) => {
  // select  grid chassis to insert these buttons into
  gridChassisElement = document.getElementById("grid-chassis")

  for (let y = 0; y < height; y++) {
    let row = document.createElement("TR")
    gridChassisElement.appendChild(row);
    pressed_down[y] = {}
    led_level[y] = {}
    for (let x = 0; x < width; x++) {
      // create grid button container
      let buttonContainer = document.createElement("TD")
      buttonContainer.className = "grid-button-container"
      buttonContainer.id = `grid-button-container-${x}-${y}`
      row.appendChild(buttonContainer);
      // button.innerText = ""

      // create actual grid button
      let button = document.createElement("DIV")
      button.className = 'grid-button grid-button-level-0'
      button.id = `grid-button-${x}-${y}`
      buttonContainer.appendChild(button);

      // set the state of this button in nglobal pressed map
      pressed_down[y][x] = false
      led_level[y][x] = 0

      if (mode == "interactive") {
        button.addEventListener("click", () => handle_button_click(x, y)) 
      }
    }    
  }
}

const handle_button_click = (x, y) => {
  if (!ws_ready) return
  
  let button = document.getElementById(`grid-button-${x}-${y}`)

  // set button class
  button.className = getButtonClass(x, y)

  // send message to c++
  const msg = {
    type: 'press_event',
    pressed: pressed_down[y][x],
    x,
    y,
  }

  socket.send(JSON.stringify(msg)) 
}

const getButtonClass = (x, y, switchState=true) => {
  // switch button state
  if (switchState) {
    pressed_down[y][x] = !pressed_down[y][x] 
  }
  
  classes = 'grid-button'

  // get the level of the led
  classes += ` grid-button-level-${led_level[y][x]}`


  // set the grid to pressed if it is pressed
  if (pressed_down[y][x]) {
    classes += ' grid-button-down'
  }

  return classes
}

const handle_press_event = (msg) => {
  x = msg.x
  y = msg.y
  
  let button = document.getElementById(`grid-button-${x}-${y}`)

  // set button class
  button.className = getButtonClass(x, y)
}

const set_led_level = (x, y, intensity) => {
  led_level[y][x] = intensity
}

const handle_led_event = (msg) => {
  x = msg.x
  y = msg.y
  let classes = ""
  
  switch (msg.action) {
  case "on":
    set_led_level(x, y, 15)
    classes = getButtonClass(x, y, false)
    break
  case "off":
    set_led_level(x, y, 0)
    classes = getButtonClass(x, y, false)
    break
  case "set":
    set_led_level(x, y, msg.intensity)
    classes = getButtonClass(x, y, false)
    break
  }

  let button = document.getElementById(`grid-button-${x}-${y}`)
  button.className = classes
}

const handle_description = msg => {
  // get details body element
  let details = document.getElementById("details-body")

  // first fade out text
  details.style.opacity = '0'

  window.setTimeout(() => {
    details.innerText = msg.body
    // fade in new text
    details.style.opacity = '1'
  }, 500)
}


const handle_midi_event = msg => {
  console.log(msg)

  // create new row at the bottom of the midi output-container
  let outputContainer = document.getElementById("midi-output-container")
  let eventRow = makeMidiOutputRow([{data: msg.event}])
  outputContainer.prepend(eventRow)

  if (outputContainer.childNodes.length > 10) {
    outputContainer.removeChild(outputContainer.childNodes[0])
  }
}

// ideas:
// midi notes:
// * be structured within a table (128x1), each note populates a specific index in the row
// * on notes will be filled in rectangles
// * off notes will be border only
// * velocity of an on note will be opacity
// * channels will be color coded to the rectangles
// * mouse hovering on a nnote will show its details in a little detail info window
//
// cc msgs etc.
// * will be like sparklines next to the midi notes section
// * color coded by channel as well
// * value will correspond to opacity?
// * hovering will make other lines a little transparent and make the current one bold and show deets in info box
// * will be canvas based?
//
// nrpn
// *????
const makeMidiOutputRow = midi_events => {
  // create a table row element
  let midiEventRow = document.createElement("TR")
  midiEventRow.className = "midi-output-row"

  // construct an empty row with 128 table row items (each for a midi note)
  for (let i = 0; i < 128; i++) {
    let emptyEventIndex = document.createElement("TD")
    emptyEventIndex.className = 'midi-event-empty'
    midiEventRow.appendChild(emptyEventIndex)
  }
  
  // parse midi events (notes, cc, nrpn)
  midi_events.forEach( event => {
    const parsedEvent = parseMidiEvent(event)
    let midiEvent
    switch (parsedEvent.type) {
    case "NoteOn":
      // index into the correct child node
      midiEvent = midiEventRow.childNodes[parsedEvent.note.num]
      midiEvent.className = "midi-on-event midi-event"
      midiEvent.dataset.notename = parsedEvent.note.name
      midiEvent.dataset.notenum = parsedEvent.note.num
      midiEvent.dataset.channel = parsedEvent.channel
      midiEvent.dataset.velocity = parsedEvent.velocity
      midiEvent.style.backgroundColor = midiChannelToColor[parsedEvent.channel]
      break
    case "NoteOff":
      // index into the correct child node
      midiEvent = midiEventRow.childNodes[parsedEvent.note.num]
      midiEvent.className = "midi-off-event midi-event"
      midiEvent.dataset.notename = parsedEvent.note.name
      midiEvent.dataset.notenum = parsedEvent.note.num
      midiEvent.dataset.channel = parsedEvent.channel
      midiEvent.style.backgroundColor = midiChannelToColor[parsedEvent.channel]
      break
    case "CC":
      // const {channel, cc, value} = parsedEvent
      break
    case "NRPN":
      // const {channel} = parsedEvent
      break
    }
  })

  return midiEventRow
}

const parseMidiEvent = event => {
  const controlByte = event.data[0]

  // midi ON events
  if (controlByte >= 145 || controlByte <= 160) {
    const type = 'NoteOn'
    const noteByte = event.data[1]

    const channel = controlByte - 144
    
    // get octave-less note
    const octavelessNote = noteByte % 12

    // get octave
    const octave = Math.floor(noteByte / 12)

    return {
      type,
      channel,
      note: { name: toMidiNoteStr(octavelessNote) + octave, num: event.data[1]},
      velocity: event.data[2],
    }
  }

  // midi OFF events
  if (controlByte >= 129 || controlByte <= 144) {
    const type = 'NoteOff'
    const noteByte = event.data[1]

    const channel = controlByte - 128
    
    // get octave-less note
    const octavelessNote = noteByte % 12

    // get octave
    const octave = Math.floor(noteByte / 12)

    return {
      type,
      channel,
      note: { name: toMidiNoteStr(octavelessNote) + octave, num: event.data[1]},
    }    
  }


  // TODO cc and nrpn.....
}

// note numbers must be at the zeroth octave
const toMidiNoteStr = noteNum => {
  switch (noteNum) {
  case 0:
    return 'C'
  case 1:
    return 'C#'
  case 2:
    return 'D'
  case 3:
    return 'D#'
  case 4:
    return 'E'
  case 5:
    return 'F'
  case 6:
    return 'F#'
  case 7:
    return 'G'
  case 8:
    return 'G#'
  case 9:
    return 'A'
  case 10:
    return 'A#'
  case 11:
    return 'B'
  }
}

const midiChannelToColor = {
  1: '#FF6666',
  2: '#FFB266',
  3: '#FFFF66',
  4: '#B2FF66',
  5: '#66FF66',
  6: '#66FFB2',
  7: '#66FFFF',
  8: '#66B2FF',
  9: '#6666FF',
  10: '#B266FF',
  11: '#FF66FF',
  12: '#FF66B2',
  13: '#FFCCFF',
  14: '#CCFFCC',
  15: '#FF00FF',
  16: '#FFFF00',
}
