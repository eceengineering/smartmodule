# smartmodule aka MakeItSmart

Description

Purpose of the Smart Module (aka MakeItSmart) is to provide a basic method to get your embedded device "smart" which means it is capable of interacting with other devices. This module has been designed for embedded Linux devices. The module runs at the user space.

Prerequirements

The module requires Mosquitto installed in the platform. GStreamer is a library for constructing graphs of media-handling components. Mosquitto is an open source message broker that implements the MQ Telemetry Transport protocol. MQTT provides a lightweight method of carrying out messaging using a publish/subscribe model. This makes it suitable for "machine to machine" messaging such as with low power sensors or mobile devices such as phones, embedded computers or microcontrollers.

Details:

    Programming Language:   C
    Compiler:               GCC
    Test Platform(s):       ARM Cortex-A9 h/w
    Test OS:                Linux Ubuntu
    Mosquitto Version:      0.15.0
    
How to Build and Run:
  
    Build:$make
    Run:  $./Test.sh

Initialization Parameters:

    struct smartmodule_arguments   module;
    module.hostport   = 1883;
    module.hostIP     = "localhost";
    module.topic 	  = "hello/smarts";
    module.message_cb = (message_callback) &cb_receivedmessage;

When you run the application:

    To quit the program press Q
    To publish a message:
      - press P and type your message 
      - press ENTER!
    
To Install Mosquitto:

    $sudo apt-get install mosquitto

  

  

