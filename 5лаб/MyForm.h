#pragma once
#include "ScrollDialog.h"
#include "StepDialogs.h"
#include "editor_wrapper.h"

namespace Project1 {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    /// <summary>
    /// Minimal MyForm - cleaned to a fresh empty window
    /// </summary>
    public ref class MyForm : public System::Windows::Forms::Form
    {
    public:
        MyForm(void)
        {
            InitializeComponent();
            editor = gcnew Project1::ManagedEditor();
            // Reduce flicker by enabling double buffering and optimized painting
            this->SetStyle(System::Windows::Forms::ControlStyles::AllPaintingInWmPaint |
                System::Windows::Forms::ControlStyles::UserPaint |
                System::Windows::Forms::ControlStyles::OptimizedDoubleBuffer, true);
            this->UpdateStyles();

            ////////////////////////////////
            // Генерація графічних іконок у пам'яті (без використання зовнішніх файлів)
            const int icoSize = 24;

            // 1. Точка
            System::Drawing::Bitmap^ bmpPoint = gcnew System::Drawing::Bitmap(icoSize, icoSize);
            {
                System::Drawing::Graphics^ g = System::Drawing::Graphics::FromImage(bmpPoint);
                g->Clear(System::Drawing::Color::Transparent);
                System::Drawing::Brush^ b = gcnew System::Drawing::SolidBrush(System::Drawing::Color::Black);
                g->FillEllipse(b, icoSize / 2 - 3, icoSize / 2 - 3, 6, 6);
                delete b; delete g;
            }

            // 2. Лінія
            System::Drawing::Bitmap^ bmpLine = gcnew System::Drawing::Bitmap(icoSize, icoSize);
            {
                System::Drawing::Graphics^ g = System::Drawing::Graphics::FromImage(bmpLine);
                g->Clear(System::Drawing::Color::Transparent);
                System::Drawing::Pen^ p = gcnew System::Drawing::Pen(System::Drawing::Color::Black, 2);
                g->DrawLine(p, 4, icoSize - 4, icoSize - 4, 4);
                delete p; delete g;
            }

            // 3. Прямокутник
            System::Drawing::Bitmap^ bmpRect = gcnew System::Drawing::Bitmap(icoSize, icoSize);
            {
                System::Drawing::Graphics^ g = System::Drawing::Graphics::FromImage(bmpRect);
                g->Clear(System::Drawing::Color::Transparent);
                System::Drawing::Pen^ p = gcnew System::Drawing::Pen(System::Drawing::Color::Black, 2);
                g->DrawRectangle(p, 4, 4, icoSize - 8, icoSize - 8);
                delete p; delete g;
            }

            // 4. Еліпс
            System::Drawing::Bitmap^ bmpEllipse = gcnew System::Drawing::Bitmap(icoSize, icoSize);
            {
                System::Drawing::Graphics^ g = System::Drawing::Graphics::FromImage(bmpEllipse);
                g->Clear(System::Drawing::Color::Transparent);
                System::Drawing::Pen^ p = gcnew System::Drawing::Pen(System::Drawing::Color::Black, 2);
                g->DrawEllipse(p, 3, 5, icoSize - 6, icoSize - 10);
                delete p; delete g;
            }

            // 5. Лінія з кружечками (LineOO)
            System::Drawing::Bitmap^ bmpLineOO = gcnew System::Drawing::Bitmap(icoSize, icoSize);
            {
                System::Drawing::Graphics^ g = System::Drawing::Graphics::FromImage(bmpLineOO);
                g->Clear(System::Drawing::Color::Transparent);
                System::Drawing::Pen^ p = gcnew System::Drawing::Pen(System::Drawing::Color::Black, 2);
                System::Drawing::Brush^ b = gcnew System::Drawing::SolidBrush(System::Drawing::Color::Black);
                g->DrawLine(p, 5, icoSize - 5, icoSize - 5, 5);
                g->FillEllipse(b, 2, icoSize - 8, 6, 6);
                g->FillEllipse(b, icoSize - 8, 2, 6, 6);
                delete b; delete p; delete g;
            }

            // 6. Куб
            System::Drawing::Bitmap^ bmpCube = gcnew System::Drawing::Bitmap(icoSize, icoSize);
            {
                System::Drawing::Graphics^ g = System::Drawing::Graphics::FromImage(bmpCube);
                g->Clear(System::Drawing::Color::Transparent);
                System::Drawing::Pen^ p = gcnew System::Drawing::Pen(System::Drawing::Color::Black, 1.5f);
                g->DrawRectangle(p, 3, 8, 12, 12);
                g->DrawRectangle(p, 8, 3, 12, 12);
                g->DrawLine(p, 3, 8, 8, 3);
                g->DrawLine(p, 15, 8, 20, 3);
                g->DrawLine(p, 3, 20, 8, 15);
                g->DrawLine(p, 15, 20, 20, 15);
                delete p; delete g;
            }

            // Присвоєння картинка-only режиму для всіх кнопок панелі
            this->toolStripButtonPoint->Image = bmpPoint;
            this->toolStripButtonPoint->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;

            this->toolStripButtonLine->Image = bmpLine;
            this->toolStripButtonLine->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;

            this->toolStripButtonRect->Image = bmpRect;
            this->toolStripButtonRect->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;

            this->toolStripButtonEllipse->Image = bmpEllipse;
            this->toolStripButtonEllipse->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;

            this->toolStripButtonLineOO->Image = bmpLineOO;
            this->toolStripButtonLineOO->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;

            this->toolStripButtonCube->Image = bmpCube;
            this->toolStripButtonCube->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;

            // Присвоєння іконок у пункті меню "Об'єкти"
            this->pointToolStripMenuItem->Image = bmpPoint;
            this->lineToolStripMenuItem->Image = bmpLine;
            this->rectToolStripMenuItem->Image = bmpRect;
            this->ellipseToolStripMenuItem->Image = bmpEllipse;

            // Вимикаємо автоматичний CheckOnClick
            this->toolStripButtonPoint->CheckOnClick = false;
            this->toolStripButtonLine->CheckOnClick = false;
            this->toolStripButtonRect->CheckOnClick = false;
            this->toolStripButtonEllipse->CheckOnClick = false;
            this->toolStripButtonLineOO->CheckOnClick = false;
            this->toolStripButtonCube->CheckOnClick = false;
        }

        // Menu actions
        System::Void work1ToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
        {
            btnScrollDialog_Click(nullptr, nullptr);
        }

        System::Void work2ToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
        {
            btnStepDialogs_Click(nullptr, nullptr);
        }

    protected:
        ~MyForm()
        {
            if (editor != nullptr)
            {
                editor->CloseTableWindow();
            }
            if (components)
            {
                delete components;
            }
        }

    private:
        /// <summary>
        /// Required designer variable.
        /// </summary>
        System::ComponentModel::Container^ components;
    private: System::Windows::Forms::MenuStrip^ menuStrip1;
    private: System::Windows::Forms::ToolStrip^ toolStrip1;
    private: System::Windows::Forms::ToolStripButton^ toolStripButtonPoint;
    private: System::Windows::Forms::ToolStripButton^ toolStripButtonLine;
    private: System::Windows::Forms::ToolStripButton^ toolStripButtonRect;
    private: System::Windows::Forms::ToolStripButton^ toolStripButtonEllipse;
    private: System::Windows::Forms::ToolStripButton^ toolStripButtonLineOO;
    private: System::Windows::Forms::ToolStripButton^ toolStripButtonCube;
    private: System::Windows::Forms::ToolStripMenuItem^ fileToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^ objectsToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^ helpToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^ pointToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^ lineToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^ rectToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^ ellipseToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^ tableToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^ clearToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^ exitToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^ saveToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^ loadToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^ deleteLastToolStripMenuItem;
    private: System::Windows::Forms::StatusStrip^ statusStrip1;
    private: System::Windows::Forms::ToolStripStatusLabel^ toolStripStatusLabel1;

    private: int displayedNumber = 0;
    private: System::String^ displayedText = nullptr;
    private: Project1::ManagedEditor^ editor;

           // Event handlers
           System::Void btnScrollDialog_Click(System::Object^ sender, System::EventArgs^ e)
           {
               Project1::ScrollDialog^ dlg = gcnew Project1::ScrollDialog();
               if (dlg->ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
                   displayedText = L"\u0422\u0430\u043A";
                   displayedNumber = dlg->SelectedNumber;
                   this->Invalidate();
               }
           }

           System::Void btnStepDialogs_Click(System::Object^ sender, System::EventArgs^ e)
           {
               while (true) {
                   Project1::Step1Form^ s1 = gcnew Project1::Step1Form();
                   System::Windows::Forms::DialogResult r1 = s1->ShowDialog(this);
                   if (r1 == System::Windows::Forms::DialogResult::Cancel) {
                       displayedText = L"\u0412\u0456\u0434\u043C\u0456\u043D\u0430";
                       displayedNumber = 0;
                       this->Invalidate();
                       break;
                   }
                   if (r1 == System::Windows::Forms::DialogResult::OK) {
                       displayedText = L"\u0422\u0430\u043A";
                       Project1::Step2Form^ s2 = gcnew Project1::Step2Form();
                       System::Windows::Forms::DialogResult r2 = s2->ShowDialog(this);
                       if (r2 == System::Windows::Forms::DialogResult::Retry) {
                           continue;
                       }
                       if (r2 == System::Windows::Forms::DialogResult::Cancel) {
                           displayedText = L"\u0412\u0456\u0434\u043C\u0456\u043D\u0430";
                           displayedNumber = 0;
                           this->Invalidate();
                       }
                       else if (r2 == System::Windows::Forms::DialogResult::OK) {
                           displayedNumber = 0;
                           this->Invalidate();
                       }
                       break;
                   }
               }
           }

           System::Void MyForm_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
           {
               if (editor != nullptr) editor->OnPaint(e);

               System::Drawing::Font^ f = gcnew System::Drawing::Font("Segoe UI", 16);
               System::Drawing::Brush^ b = System::Drawing::Brushes::Black;
               if (!System::String::IsNullOrEmpty(displayedText)) {
                   if (displayedNumber != 0) {
                       System::String^ combined = System::String::Concat(displayedText, L" ", displayedNumber.ToString());
                       e->Graphics->DrawString(combined, f, b, System::Drawing::PointF(230.0f, 20.0f));
                   }
                   else {
                       e->Graphics->DrawString(displayedText, f, b, System::Drawing::PointF(230.0f, 20.0f));
                   }
               }
               else if (displayedNumber != 0) {
                   e->Graphics->DrawString(displayedNumber.ToString(), f, b, System::Drawing::PointF(230.0f, 20.0f));
               }
           }

           System::Void MyForm_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
           {
               if (editor == nullptr) return;
               if (e->Button == System::Windows::Forms::MouseButtons::Left) {
                   editor->OnLBdown(this, e->X, e->Y);
                   this->Invalidate();
               }
           }

           System::Void MyForm_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
           {
               if (editor == nullptr) return;
               editor->OnMouseMove(this, e->X, e->Y);
               if (System::Windows::Forms::Control::MouseButtons == System::Windows::Forms::MouseButtons::Left) {
                   this->Invalidate();
               }
           }

           System::Void MyForm_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
           {
               if (editor == nullptr) return;
               if (e->Button == System::Windows::Forms::MouseButtons::Left) {
                   editor->OnLBup(this, e->X, e->Y);
                   this->Invalidate();
               }
           }

           // Object menu handlers
           System::Void pointToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
           {
               if (editor != nullptr) editor->StartByMode(Project1::ManagedShapeMode::Point);
               this->Text = L"\u0413\u0440\u0430\u0444\u0456\u0447\u043D\u0438\u0439 \u0440\u0435\u0434\u0430\u043A\u0442\u043E\u0440 - \u0420\u0435\u0436\u0438\u043C \u0432\u0432\u043E\u0434\u0443 \u0442\u043E\u0447\u043E\u043A";
           }

           System::Void lineToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
           {
               if (editor != nullptr) editor->StartByMode(Project1::ManagedShapeMode::Line);
               this->pointToolStripMenuItem->Checked = false;
               this->lineToolStripMenuItem->Checked = true;
               this->rectToolStripMenuItem->Checked = false;
               this->ellipseToolStripMenuItem->Checked = false;
               this->Text = L"\u0413\u0440\u0430\u0444\u0456\u0447\u043D\u0438\u0439 \u0440\u0435\u0434\u0430\u043A\u0442\u043E\u0440 - \u0420\u0435\u0436\u0438\u043C \u0432\u0432\u043E\u0434\u0443 \u043B\u0456\u043D\u0456\u044F";
           }

           System::Void rectToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
           {
               if (editor != nullptr) editor->StartByMode(Project1::ManagedShapeMode::Rect);
               this->pointToolStripMenuItem->Checked = false;
               this->lineToolStripMenuItem->Checked = false;
               this->rectToolStripMenuItem->Checked = true;
               this->ellipseToolStripMenuItem->Checked = false;
               this->Text = L"\u0413\u0440\u0430\u0444\u0456\u0447\u043D\u0438\u0439 \u0440\u0435\u0434\u0430\u043A\u0442\u043E\u0440 - \u0420\u0435\u0436\u0438\u043C \u0432\u0432\u043E\u0434\u0443 \u043F\u0440\u044F\u043C\u043E\u043A\u0443\u0442\u043D\u0438\u043A\u0456\u0432";
           }

           System::Void ellipseToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
           {
               if (editor != nullptr) editor->StartByMode(Project1::ManagedShapeMode::Ellipse);
               this->pointToolStripMenuItem->Checked = false;
               this->lineToolStripMenuItem->Checked = false;
               this->rectToolStripMenuItem->Checked = false;
               this->ellipseToolStripMenuItem->Checked = true;
               this->Text = L"\u0413\u0440\u0430\u0444\u0456\u0447\u043D\u0438\u0439 \u0440\u0435\u0434\u0430\u043A\u0442\u043E\u0440 - \u0420\u0435\u0436\u0438\u043C \u0432\u0432\u043E\u0434\u0443 \u0435\u043B\u0456\u043F\u0441\u0456\u0432";
           }

           System::Void lineOO_ToolStripButton_Click(System::Object^ sender, System::EventArgs^ e)
           {
               if (editor != nullptr) editor->StartByMode(Project1::ManagedShapeMode::LineOO);
               this->Text = L"\u0413\u0440\u0430\u0444\u0456\u0447\u043D\u0438\u0439 \u0440\u0435\u0434\u0430\u043A\u0442\u043E\u0440 - \u041B\u0456\u043D\u0456\u044F \u0437 \u043A\u0440\u0443\u0436\u0435\u0447\u043A\u0430\u043C\u0438";
           }

           System::Void cube_ToolStripButton_Click(System::Object^ sender, System::EventArgs^ e)
           {
               if (editor != nullptr) editor->StartByMode(Project1::ManagedShapeMode::Cube);
               this->Text = L"\u0413\u0440\u0430\u0444\u0456\u0447\u043D\u0438\u0439 \u0440\u0435\u0434\u0430\u043A\u0442\u043E\u0440 - \u041A\u0443\u0431";
           }

           System::Void tableToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
           {
               if (editor != nullptr) editor->ToggleTableWindow(this);
           }

           // File menu handlers
           System::Void clearToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
           {
               if (editor != nullptr) editor->ClearAll(this);
           }

           System::Void exitToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
           {
               Application::Exit();
           }

           void InitializeComponent(void)
           {
               this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
               this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
               this->objectsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
               this->pointToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
               this->lineToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
               this->rectToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
               this->ellipseToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
               this->tableToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
               this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
               this->menuStrip1->SuspendLayout();
               this->SuspendLayout();

               // MenuStrip Setup
               if (this->fileToolStripMenuItem != nullptr) this->menuStrip1->Items->Add(this->fileToolStripMenuItem);
               if (this->objectsToolStripMenuItem != nullptr) this->menuStrip1->Items->Add(this->objectsToolStripMenuItem);
               if (this->helpToolStripMenuItem != nullptr) this->menuStrip1->Items->Add(this->helpToolStripMenuItem);
               this->menuStrip1->Location = System::Drawing::Point(0, 0);
               this->menuStrip1->Name = L"menuStrip1";
               this->menuStrip1->Size = System::Drawing::Size(400, 24);
               this->menuStrip1->TabIndex = 0;
               this->menuStrip1->Text = L"menuStrip1";

               // ToolStrip Setup
               this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
               this->toolStripButtonPoint = (gcnew System::Windows::Forms::ToolStripButton());
               this->toolStripButtonLine = (gcnew System::Windows::Forms::ToolStripButton());
               this->toolStripButtonRect = (gcnew System::Windows::Forms::ToolStripButton());
               this->toolStripButtonEllipse = (gcnew System::Windows::Forms::ToolStripButton());
               this->toolStripButtonLineOO = (gcnew System::Windows::Forms::ToolStripButton());
               this->toolStripButtonCube = (gcnew System::Windows::Forms::ToolStripButton());
               this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {
                   this->toolStripButtonPoint, this->toolStripButtonLine, this->toolStripButtonRect,
                       this->toolStripButtonEllipse, this->toolStripButtonLineOO, this->toolStripButtonCube
               });
               this->toolStrip1->Location = System::Drawing::Point(0, 24);
               this->toolStrip1->Name = L"toolStrip1";
               this->toolStrip1->Size = System::Drawing::Size(400, 25);
               this->toolStrip1->TabIndex = 1;
               this->toolStrip1->Text = L"toolStrip1";
               this->toolStrip1->Dock = System::Windows::Forms::DockStyle::Top;

               // Налаштування підказок для кнопок
               this->toolStripButtonPoint->ToolTipText = L"Точка";
               this->toolStripButtonPoint->Click += gcnew System::EventHandler(this, &MyForm::pointToolStripMenuItem_Click);

               this->toolStripButtonLine->ToolTipText = L"Лінія";
               this->toolStripButtonLine->Click += gcnew System::EventHandler(this, &MyForm::lineToolStripMenuItem_Click);

               this->toolStripButtonRect->ToolTipText = L"Прямокутник";
               this->toolStripButtonRect->Click += gcnew System::EventHandler(this, &MyForm::rectToolStripMenuItem_Click);

               this->toolStripButtonEllipse->ToolTipText = L"Еліпс";
               this->toolStripButtonEllipse->Click += gcnew System::EventHandler(this, &MyForm::ellipseToolStripMenuItem_Click);

               this->toolStripButtonLineOO->ToolTipText = L"Лінія з кружечками";
               this->toolStripButtonLineOO->Click += gcnew System::EventHandler(this, &MyForm::lineOO_ToolStripButton_Click);

               this->toolStripButtonCube->ToolTipText = L"Куб";
               this->toolStripButtonCube->Click += gcnew System::EventHandler(this, &MyForm::cube_ToolStripButton_Click);

               // File Menu
               this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
               this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
               this->fileToolStripMenuItem->Text = L"File";

               // Objects Menu
               this->objectsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
                   this->pointToolStripMenuItem, this->lineToolStripMenuItem, this->rectToolStripMenuItem, this->ellipseToolStripMenuItem, this->tableToolStripMenuItem
               });
               this->objectsToolStripMenuItem->Name = L"objectsToolStripMenuItem";
               this->objectsToolStripMenuItem->Size = System::Drawing::Size(70, 20);
               this->objectsToolStripMenuItem->Text = L"\u041E\u0431\u0027\u0454\u043A\u0442\u0438"; // Об'єкти

               // Submenu Items
               this->pointToolStripMenuItem->Name = L"pointToolStripMenuItem";
               this->pointToolStripMenuItem->Size = System::Drawing::Size(180, 22);
               this->pointToolStripMenuItem->Text = L"\u0422\u043E\u0447\u043A\u0430"; // Точка

               this->lineToolStripMenuItem->Name = L"lineToolStripMenuItem";
               this->lineToolStripMenuItem->Size = System::Drawing::Size(180, 22);
               this->lineToolStripMenuItem->Text = L"\u041B\u0456\u043D\u0456\u044F"; // Лінія

               this->rectToolStripMenuItem->Name = L"rectToolStripMenuItem";
               this->rectToolStripMenuItem->Size = System::Drawing::Size(180, 22);
               this->rectToolStripMenuItem->Text = L"\u041F\u0440\u044F\u043C\u043E\u043A\u0443\u0442\u043D\u0438\u043A"; // Прямокутник

               this->ellipseToolStripMenuItem->Name = L"ellipseToolStripMenuItem";
               this->ellipseToolStripMenuItem->Size = System::Drawing::Size(180, 22);
               this->ellipseToolStripMenuItem->Text = L"\u0415\u043B\u0456\u043F\u0441"; // Еліпс

               this->tableToolStripMenuItem->Name = L"tableToolStripMenuItem";
               this->tableToolStripMenuItem->Size = System::Drawing::Size(180, 22);
               this->tableToolStripMenuItem->Text = L"\u0422\u0430\u0431\u043B\u0438\u0446\u044F \u043E\u0431'\u0454\u043A\u0442\u0456\u0432"; // Таблиця об'єктів

               // Click Handlers
               this->pointToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::pointToolStripMenuItem_Click);
               this->lineToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::lineToolStripMenuItem_Click);
               this->rectToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::rectToolStripMenuItem_Click);
               this->ellipseToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::ellipseToolStripMenuItem_Click);
               this->tableToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::tableToolStripMenuItem_Click);

               // Clear / Exit Items
               this->clearToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
               this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
               this->clearToolStripMenuItem->Text = L"\u041E\u0447\u0438\u0441\u0442\u0438\u0442\u0438"; // Очистити
               this->exitToolStripMenuItem->Text = L"\u0412\u0438\u0445\u0456\u0434"; // Вихід
               this->clearToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::clearToolStripMenuItem_Click);
               this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::exitToolStripMenuItem_Click);

               if (this->fileToolStripMenuItem != nullptr) {
                   this->fileToolStripMenuItem->DropDownItems->Add(this->clearToolStripMenuItem);
                   this->fileToolStripMenuItem->DropDownItems->Add(this->exitToolStripMenuItem);
               }

               // Help Menu
               this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
               this->helpToolStripMenuItem->Size = System::Drawing::Size(44, 20);
               this->helpToolStripMenuItem->Text = L"\u0414\u043E\u0432\u0456\u0434\u043A\u0430"; // Довідка

               // Form Main Configuration
               this->ClientSize = System::Drawing::Size(400, 240);
               this->Controls->Add(this->toolStrip1);
               this->Controls->Add(this->menuStrip1);
               this->MainMenuStrip = this->menuStrip1;
               this->Name = L"MyForm";
               this->Text = L"MyForm";

               // Event Subscriptions
               this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::MyForm_Paint);
               this->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MyForm_MouseDown);
               this->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MyForm_MouseMove);
               this->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MyForm_MouseUp);
               this->menuStrip1->ResumeLayout(false);
               this->menuStrip1->PerformLayout();
               this->ResumeLayout(false);
               this->PerformLayout();
           }

    private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {
    }

    };

}