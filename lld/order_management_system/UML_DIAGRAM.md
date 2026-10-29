# Order Management System - UML Diagram

## Class Diagram

```
┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                    ORDER MANAGEMENT SYSTEM - CLASS DIAGRAM                                    │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────┐          ┌─────────────────────┐          ┌─────────────────────┐
│   ProductCategory   │          │      Product        │          │      Address        │
├─────────────────────┤          ├─────────────────────┤          ├─────────────────────┤
│ <<enumeration>>     │          │ - productId: string │          │ - street: string    │
│ ELECTRONICS         │◄─────────│ - name: string      │          │ - city: string      │
│ FASHION             │          │ - description: str  │          │ - state: string     │
│ GROCERY             │          │ - price: double     │          │ - zipCode: string   │
│ FURNITURE           │          │ - category: enum    │          │ - country: string   │
│ SPORTS              │          ├─────────────────────┤          │ - latitude: double  │
│ BOOKS               │          │ + getPrice()        │          │ - longitude: double │
│ HOME_APPLIANCES     │          │ + getName()         │          ├─────────────────────┤
└─────────────────────┘          │ + toString()        │          │ + distanceTo()      │
                                 └─────────────────────┘          │ + toString()        │
                                           │                      └─────────────────────┘
                                           │                                │
                                           ▼                                ▼
┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                              CORE ENTITIES                                                    │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────┐          ┌─────────────────────┐          ┌─────────────────────┐
│       User          │          │       Cart          │          │     CartItem        │
├─────────────────────┤          ├─────────────────────┤          ├─────────────────────┤
│ - userId: string    │          │ - cartId: string    │◆─────────│ + product: Product* │
│ - name: string      │◄─────────│ - userId: string    │          │ + quantity: int     │
│ - email: string     │          │ - items: map        │          ├─────────────────────┤
│ - phone: string     │          ├─────────────────────┤          │ + getTotalPrice()   │
│ - address: Address* │          │ + addProduct()      │          └─────────────────────┘
├─────────────────────┤          │ + removeProduct()   │
│ + getAddress()      │          │ + getTotalValue()   │
│ + toString()        │          │ + isEmpty()         │
└─────────────────────┘          └─────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                         WAREHOUSE & INVENTORY                                                 │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────┐          ┌─────────────────────┐          ┌─────────────────────────────────────┐
│     Inventory       │          │     Warehouse       │          │   WarehouseSelectionStrategy        │
├─────────────────────┤          ├─────────────────────┤          │        <<interface>>                │
│ - products: map     │          │ - warehouseId: str  │          ├─────────────────────────────────────┤
│ - stock: map        │◄─────────│ - name: string      │◄─────────│ + selectWarehouse(warehouses,      │
├─────────────────────┤          │ - address: Address* │          │     cart, deliveryAddress)          │
│ + addProduct()      │          │ - inventory: ptr    │          │ + getStrategyName()                 │
│ + removeProduct()   │          ├─────────────────────┤          └─────────────────────────────────────┘
│ + isAvailable()     │          │ + addProduct()      │                          ▲
│ + getQuantity()     │          │ + hasProduct()      │                          │
└─────────────────────┘          │ + distanceTo()      │          ┌───────────────┴───────────────┐
                                 └─────────────────────┘          │                               │
                                                    ┌─────────────────────────┐  ┌─────────────────────────┐
                                                    │ NearestWarehouse        │  │ HighestStockWarehouse   │
                                                    │ SelectionStrategy       │  │ SelectionStrategy       │
                                                    ├─────────────────────────┤  ├─────────────────────────┤
                                                    │ + selectWarehouse()     │  │ + selectWarehouse()     │
                                                    │ + getStrategyName()     │  │ + getStrategyName()     │
                                                    └─────────────────────────┘  └─────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                           PAYMENT SYSTEM                                                      │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

┌───────────────────────────────────────┐          ┌─────────────────────┐
│         PaymentMode                   │          │      Payment        │
│          <<interface>>                │          ├─────────────────────┤
├───────────────────────────────────────┤          │ - paymentId: string │
│ + processPayment(amount): bool        │◄─────────│ - orderId: string   │
│ + getPaymentModeName(): string        │          │ - amount: double    │
│ + validateDetails(): bool             │          │ - paymentMode: ptr  │
│ + refund(amount): bool                │          │ - status: enum      │
└───────────────────────────────────────┘          │ - transactionTime   │
              ▲                                    ├─────────────────────┤
              │                                    │ + processPayment()  │
    ┌─────────┴─────────┐                          │ + refund()          │
    │                   │                          └─────────────────────┘
┌─────────────────┐  ┌─────────────────┐
│ UPIPaymentMode  │  │ CardPaymentMode │
├─────────────────┤  ├─────────────────┤
│ - upiId: string │  │ - cardNumber    │
│ - pin: string   │  │ - cardHolder    │
├─────────────────┤  │ - expiryDate    │
│ + processPayment│  │ - cvv: string   │
│ + validate()    │  │ - cardType: enum│
│ + refund()      │  ├─────────────────┤
└─────────────────┘  │ + processPayment│
                     │ + validate()    │
                     │ + getMasked()   │
                     └─────────────────┘

┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                              ORDER SYSTEM                                                     │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────┐          ┌─────────────────────────┐          ┌─────────────────────┐
│    OrderStatus      │          │         Order           │          │      Invoice        │
├─────────────────────┤          ├─────────────────────────┤          ├─────────────────────┤
│ <<enumeration>>     │          │ - orderId: string       │◄─────────│ - invoiceId: string │
│ PENDING             │◄─────────│ - user: User*           │          │ - order: Order*     │
│ CONFIRMED           │          │ - items: vector         │          │ - subtotal: double  │
│ PROCESSING          │          │ - totalAmount: double   │          │ - taxRate: double   │
│ SHIPPED             │          │ - status: OrderStatus   │          │ - taxAmount: double │
│ OUT_FOR_DELIVERY    │          │ - deliveryAddress       │          │ - shippingCost      │
│ DELIVERED           │          │ - warehouse: Warehouse* │          │ - discount: double  │
│ CANCELLED           │          │ - payment: Payment*     │          │ - totalAmount       │
│ RETURNED            │          │ - orderTime: string     │          ├─────────────────────┤
│ REFUNDED            │          ├─────────────────────────┤          │ + generateInvoice() │
└─────────────────────┘          │ + confirm()             │          │ + calculateTotals() │
                                 │ + ship()                │          └─────────────────────┘
                                 │ + deliver()             │
                                 │ + cancel()              │
                                 └─────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                            CONTROLLERS (SINGLETON)                                            │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────┐     ┌─────────────────────────┐     ┌─────────────────────────┐
│    UserController       │     │  WarehouseController    │     │    OrderController      │
├─────────────────────────┤     ├─────────────────────────┤     ├─────────────────────────┤
│ - users: map            │     │ - warehouses: map       │     │ - orders: map           │
│ - userIdCounter: int    │     │ - selectionStrategy     │     │ - userOrders: map       │
├─────────────────────────┤     │ - warehouseIdCounter    │     │ - orderIdCounter: int   │
│ + getInstance()         │     ├─────────────────────────┤     ├─────────────────────────┤
│ + registerUser()        │     │ + getInstance()         │     │ + getInstance()         │
│ + getUser()             │     │ + addWarehouse()        │     │ + createOrder()         │
│ + getAllUsers()         │     │ + getWarehouse()        │     │ + processPayment()      │
│ + updateUserAddress()   │     │ + selectWarehouse()     │     │ + getOrder()            │
│ + removeUser()          │     │ + setSelectionStrategy()│     │ + getOrdersByUser()     │
└─────────────────────────┘     └─────────────────────────┘     │ + cancelOrder()         │
                                                                └─────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                          FACADE - MAIN ENTRY POINT                                            │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

                              ┌───────────────────────────────────────────┐
                              │        ProductDeliverySystem              │
                              │           <<Singleton Facade>>            │
                              ├───────────────────────────────────────────┤
                              │ - userController: UserController&         │
                              │ - warehouseController: WarehouseController│
                              │ - orderController: OrderController&       │
                              │ - userCarts: map<string, Cart*>           │
                              ├───────────────────────────────────────────┤
                              │ + getInstance()                           │
                              │ // User Management                        │
                              │ + registerUser()                          │
                              │ + getUser()                               │
                              │ // Warehouse Management                   │
                              │ + addWarehouse()                          │
                              │ + addProductToWarehouse()                 │
                              │ + setWarehouseSelectionStrategy()         │
                              │ // Cart Management                        │
                              │ + addToCart()                             │
                              │ + removeFromCart()                        │
                              │ + clearCart()                             │
                              │ // Order Management                       │
                              │ + placeOrder()                            │
                              │ + payWithUPI()                            │
                              │ + payWithCard()                           │
                              │ + cancelOrder()                           │
                              │ + updateOrderStatus()                     │
                              │ // Invoice                                │
                              │ + generateInvoice()                       │
                              └───────────────────────────────────────────┘
```

