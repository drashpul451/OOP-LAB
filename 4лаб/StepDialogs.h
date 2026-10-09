#pragma once

namespace Project1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Drawing;

	public ref class Step1Form : public System::Windows::Forms::Form
	{
	public:
		Step1Form(void)
		{
			InitializeComponent();
		}

	private:
		System::Windows::Forms::Button^ btnNext;
		System::Windows::Forms::Button^ btnCancel;

		void InitializeComponent(void)
		{
			this->btnNext = gcnew System::Windows::Forms::Button();
			this->btnCancel = gcnew System::Windows::Forms::Button();
			this->SuspendLayout();
			// 
			// btnNext
			// 
			this->btnNext->Location = System::Drawing::Point(12, 12);
			this->btnNext->Size = System::Drawing::Size(75, 23);
			this->btnNext->Text = L"\u0414\u0430\u043B\u0456 \u003E";
			this->btnNext->Click += gcnew System::EventHandler(this, &Step1Form::btnNext_Click);
			// 
			// btnCancel
			// 
			this->btnCancel->Location = System::Drawing::Point(197, 12);
			this->btnCancel->Size = System::Drawing::Size(75, 23);
			this->btnCancel->Text = L"\u0412\u0456\u0434\u043C\u0456\u043D\u0430";
			this->btnCancel->Click += gcnew System::EventHandler(this, &Step1Form::btnCancel_Click);
			// 
			// Step1Form
			// 
			this->ClientSize = System::Drawing::Size(284, 61);
			this->Controls->Add(this->btnNext);
			this->Controls->Add(this->btnCancel);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->StartPosition = FormStartPosition::CenterParent;
			this->Text = L"Step 1";
			this->ResumeLayout(false);
		}

		System::Void btnNext_Click(System::Object^ sender, System::EventArgs^ e)
		{
			this->DialogResult = System::Windows::Forms::DialogResult::OK; // act like EndDialog(...,1)
			this->Close();
		}

		System::Void btnCancel_Click(System::Object^ sender, System::EventArgs^ e)
		{
			this->DialogResult = System::Windows::Forms::DialogResult::Cancel; // act like EndDialog(...,0)
			this->Close();
		}
	};

	public ref class Step2Form : public System::Windows::Forms::Form
	{
	public:
		Step2Form(void)
		{
			InitializeComponent();
		}

	private:
		System::Windows::Forms::Button^ btnBack;
		System::Windows::Forms::Button^ btnYes;
		System::Windows::Forms::Button^ btnCancel;

		void InitializeComponent(void)
		{
			this->btnBack = gcnew System::Windows::Forms::Button();
			this->btnYes = gcnew System::Windows::Forms::Button();
			this->btnCancel = gcnew System::Windows::Forms::Button();
			this->SuspendLayout();
			// 
			// btnBack
			// 
			this->btnBack->Location = System::Drawing::Point(12, 12);
			this->btnBack->Size = System::Drawing::Size(75, 23);
			this->btnBack->Text = L"\u003C \u041D\u0430\u0437\u0430\u0434";
			this->btnBack->Click += gcnew System::EventHandler(this, &Step2Form::btnBack_Click);
			// 
			// btnYes
			// 
			this->btnYes->Location = System::Drawing::Point(103, 12);
			this->btnYes->Size = System::Drawing::Size(75, 23);
			this->btnYes->Text = L"\u0422\u0430\u043A";
			this->btnYes->Click += gcnew System::EventHandler(this, &Step2Form::btnYes_Click);
			// 
			// btnCancel
			// 
			this->btnCancel->Location = System::Drawing::Point(197, 12);
			this->btnCancel->Size = System::Drawing::Size(75, 23);
			this->btnCancel->Text = L"\u0412\u0456\u0434\u043C\u0456\u043D\u0430";
			this->btnCancel->Click += gcnew System::EventHandler(this, &Step2Form::btnCancel_Click);
			// 
			// Step2Form
			// 
			this->ClientSize = System::Drawing::Size(284, 61);
			this->Controls->Add(this->btnBack);
			this->Controls->Add(this->btnYes);
			this->Controls->Add(this->btnCancel);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->StartPosition = FormStartPosition::CenterParent;
			this->Text = L"Step 2";
			this->ResumeLayout(false);
		}

		System::Void btnBack_Click(System::Object^ sender, System::EventArgs^ e)
		{
			// use Retry to represent Back (like EndDialog(...,-1))
			this->DialogResult = System::Windows::Forms::DialogResult::Retry;
			this->Close();
		}

		System::Void btnYes_Click(System::Object^ sender, System::EventArgs^ e)
		{
			this->DialogResult = System::Windows::Forms::DialogResult::OK; // act like EndDialog(...,1)
			this->Close();

			
		}

		System::Void btnCancel_Click(System::Object^ sender, System::EventArgs^ e)
		{
			this->DialogResult = System::Windows::Forms::DialogResult::Cancel; // act like EndDialog(...,0)
			this->Close();
		}
	};

}
