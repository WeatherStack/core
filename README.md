# WeatherStack Core
For more information, please visit [the documentation](https://weatherstack.wtdawson.info/docs/core/).

This is the core of the project, the server in the middle that controls configuration, and collects data.

## Internal structure
WeatherStack Core is made up of a number of different components.
The main two are `ws_core`, and `ws_utils`. `ws_core` is the actual core, the part that runs on a server.
`ws_utils` is a separate utility CLI with utilities to make it easier to setup, maintain, and manage
a WeatherStack Core instance. It comes with a lot of the same components as `ws_core`, but provides a way
for the user to use them.

| Part       | Description                                    |
| :--------- | :--------------------------------------------- |
| `ws_core`  | WeatherStack Core                              |
| `ws_utils` | Utilities for WeatherStack Core                |
| `ws_auth`  | WeatherStack Core authentication library (ish) |

## Databases
This will most likely end up using 2 databases: one for configuration data, and another for time series
data.