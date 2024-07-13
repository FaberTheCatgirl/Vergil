/**
 * Creates a new S3dError object which represents an Halo Online communication error.
 *
 * @class
 * @param {string} message - The error message.
 * @param {S3dErrorCode} [code] - The error code.
 * @param {string} [method] - The internal method name.
 */
function S3dError(message, code, method) {
    Error.captureStackTrace(this, this.constructor);

    /**
     * @member {string}
     */
    this.name = this.constructor.name;

    /**
     * The error message.
     * @member {string}
     */
    this.message = message || "";

    /**
     * The error code.
     * @member {S3dErrorCode}
     */
    this.code = (typeof code === "number") ? code : S3dErrorCode.UNKNOWN_ERROR;

    /**
     * The internal method name.
     * @member {string}
     */
    this.method = method || "";
}

/**
 * Gets the name of the error's code.
 *
 * @returns {string} A string representing the error's code.
 */
S3dError.prototype.getCodeName = function () {
    for (var name in S3dErrorCode) {
        if (S3dErrorCode.hasOwnProperty(name) && S3dErrorCode[name] === this.code) {
            return name;
        }
    }
    return this.code.toString();
}

/**
 * Error codes.
 *
 * @readonly
 * @enum {number}
 */
S3dErrorCode = {
    /**
     * The method ran successfully.
     */
    OK: 0,

    /**
     * An unknown error occurred while running the method.
     */
    UNKNOWN_ERROR: 1,

    /**
     * A JavaScript exception occurred while running the method.
     */
    JS_EXCEPTION: 2,

    /**
     * A game communication error occurred.
     */
    BAD_QUERY: 3,

    /**
     * The method is not supported by the game.
     */
    UNSUPPORTED_METHOD: 4,

    /**
     * An invalid argument was passed to the method.
     */
    INVALID_ARGUMENT: 5,

    /**
     * An error occurred while performing a network operation.
     */
    NETWORK_ERROR: 6,

    /**
     * The requested information is not available.
     */
    NOT_AVAILABLE: 7,

    /**
     * The console command failed to execute successfully.
     */
    COMMAND_FAILED: 8
};

/**
 * Multiplayer event audiences.
 *
 * @readonly
 * @enum {number}
 */
MPEventAudience = {
    CAUSE_PLAYER: 0,
    CAUSE_TEAM: 1,
    EFFECT_PLAYER: 2,
    EFFECT_TEAM: 3,
    ALL: 4
};

/**
 * Multiplayer event categories.
 *
 * @readonly
 * @enum {number}
 */
MPEventCategory = {
    GENERAL: 0,
    FLAVOR: 1,
    SLAYER: 2,
    CTF: 3,
    ODDBALL: 4,
    FORGE: 5,
    KOTH: 6,
    VIP: 7,
    JUGGERNAUT: 8,
    TERRITORIES: 9,
    ASSAULT: 10,
    INFECTION: 11,
    SURVIVAL: 12,
    WP: 13
};

/**
 * Game modes.
 *
 * @readonly
 * @enum {number}
 */
GameMode = {
    NONE: 0,
    CTF: 1,
    SLAYER: 2,
    ODDBALL: 3,
    KOTH: 4,
    FORGE: 5,
    VIP: 6,
    JUGGERNAUT: 7,
    TERRITORIES: 8,
    ASSAULT: 9,
    INFECTION: 10
};

/**
 * Console command and variable types.
 *
 * @readonly
 * @enum {number}
 */
CommandType = {
    /**
     * A command.
     */
    COMMAND: 0,

    /**
     * An integer variable.
     */
    INT: 1,

    /**
     * A 64-bit integer variable.
     */
    INT64: 2,

    /**
     * A floating-point variable.
     */
    FLOAT: 3,

    /**
     * A string variable.
     */
    STRING: 4
};

