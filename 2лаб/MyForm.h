#pragma once
#include "ScrollDialog.h"
#include "StepDialogs.h"
#include "shape.h"
#include "shape_editor.h"

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
            editor = gcnew Project1::ShapeObjectsEditor();
            // Reduce flicker by enabling double buffering and optimized painting
            this->SetStyle(System::Windows::Forms::ControlStyles::AllPaintingInWmPaint |
                System::Windows::Forms::ControlStyles::UserPaint |
                System::Windows::Forms::ControlStyles::OptimizedDoubleBuffer, true);
            this->UpdateStyles();
        }

    // Menu actions
    System::Void work1ToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
    {
        // Trigger the same action as the old Scroll Dialog button
        btnScrollDialog_Click(nullptr, nullptr);
    }

    System::Void work2ToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
    {
        // Trigger the same action as the old Step Dialogs button
        btnStepDialogs_Click(nullptr, nullptr);
    }

    protected:
        ~MyForm()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        /// <summary>
        /// Required designer variable.
        /// </summary>
        System::ComponentModel::Container ^components;
    private: System::Windows::Forms::MenuStrip^ menuStrip1;
    private: System::Windows::Forms::ToolStripMenuItem^ fileToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^ objectsToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^ helpToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^ pointToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^ lineToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^ rectToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^ ellipseToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^ clearToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^ exitToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^ saveToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^ loadToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^ deleteLastToolStripMenuItem;
    private: System::Windows::Forms::StatusStrip^ statusStrip1;
    private: System::Windows::Forms::ToolStripStatusLabel^ toolStripStatusLabel1;


    private: int displayedNumber = 0;



    private: System::String^ displayedText = nullptr;
    private: Project1::ShapeObjectsEditor^ editor;


    // Event handlers
    System::Void btnScrollDialog_Click(System::Object^ sender, System::EventArgs^ e)
    {
        Project1::ScrollDialog^ dlg = gcnew Project1::ScrollDialog();
        if (dlg->ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
            // Show the Ukrainian word "Так" and the selected number in the main window
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
                break; // EndDialog(...,0)
            }
            if (r1 == System::Windows::Forms::DialogResult::OK) {
                displayedText = L"\u0422\u0430\u043A";
                Project1::Step2Form^ s2 = gcnew Project1::Step2Form();
                System::Windows::Forms::DialogResult r2 = s2->ShowDialog(this);
                if (r2 == System::Windows::Forms::DialogResult::Retry) {
                    // Back: loop to show Step1 again this->btnCancel->Text = L"\u0412\u0456\u0434\u043C\u0456\u043D\u0430";
                    continue;
                }
                if (r2 == System::Windows::Forms::DialogResult::Cancel) {
                    displayedText = L"\u0412\u0456\u0434\u043C\u0456\u043D\u0430";
                    displayedNumber = 0;
                    this->Invalidate();
                }
                else if (r2 == System::Windows::Forms::DialogResult::OK) {
                    // Yes pressed on Step2 - for demo set displayedNumber to 0 and repaint
                    displayedNumber = 0;
                    this->Invalidate();
                }
                break;
            }
        }
    }

    System::Void MyForm_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
    {
        // Let the editor draw all shapes first
        if (editor != nullptr) editor->OnPaint(e);

        // Then draw overlay text (status)
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

    // Forward mouse events to the editor
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
        // Only repaint when the left button is down (user is actively drawing)
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

    // Object menu handlers (Ukr)
    System::Void pointToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (editor != nullptr) editor->StartPointEditor();
        // Show current editing mode in window title (per variant requirement)
        this->Text = L"\u0413\u0440\u0430\u0444\u0456\u0447\u043D\u0438\u0439 \u0440\u0435\u0434\u0430\u043A\u0442\u043E\u0440 - \u0420\u0435\u0436\u0438\u043C \u0432\u0432\u043E\u0434\u0443 \u0442\u043E\u0447\u043E\u043A";
    }

    System::Void lineToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (editor != nullptr) editor->StartLineEditor();
        this->pointToolStripMenuItem->Checked = false;
        this->lineToolStripMenuItem->Checked = true;
        this->rectToolStripMenuItem->Checked = false;
        this->ellipseToolStripMenuItem->Checked = false;
        this->Text = L"\u0413\u0440\u0430\u0444\u0456\u0447\u043D\u0438\u0439 \u0440\u0435\u0434\u0430\u043A\u0442\u043E\u0440 - \u0420\u0435\u0436\u0438\u043C \u0432\u0432\u043E\u0434\u0443 \u043B\u0456\u043D\u0456\u044F";
    }

    System::Void rectToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (editor != nullptr) editor->StartRectEditor();
        this->pointToolStripMenuItem->Checked = false;
        this->lineToolStripMenuItem->Checked = false;
        this->rectToolStripMenuItem->Checked = true;
        this->ellipseToolStripMenuItem->Checked = false;
        this->Text = L"\u0413\u0440\u0430\u0444\u0456\u0447\u043D\u0438\u0439 \u0440\u0435\u0434\u0430\u043A\u0442\u043E\u0440 - \u0420\u0435\u0436\u0438\u043C \u0432\u0432\u043E\u0434\u0443 \u043F\u0440\u044F\u043C\u043E\u043A\u0443\u0442\u043D\u0438\u043A\u0456\u0432";
    }

    System::Void ellipseToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (editor != nullptr) editor->StartEllipseEditor();
        this->pointToolStripMenuItem->Checked = false;
        this->lineToolStripMenuItem->Checked = false;
        this->rectToolStripMenuItem->Checked = false;
        this->ellipseToolStripMenuItem->Checked = true;
        this->Text = L"\u0413\u0440\u0430\u0444\u0456\u0447\u043D\u0438\u0439 \u0440\u0435\u0434\u0430\u043A\u0442\u043E\u0440 - \u0420\u0435\u0436\u0438\u043C \u0432\u0432\u043E\u0434\u0443 \u0435\u043B\u0456\u043F\u0441\u0456\u0432";
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
            this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuStrip1->SuspendLayout();
            this->SuspendLayout();
            // 
            // menuStrip1
            // 
            // Add top-level menu items (guarded to avoid nulls)
            if (this->fileToolStripMenuItem != nullptr) this->menuStrip1->Items->Add(this->fileToolStripMenuItem);
            if (this->objectsToolStripMenuItem != nullptr) this->menuStrip1->Items->Add(this->objectsToolStripMenuItem);
            if (this->helpToolStripMenuItem != nullptr) this->menuStrip1->Items->Add(this->helpToolStripMenuItem);
            this->menuStrip1->Location = System::Drawing::Point(0, 0);
            this->menuStrip1->Name = L"menuStrip1";
            this->menuStrip1->Size = System::Drawing::Size(400, 24);
            this->menuStrip1->TabIndex = 0;
            this->menuStrip1->Text = L"menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
            this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
            this->fileToolStripMenuItem->Text = L"File";
            // 
            // objectsToolStripMenuItem
            // 
            this->objectsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) { this->pointToolStripMenuItem, this->lineToolStripMenuItem, this->rectToolStripMenuItem, this->ellipseToolStripMenuItem });
            this->objectsToolStripMenuItem->Name = L"objectsToolStripMenuItem";
            this->objectsToolStripMenuItem->Size = System::Drawing::Size(70, 20);
            this->objectsToolStripMenuItem->Text = L"\u041E\u0431\u0027\u0454\u043A\u0442\u0438"; // Об'єкти
            // 
            // pointToolStripMenuItem
            // 
            this->pointToolStripMenuItem->Name = L"pointToolStripMenuItem";
            this->pointToolStripMenuItem->Size = System::Drawing::Size(180, 22);
            this->pointToolStripMenuItem->Text = L"\u0422\u043E\u0447\u043A\u0430"; // Точка
            // 
            // lineToolStripMenuItem
            // 
            this->lineToolStripMenuItem->Name = L"lineToolStripMenuItem";
            this->lineToolStripMenuItem->Size = System::Drawing::Size(180, 22);
            this->lineToolStripMenuItem->Text = L"\u041B\u0456\u043D\u0456\u044F"; // Лінія
            // 
            // rectToolStripMenuItem
            // 
            this->rectToolStripMenuItem->Name = L"rectToolStripMenuItem";
            this->rectToolStripMenuItem->Size = System::Drawing::Size(180, 22);
            this->rectToolStripMenuItem->Text = L"\u041F\u0440\u044F\u043C\u043E\u043A\u0443\u0442\u043D\u0438\u043A"; // Прямокутник
            // 
            // ellipseToolStripMenuItem
            // 
            this->ellipseToolStripMenuItem->Name = L"ellipseToolStripMenuItem";
            this->ellipseToolStripMenuItem->Size = System::Drawing::Size(180, 22);
            this->ellipseToolStripMenuItem->Text = L"\u0415\u043B\u0456\u043F\u0441"; // Еліпс
            // attach click handlers
            this->pointToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::pointToolStripMenuItem_Click);
            this->lineToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::lineToolStripMenuItem_Click);
            this->rectToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::rectToolStripMenuItem_Click);
            this->ellipseToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::ellipseToolStripMenuItem_Click);
            // create File->Очистити,Вихід items and add them
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
            // 
            // helpToolStripMenuItem
            // 
            this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
            this->helpToolStripMenuItem->Size = System::Drawing::Size(44, 20);
            this->helpToolStripMenuItem->Text = L"\u0414\u043E\u0432\u0456\u0434\u043A\u0430"; // Довідка
            // File menu items (Очистити, Вихід)
            this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) { this->clearToolStripMenuItem, this->exitToolStripMenuItem });
            this->clearToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->clearToolStripMenuItem->Text = L"\u041E\u0447\u0438\u0441\u0442\u0438\u0442\u0438"; // Очистити
            this->exitToolStripMenuItem->Text = L"\u0412\u0438\u0445\u0456\u0434"; // Вихід
            this->clearToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::clearToolStripMenuItem_Click);
            this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::exitToolStripMenuItem_Click);
            // 
            // MyForm
            // 
            this->ClientSize = System::Drawing::Size(400, 200);
            this->Controls->Add(this->menuStrip1);
            this->MainMenuStrip = this->menuStrip1;
            this->Name = L"MyForm";
            this->Text = L"MyForm";
            // Attach paint and mouse handlers so the editor receives input
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

