#pragma once

namespace Project1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Drawing;

	public ref class ScrollDialog : public System::Windows::Forms::Form
	{
	public:
		ScrollDialog(void)
		{
			InitializeComponent();
		}

		property int SelectedNumber {
			int get() { return selectedNumber; }
		}

	private:
		System::Windows::Forms::HScrollBar^ hScrollBar;
		System::Windows::Forms::Label^ lblValue;
		System::Windows::Forms::Button^ btnYes;
		System::Windows::Forms::Button^ btnCancel;
		int selectedNumber = 1;

		void InitializeComponent(void)
		{
			this->hScrollBar = gcnew System::Windows::Forms::HScrollBar();
			this->lblValue = gcnew System::Windows::Forms::Label();
			this->btnYes = gcnew System::Windows::Forms::Button();
			this->btnCancel = gcnew System::Windows::Forms::Button();
			this->SuspendLayout();
			// 
			// hScrollBar
			// 
			this->hScrollBar->Location = System::Drawing::Point(12, 12);
			this->hScrollBar->Name = L"hScrollBar";
			this->hScrollBar->Size = System::Drawing::Size(260, 20);
			this->hScrollBar->Minimum = 1;
			this->hScrollBar->Maximum = 100;
			this->hScrollBar->Value = 1;
			this->hScrollBar->SmallChange = 1;
			this->hScrollBar->LargeChange = 1;
			this->hScrollBar->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &ScrollDialog::hScrollBar_Scroll);
			// 
			// lblValue
			// 
			this->lblValue->Location = System::Drawing::Point(12, 40);
			this->lblValue->Size = System::Drawing::Size(100, 23);
			this->lblValue->Text = L"1";
			// 
			// btnYes
			// 
			this->btnYes->Location = System::Drawing::Point(12, 70);
			this->btnYes->Size = System::Drawing::Size(75, 23);
			this->btnYes->Text = L"\u0422\u0430\u043A";
			this->btnYes->Click += gcnew System::EventHandler(this, &ScrollDialog::btnYes_Click);
			// 
			// btnCancel
			// 
			this->btnCancel->Location = System::Drawing::Point(197, 70);
			this->btnCancel->Size = System::Drawing::Size(75, 23);
			this->btnCancel->Text = L"\u0412\u0456\u0434\u043C\u0456\u043D\u0430";
			this->btnCancel->Click += gcnew System::EventHandler(this, &ScrollDialog::btnCancel_Click);
			// 
			// ScrollDialog
			// 
			this->ClientSize = System::Drawing::Size(284, 111);
			this->Controls->Add(this->hScrollBar);
			this->Controls->Add(this->lblValue);
			this->Controls->Add(this->btnYes);
			this->Controls->Add(this->btnCancel);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->StartPosition = FormStartPosition::CenterParent;
			this->Text = L"Select number";
			this->ResumeLayout(false);
		}

		System::Void hScrollBar_Scroll(System::Object^ sender, System::Windows::Forms::ScrollEventArgs^ e)
		{
			selectedNumber = this->hScrollBar->Value;
			this->lblValue->Text = selectedNumber.ToString();
		}

		System::Void btnYes_Click(System::Object^ sender, System::EventArgs^ e)
		{
			selectedNumber = this->hScrollBar->Value;
			this->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->Close();
		}

		System::Void btnCancel_Click(System::Object^ sender, System::EventArgs^ e)
		{
			this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->Close();
		}
	};

}