## Sequence Diagram - Place Order Flow

```
┌────────┐  ┌─────────────────────┐  ┌────────┐  ┌────────────────────┐  ┌──────────────┐  ┌─────────┐  ┌─────────┐
│ Client │  │ProductDeliverySystem│  │  Cart  │  │ WarehouseController│  │OrderController│  │ Payment │  │ Invoice │
└───┬────┘  └──────────┬──────────┘  └───┬────┘  └─────────┬──────────┘  └──────┬───────┘  └────┬────┘  └────┬────┘
    │                  │                 │                  │                   │               │            │
    │  addToCart()     │                 │                  │                   │               │            │
    │─────────────────>│                 │                  │                   │               │            │
    │                  │ addProduct()    │                  │                   │               │            │
    │                  │────────────────>│                  │                   │               │            │
    │                  │<────────────────│                  │                   │               │            │
    │                  │                 │                  │                   │               │            │
    │  placeOrder()    │                 │                  │                   │               │            │
    │─────────────────>│                 │                  │                   │               │            │
    │                  │  getItems()     │                  │                   │               │            │
    │                  │────────────────>│                  │                   │               │            │
    │                  │     items       │                  │                   │               │            │
    │                  │<────────────────│                  │                   │               │            │
    │                  │                 │                  │                   │               │            │
    │                  │       selectWarehouse()            │                   │               │            │
    │                  │───────────────────────────────────>│                   │               │            │
    │                  │        selectedWarehouse           │                   │               │            │
    │                  │<───────────────────────────────────│                   │               │            │
    │                  │                 │                  │                   │               │            │
    │                  │                      createOrder(user, cart, address)  │               │            │
    │                  │────────────────────────────────────────────────────────>               │            │
    │                  │                          order                         │               │            │
    │                  │<────────────────────────────────────────────────────────               │            │
    │       order      │                 │                  │                   │               │            │
    │<─────────────────│                 │                  │                   │               │            │
    │                  │                 │                  │                   │               │            │
    │  payWithCard()   │                 │                  │                   │               │            │
    │─────────────────>│                 │                  │                   │               │            │
    │                  │                         processPayment(order, mode)    │               │            │
    │                  │────────────────────────────────────────────────────────>               │            │
    │                  │                 │                  │                   │  create       │            │
    │                  │                 │                  │                   │──────────────>│            │
    │                  │                 │                  │                   │  process()    │            │
    │                  │                 │                  │                   │──────────────>│            │
    │                  │                 │                  │                   │   success     │            │
    │                  │                 │                  │                   │<──────────────│            │
    │                  │                        true        │                   │               │            │
    │                  │<────────────────────────────────────────────────────────               │            │
    │      success     │                 │                  │                   │               │            │
    │<─────────────────│                 │                  │                   │               │            │
    │                  │                 │                  │                   │               │            │
    │generateInvoice() │                 │                  │                   │               │            │
    │─────────────────>│                 │                  │                   │               │            │
    │                  │                 │                  │                   │        create │            │
    │                  │────────────────────────────────────────────────────────────────────────────────────>│
    │                  │                 │                  │                   │       invoice │            │
    │                  │<────────────────────────────────────────────────────────────────────────────────────│
    │      invoice     │                 │                  │                   │               │            │
    │<─────────────────│                 │                  │                   │               │            │
    │                  │                 │                  │                   │               │            │
```

## Design Patterns Used

| Pattern | Implementation | Purpose |
|---------|---------------|---------|
| **Strategy** | `WarehouseSelectionStrategy`, `PaymentMode` | Interchangeable algorithms for warehouse selection and payment processing |
| **Singleton** | Controllers, `ProductDeliverySystem` | Single point of access for system management |
| **Facade** | `ProductDeliverySystem` | Simplified interface to the complex subsystem |
| **Composite** | `Cart` with `CartItem` | Tree structure for cart items |

## Key Relationships

1. **User** has an **Address** and multiple **Orders**
2. **Cart** contains multiple **CartItems** (Product + Quantity)
3. **Warehouse** has an **Inventory** with **Products**
4. **Order** is fulfilled by a **Warehouse** and paid via **Payment**
5. **Invoice** is generated for an **Order**
6. **WarehouseSelectionStrategy** determines which warehouse fulfills an order
7. **PaymentMode** strategy handles different payment methods
