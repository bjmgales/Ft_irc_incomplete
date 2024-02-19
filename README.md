
# IN ORDER TO MAKE THE PROJECT RUN : # 

### You will need to install the kiwiIRC client: ###

Download the Kiwi source or clone the git repository:

```$ git clone https://github.com/prawnsalad/KiwiIRC.git && cd KiwiIRC```

Install the dependencies:

```$ npm install```

Copy and edit the configuration as needed:

```$ cp config.example.js config.js```

```$ nano config.js```

Make sure the client code is built:

```$ ./kiwi build```

Then, inside the kiwiIRC folder, run the command :

```$ ./kiwi start ```

### Everything is now set, you may now connect to "localhost:port" with your web browser. ###

⚠️ the port of localhost:port is the one you set INSIDE the kiwi configuration file and that MUST DIFFERS from the one you will use to launch ft_irc.

Also, be sure you're not connecting with SSL set on, otherwise you won't be able to connect to the server.

#### MADE ON MACOSX ####
