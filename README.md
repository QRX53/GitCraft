# GitCraft
 Exceedingly shitty minecraft save system i made

Reads config from `~/.config/gitcraftd/config/txt`

Config file should be 2 lines:
1. Your minecraft saves folder (`/Users/qrx53/Library/Application\ Support/minecraft/saves`) for me
2. The amount of time (in seconds) to sleep inbetween autosaves.

Uses git to save the state of your saves folder, so you have to have git installed.

This is likely the most inefficient way to go about this, but that's okay.
