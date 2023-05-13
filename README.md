# Some C game

Text game in which you navigate by writing text. The terminal and imagination are your's GPU.

<p align="center">
    <img src="img/ss_GLore_0.0.2.PNG" width="500" alt="Recent text game state">
</p>

## Current state
- Player can walk around test world and look at rooms content. Cool!
- Player can talk to NPCs. NPCs can have complex dialog based on keywords and dialog stages.

## Changelog
- Processing user input,
- Keywords detection,
- Room roads introduced,
- Player moving on roads,
- Player talking to NPCs,
- Colored text on Windows target,

## Bugs
- [ ] keywords match substring of words e.g. go is inside good,
- [ ] keywords should ignorecase,

## Todo
- [ ] bye should drop from any state,
- [ ] bye could not require NPC name if only 1 conversation at the time,
- [ ] Drop should be included as farawell block, leave room,
- [ ] If player has active talk, cannot change to other NPC,
- [ ] Use wildcards in responses,
- [ ] strip spaces from end of user input,
- [ ] room iterator not working with pointer array (see system_user_input 'go'),
- [ ] some option to wrap words for preset display width, center text, draw boxes, option butlabels (yes/no),
- [ ] when building loader consider reducing stack usage,

## Ideas
- [ ] Long text from game/narrator - hang game, press enter to continue,
- [ ] Enemy/boss room - combat stage + dialogs to choose actions like attack, use object (lever, climb...),