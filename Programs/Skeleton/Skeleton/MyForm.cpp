#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
void Main(array<String^>^ args) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    //WinformCDemo is your project name
    ModellingOfRelativisticPhysics::MyForm form;
    Application::Run(%form);
}

System::Void ModellingOfRelativisticPhysics::MyForm::startB_Click(System::Object^ sender, System::EventArgs^ e)
{

    inputHandler.startSym(0, NULL);
    return System::Void();
}

System::Void ModellingOfRelativisticPhysics::MyForm::MyForm_Load(System::Object^ sender, System::EventArgs^ e)
{
    inputHandler.closeSym();
    return System::Void();
}
