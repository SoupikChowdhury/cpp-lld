# Order Management System - LLD Interview Guide

A step-by-step framework for delivering the e-commerce order management system low-level design interview.

---

## 📁 Code Reference

| File | Purpose | Key Functions |
|------|---------|---------------|
| [ProductDeliverySystem.hpp](ProductDeliverySystem.hpp) | Main facade (Singleton) | `registerUser()`, `addWarehouse()`, `addToCart()`, `placeOrder()`, `setWarehouseSelectionStrategy()` |
| [UserController.hpp](UserController.hpp) | User management (Singleton) | `addUser()`, `getUser()`, `getAllUsers()` |
| [WarehouseController.hpp](WarehouseController.hpp) | Warehouse management (Singleton) | `addWarehouse()`, `getWarehouses()` |
| [OrderController.hpp](OrderController.hpp) | Order management (Singleton) | `createOrder()`, `getOrderById()`, `updateOrderStatus()` |
| [Warehouse.hpp](Warehouse.hpp) | Warehouse with inventory | `addProduct()`, `hasProduct()`, `removeProduct()`, `distanceTo()` |
| [Inventory.hpp](Inventory.hpp) | Product inventory | `addProduct()`, `getProduct()`, `hasStock()` |
| [Order.hpp](Order.hpp) | Order entity | `confirm()`, `ship()`, `deliver()`, `cancel()` |
| [OrderStatus.hpp](OrderStatus.hpp) | Order status enum | `PENDING`, `CONFIRMED`, `SHIPPED`, `DELIVERED`, `CANCELLED` |
| [Cart.hpp](Cart.hpp) | Shopping cart | `addProduct()`, `removeProduct()`, `getProducts()`, `getTotalPrice()` |
| [Product.hpp](Product.hpp) | Product entity | `getName()`, `getPrice()`, `getCategory()` |
| [ProductCategory.hpp](ProductCategory.hpp) | Product category enum | `ELECTRONICS`, `CLOTHING`, `FOOD` |
| [Invoice.hpp](Invoice.hpp) | Invoice generation | `generateInvoice()` |
| [Payment.hpp](Payment.hpp) | Payment entity | `processPayment()` |
| [PaymentMode.hpp](PaymentMode.hpp) | Payment strategy interface | `pay()` |
| [UPIPaymentMode.hpp](UPIPaymentMode.hpp) | UPI payment implementation | `pay()` - UPI-specific logic |
| [CardPaymentMode.hpp](CardPaymentMode.hpp) | Card payment implementation | `pay()` - Card-specific logic |
| [WarehouseSelectionStrategy.hpp](WarehouseSelectionStrategy.hpp) | Warehouse selection interface | `selectWarehouse()` |
| [NearestWarehouseSelectionStrategy.hpp](NearestWarehouseSelectionStrategy.hpp) | Nearest warehouse strategy | `selectWarehouse()` - selects by proximity |
| [User.hpp](User.hpp) | User entity | `getName()`, `getAddress()`, `getCart()` |
| [Address.hpp](Address.hpp) | Address entity | `getCity()`, `getZipCode()` |

---

## 🎯 Design Patterns Used

| Pattern | Implementation Location | Purpose |
|---------|------------------------|----------|
| **Strategy Pattern** | [WarehouseSelectionStrategy.hpp](WarehouseSelectionStrategy.hpp), [PaymentMode.hpp](PaymentMode.hpp) | Pluggable warehouse selection and payment algorithms |
| **Singleton Pattern** | [ProductDeliverySystem.hpp](ProductDeliverySystem.hpp), [UserController.hpp](UserController.hpp), [WarehouseController.hpp](WarehouseController.hpp), [OrderController.hpp](OrderController.hpp) | Single instance for controllers |
| **Facade Pattern** | [ProductDeliverySystem.hpp](ProductDeliverySystem.hpp) | Unified interface for order management |

---

## 1) Requirements (~5 minutes)

### Clarifying Questions to Ask
- "What operations should users be able to perform? Browse, add to cart, checkout?"
- "Are there multiple warehouses? How should order fulfillment work?"
- "What payment methods should we support?"
- "Should we handle inventory tracking and updates?"
- "Do orders need status tracking (pending, shipped, delivered)?"

### Requirements

```
Requirements:
1. Users can register and manage their account with delivery addresses.
2. Users can browse products from a catalog.
3. Users can add/remove products to/from a shopping cart.
4. Users can place orders from their cart.
5. System selects appropriate warehouse based on inventory and proximity.
6. Payment is processed with multiple method support (UPI, Card).
7. Order status is tracked through its lifecycle.
8. Invoice is generated after successful order placement.

Out of Scope:
- Product reviews and ratings
- Recommendations engine
- Wishlist functionality
- Order cancellation and refunds
- Delivery partner integration
- Real-time inventory sync across warehouses
```

---

## 2) Entities and Relationships (~3 minutes)

### Identify Entities

Scan the requirements and pull out meaningful nouns:

