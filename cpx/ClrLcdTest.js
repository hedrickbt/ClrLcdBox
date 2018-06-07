input.buttonA.onEvent(ButtonEvent.Click, function () {
    messageToSend = "^Goodbye My Friend!"
    for (let index = 0; index <= messageToSend.length - 1; index++) {
        pins.i2cWriteNumber(
        10,
        messageToSend.charCodeAt(index),
        NumberFormat.Int8LE,
        false
        )
    }
})
let messageToSend = ""
