#pragma once

#using <System.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>

public ref class LoginForm : public System::Windows::Forms::Form
{
public:
    // Обычный режим «Вход»
    LoginForm()
    {
        InitializeComponent();
        this->Text = "Вход в систему";
        labelTitle->Text = "Привет";
        labelSubtitle->Text = "Интернет–магазин";
    }

    // Перегрузка конструктора – можно задать свой заголовок
    LoginForm(System::String^ customTitle)
    {
        InitializeComponent();
        this->Text = customTitle;
        labelTitle->Text = customTitle;
        labelSubtitle->Text = "Интернет–магазин";
    }

    // Данные, которые заберёт Program.cpp
    property System::String^ UserName;
    property bool IsAdmin;

protected:
    ~LoginForm()
    {
        if (components != nullptr)
            delete components;
    }

private:
    // Левая цветная панель
    System::Windows::Forms::Panel^ panelLeft;
    System::Windows::Forms::Label^ labelTitle;
    System::Windows::Forms::Label^ labelSubtitle;

    // Правая часть – поля ввода
    System::Windows::Forms::Label^ labelUser;
    System::Windows::Forms::Label^ labelPass;
    System::Windows::Forms::TextBox^ textUser;
    System::Windows::Forms::TextBox^ textPass;
    System::Windows::Forms::CheckBox^ checkAdmin;
    System::Windows::Forms::Button^ btnOK;
    System::Windows::Forms::Button^ btnCancel;
    System::Windows::Forms::ErrorProvider^ errorProvider;
    System::ComponentModel::IContainer^ components;

    void InitializeComponent()
    {
        components = gcnew System::ComponentModel::Container();

        this->Width = 520;
        this->Height = 320;
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
        this->MaximizeBox = false;
        this->MinimizeBox = false;
        this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
        this->BackColor = System::Drawing::Color::FromArgb(245, 248, 255);

        // Левая панель
        panelLeft = gcnew System::Windows::Forms::Panel();
        panelLeft->Dock = System::Windows::Forms::DockStyle::Left;
        panelLeft->Width = 180;
        panelLeft->BackColor = System::Drawing::Color::FromArgb(52, 152, 219);
        this->Controls->Add(panelLeft);

        // Заголовок на левой панели
        labelTitle = gcnew System::Windows::Forms::Label();
        labelTitle->AutoSize = true;
        labelTitle->ForeColor = System::Drawing::Color::White;
        labelTitle->Font = gcnew System::Drawing::Font("Segoe UI", 14,
            System::Drawing::FontStyle::Bold);
        labelTitle->Location = System::Drawing::Point(20, 40);
        labelTitle->Text = "Добро пожаловать";
        panelLeft->Controls->Add(labelTitle);

        // Подзаголовок
        labelSubtitle = gcnew System::Windows::Forms::Label();
        labelSubtitle->AutoSize = true;
        labelSubtitle->ForeColor = System::Drawing::Color::FromArgb(236, 240, 241);
        labelSubtitle->Font = gcnew System::Drawing::Font("Segoe UI", 10,
            System::Drawing::FontStyle::Regular);
        labelSubtitle->Location = System::Drawing::Point(20, 80);
        labelSubtitle->Text = "Интернет–магазин";
        panelLeft->Controls->Add(labelSubtitle);

        // Небольшое пояснение
        System::Windows::Forms::Label^ labelHint =
            gcnew System::Windows::Forms::Label();
        labelHint->AutoSize = true;
        labelHint->ForeColor = System::Drawing::Color::FromArgb(189, 195, 199);
        labelHint->Font = gcnew System::Drawing::Font("Segoe UI", 8,
            System::Drawing::FontStyle::Regular);
        labelHint->Location = System::Drawing::Point(20, 130);
        labelHint->Text = "Войдите как обычный\n" +
            "пользователь или\n" +
            "администратор отдела";
        panelLeft->Controls->Add(labelHint);

        // Координаты правой части
        int baseX = 200;
        int baseY = 40;
        int labelW = 140;
        int textW = 260;
        int rowH = 28;
        int gap = 10;

        // «Имя пользователя»
        labelUser = gcnew System::Windows::Forms::Label();
        labelUser->Text = "Имя пользователя:";
        labelUser->Location = System::Drawing::Point(baseX, baseY);
        labelUser->AutoSize = true;
        labelUser->Font = gcnew System::Drawing::Font("Segoe UI", 9,
            System::Drawing::FontStyle::Regular);
        this->Controls->Add(labelUser);

        textUser = gcnew System::Windows::Forms::TextBox();
        textUser->Location = System::Drawing::Point(baseX, baseY + rowH);
        textUser->Width = textW;
        textUser->Font = gcnew System::Drawing::Font("Segoe UI", 9,
            System::Drawing::FontStyle::Regular);
        this->Controls->Add(textUser);

        // Чекбокс «Администратор»
        checkAdmin = gcnew System::Windows::Forms::CheckBox();
        checkAdmin->Text = "Войти как администратор";
        checkAdmin->Location = System::Drawing::Point(baseX, baseY + rowH * 2 + gap);
        checkAdmin->AutoSize = true;
        checkAdmin->Font = gcnew System::Drawing::Font("Segoe UI", 9,
            System::Drawing::FontStyle::Regular);
        checkAdmin->CheckedChanged += gcnew System::EventHandler(this, &LoginForm::OnAdminCheckedChanged);
        this->Controls->Add(checkAdmin);

        // «Пароль администратора»
        labelPass = gcnew System::Windows::Forms::Label();
        labelPass->Text = "Пароль администратора:";
        labelPass->Location = System::Drawing::Point(baseX, baseY + rowH * 3 + gap * 2);
        labelPass->AutoSize = true;
        labelPass->Font = gcnew System::Drawing::Font("Segoe UI", 9,
            System::Drawing::FontStyle::Regular);
        this->Controls->Add(labelPass);

        textPass = gcnew System::Windows::Forms::TextBox();
        textPass->Location = System::Drawing::Point(baseX, baseY + rowH * 4 + gap * 2);
        textPass->Width = textW;
        textPass->PasswordChar = '*';
        textPass->Font = gcnew System::Drawing::Font("Segoe UI", 9,
            System::Drawing::FontStyle::Regular);
        this->Controls->Add(textPass);

        // Кнопка OK
        btnOK = gcnew System::Windows::Forms::Button();
        btnOK->Text = "Войти";
        btnOK->Location = System::Drawing::Point(baseX, baseY + rowH * 6 + gap * 3);
        btnOK->Size = System::Drawing::Size(120, 32);
        btnOK->BackColor = System::Drawing::Color::FromArgb(52, 152, 219);
        btnOK->ForeColor = System::Drawing::Color::White;
        btnOK->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
        btnOK->FlatAppearance->BorderSize = 0;
        btnOK->Font = gcnew System::Drawing::Font("Segoe UI", 9,
            System::Drawing::FontStyle::Bold);
        btnOK->Click += gcnew System::EventHandler(this, &LoginForm::OnOkClick);
        this->Controls->Add(btnOK);

        // Кнопка Cancel
        btnCancel = gcnew System::Windows::Forms::Button();
        btnCancel->Text = "Отмена";
        btnCancel->Location = System::Drawing::Point(baseX + 140, baseY + rowH * 6 + gap * 3);
        btnCancel->Size = System::Drawing::Size(120, 32);
        btnCancel->BackColor = System::Drawing::Color::FromArgb(149, 165, 166);
        btnCancel->ForeColor = System::Drawing::Color::White;
        btnCancel->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
        btnCancel->FlatAppearance->BorderSize = 0;
        btnCancel->Font = gcnew System::Drawing::Font("Segoe UI", 9,
            System::Drawing::FontStyle::Regular);
        btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
        this->Controls->Add(btnCancel);

        this->AcceptButton = btnOK;
        this->CancelButton = btnCancel;

        // ErrorProvider
        errorProvider = gcnew System::Windows::Forms::ErrorProvider(components);
        errorProvider->ContainerControl = this;

        // По умолчанию поля пароля выключены, пока не выбрана галка «Администратор»
        SetAdminControlsEnabled(false);
    }

    void SetAdminControlsEnabled(bool enabled)
    {
        labelPass->Enabled = enabled;
        textPass->Enabled = enabled;
        if (!enabled)
        {
            textPass->Text = "";
            errorProvider->SetError(textPass, "");
        }
    }

    void OnAdminCheckedChanged(System::Object^ sender, System::EventArgs^ e)
    {
        SetAdminControlsEnabled(checkAdmin->Checked);
    }

    bool ValidateAll()
    {
        errorProvider->Clear();
        bool ok = true;

        System::String^ name = textUser->Text->Trim();
        if (System::String::IsNullOrEmpty(name))
        {
            errorProvider->SetError(textUser, "Имя не может быть пустым");
            ok = false;
        }
        else if (name->Length > 30)
        {
            errorProvider->SetError(textUser, "Слишком длинное имя (макс. 30 символов)");
            ok = false;
        }

        // Если выбран админ – проверяем и имя, и пароль
        if (checkAdmin->Checked)
        {
            System::String^ pass = textPass->Text;
            System::String^ correctPass = "admin";

            if (name != "Admin")
            {
                errorProvider->SetError(textUser,
                    "Неверное имя администратора");
                ok = false;
            }

            if (System::String::IsNullOrEmpty(pass))
            {
                errorProvider->SetError(textPass, "Введите пароль администратора");
                ok = false;
            }
            else if (pass != correctPass)
            {
                errorProvider->SetError(textPass, "Неверный пароль");
                ok = false;
            }
        }

        return ok;
    }

    void OnOkClick(System::Object^ sender, System::EventArgs^ e)
    {
        if (!ValidateAll()) return;

        UserName = textUser->Text->Trim();
        IsAdmin = checkAdmin->Checked;

        this->DialogResult = System::Windows::Forms::DialogResult::OK;
        this->Close();
    }
};
