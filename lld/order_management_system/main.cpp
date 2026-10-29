#include <iostream>
#include <memory>
#include "ProductDeliverySystem.hpp"
#include "NearestWarehouseSelectionStrategy.hpp"

using namespace OrderManagement;

int main() {
    std::cout << "╔═══════════════════════════════════════════════════════════╗\n";
    std::cout << "║         ORDER MANAGEMENT SYSTEM - DEMO                     ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════╝\n\n";

    auto& system = ProductDeliverySystem::getInstance();

    // ============ Setup Warehouses ============
    std::cout << ">>> Setting up warehouses...\n\n";

    auto warehouseAddress1 = std::make_shared<Address>(
        "100 Warehouse Ave", "Seattle", "WA", "98101", "USA", 47.6062, -122.3321);
    auto warehouseAddress2 = std::make_shared<Address>(
        "200 Distribution Blvd", "Portland", "OR", "97201", "USA", 45.5152, -122.6784);

    auto warehouse1 = system.addWarehouse("Seattle Distribution Center", warehouseAddress1);
    auto warehouse2 = system.addWarehouse("Portland Fulfillment Center", warehouseAddress2);

    // ============ Create Products ============
    std::cout << ">>> Creating products...\n\n";

    auto laptop = std::make_shared<Product>("P001", "MacBook Pro 14\"", 
        "Apple MacBook Pro with M3 chip", 1999.99, ProductCategory::ELECTRONICS);
    auto phone = std::make_shared<Product>("P002", "iPhone 15 Pro", 
        "Apple iPhone 15 Pro 256GB", 1199.99, ProductCategory::ELECTRONICS);
    auto headphones = std::make_shared<Product>("P003", "AirPods Pro", 
        "Apple AirPods Pro 2nd Gen", 249.99, ProductCategory::ELECTRONICS);
    auto book = std::make_shared<Product>("P004", "Clean Code", 
        "A Handbook of Agile Software Craftsmanship", 39.99, ProductCategory::BOOKS);

    // Add products to warehouses
    warehouse1->addProduct(laptop, 50);
    warehouse1->addProduct(phone, 100);
    warehouse1->addProduct(headphones, 200);
    warehouse1->addProduct(book, 500);

    warehouse2->addProduct(laptop, 30);
    warehouse2->addProduct(phone, 80);
    warehouse2->addProduct(headphones, 150);

    std::cout << "Products added to warehouses.\n\n";

    // ============ Register Users ============
    std::cout << ">>> Registering users...\n\n";

    auto userAddress1 = std::make_shared<Address>(
        "123 Main St", "Seattle", "WA", "98102", "USA", 47.6205, -122.3493);
    auto userAddress2 = std::make_shared<Address>(
        "456 Oak Ave", "Portland", "OR", "97205", "USA", 45.5231, -122.6765);

    auto user1 = system.registerUser("John Doe", "john.doe@email.com", "555-0101", userAddress1);
    auto user2 = system.registerUser("Jane Smith", "jane.smith@email.com", "555-0102", userAddress2);

    std::cout << "Registered: " << user1->toString() << "\n\n";
    std::cout << "Registered: " << user2->toString() << "\n\n";

    // ============ Shopping - User 1 ============
    std::cout << ">>> User 1 shopping...\n\n";

    system.addToCart(user1->getUserId(), laptop, 1);
    system.addToCart(user1->getUserId(), headphones, 2);
    system.addToCart(user1->getUserId(), book, 1);

    auto cart1 = system.getOrCreateCart(user1->getUserId());
    std::cout << cart1->toString() << "\n\n";

    // ============ Place Order - User 1 ============
    std::cout << ">>> Placing order for User 1...\n\n";

    try {
        auto order1 = system.placeOrder(user1->getUserId());
        std::cout << order1->toString() << "\n";

        // Pay with Card
        std::cout << "\n>>> Processing payment with Credit Card...\n\n";
        bool paymentSuccess = system.payWithCard(
            order1->getOrderId(),
            "4111111111111234",
            "John Doe",
            "12/28",
            "123",
            CardType::CREDIT
        );

        if (paymentSuccess) {
            std::cout << "\nOrder confirmed! Status: " 
                      << orderStatusToString(order1->getStatus()) << "\n\n";

            // Generate Invoice
            std::cout << ">>> Generating invoice...\n\n";
            auto invoice = system.generateInvoice(order1->getOrderId(), 0.08, 9.99, 50.0);
            std::cout << invoice->generateInvoice() << "\n";
        }

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }

    // ============ Shopping - User 2 ============
    std::cout << ">>> User 2 shopping...\n\n";

    system.addToCart(user2->getUserId(), phone, 1);
    system.addToCart(user2->getUserId(), headphones, 1);

    auto cart2 = system.getOrCreateCart(user2->getUserId());
    std::cout << cart2->toString() << "\n\n";

    // ============ Place Order - User 2 ============
    std::cout << ">>> Placing order for User 2...\n\n";

    try {
        auto order2 = system.placeOrder(user2->getUserId());
        std::cout << order2->toString() << "\n";

        // Pay with UPI
        std::cout << "\n>>> Processing payment with UPI...\n\n";
        bool paymentSuccess = system.payWithUPI(
            order2->getOrderId(),
            "jane@upibank",
            "1234"
        );

        if (paymentSuccess) {
            std::cout << "\nOrder confirmed! Status: " 
                      << orderStatusToString(order2->getStatus()) << "\n\n";

            // Update order status to simulate delivery process
            system.updateOrderStatus(order2->getOrderId(), OrderStatus::SHIPPED);
            std::cout << "Order shipped! New status: " 
                      << orderStatusToString(order2->getStatus()) << "\n\n";
        }

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }

    // ============ Test Order Cancellation ============
    std::cout << ">>> Testing order cancellation...\n\n";

    // User 1 creates another order
    system.addToCart(user1->getUserId(), book, 3);
    auto order3 = system.placeOrder(user1->getUserId());
    std::cout << "New order created: " << order3->getOrderId() << "\n";
    std::cout << "Status: " << orderStatusToString(order3->getStatus()) << "\n";

    if (system.cancelOrder(order3->getOrderId())) {
        std::cout << "Order cancelled successfully!\n";
        std::cout << "New status: " << orderStatusToString(order3->getStatus()) << "\n\n";
    }

    // ============ Change Strategy ============
    std::cout << ">>> Changing warehouse selection strategy...\n\n";

    system.setWarehouseSelectionStrategy(
        std::make_unique<HighestStockWarehouseSelectionStrategy>());
    
    system.printSystemStatus();

    // ============ View User Order History ============
    std::cout << "\n>>> User 1's Order History:\n\n";
    auto user1Orders = system.getUserOrders(user1->getUserId());
    for (const auto& order : user1Orders) {
        std::cout << "Order: " << order->getOrderId() 
                  << " | Status: " << orderStatusToString(order->getStatus())
                  << " | Amount: $" << order->getTotalAmount() << "\n";
    }

    std::cout << "\n╔═══════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    DEMO COMPLETED                          ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════╝\n";

    return 0;
}
