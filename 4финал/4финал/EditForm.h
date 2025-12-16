#pragma once

#using <System.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>

public ref class EditForm : public System::Windows::Forms::Form
{
public:
    // Добавление нового товара
    EditForm()
    {
        InitializeComponent();
        this->Text = "Добавление товара";
    }

    // Изменение существующего товара
    EditForm(System::String^ title,
        System::String^ name,
        double basePrice,
        double discountPercent)
    {
        InitializeComponent();
        this->Text = title;

        textName->Text = name;
        textPrice->Text = basePrice.ToString("F2");
        textDiscount->Text = discountPercent.ToString("F2");
    }

    property System::String^ ProductName;
    property double BasePrice;
    property double DiscountPercent;

protected:
    ~EditForm()
    {
        if (components != nullptr)
            delete components;
    }

private:
    System::Windows::Forms::Label^ labelName;
    System::Windows::Forms::Label^ labelPrice;
    System::Windows::Forms::Label^ labelDiscount;
    System::Windows::Forms::TextBox^ textName;
    System::Windows::Forms::TextBox^ textPrice;
    System::Windows::Forms::TextBox^ textDiscount;
    System::Windows::Forms::Button^ btnOK;
    System::Windows::Forms::Button^ btnCancel;
    System::Windows::Forms::ErrorProvider^ errorProvider;
    System::ComponentModel::IContainer^ components;

    void InitializeComponent()
    {
        components = gcnew System::ComponentModel::Container();

        this->ClientSize = System::Drawing::Size(380, 200);
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
        this->MaximizeBox = false;
        this->MinimizeBox = false;
        this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
        this->Text = "Товар";

        labelName = gcnew System::Windows::Forms::Label();
        labelName->Text = "Название товара:";
        labelName->Location = System::Drawing::Point(10, 20);
        labelName->AutoSize = true;
        this->Controls->Add(labelName);

        textName = gcnew System::Windows::Forms::TextBox();
        textName->Location = System::Drawing::Point(150, 17);
        textName->Width = 200;
        this->Controls->Add(textName);

        labelPrice = gcnew System::Windows::Forms::Label();
        labelPrice->Text = "Базовая цена:";
        labelPrice->Location = System::Drawing::Point(10, 55);
        labelPrice->AutoSize = true;
        this->Controls->Add(labelPrice);

        textPrice = gcnew System::Windows::Forms::TextBox();
        textPrice->Location = System::Drawing::Point(150, 52);
        textPrice->Width = 200;
        this->Controls->Add(textPrice);

        labelDiscount = gcnew System::Windows::Forms::Label();
        labelDiscount->Text = "Скидка, %:";
        labelDiscount->Location = System::Drawing::Point(10, 90);
        labelDiscount->AutoSize = true;
        this->Controls->Add(labelDiscount);

        textDiscount = gcnew System::Windows::Forms::TextBox();
        textDiscount->Location = System::Drawing::Point(150, 87);
        textDiscount->Width = 200;
        this->Controls->Add(textDiscount);

        btnOK = gcnew System::Windows::Forms::Button();
        btnOK->Text = "OK";
        btnOK->Location = System::Drawing::Point(100, 130);
        btnOK->Click += gcnew System::EventHandler(this, &EditForm::OnOkClick);
        this->Controls->Add(btnOK);

        btnCancel = gcnew System::Windows::Forms::Button();
        btnCancel->Text = "Отмена";
        btnCancel->Location = System::Drawing::Point(200, 130);
        btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
        this->Controls->Add(btnCancel);

        this->AcceptButton = btnOK;
        this->CancelButton = btnCancel;

        errorProvider = gcnew System::Windows::Forms::ErrorProvider(components);
        errorProvider->ContainerControl = this;
    }

    bool ValidateAll()
    {
        errorProvider->Clear();
        bool ok = true;

        System::String^ name = textName->Text->Trim();
        if (System::String::IsNullOrEmpty(name))
        {
            errorProvider->SetError(textName, "Название не может быть пустым");
            ok = false;
        }
        else if (name->Length > 50)
        {
            errorProvider->SetError(textName, "Слишком длинное название (макс. 50 символов)");
            ok = false;
        }

        double price;
        if (!System::Double::TryParse(textPrice->Text->Trim(), price))
        {
            errorProvider->SetError(textPrice, "Введите корректную цену");
            ok = false;
        }
        else if (price <= 0.0 || price > 10000000.0)
        {
            errorProvider->SetError(textPrice, "Цена должна быть в диапазоне 1–10 000 000");
            ok = false;
        }

        double discount = 0.0;
        System::String^ discText = textDiscount->Text->Trim();
        if (discText->Length > 0)
        {
            if (!System::Double::TryParse(discText, discount))
            {
                errorProvider->SetError(textDiscount, "Введите корректную скидку");
                ok = false;
            }
            else if (discount < 0.0 || discount > 100.0)
            {
                errorProvider->SetError(textDiscount, "Скидка должна быть от 0 до 100");
                ok = false;
            }
        }

        if (!ok) return false;

        ProductName = name;
        BasePrice = price;
        DiscountPercent = discount;

        return true;
    }

    void OnOkClick(System::Object^ sender, System::EventArgs^ e)
    {
        if (!ValidateAll()) return;

        this->DialogResult = System::Windows::Forms::DialogResult::OK;
        this->Close();
    }
};
