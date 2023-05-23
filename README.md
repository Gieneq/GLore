# Some C game

Text game in which you navigate by writing text. The terminal and imagination are your's GPU.

<p align="center">
    <img src="img/ss_GLore_0.0.2.PNG" width="500" alt="Recent text game state">
</p>

## Current game state
- Player can walk around test world and look at rooms content. Cool!
- Player can talk to NPCs. NPCs can have complex dialog based on keywords and dialog stages.

## Changelog
- Processing user input,
- Keywords detection,
- Room roads introduced,
- Player moving on roads,
- Player talking to NPCs,
- Colored text on Windows target,
- Introduced buildsystem with google (unit) tests,
- Keywords matching accept leading whitespace,
- Data file (json) for world with rooms and roads, player, npcs,
- Adde 'hint' command to print possible dialog keywords,

## Bugs
- [x] keywords match substring of words e.g. go is inside good,
- [x] keywords should ignorecase,

## Todo
- [ ] player and npc set name different return
- [ ] use cmake project version in C code,
- [ ] bye should drop from any state,
- [x] bye could not require NPC name if only 1 conversation at the time,
- [x] Drop should be included as farawell block, leave room,
- [ ] Inventory for player, inventory system,
- [ ] Stats and skills for player,
- [x] If player has active talk, cannot change to other NPC,
- [ ] Use wildcards in responses,
- [x] strip spaces from end of user input - normalize string,
- [ ] room iterator not working with pointer array (see system_user_input 'go'),
- [ ] some option to wrap words for preset display width, center text, draw boxes, option butlabels (yes/no),
- [ ] when building loader consider reducing stack usage,
- [ ] in 'keywords_list_from_delimited_string' replace stirng delim with a char,

## Ideas
- [ ] Long text from game/narrator - hang game, press enter to continue,
- [ ] Enemy/boss room - combat stage + dialogs to choose actions like attack, use object (lever, climb...),
- [ ] Ambient sound player from time to time, or 'hear' keyword.
- [ ] Property for room e.g. to indicate fishing spot with skill requirements,