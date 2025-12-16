#pragma once

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

// ======================== Стратегии скидки ========================

class IDiscountStrategy {
public:
    virtual ~IDiscountStrategy() = default;

    // Расчёт итоговой цены по базовой
    virtual double apply(double basePrice) const = 0;

    // Процент скидки (для отображения и сохранения)
    virtual double getPercent() const = 0;
};

class NoDiscountStrategy : public IDiscountStrategy {
public:
    double apply(double basePrice) const override;
    double getPercent() const override;
};

class PercentDiscountStrategy : public IDiscountStrategy {
private:
    double percent; // 0–100
public:
    explicit PercentDiscountStrategy(double percent);

    double apply(double basePrice) const override;
    double getPercent() const override;
};

// ======================== Класс товара ========================

class Product {
private:
    std::string name;
    double basePrice;
    std::shared_ptr<IDiscountStrategy> discount;

public:
    Product(const std::string& name,
        double basePrice,
        std::shared_ptr<IDiscountStrategy> discount);

    const std::string& getName() const;
    double getBasePrice() const;
    double getFinalPrice() const;
    double getDiscountPercent() const;
};

// ======================== Класс магазина ========================

class Shop {
private:
    std::vector<std::shared_ptr<Product>> products;

    bool existsProduct(const std::string& name) const;

public:
    Shop();

    void clear();

    // Добавить товар без скидки
    void addProduct(const std::string& name, double price);

    // Добавить товар со скидкой
    void addDiscountedProduct(const std::string& name,
        double price,
        double percent);

    // Изменить товар по индексу
    void updateProduct(std::size_t index,
        const std::string& name,
        double price,
        double percent);

    // Удалить товар по индексу
    void removeProduct(std::size_t index);

    const std::vector<std::shared_ptr<Product>>& getProducts() const;

    // Средняя итоговая цена
    double calculateAverageFinalPrice() const;

    // Товар с минимальной итоговой ценой
    const Product& findCheapestProduct() const;

    // Работа с файлами: name;price;discountPercent
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);

    // Сортировки
    void sortByName(bool ascending);
    void sortByFinalPrice(bool ascending);
};
