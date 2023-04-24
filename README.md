# Some C game

## Iteration 1
- [x] create player,
- [x] add option to pass commands,
- [x] add command to check player stats,
- [x] add command to quit,

## Iteration 2
- [x] add data files,
- [x] load items data,
- [ ] load world/rooms data,
- [ ] load quest data,
- [ ] load objects data,
- [ ] load NPCs data,
- [ ] load player data,

## Iteration 3
- [ ] Affter loading check id's between database tables,


## toremove

typy questow
- przynies przedmioty,
- nic, pogadaj z kims,
- zabij cos,

nagrody:
- itemy
- exp

quest ma etapy
questlog ma obecny etap dla gracza

tak:
- quest ma id od 1 do np 255 ale beda maskowane
- dodac wspolna maske id
- quest powinien zawierac dialogi, przejscia stanow
- watek kquestu dodac z entrypoint:
   - jakis stan, kewords na te z questu <$quest_entry_kw$>

WIELE QUESTOW - ok ten sam system stagow, ale dodac dla npc zmienna kontekst id questa i kaskade w ifach od wykonania z questlogu

podobnie dodaje sklep do npc
dopiero w if/then info o id questu
