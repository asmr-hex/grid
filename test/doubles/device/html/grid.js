// wait for DOM to load
document.addEventListener('DOMContentLoaded', (event) => {
  setupGridElements()
})

// establish websocket connection with Browser Grid Device
let socket = new WebSocket("ws://127.0.0.1:2718")

// setup ws msg handler
socket.onmessage = (event) => {
  console.log(event.data)
}

// once the socket is open, we may begin.
socket.onopen = (event) => {
  const msg = {
    'hello': 666,
  }

  socket.send(JSON.stringify(msg)) 
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
      
      button.addEventListener("click", () => handle_button_click(x, y))
    }    
  }
}

const handle_button_click = (x, y) => {
  let button = document.getElementById(`grid-button-${x}-${y}`)

  // set button class
  button.className = getButtonClass(x, y)

  // send message to c++
  const msg = {
    pressed: pressed_down[y][x],
    x,
    y,
  }

  socket.send(JSON.stringify(msg)) 
}

const getButtonClass = (x, y) => {
  classes = 'grid-button'

  // get the level of the led
  classes += ` grid-button-level-${led_level[y][x]}`

  // set the grid to pressed down if it previously wasn't
  if (!pressed_down[y][x]) {
    classes += ' grid-button-down'
  }

  // switch button state
  pressed_down[y][x] = !pressed_down[y][x]
  
  return classes
}