- **User** - Customer with cart and addresses
- **Product** - Item in catalog
- **Cart** - Shopping cart with items
- **Order** - Placed order with status
- **Warehouse** - Stores inventory
- **Inventory** - Stock management per warehouse
- **Payment** - Payment processing
- **Invoice** - Order receipt

### Define Relationships

```
Entities:
- User
- Product
- Cart
- Order
- Warehouse
- Inventory
- Payment (abstract, PaymentMode implementations)
- Invoice
- Controllers (UserController, OrderController, WarehouseController)
- ProductDeliverySystem (Facade)
- WarehouseSelectionStrategy (abstract strategy)

Relationships:
- User -> Cart (1:1)
- User -> Address (1:N)
- User -> Order (1:N)
- Cart -> Product (N:M with quantity)
- Order -> Product (1:N)
- Warehouse -> Inventory (1:1)
- Inventory -> Product (N:M with quantity)
- Order -> Payment (1:1)
- Order -> Invoice (1:1)
```

---

## 3) Class Design (~10-15 minutes)

### User

| Requirement | State Needed |
|-------------|--------------|
| User identity | userId, name, email |
| Shopping cart | cart |
| Delivery addresses | addresses |

```
class User:
  - userId: string
  - name: string
  - email: string
  - phone: string
  - cart: Cart
  - addresses: vector<Address>

  + getUserId() -> string
  + getCart() -> Cart
  + addAddress(address: Address)
  + getAddresses() -> vector<Address>
```

### Cart

```
class Cart:
  - items: map<Product, int>  // product -> quantity

  + addProduct(product: Product, quantity: int)
  + removeProduct(product: Product)
  + updateQuantity(product: Product, quantity: int)
  + getItems() -> map<Product, int>
  + getTotalAmount() -> double
  + clear()
```

### Order

| Requirement | State Needed |
|-------------|--------------|
| Order identity | orderId |
| User reference | user |
| Products ordered | items |
| Delivery address | deliveryAddress |
| Order state | status |
| Payment | payment |

```
class Order:
  - orderId: string
  - user: User
  - items: map<Product, int>
  - deliveryAddress: Address
  - status: OrderStatus (PENDING, CONFIRMED, SHIPPED, DELIVERED)
  - payment: Payment
  - warehouse: Warehouse
  - createdAt: timestamp

  + getOrderId() -> string
  + getTotalAmount() -> double
  + getStatus() -> OrderStatus
  + setStatus(status: OrderStatus)
  + getInvoice() -> Invoice
```

### Warehouse

```
class Warehouse:
  - warehouseId: string
  - name: string
  - location: Address
  - inventory: Inventory

  + getInventory() -> Inventory
  + getLocation() -> Address
  + hasProduct(product: Product, quantity: int) -> bool
```

### Inventory

```
class Inventory:
  - stock: map<Product, int>

  + addProduct(product: Product, quantity: int)
  + removeProduct(product: Product, quantity: int) -> bool
  + getQuantity(product: Product) -> int
  + hasStock(product: Product, quantity: int) -> bool
```

### WarehouseSelectionStrategy (Abstract - Strategy Pattern)

```
class WarehouseSelectionStrategy:
  + selectWarehouse(warehouses: vector<Warehouse>, 
                    cart: Cart, 
                    deliveryAddress: Address) -> Warehouse

class NearestWarehouseSelectionStrategy : WarehouseSelectionStrategy
class HighestStockWarehouseSelectionStrategy : WarehouseSelectionStrategy
```

### PaymentMode (Abstract - Strategy Pattern)

```
class PaymentMode:
  + processPayment(amount: double) -> bool
  + refund(amount: double) -> bool
  + validateDetails() -> bool

class UPIPaymentMode : PaymentMode
class CardPaymentMode : PaymentMode
```

### ProductDeliverySystem (Facade)

```
class ProductDeliverySystem:
  - userController: UserController
  - orderController: OrderController
  - warehouseController: WarehouseController
  - warehouseStrategy: WarehouseSelectionStrategy

  + registerUser(name, email, phone, address) -> User
  + addToCart(userId, product, quantity)
  + removeFromCart(userId, product)
  + placeOrder(userId, paymentMode) -> Order
  + getOrderStatus(orderId) -> OrderStatus
```

---

## 4) Implementation (~10 minutes)

### Key Method: ProductDeliverySystem.placeOrder()

```
placeOrder(userId, paymentMode) -> Order
    user = userController.getUser(userId)
    cart = user.getCart()
    
    if cart.isEmpty()
        throw "Cart is empty"
    
    // Select warehouse using strategy
    warehouses = warehouseController.getAllWarehouses()
    deliveryAddress = user.getDefaultAddress()
    
    warehouse = warehouseStrategy.selectWarehouse(
        warehouses, cart, deliveryAddress)
    
    if warehouse == null
        throw "No warehouse can fulfill this order"
    
    // Reserve inventory
    for (product, quantity) in cart.getItems():
        if not warehouse.getInventory().removeProduct(product, quantity)
            // Rollback previous reservations
            throw "Insufficient inventory"
    
    // Process payment
    totalAmount = cart.getTotalAmount()
    payment = new Payment(totalAmount, paymentMode)
    
    if not payment.process()
        // Rollback inventory
        throw "Payment failed"
    
    // Create order
    order = orderController.createOrder(user, cart.getItems(), 
                                        deliveryAddress, warehouse, payment)
    
    // Clear cart
    cart.clear()
    
    // Generate invoice
    order.generateInvoice()
    
    return order
```

