# Data files concepts

By default NPC has dummy behaviour "Hi - Bye". It can be extended with:
 - some dialog, informational, changing dialog state,
 - shop dialog
 - quests dialog with multiple behaviours.

NPC dialog has its stages. Initial stage is 0 - no converstion. When saying "Hi" to NPC, it's dialog stage goes to 1. When saying "Bye" it goes back to 0.

If NPC don't understand any kewords it's dialog state remains the same.

Quests and shop has their own dialog states with bitmask.

Quest state is not dialog state.

If in stage 1 kewords are assigned to quest with id let's say 1 and stage 0 (start):

```json
{
    "if": {
        "stage": 0,
        "keywords": [
            "Hi",
            "Hello"
        ]
    },
    "then": {
        "responses": [
            "Hello, how are you?",
            "Hi, how are you?"
        ],
        "next_stage": 1
    }
}
```

Default usabe dialog state is 1

Stage 0 only has keword for start - it will be treated as condition requred to enter quest dialog:
 - has keword,
 - has quest id,
 - has proper quest stage.


## Special dialog wildcards

To make dialogos more elasti there are wildcards to replace player name, npc name, item name and similar.

Wildcards:
 - NPC name: "<$npc.name$>"
 - Player name: "<$player.name$>"
 - Quest requirements: "<$quest.requirements$>"



## Quest


```json
```