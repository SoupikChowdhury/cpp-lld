# Vending Machine LLD - UML Class Diagram

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                         VENDING MACHINE SYSTEM                               │
│                    Patterns: State, Singleton (optional)                     │
└─────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│                              ENTITIES                                        │
└─────────────────────────────────────────────────────────────────────────────┘

    <<enum>>                     <<enum>>
┌────────────────┐          ┌────────────────┐
│     Coin       │          │    ItemType    │
├────────────────┤          ├────────────────┤
│ PENNY = 1      │          │ COKE           │
│ NICKEL = 5     │          │ PEPSI          │
│ DIME = 10      │          │ JUICE          │
│ QUARTER = 25   │          │ SODA           │
└────────────────┘          └────────────────┘

┌────────────────┐          ┌────────────────┐          ┌────────────────────┐
│     Item       │          │   ItemShelf    │          │     Inventory      │
├────────────────┤          ├────────────────┤          ├────────────────────┤
│ - type: ItemType│◄────────│ - code: int    │          │ - shelves: vector  │
│ - price: int   │  has     │ - item: Item*  │◄─────────│   <ItemShelf>      │
├────────────────┤          │ - soldOut: bool│  contains├────────────────────┤
│ + getType()    │          ├────────────────┤          │ + addItem()        │
│ + getPrice()   │          │ + getCode()    │          │ + getItem()        │
│ + setType()    │          │ + getItem()    │          │ + updateSoldOut()  │
│ + setPrice()   │          │ + isSoldOut()  │          └────────────────────┘
└────────────────┘          └────────────────┘


┌─────────────────────────────────────────────────────────────────────────────┐
│                           STATE PATTERN                                      │
└─────────────────────────────────────────────────────────────────────────────┘

                          ┌─────────────────────────────┐
                          │        <<interface>>        │
                          │           State             │
                          ├─────────────────────────────┤
                          │ + clickOnInsertCoinButton() │
                          │ + clickOnStartSelection()   │
                          │ + insertCoin()              │
                          │ + chooseProduct()           │
                          │ + getChange()               │
                          │ + refundFullMoney()         │
                          │ + dispenseProduct()         │
                          │ + updateInventory()         │
                          └─────────────────────────────┘
                                        ▲
                                        │ implements
          ┌─────────────────┬───────────┼───────────┬─────────────────┐
          │                 │           │           │                 │
┌─────────┴───────┐ ┌───────┴─────┐ ┌───┴─────────┐ ┌───────┴───────┐
│   IdleState     │ │HasMoneyState│ │SelectionState│ │ DispenseState │
├─────────────────┤ ├─────────────┤ ├──────────────┤ ├───────────────┤
│                 │ │             │ │              │ │               │
├─────────────────┤ ├─────────────┤ ├──────────────┤ ├───────────────┤
│ Only allows:    │ │ Allows:     │ │ Allows:      │ │ Only:         │
│ - insertCoinBtn │ │ - insertCoin│ │ - choose     │ │ - dispense    │
│ - updateInventory│ │ - selectBtn │ │   Product    │ │               │
└─────────────────┘ │ - refund    │ │ - refund     │ │ Then → Idle   │
                    └─────────────┘ │ - getChange  │ └───────────────┘
                                    └──────────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│                         CONTEXT (VENDING MACHINE)                            │
└─────────────────────────────────────────────────────────────────────────────┘

                    ┌───────────────────────────────┐
                    │        VendingMachine         │
                    │          (Context)            │
                    ├───────────────────────────────┤
                    │ - currentState: State*        │─────────► State
                    │ - inventory: Inventory*       │─────────► Inventory
                    │ - coinList: vector<Coin>      │
                    ├───────────────────────────────┤
                    │ + getState()                  │
                    │ + setState(State*)            │
                    │ + getInventory()              │
                    │ + getCoinList()               │
                    │ + addCoin()                   │
                    └───────────────────────────────┘


┌─────────────────────────────────────────────────────────────────────────────┐
│                         STATE TRANSITION DIAGRAM                             │
└─────────────────────────────────────────────────────────────────────────────┘

    ┌─────────┐    clickInsertCoin    ┌─────────────┐
    │  IDLE   │ ────────────────────► │  HAS_MONEY  │
    └─────────┘                       └─────────────┘
         ▲                                   │
         │                                   │ clickSelection
         │ refund                            ▼
         │                            ┌─────────────┐
         │                            │  SELECTION  │
         │                            └─────────────┘
         │                                   │
         │ refund                            │ chooseProduct
         │◄──────────────────────────────────┤ (if sufficient money)
         │                                   ▼
         │                            ┌─────────────┐
         └────────────────────────────│  DISPENSE   │
              after dispense          └─────────────┘
```

## Key Design Decisions

1. **State Pattern**: Machine behavior changes based on current state
2. **Immutable Items**: Items are value objects
3. **Inventory Management**: Centralized slot management
4. **Error Handling**: Invalid operations throw exceptions
