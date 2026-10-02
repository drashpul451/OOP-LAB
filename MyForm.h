#pragma once
#include "ScrollDialog.h"
#include "StepDialogs.h"

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
    private: System::Windows::Forms::Button^ btnScrollDialog;
    private: System::Windows::Forms::Button^ btnStepDialogs;

    private: int displayedNumber = 0;



    private: System::String^ displayedText = nullptr;

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

        void InitializeComponent(void)
        {
            this->btnScrollDialog = (gcnew System::Windows::Forms::Button());
            this->btnStepDialogs = (gcnew System::Windows::Forms::Button());
            this->SuspendLayout();
            // 
            // btnScrollDialog
            // 
            this->btnScrollDialog->Location = System::Drawing::Point(12, 12);
            this->btnScrollDialog->Name = L"btnScrollDialog";
            this->btnScrollDialog->Size = System::Drawing::Size(200, 30);
            this->btnScrollDialog->TabIndex = 0;
            this->btnScrollDialog->Text = L"Open Scroll Dialog";
            this->btnScrollDialog->UseVisualStyleBackColor = true;
            this->btnScrollDialog->Click += gcnew System::EventHandler(this, &MyForm::btnScrollDialog_Click);
            // 
            // btnStepDialogs
            // 
            this->btnStepDialogs->Location = System::Drawing::Point(12, 52);
            this->btnStepDialogs->Name = L"btnStepDialogs";
            this->btnStepDialogs->Size = System::Drawing::Size(200, 30);
            this->btnStepDialogs->TabIndex = 1;
            this->btnStepDialogs->Text = L"Open Step Dialogs";
            this->btnStepDialogs->UseVisualStyleBackColor = true;
            this->btnStepDialogs->Click += gcnew System::EventHandler(this, &MyForm::btnStepDialogs_Click);
            // 
            // MyForm
            // 
            this->ClientSize = System::Drawing::Size(400, 200);
            this->Controls->Add(this->btnScrollDialog);
            this->Controls->Add(this->btnStepDialogs);
            this->Name = L"MyForm";
            this->Text = L"MyForm";
            this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::MyForm_Paint);
            this->ResumeLayout(false);

        }
    private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {
    }

};

}

