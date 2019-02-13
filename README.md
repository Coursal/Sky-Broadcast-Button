# Sky-Broadcast-Button

A Contiki program for Sky Tmote where each node broadcasts a message every time its button is pressed (counting the number of clicks it got), waits for 5 seconds and lights up its red LED. When each node receives this message, it prints the the sender's Rime address along with the message (counting the total number of messages it received) and toggles the green and blue LEDs.

The folder must be put under the `contiki/examples/` directory

### Output example (Cooja)
![](http://i66.tinypic.com/16geqo2.png)

Tested on **Contiki 2.7**.
