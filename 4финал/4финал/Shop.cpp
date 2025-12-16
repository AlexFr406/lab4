#include "Shop.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cmath>

// ======================== Вспомогательная функция trim ========================

namespace {
    std::string trim(const std::string& s) {
        std::size_t start = s.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) return std::string();
        std::size_t end = s.find_last_not_of(" \t\r\n");
        return s.substr(start, end - start + 1);
    }

    const double MAX_PRICE = 10'000'000.0;
}

// ======================== NoDiscountStrategy ========================

double NoDiscountStrategy::apply(double basePrice) const {
    return basePrice;
}

double NoDiscountStrategy::getPercent() const {
    return 0.0;
}

// ======================== PercentDiscountStrategy ========================

PercentDiscountStrategy::PercentDiscountStrategy(double percent)
    : percent(percent)
{
    if (!std::isfinite(percent) || percent < 0.0 || percent > 100.0) {
        throw std::invalid_argument("Скидка должна быть в диапазоне 0–100 процентов");
    }
}

double PercentDiscountStrategy::apply(double basePrice) const {
    return basePrice * (1.0 - percent / 100.0);
}

double PercentDiscountStrategy::getPercent() const {
    return percent;
}

// ======================== Product ========================

Product::Product(const std::string& name,
    double basePrice,
    std::shared_ptr<IDiscountStrategy> discount)
    : name(name),
    basePrice(basePrice),
    discount(std::move(discount))
{
    if (this->name.empty()) {
        throw std::invalid_argument("Название товара не может быть пустым");
    }

    if (!std::isfinite(this->basePrice) ||
        this->basePrice <= 0.0 ||
        this->basePrice > MAX_PRICE)
    {
        throw std::invalid_argument("Цена должна быть в диапазоне 1–10 000 000");
    }

    if (!this->discount) {
        throw std::invalid_argument("Стратегия скидки не задана");
    }
}

const std::string& Product::getName() const {
    return name;
}

double Product::getBasePrice() const {
    return basePrice;
}

double Product::getFinalPrice() const {
    return discount->apply(basePrice);
}

double Product::getDiscountPercent() const {
    return discount->getPercent();
}

// ======================== Shop ========================

Shop::Shop() = default;

bool Shop::existsProduct(const std::string& name) const {
    for (const auto& p : products) {
        if (p->getName() == name) return true;
    }
    return false;
}

void Shop::clear() {
    products.clear();
}

void Shop::addProduct(const std::string& name, double price) {
    if (existsProduct(name)) {
        throw std::runtime_error("Товар '" + name + "' уже существует");
    }
    auto strategy = std::make_shared<NoDiscountStrategy>();
    products.push_back(std::make_shared<Product>(name, price, strategy));
}

void Shop::addDiscountedProduct(const std::string& name,
    double price,
    double percent)
{
    if (existsProduct(name)) {
        throw std::runtime_error("Товар '" + name + "' уже существует");
    }
    auto strategy = std::make_shared<PercentDiscountStrategy>(percent);
    products.push_back(std::make_shared<Product>(name, price, strategy));
}

void Shop::updateProduct(std::size_t index,
    const std::string& name,
    double price,
    double percent)
{
    if (index >= products.size()) {
        throw std::runtime_error("Индекс вне диапазона");
    }

    for (std::size_t i = 0; i < products.size(); ++i) {
        if (i != index && products[i]->getName() == name) {
            throw std::runtime_error("Товар с именем '" + name + "' уже существует");
        }
    }

    std::shared_ptr<IDiscountStrategy> strategy;
    if (percent <= 0.0) {
        strategy = std::make_shared<NoDiscountStrategy>();
    }
    else {
        strategy = std::make_shared<PercentDiscountStrategy>(percent);
    }

    products[index] = std::make_shared<Product>(name, price, strategy);
}

void Shop::removeProduct(std::size_t index) {
    if (index >= products.size()) {
        throw std::runtime_error("Индекс вне диапазона");
    }
    products.erase(products.begin() + index);
}

const std::vector<std::shared_ptr<Product>>& Shop::getProducts() const {
    return products;
}

double Shop::calculateAverageFinalPrice() const {
    if (products.empty()) {
        throw std::runtime_error("Список товаров пуст – нечего считать");
    }
    double sum = 0.0;
    for (const auto& p : products) {
        sum += p->getFinalPrice();
    }
    return sum / static_cast<double>(products.size());
}

const Product& Shop::findCheapestProduct() const {
    if (products.empty()) {
        throw std::runtime_error("Список товаров пуст – нечего искать");
    }

    std::size_t minIndex = 0;
    double minPrice = products[0]->getFinalPrice();

    for (std::size_t i = 1; i < products.size(); ++i) {
        double cur = products[i]->getFinalPrice();
        if (cur < minPrice) {
            minPrice = cur;
            minIndex = i;
        }
    }
    return *products[minIndex];
}

void Shop::saveToFile(const std::string& filename) const {
    std::ofstream out(filename.c_str());
    if (!out) {
        throw std::runtime_error("Не удалось открыть файл для записи: " + filename);
    }

    for (const auto& p : products) {
        out << p->getName() << ';'
            << p->getBasePrice() << ';'
            << p->getDiscountPercent() << '\n';
    }
}

void Shop::loadFromFile(const std::string& filename) {
    std::ifstream in(filename.c_str());
    if (!in) {
        throw std::runtime_error("Не удалось открыть файл для чтения: " + filename);
    }

    products.clear();

    std::string line;
    std::size_t lineNo = 0;

    while (std::getline(in, line)) {
        ++lineNo;
        line = trim(line);
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string name, priceStr, discountStr;

        if (!std::getline(iss, name, ';') ||
            !std::getline(iss, priceStr, ';') ||
            !std::getline(iss, discountStr))
        {
            throw std::runtime_error("Неверный формат в строке " +
                std::to_string(lineNo));
        }

        name = trim(name);
        priceStr = trim(priceStr);
        discountStr = trim(discountStr);

        double price = std::stod(priceStr);
        double percent = std::stod(discountStr);

        if (percent <= 0.0) {
            addProduct(name, price);
        }
        else {
            addDiscountedProduct(name, price, percent);
        }
    }
}

void Shop::sortByName(bool ascending) {
    std::sort(products.begin(), products.end(),
        [ascending](const std::shared_ptr<Product>& a,
            const std::shared_ptr<Product>& b)
        {
            return ascending
                ? a->getName() < b->getName()
                : a->getName() > b->getName();
        });
}

void Shop::sortByFinalPrice(bool ascending) {
    std::sort(products.begin(), products.end(),
        [ascending](const std::shared_ptr<Product>& a,
            const std::shared_ptr<Product>& b)
        {
            return ascending
                ? a->getFinalPrice() < b->getFinalPrice()
                : a->getFinalPrice() > b->getFinalPrice();
        });
}
