#pragma once

#using <System.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>

#include "Shop.h"
#include "EditForm.h"
#include <msclr/marshal_cppstd.h>
#include <exception>

public ref class MainForm : public System::Windows::Forms::Form
{
public:
    MainForm()
    {
        InitializeLogic(nullptr, true); // по умолчанию админ
    }

    MainForm(System::String^ userName, bool isAdminFlag)
    {
        InitializeLogic(userName, isAdminFlag);
    }

protected:
    ~MainForm()
    {
        if (shop) delete shop;
    }

private:
    Shop* shop;
    bool isAdmin;

    System::Windows::Forms::DataGridView^ dgv;
    System::Windows::Forms::Button^ btnAdd;
    System::Windows::Forms::Button^ btnEdit;
    System::Windows::Forms::Button^ btnDelete;
    System::Windows::Forms::Button^ btnLoad;
    System::Windows::Forms::Button^ btnSave;
    System::Windows::Forms::Button^ btnSortName;
    System::Windows::Forms::Button^ btnSortPrice;
    System::Windows::Forms::Button^ btnCheapest;
    System::Windows::Forms::Label^ lblCheapest;

    void InitializeLogic(System::String^ userName, bool isAdminFlag)
    {
        shop = new Shop();
        isAdmin = isAdminFlag;

        InitializeComponent();

        if (!System::String::IsNullOrEmpty(userName))
        {
            this->Text = this->Text + " – " + userName;
        }

        RefreshGrid();
        UpdateAccessRights();
    }

    void InitializeComponent()
    {
        this->Text = "Интернет-магазин";
        this->ClientSize = System::Drawing::Size(900, 600);
        this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;

        // Нежный фон формы
        this->BackColor = System::Drawing::Color::FromArgb(245, 248, 255);

        // Размеры для таблицы
        int dgvX = 10;
        int dgvY = 10;
        int dgvW = 650;
        int dgvH = 580;

        // Координаты панели кнопок справа
        int panelX = dgvX + dgvW + 10; // примерно 670
        int panelY = 10;
        int w = 200;
        int h = 30;
        int gap = 8;

        // Цвета для кнопок
        System::Drawing::Color buttonBack = System::Drawing::Color::FromArgb(52, 152, 219);
        System::Drawing::Color buttonBackSecondary = System::Drawing::Color::FromArgb(46, 204, 113);
        System::Drawing::Color buttonText = System::Drawing::Color::White;

        // DataGridView слева
        dgv = gcnew System::Windows::Forms::DataGridView();
        dgv->Location = System::Drawing::Point(dgvX, dgvY);
        dgv->Size = System::Drawing::Size(dgvW, dgvH);
        dgv->AllowUserToAddRows = false;
        dgv->AllowUserToDeleteRows = false;
        dgv->ReadOnly = true;
        dgv->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
        dgv->MultiSelect = false;
        dgv->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;

        dgv->BackgroundColor = System::Drawing::Color::White;
        dgv->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
        dgv->GridColor = System::Drawing::Color::FromArgb(220, 230, 245);
        dgv->EnableHeadersVisualStyles = false;

        // Стиль заголовков
        System::Windows::Forms::DataGridViewCellStyle^ headerStyle =
            gcnew System::Windows::Forms::DataGridViewCellStyle();
        headerStyle->BackColor = System::Drawing::Color::FromArgb(52, 73, 94);
        headerStyle->ForeColor = System::Drawing::Color::White;
        headerStyle->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
        headerStyle->Font = gcnew System::Drawing::Font("Segoe UI", 9,
            System::Drawing::FontStyle::Bold);
        dgv->ColumnHeadersDefaultCellStyle = headerStyle;
        dgv->ColumnHeadersHeight = 28;

        // Чередующиеся строки
        System::Windows::Forms::DataGridViewCellStyle^ altStyle =
            gcnew System::Windows::Forms::DataGridViewCellStyle();
        altStyle->BackColor = System::Drawing::Color::FromArgb(239, 244, 255);
        dgv->AlternatingRowsDefaultCellStyle = altStyle;

        dgv->Columns->Add("colName", "Название товара");
        dgv->Columns->Add("colBasePrice", "Базовая цена");
        dgv->Columns->Add("colDiscount", "Скидка, %");
        dgv->Columns->Add("colFinalPrice", "Итоговая цена");

        this->Controls->Add(dgv);

        // Кнопка «Добавить»
        btnAdd = gcnew System::Windows::Forms::Button();
        btnAdd->Text = "Добавить";
        btnAdd->Location = System::Drawing::Point(panelX, panelY);
        btnAdd->Size = System::Drawing::Size(w, h);
        btnAdd->BackColor = buttonBack;
        btnAdd->ForeColor = buttonText;
        btnAdd->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
        btnAdd->FlatAppearance->BorderSize = 0;
        btnAdd->Click += gcnew System::EventHandler(this, &MainForm::OnAddClick);
        this->Controls->Add(btnAdd);

        // Кнопка «Изменить»
        btnEdit = gcnew System::Windows::Forms::Button();
        btnEdit->Text = "Изменить";
        btnEdit->Location = System::Drawing::Point(panelX, panelY + (h + gap));
        btnEdit->Size = System::Drawing::Size(w, h);
        btnEdit->BackColor = buttonBack;
        btnEdit->ForeColor = buttonText;
        btnEdit->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
        btnEdit->FlatAppearance->BorderSize = 0;
        btnEdit->Click += gcnew System::EventHandler(this, &MainForm::OnEditClick);
        this->Controls->Add(btnEdit);

        // Кнопка «Удалить»
        btnDelete = gcnew System::Windows::Forms::Button();
        btnDelete->Text = "Удалить";
        btnDelete->Location = System::Drawing::Point(panelX, panelY + 2 * (h + gap));
        btnDelete->Size = System::Drawing::Size(w, h);
        btnDelete->BackColor = System::Drawing::Color::FromArgb(231, 76, 60);
        btnDelete->ForeColor = buttonText;
        btnDelete->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
        btnDelete->FlatAppearance->BorderSize = 0;
        btnDelete->Click += gcnew System::EventHandler(this, &MainForm::OnDeleteClick);
        this->Controls->Add(btnDelete);

        // Кнопка «Загрузить»
        btnLoad = gcnew System::Windows::Forms::Button();
        btnLoad->Text = "Загрузить";
        btnLoad->Location = System::Drawing::Point(panelX, panelY + 3 * (h + gap));
        btnLoad->Size = System::Drawing::Size(w, h);
        btnLoad->BackColor = buttonBackSecondary;
        btnLoad->ForeColor = buttonText;
        btnLoad->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
        btnLoad->FlatAppearance->BorderSize = 0;
        btnLoad->Click += gcnew System::EventHandler(this, &MainForm::OnLoadClick);
        this->Controls->Add(btnLoad);

        // Кнопка «Сохранить»
        btnSave = gcnew System::Windows::Forms::Button();
        btnSave->Text = "Сохранить";
        btnSave->Location = System::Drawing::Point(panelX, panelY + 4 * (h + gap));
        btnSave->Size = System::Drawing::Size(w, h);
        btnSave->BackColor = buttonBackSecondary;
        btnSave->ForeColor = buttonText;
        btnSave->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
        btnSave->FlatAppearance->BorderSize = 0;
        btnSave->Click += gcnew System::EventHandler(this, &MainForm::OnSaveClick);
        this->Controls->Add(btnSave);

        // Кнопка «Сорт. по названию»
        btnSortName = gcnew System::Windows::Forms::Button();
        btnSortName->Text = "Сорт. по названию";
        btnSortName->Location = System::Drawing::Point(panelX, panelY + 5 * (h + gap));
        btnSortName->Size = System::Drawing::Size(w, h);
        btnSortName->BackColor = buttonBack;
        btnSortName->ForeColor = buttonText;
        btnSortName->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
        btnSortName->FlatAppearance->BorderSize = 0;
        btnSortName->Click += gcnew System::EventHandler(this, &MainForm::OnSortNameClick);
        this->Controls->Add(btnSortName);

        // Кнопка «Сорт. по цене»
        btnSortPrice = gcnew System::Windows::Forms::Button();
        btnSortPrice->Text = "Сорт. по цене";
        btnSortPrice->Location = System::Drawing::Point(panelX, panelY + 6 * (h + gap));
        btnSortPrice->Size = System::Drawing::Size(w, h);
        btnSortPrice->BackColor = buttonBack;
        btnSortPrice->ForeColor = buttonText;
        btnSortPrice->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
        btnSortPrice->FlatAppearance->BorderSize = 0;
        btnSortPrice->Click += gcnew System::EventHandler(this, &MainForm::OnSortPriceClick);
        this->Controls->Add(btnSortPrice);

        // Кнопка «Мин. цена»
        btnCheapest = gcnew System::Windows::Forms::Button();
        btnCheapest->Text = "Мин. цена";
        btnCheapest->Location = System::Drawing::Point(panelX, panelY + 7 * (h + gap));
        btnCheapest->Size = System::Drawing::Size(w, h);
        btnCheapest->BackColor = System::Drawing::Color::FromArgb(155, 89, 182);
        btnCheapest->ForeColor = buttonText;
        btnCheapest->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
        btnCheapest->FlatAppearance->BorderSize = 0;
        btnCheapest->Click += gcnew System::EventHandler(this, &MainForm::OnCheapestClick);
        this->Controls->Add(btnCheapest);

        // Label для вывода минимальной цены – под кнопками
        lblCheapest = gcnew System::Windows::Forms::Label();
        lblCheapest->Text = "Минимальная цена: –";
        lblCheapest->Location = System::Drawing::Point(panelX, panelY + 7 * (h + gap) + h + 10);
        lblCheapest->AutoSize = true;
        lblCheapest->ForeColor = System::Drawing::Color::FromArgb(44, 62, 80);
        lblCheapest->Font = gcnew System::Drawing::Font("Segoe UI", 9,
            System::Drawing::FontStyle::Bold);
        this->Controls->Add(lblCheapest);
    }

    void UpdateAccessRights()
    {
        if (!isAdmin)
        {
            btnAdd->Enabled = false;
            btnEdit->Enabled = false;
            btnDelete->Enabled = false;
            btnSave->Enabled = false;

            btnLoad->Enabled = true;
            btnSortName->Enabled = true;
            btnSortPrice->Enabled = true;
            btnCheapest->Enabled = true;
        }
        else
        {
            btnAdd->Enabled = true;
            btnEdit->Enabled = true;
            btnDelete->Enabled = true;
            btnSave->Enabled = true;
        }
    }

    void RefreshGrid()
    {
        dgv->Rows->Clear();
        lblCheapest->Text = "Минимальная цена: –";

        const auto& list = shop->getProducts();

        for (std::size_t i = 0; i < list.size(); ++i)
        {
            auto p = list[i];
            System::String^ name = gcnew System::String(p->getName().c_str());
            double basePrice = p->getBasePrice();
            double discount = p->getDiscountPercent();
            double finalPrice = p->getFinalPrice();

            int rowIndex = dgv->Rows->Add();
            dgv->Rows[rowIndex]->Cells[0]->Value = name;
            dgv->Rows[rowIndex]->Cells[1]->Value = basePrice;
            dgv->Rows[rowIndex]->Cells[2]->Value = discount;
            dgv->Rows[rowIndex]->Cells[3]->Value = finalPrice;
        }
    }

    void ShowException(const std::exception& ex)
    {
        System::String^ msg = gcnew System::String(ex.what());
        System::Windows::Forms::MessageBox::Show(
            msg,
            "Ошибка",
            System::Windows::Forms::MessageBoxButtons::OK,
            System::Windows::Forms::MessageBoxIcon::Error);
    }

    // === Обработчики кнопок ===

    void OnAddClick(System::Object^ sender, System::EventArgs^ e)
    {
        EditForm^ form = gcnew EditForm();
        if (form->ShowDialog(this) == System::Windows::Forms::DialogResult::OK)
        {
            try {
                std::string name = msclr::interop::marshal_as<std::string>(form->ProductName);
                double price = form->BasePrice;
                double discount = form->DiscountPercent;

                if (discount <= 0.0)
                    shop->addProduct(name, price);
                else
                    shop->addDiscountedProduct(name, price, discount);

                RefreshGrid();
            }
            catch (const std::exception& ex) {
                ShowException(ex);
            }
        }
    }

    void OnEditClick(System::Object^ sender, System::EventArgs^ e)
    {
        if (dgv->SelectedRows->Count == 0)
        {
            System::Windows::Forms::MessageBox::Show(
                "Выберите строку в таблице",
                "Внимание",
                System::Windows::Forms::MessageBoxButtons::OK,
                System::Windows::Forms::MessageBoxIcon::Information);
            return;
        }

        int rowIndex = dgv->SelectedRows[0]->Index;
        const auto& list = shop->getProducts();
        if (rowIndex < 0 || rowIndex >= static_cast<int>(list.size()))
            return;

        auto p = list[rowIndex];

        EditForm^ form = gcnew EditForm(
            "Изменение товара",
            gcnew System::String(p->getName().c_str()),
            p->getBasePrice(),
            p->getDiscountPercent());

        if (form->ShowDialog(this) == System::Windows::Forms::DialogResult::OK)
        {
            try {
                std::string name = msclr::interop::marshal_as<std::string>(form->ProductName);
                double price = form->BasePrice;
                double discount = form->DiscountPercent;

                shop->updateProduct(static_cast<std::size_t>(rowIndex),
                    name, price, discount);

                RefreshGrid();
            }
            catch (const std::exception& ex) {
                ShowException(ex);
            }
        }
    }

    void OnDeleteClick(System::Object^ sender, System::EventArgs^ e)
    {
        if (dgv->SelectedRows->Count == 0)
        {
            System::Windows::Forms::MessageBox::Show(
                "Выберите строку для удаления",
                "Внимание",
                System::Windows::Forms::MessageBoxButtons::OK,
                System::Windows::Forms::MessageBoxIcon::Information);
            return;
        }

        int rowIndex = dgv->SelectedRows[0]->Index;

        auto result = System::Windows::Forms::MessageBox::Show(
            "Удалить выбранный товар?",
            "Подтверждение",
            System::Windows::Forms::MessageBoxButtons::YesNo,
            System::Windows::Forms::MessageBoxIcon::Question);

        if (result != System::Windows::Forms::DialogResult::Yes)
            return;

        try {
            shop->removeProduct(static_cast<std::size_t>(rowIndex));
            RefreshGrid();
        }
        catch (const std::exception& ex) {
            ShowException(ex);
        }
    }

    void OnLoadClick(System::Object^ sender, System::EventArgs^ e)
    {
        System::Windows::Forms::OpenFileDialog^ dlg =
            gcnew System::Windows::Forms::OpenFileDialog();
        dlg->Filter = "Текстовые файлы (*.txt)|*.txt|Все файлы (*.*)|*.*";
        dlg->Title = "Открыть файл с товарами";

        if (dlg->ShowDialog(this) == System::Windows::Forms::DialogResult::OK)
        {
            try {
                std::string filename = msclr::interop::marshal_as<std::string>(dlg->FileName);
                shop->loadFromFile(filename);
                RefreshGrid();
            }
            catch (const std::exception& ex) {
                ShowException(ex);
            }
        }
    }

    void OnSaveClick(System::Object^ sender, System::EventArgs^ e)
    {
        System::Windows::Forms::SaveFileDialog^ dlg =
            gcnew System::Windows::Forms::SaveFileDialog();
        dlg->Filter = "Текстовые файлы (*.txt)|*.txt|Все файлы (*.*)|*.*";
        dlg->Title = "Сохранить товары в файл";

        if (dlg->ShowDialog(this) == System::Windows::Forms::DialogResult::OK)
        {
            try {
                std::string filename = msclr::interop::marshal_as<std::string>(dlg->FileName);
                shop->saveToFile(filename);
            }
            catch (const std::exception& ex) {
                ShowException(ex);
            }
        }
    }

    void OnSortNameClick(System::Object^ sender, System::EventArgs^ e)
    {
        try {
            shop->sortByName(true);
            RefreshGrid();
        }
        catch (const std::exception& ex) {
            ShowException(ex);
        }
    }

    void OnSortPriceClick(System::Object^ sender, System::EventArgs^ e)
    {
        try {
            shop->sortByFinalPrice(true);
            RefreshGrid();
        }
        catch (const std::exception& ex) {
            ShowException(ex);
        }
    }

    void OnCheapestClick(System::Object^ sender, System::EventArgs^ e)
    {
        try {
            const Product& p = shop->findCheapestProduct();
            System::String^ name = gcnew System::String(p.getName().c_str());
            double finalPrice = p.getFinalPrice();

            lblCheapest->Text = "Минимальная цена: " + name +
                " – " + finalPrice.ToString("F2");
        }
        catch (const std::exception& ex) {
            ShowException(ex);
        }
    }
};
