#pragma once
#include "InputHandling.h"

namespace RelativisticPhysics {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	static int argc;
	static std::vector<const char*> argv;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
		InputHandler inputHandler;
	private: System::Windows::Forms::Label^ label1;

		public:
			MyForm(void)
			{
				InitializeComponent();
				//
				//TODO: Add the constructor code here
				//
			}


		protected:
			/// <summary>
			/// Clean up any resources being used.
			/// </summary>
			~MyForm()
			{
				if (components)
				{
					delete components;
				}
			}
		private: System::Windows::Forms::Button^ startB;
		protected:

		private:
			/// <summary>
			/// Required designer variable.
			/// </summary>
			System::ComponentModel::Container ^components;

		#pragma region Windows Form Designer generated code
			/// <summary>
			/// Required method for Designer support - do not modify
			/// the contents of this method with the code editor.
			/// </summary>
			void InitializeComponent(void)
			{
				this->startB = (gcnew System::Windows::Forms::Button());
				this->label1 = (gcnew System::Windows::Forms::Label());
				this->SuspendLayout();
				// 
				// startB
				// 
				this->startB->Location = System::Drawing::Point(12, 62);
				this->startB->Name = L"startB";
				this->startB->Size = System::Drawing::Size(257, 23);
				this->startB->TabIndex = 0;
				this->startB->Text = L"Start";
				this->startB->UseVisualStyleBackColor = true;
				this->startB->Click += gcnew System::EventHandler(this, &MyForm::startB_Click);
				// 
				// label1
				// 
				this->label1->AutoSize = true;
				this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
					static_cast<System::Byte>(0)));
				this->label1->Location = System::Drawing::Point(13, 13);
				this->label1->Name = L"label1";
				this->label1->Size = System::Drawing::Size(248, 20);
				this->label1->TabIndex = 1;
				this->label1->Text = L"Visualization of relativistyc physics";
				this->label1->TextAlign = System::Drawing::ContentAlignment::TopCenter;
				this->label1->Click += gcnew System::EventHandler(this, &MyForm::label1_Click);
				// 
				// MyForm
				// 
				this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->ClientSize = System::Drawing::Size(282, 412);
				this->Controls->Add(this->label1);
				this->Controls->Add(this->startB);
				this->Name = L"MyForm";
				this->Text = L"Visualization of relativistic physics";
				this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
				this->ResumeLayout(false);
				this->PerformLayout();

			}
		#pragma endregion
	private: 
		System::Void startB_Click(System::Object^ sender, System::EventArgs^ e);

		System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e);
	private: System::Void label1_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	};
}