(function () {
    window.s3d = window.s3d || {};

    // Default 1:1 result mapping.
    function defaultResultMapping(result) {
        return result;
    }

    // JSON result mapping.
    function jsonResultMapping(result) {
        return JSON.parse(result);
    }

    // Calls a native method and returns a Promise for it.
    s3d.callMethod = function (method, args, resultMapping) {
        resultMapping = resultMapping || defaultResultMapping;
        args = args || {};
        return new Promise(function (resolve, reject) {
            // Ensure that we have a function to send queries to ED
            if (!window.s3dQuery) {
                reject(new S3dError("Unsupported method: window.s3dQuery() is not available", S3dErrorCode.UNSUPPORTED_METHOD, method));
                return;
            }

            // If args is a function, run it to get the actual arguments
            // This is useful in order to ensure that JS exceptions cause the promise to fail.
            if (typeof args === "function") {
                args = args();
            }

            // Send a CEF query
            window.s3dQuery({
                request: JSON.stringify({
                    method: method,
                    args: args
                }),
                persistent: false,
                onSuccess: function (resultStr) {
                    // Try to map the result string and resolve the promise with it
                    var value;
                    try {
                        value = resultMapping(resultStr);
                    } catch (e) {
                        reject(e);
                        return;
                    }
                    resolve(value);
                },
                onFailure: function (code, message) {
                    if(method=="command"){
                        method = method+": "+args.command;
                    }
                    reject(new S3dError(message, code, method));
                }
            });
        });
    }

    // Registers an event handler for a UI event.
    function registerEvent(name, callback) {
        window.addEventListener("message", function (event) {
            var origin = event.origin;
            if (!origin.startsWith("s3d:")) {
                return; // Only recognize messages sent by the UI
            }
            var data = event.data;
            if (typeof data !== "object" || !("event" in data)) {
                return;
            }
            if (data.event === name) {
                callback(data);
            }
        }, false);
    }

    // Posts a message to the UI.
    function postUiMessage(message, data) {
        // If this is running in the UI layer, then post to the current window, otherwise post to the parent
        var targetWindow = (window.location.href.startsWith("s3d://ui/")) ? window : window.parent;
        if (targetWindow) {
            targetWindow.postMessage({
                message: message,
                data: data
            }, "*");
        }
    }

    // displays a toast. example: s3d.toast({body:'<b>Hello World</b>'})
    s3d.toast = function(toast) {
        postUiMessage('toast', toast);
    }

    // Disable text selection by default since most screens won't want it
    window.addEventListener("load", function (event) {
        document.body.style["-webkit-user-select"] = "none";
    });

    /**
     * Methods for interfacing with ElDewrito.
     *
     * @namespace s3d
     */

    /**
     * A promise made by an asynchronous ElDewrito method.
     *
     * If the promise is rejected for a reason related to ElDewrito, it will be rejected with a {@link S3dError} object.
     * This object includes an [error code]{@link S3dErrorCode} which can be used to easily figure out what went wrong.
     *
     * If the promise is rejected for any other reason, it may be rejected with a generic Error.
     * Therefore, you must use `instanceof S3dError` to check that the error is actually an ElDewrito error before getting specific information.
     *
     * @typedef {Promise<*|Error>} S3dPromise
     */

    /**
     * (ASYNCHRONOUS) Retrieves the current version of ElDewrito.
     *
     * @returns {S3dPromise<string>} - A promise for the version string.
     */
    s3d.getVersion = function () {
        return s3d.callMethod("version");
    }

    /**
     * Requests to show a screen.
     *
     * If the requested screen is still loading, it will not be shown until it finishes.
     *
     * This will always send a [show]{@link event:show} event to the screen, even if it is already visible.
     * You can use this as a simple means of sending messages between screens.
     *
     * @param {string} [id] - The ID of the screen to show. If this is null or omitted, the current screen will be shown.
     * @param {object} [data] - Data to pass to the screen's [show]{@link event:show} event.
     */
    s3d.show = function (id, data) {
        postUiMessage("show", {
            screen: id || null,
            data: data || {}
        });
    }

    /**
     * Requests to hide a screen.
     *
     * This will send a [hide]{@link event:hide} event to the screen if it is visible.
     *
     * @param {string} [id] - The ID of the screen to hide. If this is null or omitted, the current screen will be hidden.
     */
    s3d.hide = function (id) {
        postUiMessage("hide", {
            screen: id || null
        });
    }
	
	/**
     * Sends a s3d event to another screen
     *
	 * @param {string} [eventName] - The name of the event for another screen to receive.
	 * @param {object} [data] - Data to pass to the screen.
     */
	s3d.notify = function(eventName, data){
		postUiMessage(eventName, {
			data: data || {}
		});
	}

    /**
     * Requests to change this screen's input capture state.
     *
     * @param {boolean} capture - true to capture mouse and keyboard input, false to release.
     */
    s3d.captureInput = function (capture) {
        postUiMessage("captureInput", {
            capture: !!capture
        });
    }
	
	/**
     * Requests to change this screen's pointer capture state.
     * This is overridden by s3d.captureInput
     *
     * @param {boolean} capture - true to only capture mouse input, false to release.
     */
    s3d.capturePointer = function (capture) {
        postUiMessage("capturePointer", {
            capture: !!capture
        });
    }

    /**
     * (ASYNCHRONOUS) Runs a console command.
     *
     * If the command does not run successfully, the promise will be rejected with a [S3dErrorCode.COMMAND_FAILED]{@link S3dErrorCode} error.
     * The error message will be the command output.
     *
     * @param {string} command - The command and its arguments, separated by spaces.
     * @param {object} [options] - Additional options that control how the command should run.
     * @param {boolean} [options.internal=false] - If set to true, then internal commands can be executed.
     * @returns {S3dPromise<string>} A promise for the command output.
     */
    s3d.command = function (command, options) {
        return s3d.callMethod("command", function () {
            options = options || {};
            return {
                command: command.toString(),
                internal: (typeof options.internal === "boolean") ? options.internal : false
            };
        });
    }

    /**
     * (ASYNCHRONOUS) Pings a server.
     *
     * The screen will receive a [pong]{@link event:pong} event if the server responds.
     *
     * **This method is currently broken and will report ping times that are much higher than they should be.**
     *
     * @param {string} address - The IPv4 address of the server to ping. Must not include a port number.
     * @returns {S3dPromise} A promise that will be resolved once the ping is sent.
     */
	 s3d.ping = function (address, port=11774) {
        return s3d.callMethod("ping", function () {
            return {
                address: address.toString(),
				port: port
            };
        });
    }

    /**
     * (ASYNCHRONOUS) Gets info about the current map variant.
     *
     * If map variant info is not available, the promise will be rejected with a [S3dErrorCode.NOT_AVAILABLE]{@link S3dErrorCode} error.
     *
     * @returns {S3dPromise<MapVariantInfo>} A promise for the map variant info.
     */
    s3d.getMapVariantInfo = function () {
        return s3d.callMethod("mapVariantInfo", {}, jsonResultMapping);
    }

    /**
     * Contains information about a map variant.
     *
     * @typedef {object} MapVariantInfo
     * @property {string} name - The name. Can be empty.
     * @property {string} description - The description. Can be empty.
     * @property {string} author - The author. Can be empty.
     * @property {number} mapId - The map ID.
     * @see s3d.getMapVariantInfo
     */

    /**
     * (ASYNCHRONOUS) Gets info about the current game variant.
     *
     * If game variant info is not available, the promise will be rejected with a [S3dErrorCode.NOT_AVAILABLE]{@link S3dErrorCode} error.
     *
     * @returns {S3dPromise<GameVariantInfo>} A promise for the game variant info.
     */
    s3d.getGameVariantInfo = function () {
        return s3d.callMethod("gameVariantInfo", {}, jsonResultMapping);
    }

    /**
     * Contains information about a game variant.
     *
     * @typedef {object} GameVariantInfo
     * @property {GameMode} mode - The base game mode.
     * @property {string} name - The name. Can be empty.
     * @property {string} description - The description. Can be empty.
     * @property {string} author - The author. Can be empty.
     * @property {boolean} teams - `true` if teams are enabled.
     * @property {number} timeLimit - The time limit in minutes (0 if unlimited).
     * @property {number} rounds - The number of rounds.
     * @property {number} scoreToWin - The score-to-win (-1 if unlimited).
     * @see s3d.getGameVariantInfo
     */

    /**
     * (ASYNCHRONOUS) Gets a list of available console commands.
     *
     * @returns {S3dPromise<ConsoleCommand[]>} A promise for the list of available console commands.
     */
    s3d.getCommands = function () {
        return s3d.callMethod("commands", {}, jsonResultMapping);
    }

    /**
     * Contains information about a console command and its current state.
     *
     * @typedef {object} ConsoleCommand
     * @property {CommandType} type - The type of the command or variable.
     * @property {string} module - The module name.
     * @property {string} name - The name of the command or variable. This includes the module prefix.
     * @property {string} shortName - The short name of the command or variable.
     * @property {string} description - A description to display in a help listing.
     * @property {*} value - The current value of the variable. For commands, this will be `null`.
     * @property {*} defaultValue - The default value of the variable. For commands, this will be `null`.
     * @property {*} minValue - The minimum value of the variable. For string variables and commands, this will be `null`.
     * @property {*} maxValue - The maximum value of the variable. For string variables and commands, this will be `null`.
     * @property {boolean} replicated - `true` if the variable should be synchronized to clients.
     * @property {boolean} archived - `true` if the variable should be saved when the config file is written.
     * @property {boolean} hidden - `true` if the command or variable should be omitted from a help listing.
     * @property {boolean} hostOnly - `true` if the command or variable can only be used by the game host.
     * @property {boolean} hideValue - `true` if the variable's value should be omitted from a help listing.
     * @property {boolean} internal - `true` if the command or variable can only be set internally.
     * @property {string[]} arguments - A list of arguments for the command. Each string will contain a value name, a space, and then a description. For variables, this will be empty.
     * @see s3d.getCommands
     */

    /**
     * (ASYNCHRONOUS) Sends a chat message.
     *
     * @param {string} message - The chat message to send.
     * @param {boolean} teamChat - If true the message is sent to team chat instead of global.
     * @returns {S3dPromise<boolean>} A promise for the success of sending the message.
     */
    s3d.sendChat = function (message, teamChat) {
        return s3d.callMethod("sendChat", function () {
            return {
                message: message.toString(),
                teamChat: teamChat
            };
        });
     }

    /**
     * (ASYNCHRONOUS) Gets information about the current game multiplayer session.
     *
     * @param {string} message - The chat message to send.
     * @param {boolean} teamChat - If true the message is sent to team chat instead of global.
     * @returns {S3dPromise<SessionInfo>} A promise for the game multiplayer session information.
     */
    s3d.getSessionInfo = function () {
        return s3d.callMethod("sessionInfo", {}, jsonResultMapping);
    }

    /**
     * Contains information about the current game multiplayer session
     *
     * @typedef {object} SessionInfo
     * @property {boolean} established - `true` if a session is established.
     * @property {boolean} hasTeams - `true` if the current session has teams.
     * @property {boolean} isHost - `true` if the player is the host of the session.
     * @property {string} mapName - Name of the currently loaded map.
     * @see s3d.getSessionInfo
     */

    /**
     * (ASYNCHRONOUS) Gets information about a player's stats.
     *
     * @param {string} playerName - The name of the player.
     * @returns {S3dPromise<StatsInfo>} A promise for the player's stats.
     */
     s3d.getStats = function (playerName) {
         return s3d.callMethod("stats", { playerName: playerName }, jsonResultMapping);
     }

    /**
     * Contains information about the player's stats
     *
     * @typedef {object} StatsInfo
     * @property {number[]} medals - The medals earned during this game.
     * @property {WeaponStats[]} weapons - Information about the weapons used during this game.
     * @see s3d.getStats
     */

    /**
     * Information about a weapon's use in the current game
     *
     * @typedef {object} WeaponStats
     * @property {number} BetrayalsWith - How many times the player used this weapon to betray their teammates.
     * @property {number} HeadshotsWith - How many headshots the player got with this weapon.
     * @property {number} KilledBy - How many times the player was killed by this weapon.
     * @property {number} Kills - How many kills the player got with this weapon.
     * @property {number} SuicidesWith - How many times the player killed themselves with this weapon.
     * @see StatsInfo
     */

	/**		
     * (ASYNCHRONOUS) Gets information about the current game's scoreboard.		
     *		
     * @returns {S3dPromise<ScoreboardInfo>} A promise for the scoreboard information.		
     */	
	 
    s3d.getScoreboard = function () {		
       return s3d.callMethod("scoreboard", {}, jsonResultMapping);		
    }		
	
   /**		
    * Contains information about the scoreboard.		
    *		
    * @typedef {object} ScoreboardInfo		
    * @property {boolean} hasTeams - `true` if the current session has teams.		
    * @property {number[]} teamScores - The scores of all of the teams in the game.		
    * @property {string} gameType - The gamemode type.		
    * @property {ScoreboardPlayer[]} players - Players listed on the scoreboard.		
    * @see s3d.getScoreboard		
    */		
	
   /**		
    * Contains information about a player on the scoreboard.		
    *		
    * @typedef {object} ScoreboardPlayer		
    * @property {string} name - The player's name.		
    * @property {number} team - The player's team index.		
    * @property {string} color - The player's primary armor color.		
    * @property {string} UID - The player's UID.		
    * @property {number} kills - The number of kills.		
    * @property {number} assists - The number of assists.		
    * @property {number} deaths - The number of deaths.		
    * @property {number} score - The player's score.		
    * @see ScoreboardInfo		
    */
	
    /**
     * (ASYNCHRONOUS) Requests to submit the currently-active virtual keyboard.
     * 
     * @param {string} value - The value to submit.
     * @returns {S3dPromise} A promise that will be resolved once the keyboard is submitted.
     */
    s3d.submitVirtualKeyboard = function (value) {
        return s3d.callMethod("submitVirtualKeyboard", function () {
            return {
                value: value.toString(),
            };
        });
    }

    /**
     * (ASYNCHRONOUS) Requests to cancel the currently-active virtual keyboard.
     * 
     * @returns {S3dPromise} A promise that will be resolved once the keyboard is cancelled.
     */
    s3d.cancelVirtualKeyboard = function (value) {
        return s3d.callMethod("cancelVirtualKeyboard");
    }

    /**
     * Registers a callback to be run when an event occurs.
     *
     * @name s3d.on
     * @function
     * @param {string} event - The name of the event to register a callback for (e.g. "show").
     * @param {EventCallback} callback - The callback to register.
     * @see event:show
     * @see event:hide
     * @see event:pong
     * @see event:console
     * @see event:mpevent
     * @see event:loadprogress
	 * @see event:signal-ready
     */
    s3d.on = function (event, callback) {
        registerEvent(event, callback);
    }
	
	/**
     * (ASYNCHRONOUS) Queues a game action to be pressed in-game
     *
     * @param {integer} actionIndex - The index of the game action to be pressed.
     */
	s3d.gameaction = function(actionIndex) {
		return s3d.callMethod("gameaction", {
			key: actionIndex
		});
	}
	
	//This is a workaround for a bug in jsdoc. Event definitions must be inside a named function
	s3d.functionforevents = function() {
		/**
		 * A callback function for responding to events.
		 *
		 * @callback EventCallback
		 * @param {object} event - The event information.
		 * @param {string} event.event - The name of the event.
		 * @param {string} event.screen - The ID of the screen the event was sent to.
		 * @param {string} event.data - Event-specific data.
		 */

		/**
		 * Fired after the current screen is shown.
		 * The data passed to this event is the data that was passed to {@link s3d.show}.
		 *
		 * @event show
		 * @type {object}
		 */

		/**
		 * Fired after the current screen is hidden.
		 *
		 * @event hide
		 * @type {object}
		 */

		/**
		 * Fired when a server replies to a [ping]{@link s3d.ping}.
		 * This can only be received while the screen is active.
		 *
		 * @event pong
		 * @type {object}
		 * @property {string} address - The IP address of the server.
		 * @property {number} latency - The round-trip time of the ping in milliseconds.
		 * @property {number} timestamp - A timestamp value representing when the ping was sent.
		 */

		/**
		 * Fired when a line is written to the in-game console.
		 *
		 * @event console
		 * @type {object}
		 * @property {string} line - The line that was written. **Make sure to escape this properly before displaying it.**
		 */

		/**		
		 * Fires when the scoreboard is updated		
		 *		
		 * @event scoreboard		
		 * @type {ScoreboardInfo}		
		 */
	 
		/**
		 * Fired when a multiplayer event occurs that affects the local player.
		 *
		 * @event mpevent
		 * @type {object}
		 * @property {string} name - The internal name of the event.
		 * @property {MPEventCategory} category - The event's category.
		 * @property {MPEventAudience} audience - The audience that the event is intended for.
		 */

		/**
		 * Fired when the loading screen is active and the loading progress changes.
		 *
		 * This event is only sent to visible screens.
		 *
		 * @event loadprogress
		 * @type {object}
		 * @property {number} currentBytes - The current number of bytes that have been decompressed.
		 * @property {number} totalBytes - The total number of bytes that need to be decompressed.
		 */

		/**
		 * Fired when a chat message is received.
		 *
		 * @event chat
		 * @type {object}
		 * @property {string} message - The chat message. **Make sure to escape this properly before displaying it.**
		 * @property {string} sender - The username that sent this message. **Make sure to escape this properly before displaying it.**
		 * @property {string} chatType - The type of message that was received. Can be "GLOBAL", "TEAM", "WHISPER", or "SERVER".
		 * @property {number} teamIndex - The team index of the player. Not included when chatType is "SERVER".
		 * @property {string} UID - The UID of the player. Not included when chatType is "SERVER".
		 * @property {string} color - The hex color of the player's primary armor color. Not included when chatType is "SERVER".
		 */

		 /**
		  * Fired when attempting to connect to a server and when the connection succeeds or fails.
		  *
		  * @event serverconnect
		  * @type {object}
		  * @property {boolean} connecting - True if the client is currently trying to connect to the server.
		  * @property {boolean} success - True if the client successfully connected to the server. Will always be false if connecting is true.
		  */
		  
		 /**
		  * Fired when the game client receives the websocket connection information packet.
		  * 
		  * @event signal-ready
		  * @type {object}
		  * @property {string} server - The ip + port of the signal server in the format ip:port
		  * @property {string} password - The password assigned for the client to connect with
		  */
	}
})();
