#include "MyForm.h"

#include "InputHandler.h"

using namespace System;
using namespace System::Windows::Forms;

array<String^>^ args;

[STAThreadAttribute]
void Main(array<String^>^ _args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    RelativisticPhysics::MyForm form();
    Application::Run(% form);
}

System::Void RelativisticPhysics::MyForm::startB_Click(System::Object^ sender, System::EventArgs^ e)
{

    InputHandler::getInstance()->startSym(args);
    return System::Void();
}

System::Void RelativisticPhysics::MyForm::MyForm_Load(System::Object^ sender, System::EventArgs^ e)
{
    InputHandler::getInstance()->closeSym();
    return System::Void();
}

System::Void RelativisticPhysics::MyForm::label1_Click(System::Object^ sender, System::EventArgs^ e) {
}