### Key Method: NearestWarehouseSelectionStrategy.selectWarehouse()

```
selectWarehouse(warehouses, cart, deliveryAddress) -> Warehouse
    nearestWarehouse = null
    minDistance = MAX_VALUE
    
    for warehouse in warehouses:
        // Check if warehouse has all items in cart
        canFulfill = true
        for (product, quantity) in cart.getItems():
            if not warehouse.hasProduct(product, quantity)
                canFulfill = false
                break
        
        if not canFulfill
            continue
        
        // Calculate distance
        distance = calculateDistance(warehouse.getLocation(), deliveryAddress)
        
        if distance < minDistance
            minDistance = distance
            nearestWarehouse = warehouse
    
    return nearestWarehouse
```

### Key Method: OrderController.createOrder()

```
createOrder(user, items, address, warehouse, payment) -> Order
    orderId = generateOrderId()
    
    order = new Order(
        orderId = orderId,
        user = user,
        items = items,
        deliveryAddress = address,
        warehouse = warehouse,
        payment = payment,
        status = OrderStatus.CONFIRMED,
        createdAt = now()
    )
    
    // Store order
    allOrders.add(order)
    
    return order
```

### Verification: Walk Through a Scenario

```
Setup: 
- User "Alice" with address in Seattle
- Warehouse W1 in Seattle (has Product A x 10, Product B x 5)
- Warehouse W2 in Portland (has Product A x 20)

1. Alice adds Product A (qty 2) to cart
2. Alice adds Product B (qty 1) to cart
3. Cart total = $50

4. placeOrder(alice.id, UPI_PAYMENT)
   - Get cart: {A: 2, B: 1}
   - Select warehouse:
     - W1: has A (2 <= 10 ✓), has B (1 <= 5 ✓), distance = 0 mi
     - W2: has A (2 <= 20 ✓), no B ✗ (skip)
     - Selected: W1
   - Reserve inventory: W1.inventory = {A: 8, B: 4}
   - Process UPI payment: $50 → Success
   - Create order: ORDER-001, status=CONFIRMED
   - Clear cart
   - Generate invoice

5. Result: Order ORDER-001 confirmed, shipping from W1 to Seattle
```

---

## 5) Extensibility (~5 minutes)

### "How would you add a new payment method?"

> "Strategy pattern makes this simple. Create a new class implementing PaymentMode (e.g., WalletPaymentMode). No changes to existing code needed."

```
class WalletPaymentMode : PaymentMode
  - walletId: string
  - balance: double
  
  + processPayment(amount) -> bool
      if balance >= amount
          balance -= amount
          return true
      return false
```

### "How would you handle order splitting across warehouses?"

> "Modify the warehouse selection to return multiple warehouses with partial fulfillment. Create sub-orders for each warehouse. Track parent-child order relationship."

```
struct OrderSplit:
  - warehouse: Warehouse
  - items: map<Product, int>

selectWarehouses(cart, address) -> vector<OrderSplit>
    // Algorithm to distribute items across warehouses
```

### "How would you add inventory reservation with timeout?"

> "Add a reservation system with TTL. Reserve inventory when user starts checkout, release if payment not completed within timeout."

```
class InventoryReservation:
  - product: Product
  - quantity: int
  - expiresAt: timestamp
  - orderId: string

class Inventory:
  - reserved: map<string, InventoryReservation>
  
  + reserve(product, quantity, ttl) -> reservationId
  + confirmReservation(reservationId)
  + cleanupExpiredReservations()
```

### "How would you implement order status notifications?"

> "Use Observer pattern. Order notifies registered observers on status change. NotificationService sends email/SMS updates."

---

## Design Patterns Summary

| Pattern | Where | Why |
|---------|-------|-----|
| **Strategy** | WarehouseSelectionStrategy | Different selection algorithms |
| **Strategy** | PaymentMode | Different payment processing |
| **Facade** | ProductDeliverySystem | Simplified interface |
| **Singleton** | Controllers | Centralized management |
| **Observer** | Notifications (extension) | Status update alerts |
| **Factory** | Order/Invoice creation | Complex object construction |

---

## Order Flow

```
1. User browses products
          ↓
2. Add to cart
          ↓
3. Proceed to checkout
          ↓
4. Select delivery address
          ↓
5. Select warehouse (via strategy)
          ↓
6. Reserve inventory
          ↓
7. Process payment
          ↓
   ├── Success → Confirm order → Generate invoice
   └── Failure → Release inventory → Show error
          ↓
8. Update order status (SHIPPED → DELIVERED)
```
