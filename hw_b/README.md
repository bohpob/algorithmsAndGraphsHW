Class template `Bestsellers`, parameterized by the type Product, which serves as an identifier for individual products. 
The `Product` type guarantees the following functionalities: copy constructor (and assignment operator), destructor, 
comparison operators, and a defined std::hash function. 

The `Bestsellers` class must implement the following public methods:

`size_t products() const`: Returns the number of recorded product types (not the total number of units sold).

`void sell(const Product& p, size_t amount)`: Records the sale of amount units of product p.

`size_t rank(const Product& p) const`: Returns the rank of the specified product in terms of total units sold, 
numbered from 1. (If multiple products have the same sales count, you may rank them in any order.)

`const Product& product(size_t rank) const`: The inverse function of rank, ensuring that product(rank(p)) == p 
and rank(product(r)) == r hold (unless an exception occurs).
